
/*!
 * \file point.h
 * \brief Header du point.c
 * \author Payard - Laurencot
 * \version pré-alpha
 * \date 8 Octobre 2021
 */
#ifndef POINT_HEADER_
#define POINT_HEADER_

#include "listePoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Polynome{
  float *p;
  int maxDeg;
}polynome;

polynome *creePolynome(int maxDeg);
polynome *creePolynomedemande();
void destroyPolynome(polynome *p);
void affichepolynome(polynome *p);
void affichepolynomePrecis(polynome *p, Liste l);
double precision(polynome *p, Liste l);
polynome *transformefloatenpoly(float unfloat);
polynome *addPolynome(polynome *p1, polynome *p2);
polynome *multPolynome(polynome *p1, polynome *p2);

polynome *AdaptePoly(polynome *p);
void ViderPoly(polynome *p);


#endif
