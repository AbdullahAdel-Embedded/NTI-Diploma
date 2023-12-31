

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "std_types.h"
#include "semphr.h"

#ifndef NULL
#define NULL (void *)0
#endif


void PushButton(void * pvparam);
#define PushButtonPr (1)

void Led(void * pvparam);
#define LedPr (2)

void init(void);

/*** Global Variable ****/
u8 ButtonState = 1;

#define ForgetGiveSemaphore pdFALSE

xSemaphoreHandle ButtonSemaphore = NULL;

#define FrogetGlobalInterrupt 0  /* 0  for Nodefect 1 for Defect **/

 int main(void)
{

	 /* Creation of the Task*/
	 xTaskHandle PushHandle  = NULL;
	 xTaskCreate(PushButton,(const signed char*)"Push Button",85,NULL,PushButtonPr,&PushHandle);

	 xTaskHandle LedHandle = NULL ;
	 xTaskCreate(Led,(const signed char*)"Led ",85,NULL,LedPr,&LedHandle);

	 /*Init Function*/
	 init();

	 /*Create Semaphore for the Button */
	 vSemaphoreCreateBinary(ButtonSemaphore);

	/*start Scheduler */
	vTaskStartScheduler();



	/*Never Enter Here */
	return 0;
}

void init(void){

	DDRB = 0xFF;

	DDRC = 0x00;

	PORTC = 0xFF;

}
 void PushButton(void * pvparam){
	 portTickType Freq = 100;
	 portTickType Start = xTaskGetTickCount();
	 while (1){
		 /* if the Push Button is Pressed */
		 if ((PINC & 0x01) == 0){

			 /* If the Resource No One is Using so Take the Semaphore  **/
			 if(pdTRUE == xSemaphoreTake(ButtonSemaphore,100)){

				 /*Change the State **/
				 ButtonState = 0;

				 /**Give the Semaphore */
				#if ForgetGiveSemaphore == pdFALSE
				 xSemaphoreGive(ButtonSemaphore);
				#endif
			 }
			 /*Else Delay 1000 Tick till the Resource is Free **/
			 else {
				 vTaskDelay(1000);
			 }
		 }

		 /*Make the Task Periodic with Period 50 Tick */
		 vTaskDelayUntil(&Start,Freq);
	 }

 }


 void Led(void * pvparam){
	 portTickType Freq = 100;
	 portTickType Start = xTaskGetTickCount();
	 while (1){
		 /* if the Push Button is Pressed */
		 if(pdTRUE == xSemaphoreTake(ButtonSemaphore,100)){
			 if (ButtonState == 0){
				 PORTB ^= 0x01;
				 ButtonState = 1;
			 }
			 /**Give the Semaphore */
			 xSemaphoreGive(ButtonSemaphore);
		 }
		 /*Else Delay 1000 Tick till the Resource is Free **/
		 else {
			 vTaskDelay(1000);
		 }

		 /*Make the Task Periodic with Period 50 Tick */
		 vTaskDelayUntil(&Start,Freq);
	 }


 }
