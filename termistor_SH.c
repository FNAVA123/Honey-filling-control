#ifndef _termistor_SH_C_S
#define _TERMISTOR_C_

#include <math.h>

#define c1 0.7319465017e-03
#define c2 2.141258451e-04
#define c3 1.027877580e-07

static int32 Rdiv=0;

void SetResTermistor(int R){
   Rdiv=R*1000;
}

float Temperatura(int16 adc){
   
   volatile float voltaje_adc=0.0, R2=0.0, Temp=0.0;
   
   voltaje_adc=adc*(5.0/1023.0);
   R2 = (Rdiv*voltaje_adc)/(5-voltaje_adc);
   
   Temp = ((1.0 / (c1 + c2*log(R2) + c3*log(R2)*log(R2)*log(R2))))-273;    // ecuacion S-H / Conversión a de Kelvin a Celsius
   
   return Temp;
}

#endif
