#include "render.h"
#include "lagrange.h"
#include "listePoint.h"
#include "polynome.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \fn int main (int argc, char **argv)
 * \brief Fonction principale du programme contenant la bouclle principale
 *
 * \param argc nombre d'argument entré lors de l'appelle du programme
 * \param argv argumentss entrés lors de l'appelle du programme
 * \return 0 - Si tout c'est bien passé
 */
Liste *RenderingInterpolation(Liste *l)
{

  SDL_Window *window = NULL; /*!< Adresse de la fenêtre que l'on va créer. */

  int Stape = 10;   /*!< Etat actuelle du jeu. */
  int SizeX = 1200; /*!< Taille de l'écran en largeur. */
  int SizeY = 750;  /*!< Taille de l'écran en longueur. */

  polynome *newt = ResolutionParNewton(*l);
  polynome *lagr = calculLagrange(*l);
  float lx = -10000000000;
  float ly = -10000000000;

  Liste pointNewt = creerListe();
  Liste pointLagr = creerListe();
  long int LastFrame;
  long int TimeCount;
  long int NowTime;
  long int timeForNewFrame = 1000000 / FPS_TO_GET;

  int fpsCount = 0;

  long int LastTick;
  long int timeForNewTick = 1000000 / TICK_TO_GET;

  int tickCount = 0;

  int sourisX;
  int sourisY;

  /* Nombre de points à chargé */
  int nbPoints = 10000;

  char StringX[50];
  char StringY[50];
  strcpy(StringX, "0");
  strcpy(StringY, "0");
  int pointeurWriteEmp = 0;

  /* statut. Si 1 alors réactualisé */
  int done = 0;
  SDL_Texture *Graph;

  int tmpCount = 0;

  SDL_Event event;
  SDL_Renderer *renderer = NULL;

  int graphXdeb = -10;
  int graphYdeb = -10;

  int graphXS = 20;
  int graphYS = 20;

  create_Win(&renderer, window, &SizeX, &SizeY, &Graph);

  /* choisit la fonte */
  TTF_Font *Font = TTF_OpenFont("Res/Roboto-Black.ttf", 50);

  /* calcul des deux courbes  */
  float sol = 0;
  point ptempo;
  for (int i = 0; i < (graphXS + 2); i++)
  {
    sol = 0;
    for (int j = 0; j < newt->maxDeg + 1; j++)
    {
      sol += newt->p[j] * pow((i + (graphXdeb)-1), j);
    }
    ptempo.x = (float)(i + graphXdeb - 1);
    ptempo.y = sol;
    ajouteFin(&pointNewt, ptempo);
  }

  for (int i = 0; i < (graphXS + 2); i++)
  {
    sol = 0;
    for (int j = 0; j < lagr->maxDeg + 1; j++)
    {
      sol += lagr->p[j] * pow((float)(i + graphXdeb - 1), j);
    }
    ptempo.x = (float)(i + graphXdeb - 1);
    ptempo.y = sol;
    ajouteFin(&pointLagr, ptempo);
  }

  /************Initialisation des variables de temps**************/
  LastFrame = getTime();
  LastTick = getTime();
  TimeCount = getTime();
  NowTime = getTime();

  /************Début de la boucle principale**********************/
  while (Stape)
  {
    NowTime = getTime();

    if (NowTime - LastFrame > timeForNewFrame)
    {

      draw(renderer, SizeX, SizeY, newt, lagr, pointNewt, pointLagr, *l, Font,
           graphXdeb, graphYdeb, graphXS, graphYS, Graph, StringX, StringY,
           pointeurWriteEmp);

      SDL_RenderPresent(renderer);
      SDL_RenderClear(renderer);

      LastFrame += timeForNewFrame;
      fpsCount++;
    }
    else if (NowTime - LastTick > timeForNewTick)
    {
      if (!done)
      {
        newt = ResolutionParNewton(*l);
        lagr = calculLagrange(*l);
        // lagr = AdaptePoly(lagr);
        ViderListe(&pointNewt);
        done = 1;
        for (int i = 0; i < nbPoints; i++)
        {
          sol = 0;
          for (int j = 0; j < newt->maxDeg + 1; j++)
          {
            sol +=
                newt->p[j] *
                pow((float)(graphXdeb + (i * (float)(graphXS) / nbPoints)), j);
          }
          ptempo.x = (float)((i * (float)(graphXS) / nbPoints) + graphXdeb);
          ptempo.y = sol;
          ajouteFin(&pointNewt, ptempo);
        }

        ViderListe(&pointLagr);
        for (int i = 0; i < nbPoints; i++)
        {
          sol = 0;
          for (int j = 0; j < lagr->maxDeg + 1; j++)
          {
            sol +=
                lagr->p[j] *
                pow((float)(graphXdeb + (i * (float)(graphXS) / nbPoints)), j);
          }
          ptempo.x = graphXdeb + (i * (float)(graphXS) / nbPoints);
          ptempo.y = sol;
          ajouteFin(&pointLagr, ptempo);
        }

        SDL_SetRenderTarget(renderer, Graph);
        // mise du fond en blanc
        SDL_Rect rectangle;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        rectangle.x = 0;
        rectangle.y = 0;
        rectangle.w = SizeX / 2;
        rectangle.h = SizeY / 2;
        SDL_RenderFillRect(renderer, &rectangle);

        // Affichage de l'echelle du graphique
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderDrawLine(renderer, (float)(SizeX / 2) / 100, (SizeY / 2) / 2,
                           SizeX / 2 - 2 * SizeX / 2 / 200, (SizeY / 2) / 2);
        SDL_RenderDrawLine(renderer, (SizeX / 2) / 2, (float)(SizeY / 2) / 100,
                           (SizeX / 2) / 2, SizeY / 2 - 2 * SizeY / 2 / 200);

        Maillon *n = pointNewt.first;
        Maillon *la = pointLagr.first;

        if (n != NULL)
        {
          while (n->suiv != NULL)
          {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderDrawLine(renderer,
                               ((n->val.x - graphXdeb) / graphXS) *
                                       (SizeX / 2 - (2 * SizeX / 2 / 100)) +
                                   (SizeX / 2 / 100),
                               ((-n->val.y - graphYdeb) / graphXS) *
                                       (SizeY / 2 - (2 * SizeY / 2 / 100)) +
                                   (SizeY / 2 / 100),
                               ((n->suiv->val.x - graphXdeb) / graphXS) *
                                       (SizeX / 2 - (2 * SizeX / 2 / 100)) +
                                   (SizeX / 2 / 100),
                               ((-n->suiv->val.y - graphYdeb) / graphXS) *
                                       (SizeY / 2 - (2 * SizeY / 2 / 100)) +
                                   (SizeY / 2 / 100));

            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderDrawLine(renderer,
                               ((la->val.x - graphXdeb) / graphXS) *
                                       (SizeX / 2 - (2 * SizeX / 2 / 100)) +
                                   (SizeX / 2 / 100),
                               ((-la->val.y - graphYdeb) / graphXS) *
                                       (SizeY / 2 - (2 * SizeY / 2 / 100)) +
                                   (SizeY / 2 / 100),
                               ((la->suiv->val.x - graphXdeb) / graphXS) *
                                       (SizeX / 2 - (2 * SizeX / 2 / 100)) +
                                   (SizeX / 2 / 100),
                               ((-la->suiv->val.y - graphYdeb) / graphXS) *
                                       (SizeY / 2 - (2 * SizeY / 2 / 100)) +
                                   (SizeY / 2 / 100));

            la = la->suiv;
            n = n->suiv;
          }
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        Maillon *m = l->first;
        while (m != NULL)
        {
          rectangle.x = ((m->val.x - graphXdeb) / graphXS) *
                            (SizeX / 2 - (2 * SizeX / 2 / 100)) +
                        (SizeX / 2 / 100) - (SizeX / 2 / 400);
          rectangle.y = ((-m->val.y - graphYdeb) / graphXS) *
                            (SizeY / 2 - (2 * SizeY / 2 / 100)) +
                        (SizeY / 2 / 100) - (SizeX / 2 / 400);
          rectangle.w = 2 * SizeX / 2 / 400;
          rectangle.h = 2 * SizeX / 2 / 400;
          SDL_RenderFillRect(renderer, &rectangle);
          m = m->suiv;
        }

        SDL_SetRenderTarget(renderer, NULL);
      }

      LastTick += timeForNewTick;
      tickCount++;
    }
    else
    {
      NowTime = getTime();
      long SleepForCPU = 0;
      if ((timeForNewTick - (NowTime - LastFrame)) <
              (timeForNewTick - (NowTime - LastTick)) &&
          (timeForNewTick - (NowTime - LastFrame)) > 0)
      {
        SleepForCPU = (long)(timeForNewFrame - (NowTime - LastFrame)) / 300;
      }
      else if ((timeForNewTick - (NowTime - LastTick)) > 0)
      {
        SleepForCPU = (long)(timeForNewTick - (NowTime - LastTick)) / 300;
      }
      SDL_Delay(SleepForCPU);
      // printf("on sleep de : %ld\n", SleepForCPU);
    }

    /* Gestion des imputs clavier */

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_KEYDOWN:
        break; // KeyDown(&event.key);break;
      case SDL_KEYUP:
        keyUp(&event.key, &Stape);
        break;
      case SDL_MOUSEWHEEL:
        if (event.wheel.y > 0)
        {
          if (graphXS < 2147483647 / 2)
          {
            graphXS *= 2;
            graphXdeb = -graphXS / 2;
            graphYS *= 2;
            graphYdeb = -graphYS / 2;
            done = 0;
          }
        }
        else if (event.wheel.y < 0)
        {
          if (graphXS > 1)
          {
            graphXS /= 2;
            graphXdeb = -graphXS / 2;
            graphYS /= 2;
            graphYdeb = -graphYS / 2;
            done = 0;
          }
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
          SDL_GetMouseState(&sourisX, &sourisY);
          if ((sourisY >= 0 && sourisY < (7 * SizeY / 8)) &&
              (sourisX >= 0 && sourisX < (6 * SizeX / 8)))
          {
            point p;
            p.x = ((((float)(sourisX - (SizeX / 100)) /
                     ((6 * SizeX / 8) - (2 * SizeX / 100))) *
                    (graphXS)) +
                   graphXdeb);
            p.y = -((((float)(sourisY - (SizeY / 100)) /
                      ((7 * SizeY / 8) - (2 * SizeY / 100))) *
                     (graphYS)) +
                    graphYdeb);
            if (p.x != lx && p.y != ly)
            {
              ajouteFin(l, p);
              done = 0;
            }
          }else if(sourisX > (6*SizeX/8) + SizeX/100 && sourisX < (6*SizeX/8) + SizeX/100 + SizeX/9 && sourisY > (SizeY/8) + (SizeY*10/25) && sourisY < (SizeY/8) + (SizeY*10/25) + SizeY*2/25){
	    Stape = 0;
	  }
        }
        else if (event.button.button == SDL_BUTTON_RIGHT)
        {
          SDL_GetMouseState(&sourisX, &sourisY);
          Maillon *m = l->first;
          float dist = graphXS / 20;
          point p;
          while (m != NULL)
          {
            if (sqrt(pow(((((float)(sourisX - (SizeX / 100)) /
                            ((6 * SizeX / 8) - (2 * SizeX / 100))) *
                           (graphXS)) +
                          graphXdeb) -
                             m->val.x,
                         2) +
                     pow(-((((float)(sourisY - (SizeY / 100)) /
                             ((7 * SizeY / 8) - (2 * SizeY / 100))) *
                            (graphYS)) +
                           graphYdeb) -
                             m->val.y,
                         2)) < dist)
            {
              p = m->val;
              dist = sqrt(pow(((((float)(sourisX - (SizeX / 100)) /
                                 ((6 * SizeX / 8) - (2 * SizeX / 100))) *
                                (graphXS)) +
                               graphXdeb) -
                                  m->val.x,
                              2) +
                          pow(-((((float)(sourisY - (SizeY / 100)) /
                                  ((7 * SizeY / 8) - (2 * SizeY / 100))) *
                                 (graphYS)) +
                                graphYdeb) -
                                  m->val.y,
                              2));
            }
            m = m->suiv;
          }
          if (dist < graphXS / 20)
          {
            supprValeur(l, p);
            done = 0;
          }
        };
        break;
      case SDL_QUIT:
        Stape = 0;
        break;
      default:
        break;
      }
    }

    if (NowTime > TimeCount)
    {
      TimeCount += 1000000;
      /* printf("%d images cette seconde et %d ticks\n", fpsCount, tickCount);
       */
      fpsCount = 0;
      tickCount = 0;
      tmpCount++;
    }
  }

  /* on referme proprement */
  end_sdl(1, "Normal ending", renderer, window);

  return l;
}

/**
 * \fn void end_sdl(char ok, char const * msg, SDL_Window *window, SDL_Renderer
 * *renderer) \brief Fonction de destruction d'une fenetre
 *
 * \param ok information sur si la fenetre est détruite à cause d'une erreur ou
 * de facon voulut \param *msg message à afficher à l'utilisateur notament en
 * cas de bug \param *window l'adresse de la fenetre à détruire \param *renderer
 * l'adresse de laffichage dans la fenêtre à détruire \return void
 */
void end_sdl(char ok, char const * msg, SDL_Renderer *renderer, SDL_Window *window){
  char msg_formated[255];
  int l;

  if (!ok){
    strncpy(msg_formated, msg, 250);
    l = strlen(msg_formated);
    strcpy(msg_formated+l, " : %s\n");

    SDL_Log(msg_formated, SDL_GetError());
  }

  if(renderer != NULL){
    SDL_DestroyRenderer(renderer);
  }
  if(window != NULL){
    SDL_DestroyWindow(window);
  }
  SDL_Quit();

  if (!ok) {exit(EXIT_FAILURE);}
}


/**
 * \fn void create_Win(SDL_Renderer **renderer)
 * \brief Fonction de création d'une fenetre
 *
 * \param **renderer l'adresse de laffichage dans la fenêtre
 * \return void
 */
void create_Win(SDL_Renderer **renderer, SDL_Window *window, int *SX, int *SY,
                SDL_Texture **Graph)
{
  SDL_DisplayMode screen_dimension;

  /* Initialisation de la SDL  + gestion de l'échec possible */
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    end_sdl(0, "ERROR SDL INIT", *renderer, window);

  /* récupère la taille de l'écran, pas utiliser */
  SDL_GetCurrentDisplayMode(0, &screen_dimension);
  //    printf("Taille de l'écran\n\tw : %d\n\th : %d\n", screen_dimension.w,
  //    screen_dimension.h);

  /* Création de la fenêtre, cas avec erreur */
  window = SDL_CreateWindow("Interpolation", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, screen_dimension.w,
                            screen_dimension.h, SDL_WINDOW_FULLSCREEN);
  if (window == NULL)
    end_sdl(0, "ERROR WINDOW CREATION", *renderer, window);

  /* Création du renderer (le truc dans la windows) */
  *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
  if (*renderer == NULL)
    end_sdl(0, "ERROR RENDERER CREATION", *renderer, window);

  /* écrire (lettre chiffre) dans le render (polynôme). Grâce à la
     bibliothèque TTF */
  if (TTF_Init() == -1)
  {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
  }

  /* Taille de écran fournit par SDL */
  SDL_GetWindowSize(window, SX, SY);

  /* créer l'image de la moitié de la taille de l'écran */
  *Graph = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA8888,
                             SDL_TEXTUREACCESS_TARGET, *SX / 2, *SY / 2);
}

/**
 * \fn long int getTime()
 * \brief Fonction qui retourne le temps en microsecondes
 *
 * \return un entier correspondant au temps
 */
long int getTime(){
  struct timespec tms;
  if (clock_gettime(CLOCK_REALTIME,&tms)) {
        return -1;
    }
  return ((tms.tv_sec*1000000) + (tms.tv_nsec/1000));
}

/**
 * \fn void keyUp(SDL_KeyboardEvent *key)
 * \brief Fonction de gestion du cas ou l'on arrete d'appuyer sur une touche
 *
 * \param *key la touche sur laquelle on arrête d'appuyer
 * \return void
 */
void keyUp(SDL_KeyboardEvent *key, int *Stape){
  //printf("%c\n", key->keysym.sym);
  switch(key->keysym.sym){
  case SDLK_ESCAPE:*Stape = 0;break;
    // case SDLK_SPACE:if(Stape == 5){Stape = 15;}else if(Stape == 15){Stape = 5;}break;
  default:break;
  }
}

/**
 * \fn void draw(SDL_Renderer *renderer)
 * \brief Dessin de l'écran de bug dans la fenetre
 *
 * \param *renderer l'adresse de l'intérieur de la fenetre que l'on veut
 * redessiner \return void
 */
void draw(SDL_Renderer *renderer, int SX, int SY, polynome *newt, polynome *lagr, Liste pointNewt, Liste pointLagr, Liste l, TTF_Font* Font, int TXdeb, int TYdeb, int TXfin, int TYfin, SDL_Texture  *Graph, char *StringX, char *StringY,  int pointeurWriteEmp){
  SDL_Rect rectangle;
  int posMX;
  int posMY;
  SDL_GetMouseState(&posMX, &posMY);

  //fond en blanc
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  rectangle.x = (6*SX/8);
  rectangle.y = 0;
  rectangle.w = SX;
  rectangle.h = SY;
  SDL_RenderFillRect(renderer, &rectangle);


  rectangle.x = (0);
  rectangle.y = (7*SY/8);
  rectangle.w = SX;
  rectangle.h = SY;
  SDL_RenderFillRect(renderer, &rectangle);

  //lignes de séparation des différentes parties
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderDrawLine(renderer, 0, (7*SY)/8, SX, (7*SY)/8);

  SDL_RenderDrawLine(renderer, (6*SX)/8, 0, (6*SX)/8, (7*SY)/8);


  
  

  //écrit le polynôme de newton
  
  char s[1000];
  SDL_Color Green = {0, 255, 0};
  strcpy(s, "Newtone : ");
  char tmp[20];
  if(newt->maxDeg != 0){
      sprintf(tmp, "%6.2f*x^%d", newt->p[newt->maxDeg], newt->maxDeg);
    }else {
      sprintf(tmp, "%6.2f", newt->p[newt->maxDeg]);
    }
  strcat(s, tmp);
  for(int i = newt->maxDeg - 1; (i >-1) && (i > newt->maxDeg - 6); i--){
    if(i != 0){
      sprintf(tmp, " + %6.2f*x^%d", newt->p[i], i);
    }else {
      sprintf(tmp, " + %6.2f", newt->p[i]);
    }
    strcat(s, tmp);
  }
  if(newt ->maxDeg > 5){
    strcat(s, " + ...");
  }
  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, s, Green); 
  SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  SDL_Rect Message_rect;
  Message_rect.x = SX/100; 
  Message_rect.y = (7*SY + (SY/100))/8; 
  Message_rect.w = (SX/9) * ((newt->maxDeg < 6)?newt->maxDeg+1 : 8);
  Message_rect.h = SY/25; 
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);


  //ecrit le polynome de lagrange
  SDL_Color Blue = {0, 0, 255};
  strcpy(s, "Lagrange : ");
  if(lagr->maxDeg != 0){
      sprintf(tmp, "%6.2f*x^%d", lagr->p[lagr->maxDeg], lagr->maxDeg);
    }else {
      sprintf(tmp, "%6.2f", lagr->p[lagr->maxDeg]);
    }
  strcat(s, tmp);
  for(int i = lagr->maxDeg - 1; (i >-1) && (i > lagr->maxDeg - 6); i--){
    if(i != 0){
      sprintf(tmp, " + %6.2f*x^%d", lagr->p[i], i);
    }else {
      sprintf(tmp, " + %6.2f", lagr->p[i]);
    }
    strcat(s, tmp);
  }
  if(lagr ->maxDeg > 5){
    strcat(s, " + ...");
  }
  surfaceMessage = TTF_RenderText_Solid(Font, s, Blue); 
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = SX/100; 
  Message_rect.y = (8*SY - (SY/100))/8 - SY/25; 
  Message_rect.w = (SX/9) * ((lagr->maxDeg < 6)?lagr->maxDeg+1 : 8);
  Message_rect.h = SY/25; 
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);


  
  //ecrit l'emplacement du pointeur
  SDL_Color Dark = {0, 0, 0};
  sprintf(s, "Pointeur :");
  surfaceMessage = TTF_RenderText_Solid(Font, s, Dark); 
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = (SX*6)/8 + (SX/100); 
  Message_rect.y = (SY/100); 
  Message_rect.w = (SX/8);
  Message_rect.h = SY/25; 
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
  if(posMX >= 0 && posMX < (6*SX/8)){
    sprintf(s, "x : %4.2f", ((((float)(posMX-(SX/100))/((6*SX/8)-(2*SX/100)))*(TXfin))+TXdeb));
  }else {
    sprintf(s, "x is out");
  }
  surfaceMessage = TTF_RenderText_Solid(Font, s, Dark); 
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = (SX*6)/8 + (SX/100); 
  Message_rect.y = (SY/100) + SY/25; 
  Message_rect.w = (SX/8);
  Message_rect.h = SY/25; 
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
  if(posMY >= 0 && posMY < (7*SY/8)){
    sprintf(s, "y : %4.2f", -((((float)(posMY-(SY/100))/((7*SY/8)-(2*SY/100)))*(TYfin))+TYdeb));
  }else {
    sprintf(s, "y is out");
  }
  surfaceMessage = TTF_RenderText_Solid(Font, s, Dark); 
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = (SX*6)/8 + (SX/100); 
  Message_rect.y = (SY/100) + (SY/100) + (2*SY)/25; 
  Message_rect.w = (SX/8);
  Message_rect.h = SY/25; 
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);



  //Ecrit les bornes maxs :
  sprintf(s, "Graph bornes :");
  surfaceMessage = TTF_RenderText_Solid(Font, s, Dark); 
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = (SX*6)/8 + (SX/100); 
  Message_rect.y = (SY/100) + (SY/100) + (4*SY)/25; 
  Message_rect.w = (SX/8);
  Message_rect.h = SY/25; 
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
  sprintf(s, "x = %d --- %d", TXdeb, TXfin+TXdeb);
  surfaceMessage = TTF_RenderText_Solid(Font, s, Dark); 
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = (SX*6)/8 + (SX/100); 
  Message_rect.y = (SY/100) + (SY/100) + (5*SY)/25; 
  Message_rect.w = (SX/8);
  Message_rect.h = SY/25; 
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
    sprintf(s, "y = %d --- %d", TYdeb, TYfin+TYdeb);
  surfaceMessage = TTF_RenderText_Solid(Font, s, Dark); 
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = (SX*6)/8 + (SX/100); 
  Message_rect.y = (SY/100) + (SY/100) + (6*SY)/25; 
  Message_rect.w = (SX/8);
  Message_rect.h = SY/25; 
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);



  // Dessine l'image de la courbe
  rectangle.x = 0;
  rectangle.y = 0;
  rectangle.w = (6*SX/8);
  rectangle.h = (7*SY/8);
  SDL_RenderFillRect(renderer, &rectangle);
  SDL_RenderCopy(renderer, Graph, NULL, &rectangle);


  //barre noire qui detour la liste des points
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderDrawLine(renderer, (SX*7)/8+ (SX/50), 0, (SX*7)/8+ (SX/50), (7*SY)/8);
  SDL_RenderDrawLine(renderer, ((SX*7)/8+ (SX/50) + SX)/2, 0, ((SX*7)/8+ (SX/50) + SX)/2, (7*SY)/8);
  for(int i = SY/25; i < (7*SY)/8; i+= SY/25){
    SDL_RenderDrawLine(renderer, (SX*7)/8+ (SX/50), i, SX, i);
  }





  // affiche la liste des points
  sprintf(s, "X    Y");
  surfaceMessage = TTF_RenderText_Solid(Font, s, Dark); 
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = (SX*7)/8 + (SX/40); 
  Message_rect.y = (SY/500); 
  Message_rect.w = (SX/11);
  Message_rect.h = SY/25; 
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
  Maillon *m = l.first; 
  for(int i = 0; i < 21 && m != NULL; i++){
    sprintf(s, "%5.3f  %5.3f", m->val.x, m->val.y);
    surfaceMessage = TTF_RenderText_Solid(Font, s, Dark);
    Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    Message_rect.x = (SX*7)/8 + (SX/40); 
    Message_rect.y = (SY/500) + ((i+1)*SY/25); 
    Message_rect.w = (SX/11);
    Message_rect.h = SY/25; 
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
    m = m->suiv;
  }


  //Bouton exit
  if(posMX > (6*SX/8) + SX/100 && posMX < (6*SX/8) + SX/100 + SX/9 && posMY > (SY/8) + (SY*10/25) && posMY < (SY/8) + (SY*10/25) + SY*2/25){
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  }else {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
  }
  rectangle.x = (6*SX/8) + SX/100;
  rectangle.y = (SY/8) + (SY*10/25);
  rectangle.w = SX/9;
  rectangle.h = SY*2/25;
  SDL_RenderFillRect(renderer, &rectangle);
  sprintf(s, "Exit");
  surfaceMessage = TTF_RenderText_Solid(Font, s, Dark); 
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = (SX*6)/8 + (SX/100); 
  Message_rect.y = (SY/8) + (SY*10/25) + (SY/100); 
  Message_rect.w = (SX/9);
  Message_rect.h = SY*3/50; 
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
  
  

  
}


