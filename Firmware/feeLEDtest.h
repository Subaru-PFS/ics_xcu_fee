#include <18F67K22.h>

#device ICD=TRUE
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES NOBROWNOUT               //No brownout reset

#use delay(internal=32000000)
#use rs232(baud=9600, xmit=PIN_C6,rcv=PIN_C7)

#define LED PIN_B4

//Voltage Pins
#define en5V PIN_E1
#define en12V PIN_E0
#define en3_3V PIN_A4
#define en24V PIN_E2
#define en54V PIN_E3
#define enPA PIN_E4
#define en4540 PIN_E6
#define en4541 PIN_E7
#define enLVDS PIN_E5
#define enRS232 PIN_C0

#define csDAC0 PIN_D2
#define ldDAC0 PIN_D3
#define csDAC1 PIN_D4
#define ldDAC1 PIN_D5
#define csDAC2 PIN_D7
#define ldDAC2 PIN_D6
#define cs1247 PIN_D1
#define st1247 PIN_D0
#define cnv7689 PIN_G0


#define DELAY 1000


