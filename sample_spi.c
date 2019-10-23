/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#include "main.h"
#include "stm32f4xx.h"
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "main.h"
#include "lcdheader.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/

# define CD GPIO_PIN_10
# define CS GPIO_PIN_11

# define CD_PORT GPIOA
# define CS_PORT GPIOA
/* Private variables ---------------------------------------------------------*/
//SPI_HandleTypeDef hspi2;

#define HSPI_INSTANCE &hspi2
#define BURST_MAX_SIZE 	500
unsigned char data;
/* USER CODE BEGIN PD */
 # define write8inline(d) {			\
		data = (uint8_t)(d&0xFF);\
		HAL_SPI_Transmit(HSPI_INSTANCE, &data, 1, 1) ;						\
}

# define write8 write8inline
// data write
# define write8DataInline(d) {	\
	HAL_GPIO_WritePin(CD_PORT, CD, GPIO_PIN_SET); 					\
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET); 					\
	write8(d);					\
}
// command or register write
# define write8RegInline(d) {	\
	HAL_GPIO_WritePin(CD_PORT, CD, GPIO_PIN_RESET); 					\
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET); 					\
	write8(d);					\
}

// inline definitions
# define write8Reg write8RegInline
# define write8Data write8DataInline




u16 cursor_x, cursor_y;  /// cursor_y and cursor_x globals
u8 textsize, rotation; /// textsize and rotation
u16
    textcolor,      ////< 16-bit background color for print()
    textbgcolor;    ////< 16-bit text color for print()
u16
    _width,         ////< Display width as modified by current rotation
    _height;        ////< Display height as modified by current rotation
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */


/* USER CODE BEGIN PV */

/* USER CODE END PV */


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */


void writeRegister8(u8 a, u8 d){
	write8Reg(a);
	write8Data(d);
}
void writeRegister16(u16 a, u16 d){
	uint16_t hi, lo;
 	hi = (a) >> 8;
 	lo = (a);

 	write8Reg(hi);
 	write8Reg(lo);
  	hi = (d) >> 8;
  	lo = (d);

  	write8Data(hi);
  	write8Data(lo);
//	IOM =1;
}





void setCursor(u16 x, u16 y){
	cursor_x = x;
	cursor_y = y;
}
// set text color
void setTextColor(u16 x, u16 y){
	textcolor =  x;
	textbgcolor = y;
}

// set text size
void setTextSize(u8 s){
	if (s > 8) return;
	textsize = (s>0) ? s : 1 ;
}

// set address definition
void setAddress(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2){
	//IOM =0;
	write8Reg(0x2A);
	write8Data(x1 >> 8);
	write8Data(x1);
	write8Data(x2 >> 8);
	write8Data(x2);

	write8Reg(0x2B);
	write8Data(y1 >> 8);
	write8Data(y1);
	write8Data(y2 >> 8);
	write8Data(y2);
	//write8Reg(0x2C);
  //IOM =1;


}

void TFT_LCD_INIT(void){
	//char ID[5];
	///int id;
	_width = TFTWIDTH;
	_height = TFTHEIGHT;

	HAL_GPIO_WritePin(CD_PORT, CD, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_SET);

	//RST = 0;
	//Hal_Delay(50);
	//RST = 1;
	write8Reg(0x00);
	write8Data(0x00);write8Data(0x00);write8Data(0x00);
	//IOM = 1;
	HAL_Delay(200);

	//IOM = 0;

	writeRegister8(ILI9341_SOFTRESET, 0);
    HAL_Delay(50);
    writeRegister8(ILI9341_DISPLAYOFF, 0);
    HAL_Delay(10);






    writeRegister8(ILI9341_POWERCONTROL1, 0x23);
    writeRegister8(ILI9341_POWERCONTROL2, 0x11);
    write8Reg(ILI9341_VCOMCONTROL1);
	write8Data(0x3d);
	write8Data(0x30);
    writeRegister8(ILI9341_VCOMCONTROL2, 0xaa);
    writeRegister8(ILI9341_MEMCONTROL, ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
    write8Reg(ILI9341_PIXELFORMAT);
	write8Data(0x55);write8Data(0x00);
    writeRegister16(ILI9341_FRAMECONTROL, 0x001B);

    writeRegister8(ILI9341_ENTRYMODE, 0x07);
    /* writeRegister32(ILI9341_DISPLAYFUNC, 0x0A822700);*/



    writeRegister8(ILI9341_SLEEPOUT, 0);
    HAL_Delay(150);
    writeRegister8(ILI9341_DISPLAYON, 0);
    HAL_Delay(500);
		setAddress(0,0,_width-1,_height-1);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_SET);
     ///************* Start Initial Sequence ILI9341 controller **********///

	 // IOM = 1;
}
void fillRect(u16 x,u16 y,u16 w,u16 h,u16 color){
	if ((x >= TFTWIDTH) || (y >= TFTHEIGHT))
	{
		return;
	}

	if ((x+w-1) >= TFTWIDTH)
	{
		w = TFTWIDTH-x;
	}

	if ((y+h-1) >= TFTHEIGHT)
	{
		h = TFTHEIGHT-y;
	}

	setAddress(x, y, x+w-1, y+h-1);
    //IOM = 0;


	write8Reg(0x2C);
	//IOM = 1; IOM = 0;
	HAL_GPIO_WritePin(CD_PORT, CD, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
	for(y=h; y>0; y--)
	{
		for(x=w; x>0; x--)
		{

			write8(color>>8); write8(color);

		}
	}
	//IOM = 1;
}

void drawPixel(u16 x3,u16 y3,u16 color1)
{

	// not using to speed up
	//if ((x3 < 0) ||(x3 >= TFTWIDTH) || (y3 < 0) || (y3 >= TFTHEIGHT))
	//{
	//	return;
	//}
	setAddress(x3,y3,x3+1,y3+1);

	//IOM = 0;

    write8Reg(0x2C);


	write8Data(color1>>8);write8Data(color1);
	//IOM = 1;
}

void drawChar(int x, int y, unsigned char c,u16 color, u16 bg, u8 size){
	if ((x >=TFTWIDTH) || // Clip right
	    (y >=TFTHEIGHT)           || // Clip bottom
	    ((x + 6 * size - 1) < 0) || // Clip left
	    ((y + 8 * size - 1) < 0))   // Clip top
	{
		return;
	}

	for (char i=0; i<6; i++ )
	{
		u8 line;

		if (i == 5)
		{
			line = 0x0;
		}
		else
		{
			line = pgm_read_byte(font+(c*5)+i);
		}

		for (char j = 0; j<8; j++)
		{
			if (line & 0x1)
			{
				if (size == 1) // default size
				{
					drawPixel(x+i, y+j, color);
				}
				else {  // big size
					fillRect(x+(i*size), y+(j*size), size, size, color);
				}
			} else if (bg != color)
			{
				if (size == 1) // default size
				{
					drawPixel(x+i, y+j, bg);
				}
				else
				{  // big size
					fillRect(x+i*size, y+j*size, size, size, bg);
				}
			}

			line >>= 1;
		}
	}

}
void write(u8 c)//write a character at setted coordinates after setting location and colour
{
	if (c == '\n')
	{
		cursor_y += textsize*8;
		cursor_x  = 0;
	}
	else if (c == '\r')
	{
		// skip em
	}
	else
	{
		drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
		cursor_x += textsize*6;
	}
}
void LCD_string_write(char *str)
{
	int i;
	for(i=0;str[i]!=0;i++)	/* Send each char of string till the NULL */
	{
		write(str[i]);	/* Call transmit data function */
	}
}

void fillScreen(unsigned int Color){
	//unsigned char VH,VL;
	uint32_t len = (uint32_t)TFTWIDTH * (uint32_t)TFTHEIGHT;

	 int blocks;

   unsigned char  i, hi = Color >> 8,
              lo = Color;

    blocks = (u16)(len / 64); // 64 pixels/block
	setAddress(0,0,TFTWIDTH-1,TFTHEIGHT-1);

	//IOM = 0;


	write8Reg(0x2C);
	//IOM = 1; IOM = 0;
	HAL_GPIO_WritePin(CD_PORT, CD, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
		write8(hi); write8(lo);

		len--;
		while(blocks--) {
      i = 16; // 64 pixels/block / 4 pixels/pass
      do {

				write8(hi); write8(lo);write8(hi); write8(lo);
				write8(hi); write8(lo);write8(hi); write8(lo);
      } while(--i);
    }
    for(i = (char)len & 63; i--; ) {

      write8(hi); write8(lo);

    }

	//IOM = 1;

}

void fillScreenv2(unsigned int Color){
	//unsigned char VH,VL;
	//SENDS COLOUR
	uint32_t len = (uint32_t)TFTWIDTH * (uint32_t)TFTHEIGHT;
	uint32_t Buffer_Size = 0;
	if((len*2) < BURST_MAX_SIZE)
	{
		Buffer_Size = len;
	}
	else
	{
		Buffer_Size = BURST_MAX_SIZE;
	}

	//IOM = 0;

	setAddress(0,0,TFTWIDTH-1,TFTHEIGHT-1);
	write8Reg(0x2C);
	//IOM = 1; IOM = 0;
	HAL_GPIO_WritePin(CD_PORT, CD, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
	unsigned char chifted = 	Color>>8;;
	unsigned char burst_buffer[Buffer_Size];
	for(uint32_t j = 0; j < Buffer_Size; j+=2)
		{
			burst_buffer[j] = 	chifted;
			burst_buffer[j+1] = Color;
		}

	uint32_t Sending_Size = len*2;
	uint32_t Sending_in_Block = Sending_Size/Buffer_Size;
	uint32_t Remainder_from_block = Sending_Size%Buffer_Size;

	if(Sending_in_Block != 0)
	{
		for(uint32_t j = 0; j < (Sending_in_Block); j++)
			{
			HAL_SPI_Transmit(HSPI_INSTANCE, (unsigned char *)burst_buffer, Buffer_Size, 10);
			}
	}

	//REMAINDER!
	HAL_SPI_Transmit(HSPI_INSTANCE, (unsigned char *)burst_buffer, Remainder_from_block, 10);

	//IOM = 1;
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_SET);
}
void drawCircle(int x0, int y0, int r, u16 color){
	int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;


    drawPixel(x0  , y0+r, color);
    drawPixel(x0  , y0-r, color);
    drawPixel(x0+r, y0  , color);
    drawPixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void testCircles(u8 radius, u16 color) {

  int  x, y, r2 = radius * 2, w = _width  + radius, h = _height + radius;


  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      drawCircle(x, y, radius, color);
    }
  }


}
void writeSomeLines(){
	fillScreenv2(RED);

	HAL_Delay(20);
	fillScreenv2(GREEN);

	HAL_Delay(20);

    //fillScreen(BLACK);
    fillScreenv2(BLACK);
	testCircles(20, RED);
	HAL_Delay(100);
    fillScreenv2(BLACK);
	setTextColor(RED, BLACK);
	setTextSize(5);
	LCD_string_write("Welcome\n");
	setTextColor(RED, BLACK);
	setTextSize(3);
	LCD_string_write("ECEN-4330\n");
	HAL_Delay(200);

	fillScreenv2(GRAY);
	setTextSize(4);
	setTextColor(BLACK, GRAY);
	setCursor(0, 0);
	LCD_string_write("  --MENU--\n");

	setTextSize(3);
	LCD_string_write(" D: DUMP\n");
	LCD_string_write(" B: MOVE\n");
	LCD_string_write(" E: EDIT\n");
	LCD_string_write(" F: FIND\n");

	setTextSize(1);
	LCD_string_write(" Press the keys to start memory \n operations\n");
}
unsigned int reverse(unsigned char d) {
	unsigned int rev = 0;
	unsigned int val = 0;
	while(d >= 1){

		val = d%10;
		d = d/10;
		rev = rev * 10 + val;
	}
	return rev;
}
void asciiToDec(unsigned char d) {
	unsigned char val;
	unsigned int id;
	id = reverse(d);
	while (id >= 1){

		val = id % 10;
		id = id/10;
		write(val + '0');
	}
	write('\n');
}
void asciiToHex(unsigned char d) {
	unsigned char val;
	unsigned char store[2];
	unsigned char i =0;
	store[0] = 0;
	store[1] = 0;
	while (d >= 1){

		val = d % 16;
		d = d/16;
		if (val <= 9) {

			store[i] = val + '0';
		}
		else {
			store[i] = (val%10) + 'A';
		}
		i++;
	}
	write(store[1]);
	write(store[0]);
	//write('\n');
}
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();

  TFT_LCD_INIT();
  fillScreenv2(GREEN);
  HAL_Delay(500);
  fillScreenv2(RED);
  HAL_Delay(500);
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  writeSomeLines();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA10 PA11 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
