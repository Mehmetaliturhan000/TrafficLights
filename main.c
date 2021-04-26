#include "main.h"
#include "stm32f4xx.h"
#include "delay.h"

GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

uint32_t sayac = 0;
uint32_t sayac2 = 0;

typedef enum
{
   red,
	 yellow,
	 yellow2,
	 green

}led;

led led_sit = red;
led pedestiral_led_sit = green;

typedef enum
{
  up,
	down
}flag;

flag button_flag = down;

void GPIO_Config() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);   //GPIOA clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);   //GPIOD clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //SYSCFG clock enable
     
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;               //GPIOA Pin0 Select   // Button
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_IN;					 //GPIOA Pin0 Input	
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;          //GPIOA Pin0 Push-Pull
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;        //GPIOA Pin0 Nopull
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;      //GPIOA Pin0 Very high speed
	 
	GPIO_Init(GPIOA,&GPIO_InitStruct);									 //Load the configuration
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;						   //GPIOD Pin 0 Select   //Red
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;          //GPIOD Pin 0 Output
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;				   //GPIOD Pin 0 Output Push-Pull
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;			   //GPIOD Pin 0 Output Nopull
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;      //GPIOD Pin 0 Very high speed
	
	GPIO_Init(GPIOD,&GPIO_InitStruct);									 //Load the configuration
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;               //GPIOD Pin 1 Select   //Yellow
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					 //GPIOD Pin 1 Output
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					 //GPIOD Pin 1 Output push pull
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;				 //GPIOD Pin 1 Output no-pull
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;      //GPIOD Pin 1 Very high speed
	
	GPIO_Init(GPIOD,&GPIO_InitStruct);                   //Load the configuration
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;               //GPIOD Pin 2 Select     //Green
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					 //GPIOD Pin 2 Output
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					 //GPIOD Pin 2 Output push pull
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;				 //GPIOD Pin 2 Output no-pull
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;      //GPIOD Pin 2 Very high speed
	
	GPIO_Init(GPIOD,&GPIO_InitStruct);                   //Load the configuration
 
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;               //GPIOD Pin 3 Select   //Pedestrian Red
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					 //GPIOD Pin 3 Output
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					 //GPIOD Pin 3 Output push pull
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;				 //GPIOD Pin 3 Output no-pull
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;      //GPIOD Pin 3 Very high speed
	
	GPIO_Init(GPIOD,&GPIO_InitStruct);                   //Load the configuration
   
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;               //GPIOD Pin 4 Select    //Pedestiran Green
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					 //GPIOD Pin 4 Output
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					 //GPIOD Pin 4 Output push pull
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;				 //GPIOD Pin 4 Output no-pull
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;      //GPIOD Pin 4 Very high speed
	
	GPIO_Init(GPIOD,&GPIO_InitStruct);                   //Load the configuration 

}
 	
void TIM_Config()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	 	  //TIMER TIM1 clock enable
	TIM_InitStruct.TIM_Prescaler = 0;												  //Prescaler set is 0 	
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;  	  //Counter mode up
	TIM_InitStruct.TIM_Period = 48999;											  // I used this formula to calculate the period--->Update Event = TIM clk/((PSC+1)*(ARR+1)*(RCR+1)) 
	TIM_InitStruct.TIM_ClockDivision = 0;                     // however it missed with small numbers  than I incremented that value a little bit experimentally. 
	TIM_InitStruct.TIM_RepetitionCounter = 0;                 //Repetition counter set 0.
	
	TIM_TimeBaseInit(TIM1,&TIM_InitStruct);                   //Load the configuration
	TIM_Cmd(TIM1,ENABLE);																		  //TIM1 enable
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                  //Configures the TIMx event by the software
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;     //NVIC channel selected.
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 1;   //NVIC Preemption Priority set as 1 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;						//NVIC Sub Priority set as 1 
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;              //NVIC Channel enabled when interrupt comes
  
	NVIC_Init(&NVIC_InitStruct);                              //Load the configuration
}	


void TIM1_UP_TIM10_IRQHandler()
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) == SET)
	{
		sayac++;
		
		if(sayac <= 10000)                         //State 1 Car -->Red LED <---> Pedestrian --> Green LED 
			{
	    GPIO_ResetBits(GPIOD,GPIO_Pin_1);        //Car yellow LED off
			GPIO_ResetBits(GPIOD,GPIO_Pin_3);        //Pedestrian red LED off
			GPIO_SetBits(GPIOD,GPIO_Pin_0);          // Car red LED on
			GPIO_SetBits(GPIOD,GPIO_Pin_4);          //Pedestrian green LED on
      led_sit = red;				                 
			}
		  
		 else if (sayac > 10000 && sayac <= 12001)   //State 2 Car -->Yellow LED <---> Pedestrian --> Red  LED  
			{
			GPIO_ResetBits(GPIOD,GPIO_Pin_0);         //Car red LED off
			GPIO_ResetBits(GPIOD,GPIO_Pin_4);	        //Pedestrian green LED off
			GPIO_SetBits(GPIOD,GPIO_Pin_1);           //Car yellow LED on 
			GPIO_SetBits(GPIOD,GPIO_Pin_3);           //Pedestrian red LED on
			led_sit = yellow;
				
			}
		
			else if (sayac > 12001 && sayac <= 24002)  //State 3 Car -->Green LED <---> Pedestrian --> Red  LED  
			{
			GPIO_ResetBits(GPIOD,GPIO_Pin_1);         //Car yellow LED off
			GPIO_SetBits(GPIOD,GPIO_Pin_2);           //Car green LED on 
			led_sit = green; 
				if(button_flag == up)                  //Interrupt check (If interrupt comes button_flag changed up)
				{
					sayac2++;                            //Sayac2 starts from 0 to 2000ms (2s)
					                                     //it controls the 2 seconds wait after pedestrian pressed the button
					if(sayac2 == 2000)                    
					{
					sayac = 24003;                      //For going to yellow state
					sayac2 = 0;                         //The variable completed it's mission, assigned to inital value back
					button_flag = down;                 //The variable completed it's mission, assigned to inital value back
					}
				}		
			}
		
		else if(sayac > 24002 && sayac <= 26003)  //State  4 Car -->Yellow  LED <---> Pedestrian --> Red  LED  
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);       //Car green LED off 
			GPIO_SetBits(GPIOD,GPIO_Pin_1);         //Car yellow LED onn
			led_sit = yellow2;
		}
		
		else if (sayac >= 26004)               //Loop ends, it's for return to beginning
		{
			sayac = 0;                           //The variable completed it's mission, assigned to inital value back
		}
				
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}	
	
void EXTI_Config()
{
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,GPIO_PinSource0); //EXTI_Line Config Pin 0 

		EXTI_InitStruct.EXTI_Line = EXTI_Line0;                   //Line 0 selected. In STM32F4 Pin 0 connected to line 0
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;                    //Line enable selected. (When interrupt comes)
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;					//Mode interrupt selected
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;       //Trigger rising mode selected

		EXTI_Init(&EXTI_InitStruct);                               //Load the configuration

		NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;              //NVIC EXTI 0 channel selected
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;               //Channel enable (When interrupt comes)
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;     // Priority set 0 (Timer priority's was 1, EXTI is more priority
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;            // to interrupt the timer)
                                                               //Because of that both selected as 0 --> inversely proportional 
		NVIC_Init(&NVIC_InitStruct);                               //Load the configuration
}

void EXTI0_IRQHandler()       //EXTI Interrupt Handler
{
		if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
     if(led_sit == green)     //If car's LED is green 
		 {
		    button_flag = up;	    //Button flag up
		 }
		
		EXTI_ClearITPendingBit(EXTI_Line0);  //Pending bit cleared 
	}
}	

int main(void)
{
	GPIO_Config();
	EXTI_Config();
  TIM_Config();
	
	while(1)
	{	
	}
	
}	
