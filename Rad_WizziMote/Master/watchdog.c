#include "header.h"
#include <stdio.h>

// Counters to keep track of task progress
static uint16_t count=0;
static uint8_t blinkcounter1=0;
static uint8_t blinkcounter2=0;
static uint8_t blinkcounter3=0;

// Other variables
static uint8_t PROGRESS=0;
static struct etimer et;

PROCESS(main_process, "Main Task");
AUTOSTART_PROCESSES(&main_process);

// Logs error or status information to stdout
void errorLogging(char* str)
{
	printf("Status Log: %s\r\n",str);
}

// Increments the blink counter relating to the passed integer
void incrementCOUNTER(uint8_t i)
{
	switch(i)   
	{  
		case 1:  
			blinkcounter1++;
			break;
		case 2:  
			blinkcounter2++;
			break; 
		case 3:  
			blinkcounter3++;
			break;
		default :  
			errorLogging("ERROR! Invalid input to incrementCOUNTER function");  
			break;
	}
}

// Returns the value of the blink counter relating to the passed integer
uint8_t getCOUNTER(uint8_t i)
{
	switch(i)   
	{  
		case 1:  
			return(blinkcounter1);  
		case 2:  
			return(blinkcounter2);  
		case 3: 
			return(blinkcounter3);
		default :  
			errorLogging("ERROR! Invalid input to getCOUNTER function");
			return 0;
	}
}


// Clears the blink counter relating to the passed integer
void clearCOUNTER(uint8_t i)
{
	switch(i)   
	{  
		case 1:  
			blinkcounter1 = 0;
			break; 
		case 2:  
			blinkcounter2 = 0;
			break;  
		case 3: 
			blinkcounter3 = 0;
			break;
		default :  
			errorLogging("ERROR! Invalid input to clearCOUNTER function");
	}	
}

// Returns the value of the progress variable
uint8_t getPROGRESS()
{
	return PROGRESS;
}

// Clears the progress variable - sets all bits to 0
void clearPROGRESS()
{
	PROGRESS=0;
}

// Takes an integer which represents which task has completed and sets the PROGRESS bit for that task
void setPROGRESS(uint8_t i)
{
	switch(i)
	{
		case 1:
			PROGRESS |= (1<<1);
			break;
		case 2:
			PROGRESS |= (1<<2);
			break;
		case 3:
			PROGRESS |= (1<<3);
			break;
		default:
			errorLogging("ERROR: invalid input to setPROGRESS function");
			break;
	}
}

PROCESS_THREAD(main_process, ev, data)
{
	// Begin the process
	PROCESS_BEGIN();	
	
	// Sets up the watchdog timer to use ACLK input and an interval of 16s
	WDTCTL = WDTPW + WDTSSEL0 + WDTHOLD + WDTIS1 + WDTIS0;

	// Start the watchdog
	WDTCTL = (WDTCTL_L&~(WDTHOLD))+ WDTPW;

	// Begin status logging
	errorLogging("Starting up the system RAD_TEAM");
	static int iCnt = 0;

	while(1)
	{
 		// Delay 1 second - start the event timer and set its event period to 1 second
		etimer_set(&et, 1*CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		// Count how many seconds the system has been running and log this information
		iCnt++;	
		char* secondCounterStr[50];
		sprintf(secondCounterStr,"System has been running for %d seconds.",iCnt);
		errorLogging(secondCounterStr);

		// Call the different processes in tasks.c
		process_start(&LED1,NULL);
		process_start(&LED2,NULL);
		process_start(&LED3,NULL);	
	}
	PROCESS_END();

}

