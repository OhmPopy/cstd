
#include <math.h>
typedef double (fun_dd_t)(double x);

int golds(fun_dd_t phi,double a, double b, double delta,double epsilon, double* s, double* phis, double* G, double* E) {
  double t, h, p, q, phia, phib, phip, phiq, ds, dphi;
  //����: 0.618����ȷ������
  //����: phi��Ŀ�꺯��, a, b ����������������˵�
  //         delta, epsilon�ֱ����Ա����ͺ���ֵ���������
  //���:  s, phis�ֱ��ǽ��Ƽ�С��ͼ�Сֵ,  G��nx4����,
  //         ���k�зֱ���a,p,q,b�ĵ�k�ε���ֵ[ak,pk,qk,bk],
  //          E=[ds,dphi], �ֱ���s��phis�������.
  int k = 0;
  t=(sqrt(5)-1)/2;  h=b-a;  phia=phi(a); phib=phi(b);
  p=a+(1-t)*h;  q=a+t*h; phip=phi(p); phiq=phi(q);
  if (G) {
    *G++ = a, *G++ = p, *G++ = q, *G++ = b;
  }
  while ((fabs(phib-phia)>epsilon)||(h>delta)) {
    if(phip<phiq) {
      b=q;  phib=phiq; q=p; phiq=phip;
      h=b-a; p=a+(1-t)*h;  phip=phi(p);
    } else {
      a=p; phia=phip;  p=q; phip=phiq;
      h=b-a;  q=a+t*h;  phiq=phi(q);
    }
    k=k+1;
    if (G) {
      *G++ = a, *G++ = p, *G++ = q, *G++ = b;
    }
  }
  ds=fabs(b-a); dphi=fabs(phib-phia);
  if(phip<=phiq) {
    *s=p;  *phis=phip;
  } else {
    *s=q;  *phis=phiq;
  }
  if (E) {
    *E++=ds, *E++=dphi;
  }
  return k;
}
