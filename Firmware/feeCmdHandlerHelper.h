
/*
DACparams tables inlcude hard coded params (safe operating limits and
post DAC amplifier scaling data), as well as a record ID to facilitate 
lookup of default settings from EEPROM
*/

/*DACParams is a lookup table used to map commands to DAC channels.
It also provides limits for new values... Scaler = 32768/Vref/gain*/
#include <AD5360.h>
#include <AD7689.h>
#define CDSmVFullScale 582.91742
#define CDSmVHalfScale CDSmVFullScale/2



typedef struct DACParams{char chName[8];DAC_channel channel;float min;float max;float scaler; int8 ADCCh0;int8 ADCCh1;};
struct DACparams const biasParams[]= {
                                       {cb_Ppos,channel_00,-10,10,3276.8,0,0},//g=1
                                       {cb_Pneg,channel_01,-10,10,3276.8,0,0},                      
                                       {cb_DGpos,channel_02,-10,10,3276.8,0,0},
                                       {cb_DGneg,channel_03,-10,10,3276.8,0,0},
                                       {cb_Spos,channel_04,-10,10,3276.8,0,0},
                                       {cb_Sneg,channel_05,-10,10,3276.8,0,0},
                                       {cb_SWpos,channel_06,-10,10,3276.8,0,0},
                                       {cb_SWneg,channel_07,-10,10,3276.8,0,0},
                                       {cb_RGpos,channel_10,-10,10,3276.8,0,0},
                                       {cb_RGneg,channel_11,-10,10,3276.8,0,0},
                                       {cb_OG,channel_12,-10,10,3276.8,0,0},
                                       {cb_RD,channel_13,-15,0,3276.8/1.5,1,5},
                                       {cb_OD,channel_14,-23,0,3276.8/2.5,2,6},
                                       {cb_BB,channel_15,0,52,3276.8/5.7,3,7},
                                       {"\0",channel_16,0,0,3276.8,0,0},
                                       {"\0",channel_17,0,0,3276.8,0,0}
                                    };
                                 
                                   
struct DACparams const offsetParams[]= {
                                       {co_0pos,channel_00,-580,580,32768/CDSmVFullScale,4,4}, //full scale count/ full scale mV
                                       {co_0neg,channel_01,-580,580,32768/CDSmVFullScale,4,4},
                                       {co_1pos,channel_02,-580,580,32768/CDSmVFullScale,4,4},
                                       {co_1neg,channel_03,-580,580,32768/CDSmVFullScale,4,4}, 
                                       {co_2pos,channel_04,-580,580,32768/CDSmVFullScale,4,4},
                                       {co_2neg,channel_05,-580,580,32768/CDSmVFullScale,4,4}, 
                                       {co_3pos,channel_06,-580,580,32768/CDSmVFullScale,4,4},
                                       {co_3neg,channel_07,-580,580,32768/CDSmVFullScale,4,4} 
                                    };

// default values (these will be copied to EEprom)
float rom biasDefRead[] =  {-5,3,5,5,-6,3,-6,5,-7.5,2,-4.5,-12,-20,30,0,0,
                             -5,3,5,5,-6,3,-6,5,-7.5,2,-4.5,-12,-20,30,0,0}; 
                              
float rom biasDefExpose[] ={3,5,5,-6,3,5,5,-7.5,-7.5,-4.5,5,-5,5,45,0,0,
                              3,5,5,-6,3,5,5,-7.5,-7.5,-4.5,5,-5,5,45,0,0}; 
                              
float rom biasDefWipe[] =  {-5,3,-6,-6,-6,-6,-6,5,-7.5,-7.5,-4.5,-12,-20,30,0,0,
                             -5,3,-6,-6,-6,-6,-6,5,-7.5,-7.5,-4.5,-12,-20,30,0,0};
                              
float rom biasDefErase[] = {6,6,6,6,6,6,6,6,6,6,6,-12,-5,0.2,0,0,
                              6,6,6,6,6,6,6,6,6,6,6,-12,-5,0.2,0,0};  
                              
//float rom biasDefTest[] =  {-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//                              -5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
                              
float rom biasDefIdle[] =   {-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              -5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

float rom biasDefFastRev[] ={-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              -5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                              
// 32 channels for DAC0 and DAC1
float biasDef[32] = {6,6,6,6,6,6,6,6,6,6,-12,-5,0.2,0,0,0,
                     6,6,6,6,6,6,6,6,6,6,-12,-5,0.2,0,0,0};


// 16 channels for DAC2
float offsetDef[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

typedef struct ADCparams{char chName[8];ADC_channel channel;float min;float max;float defScaler;};
struct ADCParams const vParams[] =  {
                                       {gv_3V3Micro,P3V3m,2.8,3.8,1.68/4095*2.048},      
                                       {gv_3V3Other, P3V3, 2.8, 3.8, 1.68/4095*2.048},
                                       {gv_5Vpos, P5V, 4.5, 5.5, 3.49/4095*2.048},      
                                       {gv_5Vneg, N5V, -5.5, -4.5, -2.55/4095*2.048},        
                                       {gv_5Vpos_pa, P5Vpa, 4.5, 5.5, 3.49/4095*2.048},    
                                       {gv_5Vneg_pa, N5Vpa, -5.5, -4.5, -2.55/4095*2.048},   
                                       {gv_12Vpos, P12V, 11, 13, 7.49/4095*2.048},    
                                       {gv_12Vneg, N12V, -13, -11, -6.19/4095*2.048},     
                                       {gv_24Vneg, N24V, -26, -22, -12.0/4095*2.048},    
                                       {gv_54Vpos, P54V, 25, 55, 30.85/4095*2.048}
                                    };   
typedef struct AD7689Params {float defOS; float defScaler;};
struct AD7689Params const vParams7689[]={           // offsets and scalers for each ADC channel
                                       {10.067,8.046*2.5/65535}, // offset & result in volts for all channels except CDS
                                       {0,-6.49*2.5/65535}, // scaler accounts for preamp gain and ADC counts
                                       {0,-11*2.5/65535},
                                       {0,25.875622*2.5/65535},
                                       {10.067*20,8.046*20*2.5/65535}, // offset & result in mV for CDS offsets
                                       {0,-6.49*2.5/65535},
                                       {0,-11*2.5/65535},
                                       {0,25.875622*2.5/65535}
                                    };


//defines EEPROM start addresses for stored parameters                                       
//typedef enum eeAddress {erase=0,read=128,wipe=256,expose=384,biasTest1=512,offset=640,osTest1=704,voltage=768,o7689=808,v7689=872,serADC=936,serPA0=940,serPA1=944,serCCD0=948,serCCD1=968};
typedef enum eeAddress {erase=0,read=128,wipe=256,expose=384,idle=512,fastRev=640,offset=768,voltage=832,o7689=872,v7689=904,serADC=936,serPA0=940,serCCD0=944,serCCD1=964,eeIniAdd=1023};

void savePresetValues(eeAddress preAdd)
{
   int i;
   //if(preAdd==offset||preAdd==osTest1) 
   if(preAdd==offset) 
   {
      for(i=0;i<64;i++)
      {
         write_eeprom(preAdd+i,*(((int8*)offsetDef) + i));
      }
   }
   else
   {
      for(i=0;i<128;i++)
      {
         write_eeprom(preAdd+i,*(((int8*)biasDef) + i));
      }
   }   
}

void loadPresetValues(eeAddress preAdd)
{
   int i;
   //if(preAdd==offset||preAdd==osTest1)
   if(preAdd==offset)
   {
      for(i=0;i<64;i++)
      {
         *(((int8*)offsetDef) + i)=read_eeprom(preAdd+i);
      }
      for(i=0;i<8;i++)
      { 
         int chID = offsetParams[i].channel;
         unsigned int16 value = 32768 + (int16) (offsetParams[i].scaler*offsetDef[i]);
         setVoltage(value, cdsOS, chID);
      }
      for(i=0;i<8;i++)
      { 
         int chID = offsetParams[i].channel;
         unsigned int16 value = 32768 +  (int16) (offsetParams[i].scaler*offsetDef[i+8]);
         setVoltage(value, cdsOS, chID+8);
      }
   }
   else
   {
      for(i=0;i<128;i++)
      {
         *(((int8*)biasDef) + i)=read_eeprom(preAdd+i);
      }
      /* channel 11 (RD) must be enabled before any of the other channels */
      int chID = biasParams[11].channel;
      unsigned int16 value = 32768 +  (int16) (biasParams[11].scaler*biasDef[11]);
      setVoltage(value, cb0, chID);
      value = 32768 +  (int16) (biasParams[11].scaler*biasDef[11+16]);
      setVoltage(value, cb1, chID);
      /* now the rest of the channels */
      
      for(i=0;i<11;i++)
      { 
         int chID = biasParams[i].channel;
         unsigned int16 value = 32768 +  (int16) (biasParams[i].scaler*biasDef[i]);
         setVoltage(value, cb0, chID);
      }
      for(i=12;i<16;i++)
      { 
         int chID = biasParams[i].channel;
         unsigned int16 value = 32768 +  (int16) (biasParams[i].scaler*biasDef[i]);
         setVoltage(value, cb0, chID);
      }
      for(i=0;i<11;i++)
      { 
         int chID = biasParams[i].channel;
         unsigned int16 value = 32768 +  (int16) (biasParams[i].scaler*biasDef[i+16]);
         setVoltage(value, cb1, chID);
      }
      for(i=12;i<16;i++)
      { 
         int chID = biasParams[i].channel;
         unsigned int16 value = 32768 +  (int16) (biasParams[i].scaler*biasDef[i+16]);
         setVoltage(value, cb1, chID);
      }
   }
}

// Stores default value in EEprom... these can be updated subsequently
void initializeEEProm()
{
   if ((int8)read_eeprom(eeIniAdd)==eeHasNotBeenInitialized) // only runs if EEprom empty
   {  
      
      rom char* ptr;
      int i;
      //save bias defaults
      printf("iniBiasErase\n");
      ptr = biasDefErase;
      for(i=0;i<128;i++)
      {
         write_eeprom(erase+i,ptr[i]);
      }
      printf("iniBiasRead\n");
      ptr=biasDefRead;
      for(i=0;i<128;i++)
      {
         write_eeprom(read+i,ptr[i]);
      }
      printf("iniBiasWipe\n");
      ptr=biasDefWipe;
      for(i=0;i<128;i++)
      {
         write_eeprom(wipe+i,ptr[i]);
      }
      printf("iniBiasExpose\n");
      ptr=biasDefExpose;
      for(i=0;i<128;i++)
      {
         write_eeprom(expose+i,ptr[i]);
      }
//!      printf("iniBiasTest");
//!      ptr=biasDefTest;
//!      for(i=0;i<128;i++)
//!      {
//!         write_eeprom(biasTest1+i,ptr[i]);
//!      }
      printf("iniBiasIdle\n");
      ptr=biasDefIdle;
      for(i=0;i<128;i++)
      {
         write_eeprom(idle+i,ptr[i]);
      }
      printf("iniBiasFastRev\n");
      ptr=biasDefFastRev;
      for(i=0;i<128;i++)
      {
         write_eeprom(fastRev+i,ptr[i]);
      }
      // save offset defaults
      printf("iniOffset\n");
      for(i=0;i<64;i++)
      {
         write_eeprom(offset+i,0);
      }
//!      printf("iniOffsetTest");
//!      for(i=0;i<64;i++)
//!      {
//!         write_eeprom(osTest1+i,0);
//!      }
      
      // save voltage calibration defaults
      printf("iniV-Scalers\n");
      float vScaler;
      char* vsPtr=&vScaler;
      int j;
      for(i=0;i<10;i++)
      {
         vScaler = vParams[i].defScaler;
         for(j=0;j<4;j++)
         {
            write_eeprom((voltage+(i*4)+j),vsPtr[j]);
         }   
      }
      //
      printf("ini7689-Offsets\n");
      
      for(i=0;i<8;i++)
      {
         vScaler = vParams7689[i].defOS;
         for(j=0;j<4;j++)
         {
            write_eeprom((o7689+(i*4)+j),vsPtr[j]);
         }   
      }
      //
      printf("ini7689-Scalers\n");
      for(i=0;i<8;i++)
      {
         vScaler = vParams7689[i].defScaler;
         for(j=0;j<4;j++)
         {
            write_eeprom((v7689+(i*4)+j),vsPtr[j]);
         }   
      }
      printf("iniSerialNums\n");
      write_eeprom(serPA0,0);
      write_eeprom(serCCD0,'\0');
      write_eeprom(serCCD1,'\0');        
          
      write_eeprom(eeIniAdd,eeHasBeenInitialized);// set last address... Used to show EE ini'ed
   }
   
}

int8 getBiasRecordIndex(char*Param)
{
   int i;
   for(i=0;i<16;i++)
   {
      char test[8];
      strcpy(test,biasParams[i].chName);
      if(strcmp(test, Param)==0)
      {
         return i;
      }
   }
   char test[8];
   strcpy(test,cb_all);
   if(strcmp(test, Param)==0)
   {
      return allRecords;
   }
   return noRecordFound;
}

int8 getOSRecordIndex(char*Param)
{
   int i;
   for(i=0;i<8;i++)
   {
      char test[8];
      strcpy(test,offsetParams[i].chName);
      if(strcmp(test, Param)==0)
      {
         return i;
      }
   }
   char test[8];
   strcpy(test,co_all);
   if(strcmp(test, Param)==0)
   {
      return allRecords;
   }
   return noRecordFound;
}

int8 getMenuIndex(char*Param,rom char*menu, int size=8)
{
   int i = 0;
   int j = 0;
   while(menu[0] !='\0')
   {
      char test[8];
      
      for(j=0;j<size;j++) // copy test string to ram
      {
         test[j]=menu[j];
      }
      
      if(strcmp(test,Param)==0) // compare test string with passed Param
      {
         return i;
      }
      i++;
      menu+=size;
   }
   return noRecordFound;
}  

// search for the voltage record index

int8 getVoltageRecordIndex(char*Param)
{
   int i;
   for(i=0;i<10;i++)
   {
      char test[8];
      strcpy(test,vParams[i].chName);
      if(strcmp(test, Param)==0)
      {
         return i;
      }
   }
   char test[8];
   strcpy(test,gv_all);
   if(strcmp(test, Param)==0)
   {
      return allRecords;
   }
   return noRecordFound;
}

// get the scaler value from EEprom
float getVscaler(int8 id)
{
   int16 address = voltage + (id*4);
   float result=0;
   int8* ptr=&result;
   int8 i;
   for(i=0;i<4;i++)
   {
      ptr[i]=read_eeprom(address+i);
   }
   return result;
}

// get the offset value from EEprom
float get7689offset(int8 id)
{
   int16 address = o7689 + (id*4);
   float result=0;
   int8* ptr=&result;
   int8 i;
   for(i=0;i<4;i++)
   {
      ptr[i]=read_eeprom(address+i);
   }
   return result;
}

// get the scaler value from EEprom
float get7689scaler(int8 id)
{
   int16 address = v7689 + (id*4);
   float result=0;
   int8* ptr=&result;
   int8 i;
   for(i=0;i<4;i++)
   {
      ptr[i]=read_eeprom(address+i);
   }
   return result;
}

// update the scaler value in EEprom based on a true measured value
void updateVscaler(float actualVoltage, int8 id)
{
   float result = actualVoltage/(float)getRawData(vParams[id].channel);
   char* ptr=&result;
   int16 address = voltage + (id*4);
   int8 i;
   for(i=0;i<4;i++)
   {
      write_eeprom(address+i,ptr[i]);
   }
}

// update the offset value in EEprom based on a true measured value
void update7689offset(float zeroOS, int8 id)
{
   char* ptr=&zeroOS;
   int16 address = o7689 + (id*4);
   int8 i;
   for(i=0;i<4;i++)
   {
      write_eeprom(address+i,ptr[i]);
   }
}

// update the scaler value in EEprom based on a true measured value
void update7689scaler(float scaler, int8 id)
{
   char* ptr=&scaler;
   int16 address = v7689 + (id*4);
   int8 i;
   for(i=0;i<4;i++)
   {
      write_eeprom(address+i,ptr[i]);
   }
}

// gets the voltage for the specified channel
float getVoltage(int8 id)
{
   float scaler= getVscaler(id);
   float result = (float)getRawData(vParams[id].channel)*scaler;
   return(result);
}

// calibrate the low voltage 7689 bias channel
// calibrate the low voltage 7689 bias channel
void cal7689LVBiasChannel()
{
   unsigned int8 channel=0; //
   float valRef = 0;
   float val5V = 0;
   int i=0;
   setDACmux(cB0, 15); // set the dac channel to an unused channel
   setVoltage(65536/2, cb0, channel_17); // OV last channel set to zero volts
   delay_ms(100);
   deselectAllSPI();
   for (i = 0;i<5;i++)
   {
      valRef += get7689Voltage(channel, 1, 0);
   }
   valRef/=5; //calculate filtered OV count
   
   setVoltage(65536/4*3, cb0, channel_17); // last channel set to 5V
   deselectAllSPI();
   for (i = 0;i<5;i++)
   {
      val5V += get7689Voltage(channel, 1, 0);
   }
   val5V/=5; // calculate filtered 5V count
   
   float scaler = 5/(val5V-ValRef);
   float offset = scaler*valRef;
   
   update7689offset(offset, 0);
   update7689scaler(scaler, 0);  
   
   // reset offets to last loaded CDS values
  
}

// calibrate the low voltage 7689 bias channel
void cal7689LVCDSChannel()
{
   unsigned int16 channel=4;
   channel*=INx_ch1; // this coverts a int (0  to 7) to real channel ID
   unsigned int16 data= (channel | CFG_overwrite | INCC_uniRefGND  | BW_quarter | REF_int2V5 | SEQ_disabled | RB_readDataOnly);
   float valRef = 0;
   float valHalfScale = 0;
   int i=0;
   setDACmux(cdsOS, 15); // 
   
   setVoltage(65536/2, cdsOS, channel_17); // OV last channel set to zero volts
   deselectAllSPI();
   for (i = 0;i<5;i++)
   {
      valRef += (float) AD7689_readData(data);
   }
   valRef/=5; //calculate filtered OV count
   
   setVoltage(65536/4*3, cdsOS, channel_17); // last channel set to 5V
   deselectAllSPI();
   for (i = 0;i<5;i++)
   {
      valHalfScale += (float) AD7689_readData(data);
   }
   valHalfScale/=5; // calculate filtered 5V count
   
   float scaler = CDSmVHalfScale/(valHalfScale-ValRef);
   float offset = scaler*valRef;
   
   update7689offset(offset, 4);
   update7689scaler(scaler, 4);  
   
   
   // reload the offset values
   for(i=0;i<8;i++)
   { 
      int chID = offsetParams[i].channel;
      unsigned int16 value = 32768 + (int16) (offsetParams[i].scaler*offsetDef[i]);
      setVoltage(value, cdsOS, chID);
   }
   for(i=0;i<8;i++)
   { 
      int chID = offsetParams[i].channel;
      unsigned int16 value = 32768 +  (int16) (offsetParams[i].scaler*offsetDef[i+8]);
      setVoltage(value, cdsOS, chID+8);
   }

}

// get serial numbers form EEProm
unsigned int32 getSerNum(int16 addr)
{
   unsigned int32 result = 0;
   int8 *ptr=&result;
   int16 i=0;
   for(i=0;i<4;i++)
   {
      ptr[i] = read_eeprom (addr+i);;
   }
   return result;
}

void getCCDserial(int16 addr, char* serialNumber)
{
   int8 i=0;
   for(i=0;i<19;i++)
   {
      serialNumber[i]=read_eeprom (addr+i);
   }
}

// save serial numbers to EEprom
void setSerNum(int16 addr, unsigned int32 sn)
{
   int8 *ptr=&sn;
   int8 i=0;
   for(i=0;i<4;i++)
   {
      write_eeprom (addr+i, ptr[i]);
   }
}

void setCCDserial(int16 addr, char* serialNumber)
{
   int8 i=0;
   for(i=0;i<18;i++)
   {
      write_eeprom (addr+i, serialNumber[i]);
   }
   write_eeprom (addr+i+1, '\0');
}


void clear_all_bias(dacID)
{
   int i;
   for(i=1;i<=10;i++)
   {
      int chID = biasParams[i].channel;
      biasDef[i+(dacID*16)]=0; // poweroff (0V)
      setVoltage(32764, dacID, chID);
   }
   for(i=12;i<=15;i++)
   {
      int chID = biasParams[i].channel;
      biasDef[i+(dacID*16)]=0; // poweroff (0V)
      setVoltage(32764, dacID, chID);
   }
   int chID = biasParams[11].channel; // power off RD last
   biasDef[11+(dacID*16)]=0; // poweroff (0V)
   setVoltage(32764, dacID, chID);
}


///////////////////
/***************************************************************************** 
enable LVDS, enable Clock, and enableFast require 12V to be enabled...
The following routines use flags to maintain the desired states...
******************************************************************************/


int1 enClkFlag = 0;
int1 fastFlag = 0;
int1 enLVDSFlag = 0;

clrFastClksLVDS()
{  
   output_low(_EN_CLKS);
   output_low(FAST);
   output_low(EN_LVDS);
}

setFastClksLVDS()
{
   if(input_state(EN_12V))
   {
      output_bit(_EN_CLKS,!enClkFlag);
      output_bit(FAST,fastFlag);
      output_bit(EN_LVDS,enLVDSFlag);    
   }
   else
   {
      output_low(_EN_CLKS);
      output_low(FAST);
      output_low(EN_LVDS);
   }  
}

typeDef enum clkEnStates {enOn = '1', enOff = 'x', disOn = '0', disOff = 'o'};
char getEnClks()
{  
   if(input_state(EN_12V))
   {
      if (input_state(_EN_CLKS))
      {
         return('0');
      }
      else
      {
         return('1');
      }
   }
   else
   {
      if (enClkFlag)
      {
         return('x');
      }
      else
      {
         return('o');
      }
   }  
}

char getEnLVDS()
{  
   if(input_state(EN_12V))
   {
      if (input_state(EN_LVDS))
      {
         return('1');
      }
      else
      {
         return('0');
      }
   }
   else
   {
      if (enClkFlag)
      {
         return('x');
      }
      else
      {
         return('o');
      }
   }  
}

typedef enum fastState {fastOn, slowOn, fastOff, slowOff};
char getFastMode()
{
   if(input_state(EN_12V))
   {
      if (input_state(FAST))
      {
         return(fastOn);
      }
      else
      {
         return(slowOn);
      }
   }
   else
   {
      if (fastFlag)
      {
         return(fastOff);
      }
      else
      {
         return(slowOff);
      }
   }  
}

char setFastMode()
{
   fastFlag = 1;
   setFastClksLVDS();
   return getFastMode();
}

char clrFastMode()
{
   fastFlag = 0;
   setFastClksLVDS();
   return getFastMode();
}

char setEnClks()
{
   enClkFlag = 1;
   setFastClksLVDS();
   return getEnClks();
}

char clrEnClks()
{
   enClkFlag = 0;
   setFastClksLVDS();
   return getEnClks();
}

char setEnLVDS()
{
   enLVDSFlag = 1;
   setFastClksLVDS();
   return getEnLVDS();
}

char clrEnLVDS()
{
   enLVDSFlag = 0;
   setFastClksLVDS();
   return getEnLVDS();
}
