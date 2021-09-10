#include <windows.h>
#include <cstdio>
#include <math.h>
////sound() => Beep()
////delay() => Sleep()
//int freq[]={523,587,659,698,784,880,988,1046};
//POW(1.06, -3 ~  )

#define DO(t) Beep((DWORD)(131*pow(1.06f,0)),(DWORD)(300*(t)));\
Sleep(100);
#define RE(t) Beep(131*pow(1.06f,2),300*(t));Sleep(100);
#define MI(t) Beep(131*pow(1.06f,4),300*(t));\
Sleep(100);
#define SO(t) Beep(131*pow(1.06f,7),300*(t));\
Sleep(100);
#define RA(t) Beep(131*pow(1.06f,9),300*(t));\
Sleep(100);

#define TBASIS_PI			((FLOAT)  3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (TBASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / TBASIS_PI))

#define A(t)    if(t) { 
#define E    }

#define StartGame void main() {

#define X int a=1;
#define P(r)printf(r);

