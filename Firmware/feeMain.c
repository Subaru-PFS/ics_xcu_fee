#define firmwareRev "PFS FEE Rev 20170613-02, copyright JHU\r\n"
#define eeHasBeenInitialized 0x00   // if there is a change in eeProm 
#define  eeHasNotBeenInitialized 0xFF   /* if there is a change in eeProm 
data locations and the EEProm must be re written then this chr must be changed.
Note, never us 0xFF!...*/

#include <feeMain.h>

#define DISABLE_3V3_POWER_CYCLE

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
   //#fill_rom 0xFF
#endif

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
   // EXT1 wakes the processor from sleep mode
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
   setup_wdt(WDT_ON);
   // Initialize with all outputs off...
   output_low(EN_RS232_DRV);
   output_low(EN_12V);  
   output_high(_EN_CLKS);
   output_low(EN_5V);          
   output_low(EN_24V);
   output_low(EN_54V);
   output_low(EN_54V_AMP0);  
   output_low(EN_54V_AMP1); 
   output_low(EN_LVDS);
   output_low(EN_PA); 
   output_low(EN_P3V3);
   output_low(_DAC_RST); // reset all DACs / ADC
   output_high(_DAC_RST);
   output_low(FAST);
#ifdef DISABLE_3V3_POWER_CYCLE
   output_high(EN_P3V3);
   initialize7869();
#endif   
   initializeComms();
   initializeADC();
   //initialize7869();
   restart_wdt();
   //delay_ms(1000);
   initializeEEProm();
   restart_wdt();
   printf(firmwareRev);
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
