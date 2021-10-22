/*!
 * \file render.h
 * \brief Header for the render.c file.
 * \author Payard - Laurencot
 * \version pré-alpha
 * \date 10 10 2021
 */
#ifndef RENDER_HEADER_
#define RENDER_HEADER_

#include "lagrange.h"
#include "listePoint.h"
#include "newton.h"
#include "polynome.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <time.h>

#define FPS_TO_GET 60  /*!< Frames render per second*/
#define TICK_TO_GET 60 /*!< Verification done per second*/

Liste *RenderingInterpolation(Liste *l);

/**
 * \fn void create_Win(SDL_Renderer **renderer)
 * \brief Fonction de création d'une fenetre
 *
 * \param **renderer l'adresse de laffichage dans la fenêtre
 * \return void
 */
void create_Win(SDL_Renderer **renderer, SDL_Window *window, int *SX, int *SY,
                SDL_Texture **Graph);

/**
 * \fn void end_sdl(char ok, char const * msg, SDL_Window *window, SDL_Renderer
 * *renderer) \brief Fonction de destruction d'une fenetre
 *
 * \param ok information sur si la fenetre est détruite à cause d'une erreur ou
 * de facon voulut \param *msg message à afficher à l'utilisateur notament en
 * cas de bug \param *window l'adresse de la fenetre à détruire \param *renderer
 * l'adresse de laffichage dans la fenêtre à détruire \return void
 */
void end_sdl(char ok, char const *msg, SDL_Renderer *renderer,
             SDL_Window *window);

/**
 * \fn long int getTime()
 * \brief Fonction qui retourne le temps en microsecondes
 *
 * \return un entier correspondant au temps
 */
long int getTime();

/**
 * \fn void keyUp(SDL_KeyboardEvent *key)
 * \brief Fonction de gestion du cas ou l'on arrete d'appuyer sur une touche
 *
 * \param *key la touche sur laquelle on arrête d'appuyer
 * \return void
 */
void keyUp(SDL_KeyboardEvent *key, int *Stape);

/**
 * \fn void draw(SDL_Renderer *renderer)
 * \brief Dessin de l'écran de bug dans la fenetre
 *
 * \param *renderer l'adresse de l'intérieur de la fenetre que l'on veut
 * redessiner \return void
 */
void draw(SDL_Renderer *renderer, int SX, int SY, polynome *newt,
          polynome *lagr, Liste pointNewt, Liste pointLagr, Liste l,
          TTF_Font *Font, int TXdeb, int TYdeb, int TXfin, int TYfin,
          SDL_Texture *Graph, char *StringX, char *StringY,
          int pointeurWriteEmp);

#endif /* RENDER_HEADER_ */
