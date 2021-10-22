#include "ajuste.h"

polynome *renvoieDroitepuissance(a, b)
{
  polynome *P = creePolynome(1);
  P->p[1] = a;
  P->p[0] = b;
  return P;
}

void affichepuissance(polynome *p) { affichepolynome(p); }

polynome *exponentiel(Liste listedepoint)
{

  polynome *P = creePolynome(1);
  int n = ListLenght(listedepoint);

  float **Tnormal = ListeToTabsPoints(listedepoint);
  float **Tln = lnListe(Tnormal, n);

  float moyenneX = moyX(Tnormal, n);
  float moyenneYln = moyY(Tln, n);
  float a;
  float b;

  /* pas bon       ici */
  P->p[1] = (moyXY(Tln, n) - moyX(Tnormal, n) * moyY(Tln, n)) /
            (moyXcar(Tnormal, n) - carmoyX(Tnormal, n));
  float temp = moyenneYln - (P->p[1] * moyenneX);
  P->p[0] = exp(1) * exp(temp);
  return P;
}

polynome *puissance(Liste listedepoint)
{
  polynome *P = creePolynome(1);
  int n = ListLenght(listedepoint);
  float **Tnormal = ListeToTabsPoints(listedepoint);
  float **Tln = lnListe(Tnormal, n);

  float moyenneXnl = moyX(Tln, n);
  float moyenneYnl = moyY(Tln, n);

  float a = 0;
  float b;
  b = (moyXY(Tln, n) - moyX(Tln, n) * moyY(Tln, n)) /
      (moyXcar(Tln, n) - carmoyX(Tln, n));

  a = log(moyenneYnl - (b * moyenneXnl));

  P->p[1] = a;
  P->p[0] = b;
  return P;
}
