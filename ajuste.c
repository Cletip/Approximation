#include "ajuste.h"

polynome *exponentiel(Liste listedepoint)
{

  polynome *P = creePolynome(1);
  int n = ListLenght(listedepoint);
  float **Tnormal = ListeToTabsPoints(listedepoint);
  float **Tln = lnListetotab(Tnormal, n);

  float moyenneX = moyX(Tnormal, n);
  float moyenneYln = moyY(Tln, n);
  float b;
  float a;

  /* TODO, reregarder, mais normalement c'est juste, du moins je vois
     pas la faute */
  a = (moyXY2tab(Tnormal, Tln, n) - moyX(Tnormal, n) * moyY(Tln, n)) /
      (moyXcar(Tnormal, n) - carmoyX(Tnormal, n));
  float temp = moyenneYln - (a * moyenneX);
  b = exp(1) * exp(temp);

  /* pas sûr si a va dans p1 ou p0 */
  P->p[1] = a;
  P->p[0] = b;
  return P;
}

polynome *puissance(Liste listedepoint)
{
  polynome *P = creePolynome(1);
  int n = ListLenght(listedepoint);
  float **Tnormal = ListeToTabsPoints(listedepoint);
  float **Tln = lnListetotab(Tnormal, n);

  float moyenneXnl = moyX(Tln, n);
  float moyenneYnl = moyY(Tln, n);

  float a = 0;
  float b;
  b = (moyXY(Tln, n) - moyX(Tln, n) * moyY(Tln, n)) /
      (moyXcar(Tln, n) - carmoyX(Tln, n));

  float temp = moyenneYnl - (b * moyenneXnl);
  a = exp(temp);

  P->p[1] = b;
  P->p[0] = a;
  return P;
}
