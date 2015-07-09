/* 
DACparams tables inlcude hard coded params (safe operating limits and
post DAC amplifier scaling data), as well as a record ID to facilitate 
lookup of default settings from EEPROM
*/

/*DACParams is a lookup table used to map commands to DAC channels.
It also provides limits for new values... Scaler = 32768/Vref/gain*/

typedef struct DACParams{char chName[8];DAC_channel channel;float min;float max;float scaler;};
struct DACparams const biasParams[]= {
                                       {cb_Ppos,channel_00,-10,10,3276.8},//g=1
                                       {cb_Pneg,channel_01,-10,10,3276.8},                      
                                       {cb_DGpos,channel_02,-10,10,3276.8},
                                       {cb_DGneg,channel_03,-10,10,3276.6},
                                       {cb_Spos,channel_04,-10,10,3276.8},
                                       {cb_Sneg,channel_05,-10,10,3276.8},
                                       {cb_SWpos,channel_06,-10,10,3276.8},
                                       {cb_SWneg,channel_07,-10,10,3276.8},
                                       {cb_RGpos,channel_10,-10,10,3276.8},
                                       {cb_RGneg,channel_11,-10,10,3276.8},
                                       {cb_OG,channel_12,-10,10,3276.8},
                                       {cb_RD,channel_13,-15,0,3276.8/1.5},
                                       {cb_OD,channel_14,-23,0,3276.8/2.5},
                                       {cb_BB,channel_15,0,52,3276.8/5.7},
                                       {"\0",channel_16,0,0,3276.8},
                                       {"\0",channel_17,0,0,3276.8}
                                    };
                                 
struct DACparams const offsetParams[]= {
                                       {co_0pos,channel_00,-1,1,6553.6},
                                       {co_0neg,channel_01,-1,1,6553.6},
                                       {co_1pos,channel_02,-1,1,6553.6},
                                       {co_1neg,channel_03,-1,1,6553.6}, 
                                       {co_2pos,channel_04,-1,1,6553.6},
                                       {co_2neg,channel_05,-1,1,6553.6}, 
                                       {co_3pos,channel_06,-1,1,6553.6},
                                       {co_3neg,channel_07,-1,1,6553.6} 
                                    };

// default values (these will be copied to EEprom)
float rom biasDefRead[] =  {-5,3,5,5,-6,3,-6,5,-7.5,2,-4.5,-12,-20,30,0,0
                              -5,3,5,5,-6,3,-6,5,-7.5,2,-4.5,-12,-20,30,0,0}; 
                              
float rom biasDefExpose[] ={-5,3,5,5,-6,3,5,5,-7.5,-7.5,-4.5,5,5,45,0,0
                              -5,3,5,5,-6,3,5,5,-7.5,-7.5,-4.5,5,5,45,0,0}; 
                              
float rom biasDefWipe[] =  {-5,3,-6,-6,-6,-6,-6,5,-7.5,-7.5,-4.5,-12,-20,30,0,0
                              -5,3,-6,-6,-6,-6,-6,5,-7.5,-7.5,-4.5,-12,-20,30,0,0};
                              
float rom biasDefErase[] = {6,6,6,6,6,6,6,6,6,6,6,-12,-5,0.2,0,0
                              6,6,6,6,6,6,6,6,6,6,6,-12,-5,0.2,0,0};                              

// 32 channels for DAC0 and DAC1
float biasDef[32] = {6,6,6,6,6,6,6,6,6,6,6,-12,-5,0.2,0,0
                     6,6,6,6,6,6,6,6,6,6,6,-12,-5,0.2,0,0};


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
                                       {gv_24Vneg, N24V, -26, -22, -12/4095*2.048},    
                                       {gv_54Vpos, P54V, 25, 55, 30.85/4095*2.048}
                                    };   

//defines EEPROM start addresses for stored parameters                                       
typedef enum eeAddress {erase=0,read=128,wipe=256,expose=384,offset=512,osTest1=576,osTest2=640,osTest3=704,voltage=768};

void savePresetValues(eeAddress preAdd)
{
   int i;
   if(preAdd==offset||preAdd==osTest1||preAdd==osTest2||preAdd==osTest3) 
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
   if(preAdd==offset||preAdd==osTest1||preAdd==osTest2||preAdd==osTest3)
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
      for(i=0;i<16;i++)
      { 
         int chID = biasParams[i].channel;
         unsigned int16 value = 32768 +  (int16) (biasParams[i].scaler*biasDef[i]);
         setVoltage(value, cb0, chID);
      }
      for(i=0;i<16;i++)
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
   if ((int8)read_eeprom(1023)==0xFF) // only runs if EEprom empty
   {  
      
      rom char* ptr;
      int i;
      //save bias defaults
      printf("iniBiasErase");
      ptr = biasDefErase;
      for(i=0;i<128;i++)
      {
         write_eeprom(erase+i,ptr[i]);
      }
      printf("iniBiasRead");
      ptr=biasDefRead;
      for(i=0;i<128;i++)
      {
         write_eeprom(read+i,ptr[i]);
      }
      printf("iniBiasWipe");
      ptr=biasDefWipe;
      for(i=0;i<128;i++)
      {
         write_eeprom(wipe+i,ptr[i]);
      }
      printf("iniBiasExpose");
      ptr=biasDefExpose;
      for(i=0;i<128;i++)
      {
         write_eeprom(expose+i,ptr[i]);
      }
      // save offset defaults
      printf("iniBiasOffset");
      for(i=0;i<64;i++)
      {
         write_eeprom(offset+i,0);
      }
      for(i=0;i<64;i++)
      {
         write_eeprom(osTest1+i,0);
      }
      for(i=0;i<64;i++)
      {
         write_eeprom(osTest2+i,0);
      }
      for(i=0;i<64;i++)
      {
         write_eeprom(osTest3+i,0);
      }
      // save voltage calibration defaults
      printf("iniV-Scalers");
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
      write_eeprom(1023,0);
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

// gets the voltage for the specified channel
float getVoltage(int8 id)
{
   float scaler= getVscaler(id);
   float result = (float)getRawData(vParams[id].channel)*scaler;
   return(result);
}

