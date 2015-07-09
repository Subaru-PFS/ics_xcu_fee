#IFNDEF _fee_adc
#DEFINE _fee_adc

#define Vref (float)2.0176


// Scaler factor = 2.048/4096 * 1/gain... result = count*scaler
//Scaler --- unit scale factor... Volts= count/scaler
/*const float scaler[17]={-166.6260,    //Negative 24V scale factor
                        64.8124,    // Positive 54V scale factor
                        -784.1222,
                        573.3265,
                        0,
                        1189.4775,
                        -323.0229,
                        266.8671,
                        -784.1222,
                        573.3265,
                        1,
                        1,
                        0,
                        0,
                        0,
                        0,
                        1189.4775}; // 3.3V scale factor*/
                        
const float scaler[17]={-12,    //Negative 24V scale factor
                        30.85,    // Positive 54V scale factor
                        2.55,
                        3.48,
                        0,
                        1.681,
                        -6.19,
                        7.49,
                        -2.55,
                        3.488,
                        1,
                        1,
                        0,
                        0,
                        0,
                        0,
                        1.681}; // 3.3V scale factor
      
// Routines to read the ADC inputs and Evaluate data for PFS
// uses 4 ADC inputs

typedef enum  {N24V=0,
               P54V=1,
               N5Vpa=2,
               P5Vpa=3,
               P3V3=5,
               N12V=6,
               P12V=7,
               N5V=8,
               P5V=9,
               TH0=10,
               TH1=11,
               P3V3m=16}ADC_channel;

int16 getRawData(ADC_channel id)
{
   set_adc_channel(id);
   delay_us(20); // allow time for channel change
   return(read_adc());
}

//!float getVoltage(ADC_channel id)
//!{
//!   float engValue = (float)getRawData(id);
//!   engValue=engvalue*scaler[id]*Vref/4095;
//!   
//!   //engValue /= Scaler[id];
//!   return(engValue);
//!}

void initializeADC()
{
   setup_adc_ports(sAN0|sAN1|sAN2|sAN3|sAN5|sAN6|sAN7|sAN8|sAN9|sAN10|sAN11|sAN16,VSS_2V048);
   setup_adc(ADC_CLOCK_DIV_64|ADC_TAD_MUL_12); //set conversion time
}

#ENDIF
