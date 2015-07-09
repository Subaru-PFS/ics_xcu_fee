#define firmwareRev "PFS BEE Rev-, copyright JHU\r\n"

#include <feeMain.h>
#include <string.h>

#include <feeComms.c>
#include <feeADC.h>
#include <feeSPI.h>

//#USE SPI (MASTER, SPI1, MODE=0, BITS=8, STREAM=SPI_1)

#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#INT_EXT1
void  EXT1_isr(void) 
{
   // EXT1 wakes te processor from sleep mode
}

#INT_AD
void  AD_isr(void) 
{

}

//!#INT_RDA
//!void  RDA_isr(void) 
//!{
//!
//!}
//! 

void main()
{
   // Initialize with all outputs off...
   output_low(EN_RS232_DRV);
   output_low(EN_12V);        
   output_low(EN_5V);          
   output_low(EN_24V);
   output_low(EN_54V);
   output_low(EN_PA); 
   output_low(EN_P3V3);
   output_low(_DAC_RST); // reset all DACs / ADC
   output_high(_DAC_RST);
   output_low(FAST);
   
   initializeComms();
   initializeADC();
   //delay_ms(1000);
   initializeEEProm();
   //enable_interrupts(INT_EXT1);
   //enable_interrupts(INT_AD); 
   //enable_interrupts(INT_RDA); see feeComms
   enable_interrupts(GLOBAL);

   
   
   while(true)
   {
      executeCmd();
      restart_wdt();
   }
}
