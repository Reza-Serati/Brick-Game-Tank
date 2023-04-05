/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32f3xx.h"
#include "stm32f3xx_it.h"

/* USER CODE BEGIN 0 */

#include "LiquidCrystal.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;
int car_x = 16, car_y;
char* map [16][2];
int selecteed_enemy[10];
int enemy_list[14];
int level = 1;
int count = 0;//timer 2
int counter = 0;//timer 7
int speed;
int heart = 8;
int light_percent = 0;
int tmp_level_score = 0;
int tee = 0;
int score = 0;
char name[16];

extern char data[16];

extern RTC_TimeTypeDef timeee;	
extern RTC_DateTypeDef dateee;	

int temp_call_enemy_maker = 0;
int temp_start_micro_time = 0;

int temp_start_with_blue_btn = 0;

int temp_level_finish = 0;
int temp_level_lost = 0;
int temp_game_over = 0;
int temp_game_pause = 0;
int temp_key_pad_confirmd = 0;
int temp_infinit_level = 0;
int start = 0;
int temp_infinit_level_dir = 0;


extern TIM_HandleTypeDef htim1, htim3;
extern RTC_HandleTypeDef hrtc;;
extern UART_HandleTypeDef huart2;
byte enemy[8] = {
0x11,
	0x0A,
	0x04,
	0x0E,
	0x0E,
	0x04,
	0x0A,-
	0x01
};
byte my_car[8] = {
0x04,
	0x04,
	0x0E,
	0x1F,
	0x1F,
	0x0E,
	0x04,
	0x04
};
void transmit_lvl(){
	char rec[2];
	rec[0] = 'u';
	rec[1] = 'l';
	HAL_UART_Transmit(&huart2, rec, 2, 0xfff);
	memset(rec, 0, sizeof(rec));
	sprintf(rec, "%d", level);
	HAL_UART_Transmit(&huart2, rec, 2, 0xFFF);
	HAL_UART_Receive_IT(&huart2, data, sizeof(data));
}
void transmit_game_over_data(){
	setCursor(0 ,0);
		print("               ");
		setCursor(0 ,1);
		print("               ");
		setCursor(4 ,0);
		print("GAME OVER");
		
	
	char rec[6];
		rec[0] = 'g';
		rec[1] = 'o';
		HAL_UART_Transmit(&huart2, rec, 2, 0xfff);
		memset(rec, 0, sizeof(rec));
		sprintf(rec, "%d", score);
		HAL_UART_Transmit(&huart2, rec, 5, 0xFFF);
		memset(rec, 0, sizeof(rec));
		
		rec[0] = 't';
		rec[1] = 'i';
		HAL_UART_Transmit(&huart2, rec, 2, 0xfff);
//		timeee.Seconds = 2;
//		timeee.Minutes = 42;
//		timeee.Hours = 6;
//		HAL_RTC_SetTime(&hrtc, &timeee, RTC_FORMAT_BIN);
		HAL_RTC_GetTime(&hrtc, &timeee, RTC_FORMAT_BIN);
		
		memset(rec, 0, sizeof(rec));
		sprintf(rec, "%d", timeee.Seconds);
		HAL_UART_Transmit(&huart2, rec, 2, 0xFFF);
		sprintf(rec, "%d", timeee.Minutes);
		HAL_UART_Transmit(&huart2, rec, 2, 0xFFF);
		sprintf(rec, "%d", timeee.Hours);
		HAL_UART_Transmit(&huart2, rec, 2, 0xFFF);
		
		rec[0] = 'd';
		rec[1] = 'a';
		HAL_UART_Transmit(&huart2, rec, 2, 0xfff);
//		dateee.Date = 16;
//		dateee.Month = 7;
//		dateee.Year = 18;
//		HAL_RTC_SetDate(&hrtc, &dateee, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &dateee, RTC_FORMAT_BIN);
		
		memset(rec, 0, sizeof(rec));
		sprintf(rec, "%d", dateee.Date);
		HAL_UART_Transmit(&huart2, rec, 2, 0xFFF);
		sprintf(rec, "%d", dateee.Month);
		HAL_UART_Transmit(&huart2, rec, 2, 0xFFF);
		sprintf(rec, "%d", dateee.Year);
		HAL_UART_Transmit(&huart2, rec, 2, 0xFFF);
		
		memset(data, 0, sizeof(data));
		
		HAL_UART_Receive_IT(&huart2, data, sizeof(data));
	
}
void cal_speed(int ii){
	if(level == 10)
		speed = 0;
	else if(level == 9){
		if(ii > 50)
			speed = 1;
		else 
			speed = 0;
	}
	else if(level == 8){
		if(ii > 66)
			speed = 2;
		else if (ii > 33)
			speed = 1;
		else
			speed = 0;
	}
	else if(level == 7){
		if(ii > 75)
			speed = 3;
		else if (ii > 50)
			speed = 2;
		else if (ii > 25)
			speed = 1;
		else
			speed = 0;
	}
	else if(level == 6){
		if(ii > 80)
			speed = 4;
		else if (ii > 60)
			speed = 3;
		else if (ii > 40)
			speed = 2;
		else if (ii > 20)
			speed = 1;
		else
			speed = 0;
	}
	else if(level == 5){
		if(ii > 85)
			speed = 5;
		else if (ii > 70)
			speed = 4;
		else if (ii > 55)
			speed = 3;
		else if (ii > 40)
			speed = 2;
		else if (ii > 25)
			speed = 1;
		else
			speed = 0;
	}
	else if(level == 4 || level == 3){
		if(ii > 88)
			speed = 6;
		else if (ii > 76)
			speed = 5;
		else if (ii > 64)
			speed = 4;
		else if (ii > 52)
			speed = 3;
		else if (ii > 40)
			speed = 2;
		else if (ii > 28)
			speed = 1;
		else
			speed = 0;
	}
		else if(level == 2){
		if(ii > 89)
			speed = 8;
		else if (ii > 78)
			speed = 7;
		else if (ii > 67)
			speed = 6;
		else if (ii > 56)
			speed = 5;
		else if (ii > 45)
			speed = 4;
		else if (ii > 34)
			speed = 3;
		else if (ii > 23)
			speed = 2;
		else if (ii > 12)
			speed = 1;
		else
			speed = 0;
	}
	if(level == 1){
		if(ii > 90)
				speed = 10;
		else if(ii > 80)
				speed = 9;
		else if(ii > 70)
				speed = 8;
		else if(ii > 60)
				speed = 7;
		else if(ii > 50)
				speed = 6;
		else if(ii > 40)
				speed = 5;
		else if(ii > 30)
				speed = 4;
		else if(ii > 20)
				speed = 3;
		else if(ii > 10)
				speed = 2;
		else if(ii > 0)
				speed = 1;
			else if(ii == 0)
				speed = 0;
	}
//	heart = speed ;
}
void cal_score(){
	tmp_level_score += (level + speed) * 10;
	score += (level + speed) * 10;
	char rec[5];
	rec[0] = 'u';
	rec[1] = 's';
	HAL_UART_Transmit(&huart2, rec, 2, 0xfff);
	memset(rec, 0, sizeof(rec));
	sprintf(rec, "%d", score);
	HAL_UART_Transmit(&huart2, rec, 5, 0xFFF);
}
void pin_reset(){
		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_4 , GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5 , GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_6 , GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_7 , GPIO_PIN_RESET);
}
void pin_set(){
		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_4 , GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5 , GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_6 , GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_7 , GPIO_PIN_SET);
}
void move_checker(){
	if(*map[car_x][car_y] == '5'){
		heart --;
		temp_level_lost = 1;
		score -= tmp_level_score;
	}
	
	char rec[2];
	rec[0] = 'u';
	rec[1] = 'h';
	HAL_UART_Transmit(&huart2, rec, 2, 2);
	memset(rec, 0, sizeof(rec));
	sprintf(rec, "%d", heart);
	HAL_UART_Transmit(&huart2, rec, 1, 2);
	memset(rec, 0, sizeof(rec));
	
		if(heart == 0){
			transmit_game_over_data();
			temp_game_over = 1;
			
		}
}
void sort(int arr[], int n){
   int i, key, j;
   for (i = 1; i < n; i++)
   {
       key = arr[i];
       j = i-1;
       while (j >= 0 && arr[j] > key)
       {
           arr[j+1] = arr[j];
           j = j-1;
       }
       arr[j+1] = key;
   }
}
int check(){
	
	int side = 0;
	for(int i=0; i< 15; i++){
		
		if(*map[i+1][side] != ' '){
			
				if(side == 0){				
					side++;
					if(*map[i][side] == '5')
						return 0;
				}
				else if(side == 1){
					
					side--;
					if(*map[i][side] == '5' )
						return 0;
				}
		}
		
	}
	return 1;
}
void enemy_maker(){

	while(1){		
		for(int i=0; i<14; i++)
			enemy_list[i] = i;
		for(int i=0; i<10; i++)
			selecteed_enemy[i] = 0;

			
			int select = 0;
		for(int i=0; i<level; i++){
			int rnd;

			rnd = rand() % (11-select);
			selecteed_enemy[select] = enemy_list[rnd]*10;
			selecteed_enemy[select++] += rand() %2;

			for(int j=rnd+1; j<14-select+1; j++)
				enemy_list[j-1] = enemy_list[j]; 
			
			if(select-1 != 0){

				for(int j=0; j<select-1; j++){
					int last = (selecteed_enemy[select-1]/10);
					if(
						last - 1 == selecteed_enemy[j] / 10 || 
						last + 1 == selecteed_enemy[j] / 10){
							last *= 10;
						 	if(selecteed_enemy[j]%10 == 1)
								last += 1;
							else if(selecteed_enemy[j]%10 == 0)
								last += 0;
							selecteed_enemy[select-1] = last;
					}
				}
			}
		}		

		sort(selecteed_enemy, 10);
		for(int i=0; i<16; i++)
			for(int j=0; j<2; j++)
				map[i][j] = " ";
		if(temp_infinit_level == 0)
			for(int i=10-level; i<10; i++)
				map[selecteed_enemy[i]/10][selecteed_enemy[i]%10] = "5";
		
		if(temp_infinit_level == 1)
			for(int i=10-level; i<10; i++)
				map[(selecteed_enemy[i]/10)+3][selecteed_enemy[i]%10] = "5";
		
		if(check())
			break;
	}

	setCursor(0,0);
	for(int i=0; i<15; i++)
		if(*map[i][0] == '5')
//			print(map[i][0]);
			write((1));
		else 
			print(" ");

	setCursor(0,1);
	for(int i=0; i<15; i++)
		if(*map[i][1] == '5')
//				print(map[i][1]);
				write((1));
			else 
				print(" "); 	
}
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc4;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim8;
extern UART_HandleTypeDef huart2;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI line0 interrupt.
*/
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */
	if(temp_start_with_blue_btn == 0 && temp_game_over == 0){
		tmp_level_score = 0;
		transmit_lvl();
		enemy_maker();
		temp_start_with_blue_btn = 1;
	}
	if(temp_level_finish == 2){
		tmp_level_score = 0;
		level++;
		transmit_lvl();
		enemy_maker();
		temp_level_finish = 0;
	}
	
	else{
		if(temp_game_over == 0){
			if(temp_game_pause == 0){
				
			setCursor(car_x, car_y);
			print(" ");
			
			if(car_y == 0)
				car_y =1;
			else if(car_y == 1)
				car_y =0;
			
			setCursor(car_x, car_y);
			write((0));
			move_checker();
			
			}
		}
	}	
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0));
  /* USER CODE END EXTI0_IRQn 1 */
}

/**
* @brief This function handles EXTI line2 and Touch Sense controller.
*/
void EXTI2_TSC_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_TSC_IRQn 0 */
//		pin_reset();	
//		if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
//				level = level;
//			while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
//			}
//			}	
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_4 , GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5 , GPIO_PIN_SET);
//			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
//				level = 3;
//				while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){}
//			}
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5 , GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_6 , GPIO_PIN_SET);
//			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
//				level = 6;
//				while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){}
//			}
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_6 , GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_7 , GPIO_PIN_SET);
//			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
//				level = 9;
//				while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){}
//			}	
//		pin_set();
  /* USER CODE END EXTI2_TSC_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_TSC_IRQn 1 */

  /* USER CODE END EXTI2_TSC_IRQn 1 */
}

/**
* @brief This function handles EXTI line3 interrupt.
*/
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */
//			pin_reset();
//		if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){
////				level = 0;
//				while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){}
//			}	
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_4 , GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5 , GPIO_PIN_SET);
//			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){
//				level = 2;
//				while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){}
//			}
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5 , GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_6 , GPIO_PIN_SET);
//			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){
//				level = 5;
//				while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){}
//			}
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_6 , GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_7 , GPIO_PIN_SET);
//			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){
//				level = 8;
//				while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){}
//			}	
//		pin_set();
  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */
	
  /* USER CODE END EXTI3_IRQn 1 */
}

/**
* @brief This function handles EXTI line4 interrupt.
*/
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */
//		pin_reset();
//			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)){
//					level = 0;
//					while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)){}
//					
//			}	
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_4 , GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5 , GPIO_PIN_SET);
//			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)){
//				level = 1;
//				while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)){}
//			}
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5 , GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_6 , GPIO_PIN_SET);
//			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)){
//				level = 4;
//				while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)){}
//			}
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_6 , GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_7 , GPIO_PIN_SET);
//			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)){
//				level = 7;
//				while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)){}
//			}	
//		pin_set();
  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
* @brief This function handles ADC1 and ADC2 interrupts.
*/
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */
	int BOO, ii;
  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc2);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */
	BOO = HAL_ADC_GetValue(&hadc2); 
	ii = (100*BOO)/64;
	ii = 100 - ii;
	light_percent= ii;
	
	if(temp_level_finish == 0 && temp_start_with_blue_btn == 1){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
		
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
		
			if(heart >= 1)
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ii);
			if(heart >= 2)
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, ii);
			if(heart >= 3)
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, ii);
			if(heart >= 4)
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ii);
		
			
			if(heart >= 5)
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, ii);
			if(heart >= 6)
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, ii);
			if(heart >= 7)
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, ii);
			if(heart >= 8)
				__HAL_TIM_SET_COMPARE	(&htim3, TIM_CHANNEL_4, ii);
		
		}
		
	HAL_ADC_Start_IT(&hadc2);
  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
* @brief This function handles TIM2 global interrupt.
*/
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */
		if(temp_start_micro_time == 0){
			timeee.Hours = rand()%24;
			timeee.Minutes = rand()%60;
			timeee.Seconds = rand()%60;
			HAL_RTC_SetTime(&hrtc, &timeee, RTC_FORMAT_BIN);	
			car_y = rand()%2;

			temp_start_micro_time = 1;
			 createChar(0, my_car);
			 createChar(1, enemy);
		}
		
		if(temp_game_over == 0){
			if(temp_game_pause == 0){
				if(temp_start_with_blue_btn == 1){
					if(temp_level_finish == 0){
						
						if (count++ % (10 + (11 - level - speed)) == 0){
							if(temp_infinit_level == 0){
								if(car_x > 0){
									setCursor(car_x, car_y);
									print(" ");
									car_x--;					
									setCursor(car_x, car_y);
									write((0));
									move_checker();
									char rec[3];
//									rec[0] = 'p';
//									rec[1] = 's';
//									HAL_UART_Transmit(&huart2, rec, 2, 2);
//									memset(rec, 0, sizeof(rec));
									
									rec[0] = 'l';
									rec[1] = 'p';
									HAL_UART_Transmit(&huart2, rec, 2, 2);
									memset(rec, 0, sizeof(rec));
									sprintf(rec, "%d", light_percent);
									HAL_UART_Transmit(&huart2, rec, 3, 2);
									memset(rec, 0, sizeof(rec));
								}
								else{
									temp_level_finish = 1;
									char rec[2];
									rec[0] = 's';
									rec[1] = 's';
									HAL_UART_Transmit(&huart2, rec, 2, 2);


									if(level == 10){
										temp_game_over = 1;
										transmit_game_over_data();
										
									}
									car_x = 16;							
									car_y = rand()%2;
								}	
							}		
							if(temp_infinit_level == 1){
								
									if(temp_infinit_level_dir == 0){
											if(car_x > 0){
												setCursor(car_x, car_y);
												print(" ");
												car_x--;					
												setCursor(car_x, car_y);
												write((0));
												move_checker();					
											}
											if(car_x == 0){
												enemy_maker();
												car_x = 0;
												car_y = rand() % 2;
												temp_infinit_level_dir = 1;
											}
										}
									if(temp_infinit_level_dir == 1){
											if(car_x < 16){
												setCursor(car_x, car_y);
												print(" ");
												car_x++;					
												setCursor(car_x, car_y);
												write((0));
												move_checker();					
											}
											if(car_x == 16){
												enemy_maker();
												car_x = 16;
												car_y = rand() % 2;
												temp_infinit_level_dir = 0;
											}
										}
									
								
							
						}
							cal_score();
							char rec[3];
							rec[0] = 'l';
							rec[1] = 'p';
							HAL_UART_Transmit(&huart2, rec, 2, 2);
							memset(rec, 0, sizeof(rec));
							sprintf(rec, "%d", light_percent);
							HAL_UART_Transmit(&huart2, rec, 3, 2);
							memset(rec, 0, sizeof(rec));
					}
				}
			}
		}
	}
		
		
  /* USER CODE END TIM2_IRQn 1 */
}

/**
* @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
*/
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */
	
	if(data[0] == 'r' && data[1] == 'e' && data[2] == 's'){	
			temp_game_over = 1;
			transmit_game_over_data();		
			memset(data, 0, sizeof(data));
	}
	else if(data[0] == 'l' && data[1] == 'r' && data[2] == 'b'){
			setCursor(car_x, car_y);
				print(" ");
				
				if(car_y == 0)
					car_y =1;
				else if(car_y == 1)
					car_y =0;
				
				setCursor(car_x, car_y);
				write((0));
				move_checker();
			
			memset(data, 0, sizeof(data));
	}
	else if(data[0] == 'p' && data[1] == 'l' && data[2] == 'y'){
			temp_game_pause = 0;
			memset(data, 0, sizeof(data));
	}
	else if(data[0] == 'p' && data[1] == 'a' && data[2] == 's'){
		temp_game_pause = 1;
		memset(data, 0, sizeof(data));
	}
	else if(data[0] == 's' && data[1] == 'p' && data[2] == 'd'){
		cal_speed(data[3] * 10);
		memset(data, 0, sizeof(data));
	}
	else if(data[0] == 's' && data[1] == 'f' && data[2] == 'l'){
			level = data[3];
			memset(data, 0, sizeof(data));
	}
	else if(data[0] == 's' && data[1] == 'i' && data[2] == 'l'){
			temp_infinit_level = 1;
			level = 10;
			memset(data, 0, sizeof(data));
	}
	else if(data[0] == 'n' && data[1] == 's' && data[2] == 't'){
			temp_call_enemy_maker = 0;
			temp_start_micro_time = 0;

			temp_start_with_blue_btn = 0;

			temp_level_finish = 0;
			temp_level_lost = 0;
			temp_game_over = 0;
			temp_game_pause = 0;
			temp_key_pad_confirmd = 0;
			score = 0;
			level = 1;
			heart = 8;
			start = 0;	
			car_x = 16;
			temp_infinit_level = 0;
			temp_infinit_level_dir = 0;
		
			memset(data, 0, sizeof(data));
	}
	else if(data[0] == 's' && data[1] == 't' && data[2] == 'r'){
		tmp_level_score = 0;
		if(temp_start_with_blue_btn == 0 && temp_game_over == 0){
			transmit_lvl();
			enemy_maker();
			temp_start_with_blue_btn = 1;
		}
		if(temp_level_finish == 2){
			level++;
			transmit_lvl();
			enemy_maker();
			temp_level_finish = 0;
			
		}
		memset(data, 0, sizeof(data));
	}
	
//	for(int i=0; i<16; i++){

//		if( data[i] == 13){
//			if(data[0] == 'p' && data[1] == 'l' && data[2] == 'a' && data[3] == 'y')
//				temp_game_pause = 0;	
//				
//			if(data[0] == 'r' && data[1] == 'e' && data[2] == 's' && data[3] == 'e' && data[4] == 't')
//				temp_game_over = 1;
//			
//			if(data[0] == 'p' && data[1] == 'a' && data[2] == 'u' && data[3] == 's' && data[4] == 'e')
//				temp_game_pause = 1;
//			
//			
//			if(temp_game_over == 2){
//				for(int j=0; j<i; j++)
//					name[j] = data[j];
//				
//				HAL_UART_Transmit(&huart2, name, sizeof(name), 3);
//				
//				edit_high_score();
//				show_high_score();
//				temp_game_over = 3;
//			}
//			memset(data, 0, sizeof(data));
//		}
//			
//	}	
	
	
	HAL_UART_Receive_IT(&huart2,  data, sizeof(data));
  /* USER CODE END USART2_IRQn 1 */
}

/**
* @brief This function handles TIM8 break global interrupt.
*/
void TIM8_BRK_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_BRK_IRQn 0 */

  /* USER CODE END TIM8_BRK_IRQn 0 */
  HAL_TIM_IRQHandler(&htim8);
  /* USER CODE BEGIN TIM8_BRK_IRQn 1 */

  /* USER CODE END TIM8_BRK_IRQn 1 */
}

/**
* @brief This function handles TIM8 update interrupt.
*/
void TIM8_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_UP_IRQn 0 */

  /* USER CODE END TIM8_UP_IRQn 0 */
  HAL_TIM_IRQHandler(&htim8);
  /* USER CODE BEGIN TIM8_UP_IRQn 1 */

  /* USER CODE END TIM8_UP_IRQn 1 */
}

/**
* @brief This function handles TIM8 trigger com interrupt.
*/
void TIM8_TRG_COM_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_TRG_COM_IRQn 0 */

  /* USER CODE END TIM8_TRG_COM_IRQn 0 */
  HAL_TIM_IRQHandler(&htim8);
  /* USER CODE BEGIN TIM8_TRG_COM_IRQn 1 */

  /* USER CODE END TIM8_TRG_COM_IRQn 1 */
}

/**
* @brief This function handles TIM8 capture compare interrupt.
*/
void TIM8_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_CC_IRQn 0 */

  /* USER CODE END TIM8_CC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim8);
  /* USER CODE BEGIN TIM8_CC_IRQn 1 */
	counter ++;

	
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_RESET);//1
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);//2
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);//3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);//4
	
	if(temp_start_with_blue_btn == 0 && start == 0){
			setCursor(0 ,0);
			print("               ");
			setCursor(0 ,1);
			print("               ");
			setCursor(2 ,0);
			print("Press To Go");
			start = 1;
	}
	if(temp_start_with_blue_btn == 0 || temp_level_finish != 0 ){

		
			if(counter % 200 == 0){
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);//a
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);//b
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);//c	
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);//d
				
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);//e	
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);//f
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//g
				
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_RESET);//1
				HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);//2
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);//3
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);//4
				
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 100);
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 100);
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 100);
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 100);
				
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 100);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 100);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 100);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 100);
			}
			if(counter % 200 == 100){

				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_SET);//1
				HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);//2
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);//3
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);//4
				
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
			
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
			}
		
	}

	if(temp_game_over == 0 && temp_start_with_blue_btn == 1 && temp_level_finish != 2){
		if(counter % 3 == 1){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			
			
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);//a
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);//b
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);//c	
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);//d
			
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);//e	
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);//f
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//g
		}
			if(counter % 3 == 2){
				HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
			
			
				if(level / 10 == 0){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);//g
				}
				if(level / 10 == 1){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);//g
				}
		}
			if(counter % 3 == 0){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);
			
			if(level % 10 == 0){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);//g
				}
				if(level % 10 == 1){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);//g
				}
				if(level % 10 == 2){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//g
				}
					if(level % 10 == 3){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//g
				}
				if(level % 10 == 4){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//g
				}
				
				if(level % 10 == 5){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//g
				}

				if(level % 10 == 6){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//g
				}

				if(level % 10 == 7){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);//g
				}	
				if(level % 10 == 8){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//g
				}
				if(level % 10 == 9){
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);//d
					
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);//e	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//f
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//g
				}			
		}
	}
	if(temp_game_over == 1){
		if(counter % 4 == 0){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_SET);//1
			
			
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);//d
			
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);//e	
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//f
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//g
		}
		if(counter % 4 == 1){
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);//2
			
			
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);//a
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);//d
			
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);//e	
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//f
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);//g
		}
		
	
		if(counter % 4 == 2){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);//3
			
			
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);//b
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);//c	
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);//d
			
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);//e	
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//f
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//g
		}
		if(counter % 4 == 3){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);//4
			
			
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);//a
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//b
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//c	
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);//d
			
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);//e	
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//f
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);//g
		}
			
		
	}
	
	if(counter == 99999996)
			counter = 0;
	if(temp_level_lost == 1){
		tmp_level_score = 0;
		enemy_maker();
		if(temp_infinit_level == 1)
			if(temp_infinit_level_dir == 0)
				car_x = 16;							
			else
				car_x = 0;							
		else
			car_x = 16;							
		car_y = rand()%2;
		temp_level_lost = 0;
		
	}
	if(temp_level_finish == 1){
		tmp_level_score = 0;
		temp_level_finish = 2;	
		setCursor(0 ,0);
		print("               ");
		setCursor(0 ,1);
		print("               ");
		setCursor(1 ,0);
		char rec[15];
		sprintf(rec, "Lvl %d Finished", level);
		print(rec);
		setCursor(2 ,1);
		sprintf(rec, "Tap To Cont");
		print(rec);
	}

		
	
  /* USER CODE END TIM8_CC_IRQn 1 */
}

/**
* @brief This function handles ADC4 interrupt.
*/
void ADC4_IRQHandler(void)
{
  /* USER CODE BEGIN ADC4_IRQn 0 */
	int ii, BOO;
	
  /* USER CODE END ADC4_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc4);
  /* USER CODE BEGIN ADC4_IRQn 1 */
	BOO = HAL_ADC_GetValue(&hadc4); 
	ii = (100*BOO)/64;

//	cal_speed(ii);
//	setCursor(0,0);
//	char* demo;
//	sprintf(demo, "%d", BOO);
//	print(demo);
	
	
	HAL_ADC_Start_IT(&hadc4);

  /* USER CODE END ADC4_IRQn 1 */
}

/* USER CODE BEGIN 1 */
	
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
