#include <feeLEDtest.h>
#define SERIAL_NUM_LOCATION 0x2C
#define USEBOOTLOADER
#ifdef USEBOOTLOADER
   /* ------------------------------------------------------------------------- */
   /* map reset vector and interrupt vector                                     */
   /* 0x000-0x3FF is used by the bootloader. The bootloader maps the original   */
   /* reset vector (0x000) to 0x400 and the interrupt vector (0x008) to 0x408.  */
   /* ------------------------------------------------------------------------- */
   #build (reset=0x400, interrupt=0x408)
   /* ------------------------------------------------------------------------- */
   /* reserve boot block area                                                   */
   /* This memory range is used by the bootloader, so the application must not  */
   /* use this area.                                                            */
   /* ------------------------------------------------------------------------- */
   #org 0,0x3FF {}
#endif


#ORG default

#define SN 0x2A
const char string[20] = {"ADCD"}; 

#ORG 0x450,0x500
void printer()
{
   printf(string);
}

#ORG default
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
      int32 serial = 0;
      int32 *ptr = &serial;
      read_program_memory(0x2c, ptr, 4);
      printf("%Lu\r\n",serial);
      printf("%s\r\n",string);
   }
}

