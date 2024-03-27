#include <16F877A.h>
#DEVICE ADC=10
#fuses XT, NOWDT
#use delay (clock = 4 MHz)
#include <lcd_d.c>
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7)  
#include "termistor_SH.c"
 //SENSOR ULTRA 
#BYTE TRISA = 0x85      
#BYTE PORTA = 0x05
#BYTE T1CON = 0X10
#BYTE TMR1H = 0X0F
#BYTE TMR1L = 0X0E
#bit PINA1 = PORTA.1
#bit PINA2 = PORTA.2
#include "DISTANCIA_LIBRERIA.c"
//GALGA
#include "hx711.c"
//SERVO
#BYTE TRISE = 0x89
#BYTE PORTE = 0x09
//LEDS
#BYTE TRISC = 0x87
#BYTE PORTC = 0x07
#bit PINN_C0 = PORTC.0
#bit PINN_C1 = PORTC.1
#bit PINN_C2 = PORTC.2
//MOTOR
#bit PINN_C3 = PORTC.3
float temp,D,VARIABLE_CONTROL;
int8 referencia=50;
float OUTPUT;
float TEMP_PROMEDIOS(int16 CANTIDAD_DE_PROMEDIOS);
VOID ABRIR_Y_CERRAR(int16 ancho);
float pid();
int16 map(float value, double in_min, double in_max, double out_min, double out_max);
char inicio='B';
#int_rda
void recibir(){
INICIO=getc();
PINN_C2=0;
PINN_C0=1;
PINN_C1=0;
pinn_c3=1;

}
VOID MAIN(){
   TRISE=0X05;
   TRISC=10010000;
   PORTC=0;
   PORTE=0X00;
   init_hx(128);
   lcd_init();
   INICIALIZAR_LIBRERIA();
   setup_adc(ADC_CLOCK_INTERNAL); 
   setup_adc_ports(AN0);
   set_adc_channel(0);
   set_scale(213.5);  
   TARE(20);
   PINN_C2=1;
   PINN_C1=0;
   PINN_C0=0;
   PINN_C3=0;
   enable_interrupts(INT_RDA);//INTERRUPCION BLUETOOH
   enable_interrupts(GLOBAL);
                                                           //1800 //850a-1800c
WHILE(1){
VARIABLE_CONTROL=get_units(20);
 if(variable_control>=52){
 variable_control=53;
 }     
if(INICIO=='A'){                                                           //LECTURA DE DATOS
output=map(pid(),0, 90, 2100, 1650);
ABRIR_Y_CERRAR(output);

}


d=abs(18-obtener_distancia());
temp=TEMP_PROMEDIOS(20)-66.31;


IF(VARIABLE_CONTROL<0){
VARIABLE_CONTROL=0;
}



lcd_GOTOXY(1,1);
printf(lcd_putc," %f %f \n%c",VARIABLE_CONTROL,output,inicio);

PUTC('I');
printf("%f",temp);
PUTC('N');
printf("%f",D);
PUTC('B');
printf("%f",VARIABLE_CONTROL);
PUTC('F');
printf("\f\n");
delay_ms(50);
//!
  
}
}

float TEMP_PROMEDIOS(int16 CANTIDAD_DE_PROMEDIOS){
FLOAT tempe_promedios=0,TEMP;
int16 valor_adc=0;
unsigned R2=10;
   for(int i=0; i<=CANTIDAD_DE_PROMEDIOS;i++){
   SetResTermistor(R2);
   delay_us(20);  
   valor_adc=read_adc(); 
   TEMP=Temperatura(read_adc());
   tempe_promedioS+=TEMP;
   }
   return (float)tempe_promedios/CANTIDAD_DE_PROMEDIOS;
}

void ABRIR_Y_CERRAR(int16 ancho){
    for(int i = 0; i<=10;i++) {
        PORTE=0X02;
        delay_us(ancho);
        PORTE=0X00;
        delay_us(20000-ancho);
    }
}

float pid() {

   float error,kp=4,ki=0.01,kd=0,ap,ai,ad,tm=0.01,output;
   static float errorant=0;
   
     error = REFERENCIA - variable_control;
     ap = kp * error;
     ai = ki * ((tm * errorant) + (tm * (error - errorant)) / 2);
     ad = kd * ((error - errorant) / tm);
     output += ap + ai + ad;
     
    IF(output<0 || error<=0){
    output=0;
    }
    IF(output>90){
    output=90;
    }
    if(variable_control<10){
     output=0;
     PINN_C2=0;
     PINN_C0=1;
     PINN_C1=0;
     PINN_C3=1;
    }
     if(variable_control>10){
        PINN_C2=0;
        PINN_C0=0;
        PINN_C1=1;
        PINN_C3=0;
    }
   
     errorant=error;
     
   
   return output;
}
int16 map(float value, double in_min, double in_max, double out_min, double out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
