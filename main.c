/*!
 * \file main.c
 * \brief Fichier principale de notre programme d'interpolation
 * \author Payard - Laurencot
 * \version pré-alpha
 * \date 8 Octobre 2021
 *
 *
 */

/*!
  \mainpage Programme d'interpolation
  \section Explication
  Ce programme applique l'interpolation de Newton et de
  Lagrange sur une liste de point. Il permet donc de trouver le
  polynôme passant par les points de la liste.

 */
#include "main.h"
#include "listePoint.h"
#include "polynome.h"

/**
 * \fn int main ()
 * \brief Fonction principale du programme
 *
 *
 * \return 0 - Ce qui termine le programme
 */
int main()
{
  //  srand(time(NULL)); // in case we need random

  polynome *Solution;
  Liste listePoints = creerListe();

  char choix = 'A';

  printf("Bonjours et bienvenue dans ce programme d'interpolation.\n");
  printf("Pour voire l'aide utiliser la lettre h.\n");
  while (choix != 'q')
  {
    printf("Que voulez vous faire ? ");
    scanf("%c", &choix);
    getchar();
    switch (choix)
    {
    case 'h':
      printf(
          "    h : Afficher cette aide.\n    g : Ouvre une fenêtre "
          "graphique.\n    f : Remplir automatiquement.\n    n : Applique "
          "l'interpolation de Newton.\n    l : applique l'interpolation de "
          "lagrange.\n    m : Applique l'interpolation de Newton avec un "
          "affichage précis.\n    l : applique l'interpolation de lagrange "
          "avec un affichage précis.\n    r : Permet de remplir des points.\n  "
          "  a : affiche la liste des points actuelles.\n    q : Quitter.\n\n");
      break;
    case 'g':
      RenderingInterpolation(&listePoints);
      break;
    case 'f':
      casAutoFill(&listePoints);
      break;
    case 'n':
      Solution = ResolutionParNewton(listePoints);
      affichepolynome(Solution);
      break;
    case 'm':
      Solution = ResolutionParNewton(listePoints);
      affichepolynomePrecis(Solution, listePoints);
      break;
    case 'l':
      Solution = calculLagrange(listePoints);
      affichepolynome(Solution);
      break;
    case 'k':
      Solution = calculLagrange(listePoints);
      affichepolynomePrecis(Solution, listePoints);
      break;
    case 'r':
      casRemplir(&listePoints);
      break;
    case 'a':
      afficheListePoints(listePoints);
      break;
    case 'q':
      printf("Au revoir!\n");
      break;
    default:
      printf("Toutes les lettres a rentrés sont "
             "en minuscule. Utiliser h pour savoir quels sont vos choix.\n\n");
      getchar();
      break;
    }
  }

  /* polynome *p1 = creePolynomedemande(); */
  /* polynome *p2 = creePolynome(0); */
  /* polynome *poly = multPolynome(p1, p2); */
  /* printf("Test\n"); */

  /* affichepolynome(poly); */

  /* Test des différentes fonctions : */
  //  float flo = 3.0;
  //  polynome *machin = transformefloatenpoly(flo);

  /* int a = 10; */
  /* printf("le truc polynome transformé est %f  d'indice %d \n", machin->p[a],
   * a); */
  /* int i = 1000; */
  /* while (machin->p[i] == 0) */
  /* { */
  /*   printf("le truc polynome transformé est %d \n", i); */
  /*   i++; */
  /* } */

  /*Liste l = creerListe();

  point p;
  p.x = 0;
  p.y = 0;
  point k;
  k.x = 2;
  k.y = 4;
  point e;
  e.x = 4;
  e.y = 0;
  point m;
  m.x = 6;
  m.y = 4;
  ajouteDebut(&l, m);
  ajouteDebut(&l, e);
  ajouteDebut(&l, k);
  ajouteDebut(&l, p);
  float **points;


   points = ListeToTabsPoints(l); */

  /* for (int i = 0; i < 4; i++) */
  /* { */
  /* printf("point : %f, %f\n", points[0][i], points[1][i]); */
  /* } */

  //  polynome *fonction = calculLagrange(l);

  //  affichepolynome(fonction);
  //  fonction = ResolutionParNewton(l);
  //  affichepolynome(fonction);

  //  for(int i = 0; i < 4; i++){
  //    printf("point : %f, %f\n", points[0][i], points[1][i]);
  //  }

  //  polynome *pute = ResolutionParNewton(l);
  //  affichepolynome(pute);printf("\n");

  //  int x; /**< Description de x. */

  return 0;
}

void casRemplir(Liste *l){
  char choix = 'A';
  char s[30];
  int quit = 0;
  int curseur = 0;
  point p;
  
  if(ListLenght(*l) != 0){
    afficheListePoints(*l);
    printf("Voulez vous vider la liste(n = non) ? ");
    scanf("%c", &choix);
    getchar();
    if(choix != 'n'){
      ViderListe(l);
    }
  }

  while(!quit){
    printf("Veuillez entrer le X(un caractère pour quitter) : ");
    scanf("%s", s);getchar();
    curseur = 0;
    while(s[curseur] != '\0'){
      if(s[curseur] == ',' || s[curseur] == '.'){
	s[curseur] = '.';
      }else if(!isdigit(s[curseur])  && s[curseur] != '-'){
	quit = 1;
	break;
      }
      curseur++;
    }
    if(!quit){
      p.x = atof(s);
      printf("Veuillez entrer le Y(un caractère pour quitter) : ");
      scanf("%s", s);getchar();
      curseur = 0;
      while(s[curseur] != '\0'){
	if(s[curseur] == ',' || s[curseur] == '.'){
	  s[curseur] = '.';
	}else if(!isdigit(s[curseur]) && s[curseur] != '-'){
	  quit = 1;
	  break;
	}
	curseur++;
      }
      if(!quit){
	p.y = atof(s);
	ajouteFin(l, p);
      }
    }
  }  
}


void casAutoFill(Liste *l){
  char choix = 'A';
  point p;
 
    printf("Comment voulez vous remplir la liste?\n        e : Densité de l'eau en fonction de la température.\n        u : S1.\n        d : S2.\n        t : S3.\n        m : Depenses mensuelle et revenu.\nVotre choix :");
    scanf("%c", &choix);
    getchar();
    switch (choix) {
    case 'e':ViderListe(l);
      p.x = 0;
      p.y = 0.99987;
      ajouteFin(l, p);
      p.x = 2;
      p.y = 0.99997;
      ajouteFin(l, p);
      p.x = 4;
      p.y = 1;
      ajouteFin(l, p);
      p.x = 6;
      p.y = 0.99997;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 0.99988;
      ajouteFin(l, p);
      p.x = 10;
      p.y = 0.99973;
      ajouteFin(l, p);
      p.x = 12;
      p.y = 0.99953;
      ajouteFin(l, p);
      p.x = 14;
      p.y = 0.99927;
      ajouteFin(l, p);
      p.x = 16;
      p.y = 0.99897;
      ajouteFin(l, p);
      p.x = 18;
      p.y = 0.99846;
      ajouteFin(l, p);
      p.x = 20;
      p.y = 0.99805;
      ajouteFin(l, p);
      p.x = 22;
      p.y = 0.99751;
      ajouteFin(l, p);
      p.x = 24;
      p.y = 0.99705;
      ajouteFin(l, p);
      p.x = 26;
      p.y = 0.99650;
      ajouteFin(l, p);
      p.x = 28;
      p.y = 0.99664;
      ajouteFin(l, p);
      p.x = 30;
      p.y = 0.99533;
      ajouteFin(l, p);
      p.x = 32;
      p.y = 0.99472;
      ajouteFin(l, p);
      p.x = 34;
      p.y = 0.99472;
      ajouteFin(l, p);
      p.x = 36;
      p.y = 0.99333;
      ajouteFin(l, p);
      p.x = 38;
      p.y = 0.99326;
      ajouteFin(l, p);
      break;
    case 'u': ViderListe(l);
      p.x = 10;
      p.y = 9.14;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 8.14;
      ajouteFin(l, p);
      p.x = 13;
      p.y = 8.74;
      ajouteFin(l, p);
      p.x = 9;
      p.y = 8.77;
      ajouteFin(l, p);
      p.x = 11;
      p.y = 9.26;
      ajouteFin(l, p);
      p.x = 14;
      p.y = 8.1;
      ajouteFin(l, p);
      p.x = 6;
      p.y = 6.13;
      ajouteFin(l, p);
      p.x = 4;
      p.y = 3.1;
      ajouteFin(l, p);
      p.x = 12;
      p.y = 9.13;
      ajouteFin(l, p);
      p.x = 7;
      p.y = 7.26;
      ajouteFin(l, p);
      p.x = 5;
      p.y = 4.74;
      ajouteFin(l, p);break;
    case 'd': ViderListe(l);
      p.x = 10;
      p.y = 7.46;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 6.77;
      ajouteFin(l, p);
      p.x = 13;
      p.y = 12.74;
      ajouteFin(l, p);
      p.x = 9;
      p.y = 7.11;
      ajouteFin(l, p);
      p.x = 11;
      p.y = 7.81;
      ajouteFin(l, p);
      p.x = 14;
      p.y = 8.84;
      ajouteFin(l, p);
      p.x = 6;
      p.y = 6.08;
      ajouteFin(l, p);
      p.x = 4;
      p.y = 5.39;
      ajouteFin(l, p);
      p.x = 12;
      p.y = 8.15;
      ajouteFin(l, p);
      p.x = 7;
      p.y = 6.42;
      ajouteFin(l, p);
      p.x = 5;
      p.y = 5.73;
      ajouteFin(l, p);break;
    case 't': ViderListe(l);
      p.x = 8;
      p.y = 6.58;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 5.76;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 7.71;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 8.84;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 8.47;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 7.04;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 5.25;
      ajouteFin(l, p);
      p.x = 19;
      p.y = 12.5;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 5.56;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 7.91;
      ajouteFin(l, p);
      p.x = 8;
      p.y = 6.89;
      ajouteFin(l, p);break;
    case 'm' :ViderListe(l);
      p.x = 752;
      p.y = 85;
      ajouteFin(l, p);
      p.x = 855;
      p.y = 83;
      ajouteFin(l, p);
      p.x = 871;
      p.y = 162;
      ajouteFin(l, p);
      p.x = 610;
      p.y = 81;
      ajouteFin(l, p);
      p.x = 582;
      p.y = 83;
      ajouteFin(l, p);
      p.x = 921;
      p.y = 281;
      ajouteFin(l, p);
      p.x = 492;
      p.y = 81;
      ajouteFin(l, p);
      p.x = 569;
      p.y = 81;
      ajouteFin(l, p);
      p.x = 462;
      p.y = 80;
      ajouteFin(l, p);
      p.x = 907;
      p.y = 243;
      ajouteFin(l, p);
      p.x = 643;
      p.y = 84;
      ajouteFin(l, p);
      p.x = 862;
      p.y = 84;
      ajouteFin(l, p);
      p.x = 524;
      p.y = 82;
      ajouteFin(l, p);
      p.x = 679;
      p.y = 80;
      ajouteFin(l, p);
      p.x = 902;
      p.y = 226;
      ajouteFin(l, p);
      p.x = 918;
      p.y = 260;
      ajouteFin(l, p);
      p.x = 828;
      p.y = 82;
      ajouteFin(l, p);
      p.x = 875;
      p.y = 186;
      ajouteFin(l, p);
      p.x = 809;
      p.y = 77;
      ajouteFin(l, p);
      p.x = 894;
      p.y = 223;
      ajouteFin(l, p);
      break;
    default:printf("Ce type de remplissage n'existe pas.\n");break;
    }
}
