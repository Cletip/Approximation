/*!
 * \file liste.h
 * \brief File corresponding to the header of liste.c.
 * \author Payard - Laurencot
 * \version pré-alpha
 * \date 8 Octobre 2021
 */
#ifndef LISTE_HEADER_
#define LISTE_HEADER_


#include <stdio.h>
#include <stdlib.h>

/**
 * \brief Structure correspondante à un point.
 *
 * Cette structure est un point, donc elle contient :
 * - x : La coordonnée en x du point.
 * - y : La coordonnée en y du point.
 */
typedef struct point
{
  float x;
  float y;
} point;

/**
 * \brief Structure corresponding to every element of a list.
 * This structure is for every element of the list.
 * it contains :
 * - suiv : a pointer on the next element of the list.
 * - val : a pointer on every thing which is the value of the element.
 */
typedef struct Maillon{
  struct Maillon *suiv;
  point val;
}Maillon;

/**
 *\brief Structure corresponding to the list.
 * This structure is just composed with a pointer on the first element of the list.
 */
typedef struct Liste{
  struct Maillon *first;
}Liste;

/**
 * \fn Liste creerListe()
 * \brief Function that create a list.
 * \return l - l is a properly initialized list
 */
Liste creerListe();
/**
 * \fn void detruireListe(Liste l)
 * \brief Function that free all the part of a list.
 * This function is very useful to avoid all memory leak while playing with
 * list. After the use of this function, you can reuse or let the pointer on a
 * list you sent here.
 * \param l - l is the list we work on 
 * \return void
 */
void detruireListe(Liste l);
/**
 * \fn void ajouteDebut(Liste *l, void *n)
 * \brief Function that put a value as the first value of the list
 * \param l - l is the list we work on
 * \param n - the value we want to add to the list
 * \return void
 */
void ajouteDebut(Liste *l, point n);
/**
 * \fn void ajouteFin(Liste *l, void *n)
 * \brief Function that put a value as the last value of the list
 * \param l - l is the list we work on
 * \param n - the value we want to add to the list
 * \return void
 */
void ajouteFin(Liste *l, point n);
/**
 * \fn void afficheListe(Liste l)
 * \brief Function that print the pointer on every element on the terminal.
 * This function is mostly useful in debugging.
 * \param l - l is the list we work on 
 * \return void
 */
void afficheListePoints(Liste l);
/**
 * \fn int ListLenght(Liste l)
 * \brief Function that get the size of a list.
 * Warning : This function as to read every element of the list, for big list it
 * can be pretty slow.
 * \param l - l is the list we work on 
 * \return result - result is the list length
 */
int ListLenght(Liste l);
/**
 * \fn void supprDebut(Liste *l)
 * \brief Function that remove the first element of the list if there is one.
 * \param *l - l is the pointer of the list we work on 
 * \return void - l is a pointer, no need to return it
 */
void supprDebut(Liste *l);
/**
 * \fn void supprFin(Liste *l)
 * \brief Function that remove the last element of the list if there is one.
 * \param *l - l is the pointer of the list we work on 
 * \return void - l is a pointer, no need to return it
 */
void supprFin(Liste *l);
/**
 * \fn void supprValeur(Liste *l, void *val)
 * \brief Function that remove one element of the list if there is one with the
 * same value as the one passed in parameter.
 * \param val - val is the value we want to destroy in the list
 * \param *l - l is the pointer of the list we work on 
 * \return void - l is a pointer, no need to return it
 */
void supprValeur(Liste *l, point val);
/**
 * \fn void supprMaillon(Liste *l, int nbMaillon)
 * \brief Function that remove the element of the list if there is one that as a
 * number equal to nbMaillon, with value 0 for the first.
 * \param *l - l is the pointer of the list we work on
 * \param nbMaillon - nbMaillon is the number of the maillon we want to destroy
 * \return void - l is a pointer, no need to return it
 */
void supprMaillon(Liste *l, int nbMaillon);


float **ListeToTabsPoints(Liste l);
void ViderListe(Liste *l);

#endif /* LISTE_HEADER_ */
