//NIVEL BY FN
void INICIAR_TIMER(){
 T1CON = 0x01;
 TMR1H=0x0;
 TMR1L=0x0;
}
int16 OBTENER_VALOR(){

return (int16) (TMR1H*256)+TMR1L ; 
}
void SET_TIMER1_0(){
 TMR1H=0x0;
 TMR1L=0x0;
}
void INICIALIZAR_LIBRERIA(){
 INICIAR_TIMER();
 TRISA =0XFD;
 }
float OBTENER_DISTANCIA(){
INT16 TIEMPO;
FLOAT DISTANCIA;
PINA1=0x01;
 delay_us(10);
 PINA1=0X0;
 WHILE(PINA2==0){}
  SET_TIMER1_0();
 WHILE(PINA2==1){}
 TIEMPO=OBTENER_VALOR();
 DISTANCIA=((tiempo/2)/29.1375);


RETURN (FLOAT) DISTANCIA;
 
 


}
