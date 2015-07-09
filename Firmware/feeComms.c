#IFNDEF _fee_comms
#DEFINE _fee_comms

#include <feeComms.h>
#include <feeCmdHandler.c>

/*****************************************************************************/
void initializeComms()
{
   output_high(EN_RS232_DRV);
   int i;
  
   for (i=0;i<numRec;++i)
   {
      rxBuff[i].Status=0;
      int j;
      for (j=0;j<numParam;j++)
      {
         rxBuff[i].Param[j][0]=0;
      }
   }

   pRec=0; // pointer to the record
   pParam=0; // pointer to the parameter in the record
   pChar=0; // pointer to the character in the parameter
   pCmd=0;

   enable_interrupts(INT_RDA);
}

/******************************************************************************/
/*COMM'S INTERUPT                                                             */
/******************************************************************************/
// The comm's interace has a circular buffer... the first field is used for
// status. The subsequent fields contain Parameters.

#INT_RDA
void rda_isr()
{
   #IFDEF USE_LED
   output_high(LED);
   #ENDIF
   while (kbhit())
   { 
      output_high(EN_RS232_DRV);
      char rxData = fgetc(PORT1);
      #IFDEF USE_ECHO
         fputc(rxData,PORT1);
      #ENDIF
      //output_low(EN_RS232_DRV);
      switch (rxData)
      {
         case startCharacter:     
            if (rxBuff[pRec].Status!=recFull)
            {
               rxBuff[pRec].Status=recActive; // activate record  
               
               int i;
               for (i=0;i<numParam;i++)
               {  
                  rxBuff[pRec].Param[i][0]=0; // clear all params 
               }
              
               pParam=0;   // reset the param and char pointers
               pChar=0;
            }    
         break;
         
         case delimiterCharacter:
            if (rxBuff[pRec].Status==recActive)
            {
               if (pParam<numParam-1) //check that this is not last param
               {
                  rxBuff[pRec].Param[pParam][pChar]=0; // set last char to NULL
                  pParam+=1;  // increment the param pointer
                  pChar=0;    // reset the character pointer
               }
               else 
               {
                  rxBuff[pRec].Status=recEmpty;
               }
            }  
         break;   
         
         case endCharacter:
            if (rxBuff[pRec].Status==recActive)
            {
               rxBuff[pRec].Param[pParam][pChar]=0; 
               rxBuff[pRec].Status=recFull;
               pRec+=1; // point to next record in buffer
               if (pRec>=numRec)
               {
                  pRec=0;
               } 
            }
         break;     
          
         //case spaceCharacter:// do nothing for spaces
         //break;
         
         default:
            if(rxBuff[pRec].Status==recActive)
            {
               rxBuff[pREC].Param[pParam][pChar]=rxData; 
               if(pChar<lenParam) // if param full overrun has occurred
               {
                  pChar+=1;
               }
               else
               {
                  rxBuff[pRec].Status=recEmpty; // buffer overrun
               }
            }   
         break;
      }
   }
   output_low(LED);
   //output_low(EN_RS232_DRV);
}  
#ENDIF
