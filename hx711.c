/************************************************************************************** 
*   hx711.c - communicates with a HX711                                               * 
*                                                                                     *
*   by Sergio Andres Casta�o Giraldo                                                  *
*   website: https://controlautomaticoeducacion.com/                                  *
*   YouTube Chanel: https://www.youtube.com/channel/UCdzSnI03LpBI_8gXJseIDuw          *
**************************************************************************************/ 



#ifndef DOUT 
   #define DOUT       PIN_B6          //input - data  
#endif 

#ifndef SCK 
   #define SCK       PIN_B7         // output clock
#endif 

unsigned int32 OFFSET=0;                // used for tare weight
float          SCALE = 1;               // used to return weight in grams, kg, ounces, whatever
int            GAIN = 1;

void set_scale(float scale1){
     SCALE = scale1;
}

void set_offset(int32 offset1) {
     OFFSET = offset1;
}

unsigned int32 hx711_read()              
{ 
   int8 i; 
   unsigned int32 data=0; 

   output_low(SCK);  
   delay_ms(100);
   while(input(DOUT) == 1);
   for (i=0;i<24;i++){ 
      output_high(SCK); 
      delay_us(1); 
      output_low(SCK); 
      delay_us(1);
      shift_left(&data,3,input(DOUT));   
   } 
   
   for (i=0;i<GAIN;i++){ 
        output_high(SCK);
        delay_us(1);
        output_low(SCK);
        delay_us(1);
   }
   data = data ^ 0x800000;
   return(data);
} 

void set_gain(int gain1) {
     switch (gain1) {
          case 128:          // channel A, gain factor 128
               GAIN = 1;
               break;
          case 64:          // channel A, gain factor 64
               GAIN = 3;
               break;
          case 32:          // channel B, gain factor 32
               GAIN = 2;
               break;
     }

     output_low(SCK);
     hx711_read();
}


void init_hx(int gain1) 
{ 
   output_low(SCK);    //Output
   output_high(DOUT);  //Input
   set_gain(gain1);
} 

int32 get_average(int times)              
{
     unsigned int32 sum = 0;
     for (int i=0;i<times;i++){
          sum = sum + hx711_read();
          //delay_ms(1);
     }
     return(sum / times);
}

float get_value(int times)              
{
     float value;
     unsigned int32 read = get_average(times);
     if(OFFSET >= read){
          value =  (OFFSET - read);
          value = -1 * value; 
     }
     else     
          value = read - OFFSET;
     return(value);
}

float get_units(int times) {
     return get_value(times) / SCALE;
}

void tare(int times){
     int32 value = get_average(times);
     set_offset(value);
}

void power_down() {
     output_low(SCK);
     output_high(SCK);
}

void power_up() {
     output_low(SCK);
}

