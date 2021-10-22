#include "DroiteReg.h"

polynome *UneDroiteReg(Liste l){
  polynome *P = creePolynome(1);
  int n = ListLenght(l);
  float **Res = ListeToTabsPoints(l);
  P->p[1] = (moyXY(Res, n) - moyX(Res, n) * moyY(Res, n))/(moyXcar(Res, n) - carmoyX(Res, n));
  P->p[0] = moyY(Res, n) - moyX(Res, n)*((moyXY(Res, n) - moyX(Res, n) * moyY(Res, n))/(moyXcar(Res, n) - carmoyX(Res, n)));
  return P;
}


//polynome *DeuxDroiteReg(Liste l){ }
