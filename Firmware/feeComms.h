#use RS232(baud=38400,parity=N,xmit=TXD,rcv=RXD,bits=8,stream=PORT1,restart_wdt)

#define USE_LED
#define USE_ECHO

/* RECIEVE BUFFER DEFINITIONS */

#define numRec 4
#define numParam 4
#define lenParam 10

typedef struct rx_record
{
   char  Status;      // used to log 
   char  Param[numParam][lenParam];
}rx_rec;

rx_rec rxBuff[numRec];

unsigned int8 pRec=0; // pointer to the record
unsigned int8 pParam=0; // pointer to the parameter in the record
unsigned int8 pChar=0; // pointer to the character in the parameter
unsigned int8 pCmd=0;// used for executing the command



