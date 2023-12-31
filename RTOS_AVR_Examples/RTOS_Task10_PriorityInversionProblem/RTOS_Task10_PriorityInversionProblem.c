

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "std_types.h"
#include "semphr.h"

#ifndef NULL
#define NULL (void *)0
#endif


void Led2(void * pvparam);
#define Led2Pr (3)

void PushButton2(void * pvparam);
#define PushButton2Pr (2)


void Led(void * pvparam);
#define LedPr (1)

void init(void);

xSemaphoreHandle ButtonSemaphore = NULL;


 int main(void)
{

	 /* Creation of the Task*/
	 xTaskHandle Led2Handle  = NULL;
	 xTaskCreate(Led2,(const signed char*)"Led 2",85,NULL,Led2Pr,&Led2Handle);

	 /* Creation of the Task*/
	 xTaskHandle PushHandle2  = NULL;
	 xTaskCreate(PushButton2,(const signed char*)"Push Button 2",85,NULL,PushButton2Pr,&PushHandle2);


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
 void Led2(void * pvparam){

	 portTickType Freq = 1000;
	 portTickType Start = xTaskGetTickCount();
	 vTaskDelay(20);
	 while (1){

			 /* If the Resource No One is Using so Take the Semaphore  **/
			 while(pdFALSE == xSemaphoreTake(ButtonSemaphore,100)){
				 vTaskDelay(100);
				 PORTB ^= 0x02;
			 }

			 vTaskDelay(10000);

			 xSemaphoreGive(ButtonSemaphore);

		 /*Make the Task Periodic with Period 50 Tick */
		 vTaskDelayUntil(&Start,Freq);
	 }

 }


 void PushButton2(void * pvparam){
	 portTickType Freq = 100;
	 portTickType Start = xTaskGetTickCount();
	 while (1){

				 while(pdFALSE == xSemaphoreTake(ButtonSemaphore,100));

				 /*Change the State **/
				 PORTB ^= 0x04;

				 vTaskDelay(1000);

				 /**Give the Semaphore */

				 xSemaphoreGive(ButtonSemaphore);

		 /*Make the Task Periodic with Period 50 Tick */
		 vTaskDelayUntil(&Start,Freq);
	 }

 }



 void Led(void * pvparam){
	 portTickType Freq = 100;
	 portTickType Start = xTaskGetTickCount();
	 vTaskDelay(30);
	 while (1){

		 PORTB ^= 0x01;PORTB ^= 0x01;

		 /*Make the Task Periodic with Period 50 Tick */
		 vTaskDelayUntil(&Start,Freq);
	 }


 }
















