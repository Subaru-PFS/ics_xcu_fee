#IFNDEF _fee_cmd
#DEFINE _fee_cmd

#include <feeCmdHandler.h>

#include <feeADC.h>
#include <feeSPI.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>


#include <feeCmdHandlerHelper.h>

char runstate[10]="\0";

/*****************************************************************************/
/* MENU AND SUBMENU LISTS USED BY COMMAND HANDLERS                           */
/*****************************************************************************/
enum mainMenu {sCB,gCB,rCB,sOS,gOS,rOS,lDAC,sDAC,gLBmode,sPower,gPower,sFast, gFast, gSupply,cSupply,gTemp,gRevision,gSerial,sSerial,calADC,gStatus,sReset,gIni,sIni,gRun,sRun};
char rom mainMenuItems[][8]=                   {setClockBias,
                                                getClockBias,
                                                rdClockBias,
                                                setCDS_OS,
                                                getCDS_OS,
                                                rdCDS_0S,
                                                loadDACPresets,
                                                saveDACPresets,
                                                getLastBiasMode,
                                                setPowerEn,
                                                getPowerEn,
                                                setFast,
                                                getFast,
                                                getSupplyVoltage,
                                                calSupplyVoltage,
                                                getTemperature,
                                                getRevision,
                                                getSerialNumber,
                                                setSerialNumber,
                                                cal7689,
                                                getStatus,
                                                sysReset,
                                                getIniByte,
                                                setIniByte,
                                                getRunState,
                                                setRunState,                             
                                                "\0"};
                                      
enum biasSubMenu {RD,Pp,Pn,DGp,DGn,Sp,Sn,SWp,SWn,RGp,RGn,OG,OD,BB,bias_all_off=254,bias_all=255};                   
char rom biasSubMenuItems[][8]=                {cb_RD,
                                                cb_Ppos,
                                                cb_Pneg,
                                                cb_DGpos,
                                                cb_DGneg,
                                                cb_Spos,
                                                cb_Sneg,
                                                cb_SWpos,
                                                cb_SWneg,
                                                cb_RGpos,
                                                cb_RGneg,
                                                cb_OG,
                                                cb_OD,
                                                cb_BB,
                                                cb_all_off,
                                                cb_all,
                                                "\0"};

enum biasSubSubMenu {bias0,bias1};                   
char rom biasSubSubMenuItems[][8]=             {cb_0,
                                                cb_1,
                                                "\0"};
                                              
enum offsetSubMenu {pO,n0,p1,n1,p2,n2,p3};
char rom offsetSubMenuItems[][8]=              {co_0pos,
                                                co_0neg,
                                                co_1pos,
                                                co_1neg,
                                                co_2pos,
                                                co_2neg,
                                                co_3pos,
                                                co_3neg,
                                                "\0"};
         
enum offsetSubSubMenu {os0,os1};                   
char rom offsetSubSubMenuItems[][8]=          { co_0,
                                                co_1,
                                                "\0"};                                         


enum presetSubMenu {pErase,pRead,pExpose,pWipe,pIdle,pFastRev,pOffset};
unsigned int8 lastBias = 0xFF;
char rom presetSubMenuItems[][8]=             { pb_erase,                                                
                                                pb_read,
                                                pb_expose,
                                                pb_wipe,
                                                pb_idle,
                                                pb_fastRev,      
                                                pb_offset,
                                                "\0"};
                                                                                              
                                              
enum powerSubMenu {p33,p5,p12,p24,p54,pPA,pLVDS,pVbb0,pVbb1,pClks,pAll};
char rom powerSubMenuItems[][8]=               {pe_3V3reg,
                                                pe_5Vreg,
                                                pe_12Vreg,
                                                pe_24Vreg,
                                                pe_54Vreg,
                                                pe_Preamp,
                                                pe_LVDS,
                                                pe_Vbb0,
                                                pe_Vbb1,
                                                pe_Clks,
                                                pe_all,
                                                "\0"}; 
                                        
enum powerSubSubMenu {pOn,pOff};
char rom powerSubSubMenuItems[][8]=            {pe_on,
                                                pe_off,
                                                "\0"}; 
                                            
enum fastSubMenu {fFast,fSlow};                                            
char rom fastSubMenuItems[][8]=                {sf_fast,     
                                                sf_slow,
                                                "\0"}; 
 
enum voltageSubMenu {v3V3M,v3V3o,v5Vp,v5Vn,vPAp,vPAn,v12Vp,v12Vn,v24Vn,v54Vp,vAll};
char rom voltageSubMenuItems[][8]=             {gv_3V3Micro,       
                                                gv_3V3Other,
                                                gv_5Vpos,        
                                                gv_5Vneg,        
                                                gv_5Vpos_pa,     
                                                gv_5Vneg_pa,     
                                                gv_12Vpos,      
                                                gv_12Vneg,      
                                                gv_24Vneg,      
                                                gv_54Vpos,
                                                gv_all,
                                                "\0"}; 
 
enum tempSubMenu {tCCD0,tCCD1,tPA,tFEE,tAll};
char rom tempSubMenuItems[][8]=                {gt_CCD0,
                                                gt_CCD1,     
                                                gt_Preamp,
                                                gt_FEE,
                                                gt_all,
                                                "\0"}; 

enum cal7SubMenu {cBias,cCDS};
char rom cal7689SubMenuItems[][8]=             {c7_bias,
                                                c7_CDS,
                                                "\0"}; 
                                                
enum snSubMenu {snFEE,snADC,snPA0,snCCD0,snCCD1};
char rom snSubMenuItems[][8]=                   {sn_FEE,
                                                sn_ADC,
                                                sn_PA0,
                                                //sn_PA1,
                                                sn_CCD0,
                                                sn_CCD1,
                                                "\0"};                                                
/*****************************************************************************/
/* CLOCK BIAS COMMAND HANDLERS                                               */
/*****************************************************************************/
void exeSetClockBias()
{
   int8 chID;
   unsigned int16 value;
   int8 dacID = getMenuIndex(rxBuff[pCmd].Param[2],biasSubSubMenuItems);
   if (dacID <=1)
   {
      int8 chIndex=getBiasRecordIndex(rxBuff[pCmd].Param[1]);
      if (chIndex<16)
      {
         int chID = biasParams[chIndex].channel;
         float voltage = atof(rxBuff[pCmd].Param[3]);
         if(voltage <= biasParams[chIndex].max && voltage >= biasParams[chIndex].min)
         {
            biasDef[chIndex+(dacID*16)]=voltage;
            float scaler = biasParams[chIndex].scaler;
            voltage *=scaler;
            if (voltage > 32767) 
            {
               value  = 0xFFFF; // (full scale)
            }
            else if (voltage < -32767) 
            {
               value = 0 ;
            }
            else
            {
               value =32768 + (signed int16)voltage;
            }
            //value =32768 + (signed int16)voltage;
            setVoltage(value, dacID, chID);
            printf(success);
         }
         else
         {
            printf(outOfRange);
         }
      }
      else if(chIndex == bias_all_off)
      {
         clear_all_bias(dacID);
         printf(success);
      }
      else
      {
         printf(badChannel);
      }
   }   
   else
   {
      printf(badChannel);
   }
}

void exeGetClockBias()
{
   //int8 chID;
   int8 dacID = getMenuIndex(rxBuff[pCmd].Param[2],biasSubSubMenuItems);
   if (dacID <=1)
   {
      int8 chIndex=getBiasRecordIndex(rxBuff[pCmd].Param[1]);
      
      switch (chIndex)
      {
         case(noRecordFound):
            printf(badChannel);
         break;
         
         case (allRecords):
            for(int i=0;i<13;i++)
            {
               printf("%f,",biasDef[i+(dacID*16)]);
            }
            chIndex=13;
            
         default:
            printf("%f\n",biasDef[chIndex+(dacID*16)]);
         break;
      }
   }   
   else
   {
      printf(badChannel);
   }
}
void exeRdClockBias()
{
   int8 dacID = getMenuIndex(rxBuff[pCmd].Param[2],biasSubSubMenuItems);
   if (dacID <=1)
   {
      int8 chIndex=getBiasRecordIndex(rxBuff[pCmd].Param[1]);
      
      switch (chIndex)
      {
         case(noRecordFound):
            printf(badChannel);
         break;
         
         case (allRecords):
            for(int i=0;i<13;i++)
            {
               int ADCchID = biasParams[i].ADCCh0;
               if (dacID==1) ADCchID = biasParams[i].ADCCh1;
               float offset = get7689Offset(ADCChID);
               float scaler = get7689Scaler(ADCChID);
               // first configure the DAC MUX
               setDACmux(dacID, (int16)i);
               // then read the ADC voltage
               float result = get7689Voltage(ADCchID, scaler, offset);
               printf("%f,",result);   
            }
            chIndex=13;// get last record by executing the default statement
         
         default:
            int ADCchID = biasParams[chindex].ADCCh0;
            if (dacID==1) ADCchID = biasParams[chindex].ADCCh1;
            float offset = get7689Offset(ADCChID);
            float scaler = get7689Scaler(ADCChID);
            // first configure the DAC MUX
            setDACmux(dacID, (int16)chIndex);
            // then read the ADC voltage
            float result = get7689Voltage(ADCchID, scaler, offset);
            printf("%f\n",result);
         break;
      } 
   }
   else
   {
      printf(badChannel);
   }
}

/*****************************************************************************/
/* OFFSET COMMAND HANDLERS                                                   */
/*****************************************************************************/
void exeSetOffset()
{
   int8 chID;
   unsigned int16 value;
   int8 bankID = getMenuIndex(rxBuff[pCmd].Param[2],offsetSubSubMenuItems);
   if (bankID <=1)
   {
      int8 chIndex=getOSRecordIndex(rxBuff[pCmd].Param[1]);
      if (chIndex<8)
      {
         int chID = offsetParams[chIndex].channel;
         chID=chID+(bankID*8);
         float voltage = atof(rxBuff[pCmd].Param[3]);
         if(voltage <= offsetParams[bankID].max && voltage >= offsetParams[chIndex].min)
         {
            
            offsetDef[chIndex+(bankID*8)]=voltage;
            float scaler = offsetParams[chIndex].scaler;
            voltage *=scaler;
            if (voltage > 32767) 
            {
               value  = 0xFFFF; // (full scale)
            }
            else if (voltage < -32767) 
            {
               value = 0 ;
            }
            else
            {
               value =32768 + (signed int16)voltage;
            }
            //value =32768 + (signed int16)voltage;
            setVoltage(value, cdsOS, chID);
            printf(success);
         }
         else
         {
            printf(outofRange);
         }
      }
      else
      {
         printf(badChannel);
      }
   }   
   else
   {
      printf(badChannel);
   }       
}

void exeGetOffset()
{
   int8 chID;
   int8 bankID = getMenuIndex(rxBuff[pCmd].Param[2],offsetSubSubMenuItems);
   if (bankID <=1)
   {
      int8 chIndex=getOSRecordIndex(rxBuff[pCmd].Param[1]);
      switch (chIndex)
      {
         case(noRecordFound):
            printf(badChannel);
         break;
         
         case (allRecords):
            for(int i=0;i<7;i++)
            {
               printf("%f,",offsetDef[i+(bankID*8)]);
            }
            chIndex=7;
            
         default:
            printf("%f\n",offsetDef[chIndex+(bankID*8)]);
         break;
      }
       
   }   
   else
   {
      printf(badChannel);
   }       
}

void exeRdOffset()
{
   int8 bankID = getMenuIndex(rxBuff[pCmd].Param[2],offsetSubSubMenuItems);
   if (bankID <=1)
   {
      int8 chIndex=getOSRecordIndex(rxBuff[pCmd].Param[1]);
      
      switch (chIndex)
      {
         case(noRecordFound):
            printf(badChannel);
         break;
         
         case (allRecords):
            for(int i=0;i<7;i++)
            {
               int ADCchID = offsetParams[i].ADCCh0;
               float offset = get7689Offset(ADCChID);
               float scaler = get7689Scaler(ADCChID);
               // first configure the DAC MUX
               chIndex=i+(bankId*8);
               setDACmux(cdsOS, (int16)chIndex);
               // then read the ADC voltage
               float result = get7689Voltage(ADCchID, scaler, offset);
               printf("%f,",result);   
            }
            chIndex=7;// get last record by executing the default statement
         
         default:
            int ADCchID = offsetParams[chindex].ADCCh0;
            float offset = get7689Offset(ADCChID);
            float scaler = get7689Scaler(ADCChID);
            // first configure the DAC MUX
            chIndex+=(bankId*8);
            setDACmux(cdsOS, (int16)chIndex);
            // then read the ADC voltage
            float result = get7689Voltage(ADCchID, scaler, offset);
            printf("%f\n",result);
         break;
      }
      
   }
   else
   {
      printf(badChannel);
   }
}
/*****************************************************************************/
/* DAC PRESET COMMAND HANDLERS                                               */
/*****************************************************************************/
void exeLoadDAC()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],presetSubMenuItems);
   switch(menuIndex)
   {
      case pErase:
         loadPresetValues(erase);
         lastBias = menuIndex;
         printf(success);
         break;
      case pRead:
         loadPresetValues(read);
         lastBias = menuIndex;
         printf(success);
         break;
      case pExpose:
         loadPresetValues(expose);
         lastBias = menuIndex;
         printf(success);
         break;
      case pWipe:
         loadPresetValues(wipe);
         lastBias = menuIndex;
         printf(success);
         break;
      case pIdle:
         loadPresetValues(idle);
         lastBias = menuIndex;
         printf(success);
         break;
      case pFastRev:
         loadPresetValues(fastRev);
         lastBias = menuIndex;
         printf(success);   
         break;
      case pOffset:
         loadPresetValues(offset);
         printf(success);   
         break;
      default:
         printf(badCmd);     
         return;
   }
}

/*****************************************************************************/

void exeSaveDAC()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],presetSubMenuItems);
   switch(menuIndex)
   {
      case pErase:
         savePresetValues(erase);
         printf(success);
         break;
      case pRead:
         savePresetValues(read);
         printf(success);
         break;
      case pExpose:
         savePresetValues(expose);
         printf(success);
         break;
      case pWipe:
         savePresetValues(wipe);
         printf(success);
         break;
      case pIdle:
         savePresetValues(idle);
         printf(success);
         break;
      case pFastRev:
         savePresetValues(fastRev);
         printf(success);
         break;  
      case pOffset:
         savePresetValues(offset);
         printf(success);
         break;  
      default:
         printf(badParam);
         return;
   }
}

void exeGetLastBias()
{
   switch(lastBias)
   {
      case pErase:
         printf("%s\n",pb_erase);
         break;   
      case pRead:
         printf("%s\n",pb_read);
         break;  
      case pWipe:
         printf("%s\n",pb_wipe);
         break;  
      case pExpose:
         printf("%s\n",pb_expose);
         break;  
      case pIdle:
         printf("%s\n",pb_idle);
         break;  
      case pFastRev:
         printf("%s\n",pb_fastRev);
         break;  
      default:
         printf(unknown);
         break;
   }
}

/*****************************************************************************/
/* POWER COMMAND HANDLERS                                                    */
/*****************************************************************************/

void exeSetPower()
{
   int8 on_off= getMenuIndex(rxBuff[pCmd].Param[2],powerSubSubMenuItems);
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],powerSubMenuItems);
   if (on_off==pOff)
   {
      switch (menuIndex)
      {
         case p33:
         #ifdef DISABLE_3V3_POWER_CYCLE
            printf(functionDisabled);
            return;
         #else   
            clear_all_bias(0);
            clear_all_bias(1);
            output_low(EN_P3V3);
         #endif  
            break;
         case p5:
            output_low(EN_5V);            
            break;
         case p12:
            clrFastClksLVDS();
            output_low(EN_12V);
            break;
         case p24:
            clear_all_bias(0);
            clear_all_bias(1);
            output_low(EN_24V);
            break;
         case p54:
            output_low(EN_54V);
            break;
         case pPA:
            output_low(EN_PA);
            break;
         case pLVDS:
            clrEnLVDS();
            //output_low(EN_LVDS);
            break;
         case pVbb0:
            output_low(EN_54V_AMP0);
            break;
         case pVbb1:
            output_low(EN_54V_AMP1);
            break;
         case pClks:           
            clrEnClks();
            break;
         case pAll:
         #ifndef DISABLE_3V3_POWER_CYCLE
            clear_all_bias(0);
            clear_all_bias(1);
            output_low(EN_P3V3);
         #endif   
            output_low(EN_5V);
            output_low(EN_PA);
            output_low(EN_LVDS);
            clrEnClks();
            clrEnLVDS();
            clrFastClksLVDS();
            output_low(EN_12V);
            output_low(EN_24V);
            output_low(EN_54V);
            output_low(EN_54V_AMP0);
            output_low(EN_54V_AMP1);        
            break;
         default:
            printf(badParam);
            return;
      }
      printf(success);
   }
   else if(on_off==pOn)
   {
      switch (menuIndex)
      {
         case p33:
         #ifdef DISABLE_3V3_POWER_CYCLE
            printf(functionDisabled);
            return;
         #else   
            output_high(EN_P3V3);
            initialize7869();
            break;
         #endif   
         case p5:
            output_high(EN_5V);
            initialize7869();
            break;
         case p12:
            output_high(EN_12V);
            setFastClksLVDS();
            break;
         case p24:
            output_high(_EN_CLKS);
            output_high(EN_24V);
            break;
         case p54:
            output_high(EN_54V);
            break;
         case pPA:
            output_high(EN_PA);
            break;
         case pLVDS:
            setEnLVDS();
//!            if(input_state(EN_12V))
//!            {
//!               output_high(EN_LVDS);
//!            }
//!            else
//!            {
//!               printf(LVDSErr);
//!               return;
//!            }
            break;
         case pVbb0:
            output_high(EN_54V_AMP0);
            break;
         case pVbb1:
            output_high(EN_54V_AMP1);
            break;
         case pClks:
            setEnClks();
//!            if(input_state(EN_24V))
//!            {
//!               output_low(_EN_CLKS);
//!            }
//!            else
//!            {
//!               printf(clkErr);
//!               return;
//!            }   
            break;
         case pAll:
            output_high(EN_P3V3);
            output_high(EN_5V);
            output_high(EN_PA);
            output_high(EN_LVDS);
            output_high(EN_12V);
            setEnLVDS();
            //setEnClks();
            setFastClksLVDS();
            output_high(EN_24V);
            output_high(EN_54V);
            output_high(EN_54V_AMP0);
            output_high(EN_54V_AMP1);
            initialize7869();
            break;
         default:
            printf(badChannel);
            return;
      }
      printf(success);
   }
   else
   {
      printf(badParam);
   }
}

/*****************************************************************************/

void exeGetPower()
{
   char retStr[20] = "\0";
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],powerSubMenuItems);
//!   int8 clkState = input_state(_EN_CLKS);
//!   
//!   if (clkState==0)
//!   {
//!      clkState = 1;
//!   }
//!   else
//!   {
//!      clkState = 0;
//!   }   
//!   
   char lvdsState = getEnLVDS();
   char clkState = getEnClks();
   switch (menuIndex)
   {
      case p33:
         printf("%d\n",input_state(EN_P3V3));     
         break;
      case p5:
         printf("%d\n",input_state(EN_5V)); 
         break;
      case p12:
         printf("%d\n",input_state(EN_12V)); 
         break;
      case p24:
         printf("%d\n",input_state(EN_24V)); 
         break;
      case p54:
         printf("%d\n",input_state(EN_54V)); 
         break;
      case pPA:
         printf("%d\n",input_state(EN_PA)); 
         break;
      case pLVDS:
         printf("%c\n",lvdsState); 
         break;
      case pVbb0:
         printf("%d\n",input_state(EN_54V_AMP0)); 
         break;
      case pVbb1:
         printf("%d\n",input_state(EN_54V_AMP1)); 
         break;
      case pClks:
         printf("%c\n",clkState); 
         break;
      case pAll:      
         printf("%d%d%d%c%d%d%d%d%d%c\n",
            input_state(EN_P3V3),
            input_state(EN_5V),
            input_state(EN_PA),
            lvdsState,
            input_state(EN_12V),
            input_state(EN_24V),
            input_state(EN_54V),
            input_state(EN_54V_AMP0),
            input_state(EN_54V_AMP1),
            clkState);
         break;
      default:
         printf(badParam);
         return;
   }
}

/*****************************************************************************/
/* FAST CLOCK COMMAND HANDLERS                                               */
/*****************************************************************************/

void exeSetFast()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],fastSubMenuItems);
   switch (menuIndex)
   {
      case fFast:
         setFastMode();
         break;
      case fSlow:
         clrFastMode();
         break;
      default:
         printf(badParam);
         return;
   }
   printf(success);
}

void exeGetFast()
{
   switch(getFastMode())
   {
      case slowOn:
         printf("%s\n", sf_slow);
         break;
      case fastOn:
         printf("%s\n", sf_fast);
         break;
      case slowOff:
         printf("*%s\n", sf_fast);
         break;
      case fastOff:
         printf("*%s\n", sf_slow);
         break;    
   }   
}

/*****************************************************************************/
/* VOLTAGE COMMAND HANDLERS                                                  */
/*****************************************************************************/
void exeGetSupplyVoltage()
{
   int8 chIndex=getVoltageRecordIndex(rxBuff[pCmd].Param[1]);
   switch (chIndex)
   {
      case(noRecordFound):
         printf(badChannel);
      break;
      
      case (allRecords):
         for(int i=0;i<9;i++)
         {
            printf("%f,",getVoltage(i));
         }
         chIndex=9;
      
      default:
         printf("%f\n",getVoltage(chIndex));
      break;
   }
}

void exeCalSupplyVoltage()
{
   int8 chIndex=getVoltageRecordIndex(rxBuff[pCmd].Param[1]);
   if (chIndex<10)
   {
      float voltage = atof(rxBuff[pCmd].Param[2]);
      if (voltage > vParams[chIndex].min && voltage < vParams[chIndex].max)
      {
         updateVscaler(voltage, chIndex);
         printf(success);
      }
      else
      {
         printf(outOfRange);
      }
   }
   else
   {
      printf(badChannel);
   }
}


/*****************************************************************************/
/* TEMPERATURE COMMAND HANDLERS                                              */
/*****************************************************************************/
void exeGetTemperature()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],tempSubMenuItems);
   float temperature[4]={0,0,0,0};
   switch(menuIndex)
   {
      case tCCD0:
         printf("%f\n",get_CCDtemperature(ch0));
         break;
      case tCCD1:
         printf("%f\n",get_CCDtemperature(ch1));
         break;
      case tFEE:
         temperature[0] = (float)getRawData(TH1);
         temperature[0] =  558.2172 * pow(temperature[0],-0.1034492F);
         //temperature =  517.01 * pow(temperature,-0.092F);
         printf("%f\n",temperature[0]);
         break;
      case tPA:
         temperature[0] = (float)getRawData(TH0);
         temperature[0] =  558.2172 * pow(temperature[0],-0.1034492F);
         printf("%f\n",temperature[0]);
         break;
      case tAll:
         temperature[0]=get_CCDtemperature(ch0);
         temperature[1]=get_CCDtemperature(ch1);
         temperature[2]=(float)getRawData(TH1);
         temperature[2] =  558.2172 * pow(temperature[2],-0.1034492F);
         temperature[3]=(float)getRawData(TH0);
         temperature[3] =  558.2172 * pow(temperature[3],-0.1034492F);
         printf("%f,%f,%f,%f\n",temperature[0],temperature[1],temperature[2],temperature[3]);
         break;
      default:
         printf(badParam);
         break;
   }
}

/*****************************************************************************/
/* CAL 7689 COMMAND HANDLER                                                  */
/*****************************************************************************/

void exeCal7689()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],cal7689SubMenuItems);
   switch(menuIndex)
   {
      case cBias:
         cal7689LVBiasChannel();
         printf(success);
         break;
      case cCDS:
         cal7689LVCDSChannel();
         printf(success);
         break;
      default:
         printf(badParam);
         break;
   }
}

/*****************************************************************************/
/* GET SERIAL NUMBER                                                         */
/*****************************************************************************/

void exeGetSerial()
{
   int32 serial = 0;
   char strSerial[20] = "\0";
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],snSubMenuItems);
   switch(menuIndex)
   {
      case snFEE:
         int32 *ptr = &serial;
         read_program_memory(0x2c, ptr, 4);
         printf("%Lu\n",serial);
         break;
      case snADC:
         serial = getSerNum(serADC);
         printf("%Lu\n",serial);
         break;
      case snPA0:
         serial = getSerNum(serPA0);
         printf("%Lu\n",serial);
         break;
      case snCCD0:
         getCCDserial(serCCD0, strSerial);
         printf("%s\n",strSerial);
         break;
      case snCCD1:
         getCCDserial(serCCD1,strSerial);
         printf("%s\n",strSerial);
         break;
      default:
         printf(badParam);
         break;
   }
}

/*****************************************************************************/
/* SET SERIAL NUMBER                                                         */
/*****************************************************************************/

void exeSetSerial()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],snSubMenuItems);
   unsigned int32 serial=atoi32(rxBuff[pCmd].Param[2]);
   switch(menuIndex)
   {
      case snADC:
         setSerNum(serADC, serial);
         printf(success);
         break;
      case snPA0:
         setSerNum(serPA0, serial);
         printf(success);
         break;
      case snCCD0:
         setCCDserial(serCCD0,rxBuff[pCmd].Param[2]);
         printf(success);
         break;
      case snCCD1:
         setCCDserial(serCCD1,rxBuff[pCmd].Param[2]);
         printf(success);
         break;
      default:
         printf(badParam);
         break;
   }
}


/*****************************************************************************/
/* MAIN COMMAND HANDLER                                                      */
/*****************************************************************************/

void executeCmd()
{
   if (rxBuff[pCmd].Status==recOverrun)
   {
      #IFDEF USE_LED
      output_high(LED);
      #ENDIF
      output_high(EN_RS232_DRV);// enable driver for response
      printf(overrun);
      rxBuff[pCmd].Status=recEmpty;
   }
   else if (rxBuff[pCmd].Status==cmdReady)
   {
      
      #IFDEF USE_LED
      output_high(LED);
      #ENDIF
      output_high(EN_RS232_DRV);// enable driver for response
   
      int8 menuIndex;
      menuIndex = getMenuIndex(rxBuff[pCmd].Param[0], mainMenuItems);
        
      switch(menuIndex)
      {
         case sCB: // setClockBias
            exeSetClockBias();
            break;        
         case gCB: // getClockBias
            exeGetClockBias();
            break;      
         case rCB: // read the actual clock bias via ADC
            exeRdClockBias();
            break;
         case sOS: // setOffset
            exeSetOffset();
            break;        
         case gOS: // getOffset
            exeGetOffset();
            break;      
         case rOS: // read the actual offset voltage via ADC
            exeRdOffset();
            break;
         case lDac: // load DAC with presets
            exeLoadDAC();
            break;        
         case sDAC: // save DAC presets
            exeSaveDAC();
            break;       
         case gLBmode:
            exeGetLastBias();
            break;
         case sPower: // enable/disable regulators
            exeSetPower();
            break;        
         case gPower: // get regulator status
            exeGetPower();
            break;        
         case sFast: // set Fast/Slow mode
            exeSetFast();
            break;   
         case gFast:
            exeGetFast();
            break;
         case gSupply: //get supply voltage
            exeGetSupplyVoltage();
            break;        
         case cSupply: //calibrate supply voltage
            exeCalSupplyVoltage();
            break;
         case gTemp: //get temperature
            exeGetTemperature();
            break;       
         case gRevision: // get firmware Revision
            printf(firmwareRev);
            break;
         case gSerial: // get serial numbers
            exeGetSerial(); 
            break;
         case sSerial: // set serial numbers
            exeSetSerial();
            break;
         case calADC: // calibrate the 7869 ADC's
            exeCal7689();
            break;
         case gStatus:
            printf("%f minutes since reset\n",(float)get_ticks()/TICKS_PER_SECOND/60);
            break;
         case sReset:
            while(1) // Causes a reset by overflowing the stack
            // Can't use reset CPU because bootloader ignores that type of reset
            {
               #asm 
               PUSH
               #endasm
            }
         case gIni:
            printf("%u\n",read_eeprom(eeIniAdd));
            break;
         case sIni:
            write_eeprom(eeIniAdd,atoi(rxBuff[pCmd].Param[1]));
            printf(success);
            break;
         case gRun:
            if (strlen(runstate)>0 & strlen(runstate) <9)
            {
               printf("%s\n",runstate);
            }
            else
            {
               printf(badRS);
            }
            break;
         case sRun:
            if (strlen(runstate)>0 & strlen(runstate) <9)
            {
               strcpy(runstate,rxBuff[pCmd].Param[1]);
               printf(success);
            }
            else
            {
               printf(outOfRange);
            }
            break; 
         default:
            printf(badCmd);
            break; 
      }
      rxBuff[pCmd].Status=recEmpty;
   }    
   else if (pCmd!=pRec) // get next record but never move ahead of rx buffer
   {
      pCmd+=1;
      if (pCmd>=numRec)
      {
         pCmd=0;
      }
   } 
   delay_ms(100);
   output_low(LED);
}

#ENDIF
