#include <feeLEDtest.h>

void main()
{

   output_low(en5V);
   output_low(en12V);
   output_low(en3_3V);
   output_low(en24V);
   output_low(en54V);
   output_low(en4540);
   output_low(en4541);
   output_low(enPA);
   output_low(enLVDS);
   output_high(enRS232);  
   
   output_high(en3_3V);
   //output_low(en3_3V);
   
   
   
   output_low(csDAC0);
   output_high(csDAC0);
   
   output_low(ldDAC0);
   output_high(ldDAC0);
   
   output_low(csDAC1);
   output_high(csDAC1);
   
   output_low(ldDAC1);
   output_high(ldDAC1);
      
   output_low(csDAC2);
   output_high(csDAC2);
   
   output_low(ldDAC2);
   output_high(ldDAC2);
   
      
   output_low(cs1247);
   output_high(cs1247);
   
   output_low(st1247);
   output_high(st1247);
   
      
   output_low(cnv7689);
   output_high(cnv7689);
   
   //output_high(en3_3V);
   output_high(en5V);
   //output_low(en5V);
   
   output_high(en12V);
   //output_low(en12V);
   
 
   
   output_high(en24V);
   //output_low(en24V);
   
   
   //output_high(en54V);
   //output_low(en54V);
   
   //output_high(enPA);
   //output_low(enPA);
   
   
   //Example blinking LED program
   while(true)
   {
      printf("Hi There");
      output_low(LED);
      delay_ms(DELAY);
      printf("Bye There");
      output_high(LED);
      delay_ms(DELAY);
   }

}
