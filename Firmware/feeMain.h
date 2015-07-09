#IFNDEF _PFSmain
#DEFINE _PFSmain

#include <18F67K22.h>

#device adc=12
//#device ICD=TRUE

//#FUSES WDT                      //Watch Dog Timer
//#FUSES WDT64                    //Watch Dog Timer uses 1:64 Postscale
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES NOBROWNOUT               //No brownout reset

#use delay(internal=32000000,restart_wdt)
#use FIXED_IO( A_outputs=PIN_A7,PIN_A6,PIN_A5,PIN_A4 )
#use FIXED_IO( C_outputs=PIN_C6,PIN_C5,PIN_C3,PIN_C2,PIN_C0 )
#use FIXED_IO( D_outputs=PIN_D7,PIN_D6,PIN_D5,PIN_D4,PIN_D3,PIN_D2,PIN_D1,PIN_D0 )
#use FIXED_IO( E_outputs=PIN_E7,PIN_E6,PIN_E5,PIN_E4,PIN_E3,PIN_E2,PIN_E1,PIN_E0 )
#use FIXED_IO( G_outputs=PIN_G3,PIN_G2,PIN_G1 )
#define MON_N24V        PIN_A0
#define MON_P54V        PIN_A1
#define MON_N5Vpa       PIN_A2
#define MON_P5Vpa       PIN_A3
#define EN_P3V3         PIN_A4
#define TP_5            PIN_A5
#define TP_2            PIN_A6
#define TP_3            PIN_A7
#define _DAC_BUSY       PIN_B0
#define _WAKE           PIN_B1
#define STAT_VBB_AMP0   PIN_B2
#define STAT_VBB_AMP1   PIN_B3
#define LED             PIN_B4
#define RB5             PIN_B5
#define PGC             PIN_B6
#define PGD             PIN_B7
#define EN_RS232_DRV    PIN_C0
#define _DAC_PARITY_ERR PIN_C1
#define _DAC_RST        PIN_C2
#define SCK1            PIN_C3
#define SDI1            PIN_C4
#define SDO1            PIN_C5
#define TXD             PIN_C6
#define RXD             PIN_C7
#define ADC_TEMP_START  PIN_D0
#define _ADC_TEMP_CS    PIN_D1
#define _DAC_BIAS0_CS   PIN_D2
#define DAC_BIAS0_LOAD  PIN_D3
#define _DAC_BIAS1_CS   PIN_D4
#define DAC_BIAS1_LOAD  PIN_D5
#define DAC_CDS_LOAD    PIN_D6
#define _DAC_CDS_CS     PIN_D7
#define EN_12V          PIN_E0
#define EN_5V           PIN_E1
#define EN_24V          PIN_E2
#define EN_54V          PIN_E3
#define EN_PA           PIN_E4
#define EN_LVDS         PIN_E5
#define EN_54V_AMP0     PIN_E6
#define EN_54V_AMP1     PIN_E7
#define MON_N12V        PIN_F1
#define MON_P12V        PIN_F2
#define MON_N5V         PIN_F3
#define MON_P5V         PIN_F4
#define MON_PREAMP_TEMP PIN_F5
#define MON_P2V5        PIN_F6
#define MON_P3V3        PIN_F7
#define ADC_BIAS_CNV    PIN_G0
#define FAST            PIN_G1
#define TP_9            PIN_G2
#define _EN_CLKS        PIN_G3
#define MON_P3V3_MICRO  PIN_G4

//#use RS232(baud=9600,parity=N,xmit=TXD,rcv=RXD,bits=8,stream=PORT1,restart_wdt)

#define DELAY 1000


/*****************************************************************************/
/* MACRO DEFINITIONS *********************************************************/
/*****************************************************************************/

#define SPI_DeselectAll                            \
                        output_high(ADC_BIAS_CNV); \
                        output_high(_ADC_TEMP_CS); \
                        output_high(_DAC_BIAS0_CS);\
                        output_high(_DAC_BIAS1_CS);\
                        output_high(_DAC_CDS_CS)
                        
#define SPI_Select_BiasADC                         \
                        SPI_DeselectAll;           \
                        output_low(ADC_BIAS_CNV)  
                        
#define SPI_Select_TempADC                         \
                        SPI_DeselectAll;           \
                        output_low(_ADC_TEMP_CS)  

#define SPI_Select_Bias0DAC                        \
                        SPI_DeselectAll;           \
                        output_low(_DAC_BIAS0_CS)  

#define SPI_Select_Bias1DAC                        \
                        SPI_DeselectAll;           \
                        output_low(_DAC_BIAS1_CS)                         
                   
#define SPI_Select_CDSDAC                          \
                        SPI_DeselectAll;           \
                        output_low(_DAC_CDS_CS)  
                        
#ENDIF
