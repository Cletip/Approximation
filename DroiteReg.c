#include "DroiteReg.h"
#include "listePoint.h"
#include "polynome.h"


polynome *UneDroiteReg(Liste l){
  polynome *P = creePolynome(1);
  int n = ListLenght(l);
  float **Res = ListeToTabsPoints(l);
  P->p[1] = (moyXY(Res, n) - moyX(Res, n) * moyY(Res, n))/(moyXcar(Res, n) - carmoyX(Res, n));
  P->p[0] = moyY(Res, n) - moyX(Res, n)*((moyXY(Res, n) - moyX(Res, n) * moyY(Res, n))/(moyXcar(Res, n) - carmoyX(Res, n)));
  free(Res[1]);free(Res[0]);free(Res);
  return P;
}


polynome *DeuxDroiteReg(Liste l){
  float **Res = ListeToTabsPoints(l);
  int n = ListLenght(l);
  float moy = moyY(Res, n);
  Maillon *m = l.first;
  Liste l1 = creerListe();
  Liste l2 = creerListe();
  for(int i = 0; i < n; i++){
    if(m->val.y < moy){
      ajouteFin(&l1, m->val);
    }else {
      ajouteFin(&l2, m->val);
    }
    m = m->suiv;
  }
  polynome *p1 = UneDroiteReg(l1);
  polynome *p2 = UneDroiteReg(l2);
  polynome *Ret = creePolynome(1);
  Ret->p[0] = (p1->p[0] + p2->p[0])/2;
  Ret->p[1] = (p1->p[1] + p2->p[1])/2;

  detruireListe(l1);
  detruireListe(l2);
  free(Res[1]);free(Res[0]);free(Res);
  return Ret;
}
