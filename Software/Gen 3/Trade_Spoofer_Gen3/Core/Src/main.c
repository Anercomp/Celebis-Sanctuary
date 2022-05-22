/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DataOutPin GPIO_PIN_7 	//PA7
#define DataInPin GPIO_PIN_5	//PB5
#define SO_Pin GPIO_PIN_7		//PB7
#define SC_Pin GPIO_PIN_6		//PB6

#define LINKCMD_SEND_LINK_TYPE 0x2222
#define LINKCMD_0x2FFE             0x2FFE
#define LINKCMD_SEND_HELD_KEYS     0x4444
#define LINKCMD_0x5555             0x5555
#define LINKCMD_0x5566             0x5566
#define LINKCMD_OPEN_TRADE_SCREEN  0x5FFF
#define LINKCMD_0x6666             0x6666
#define LINKCMD_0x7777             0x7777
#define LINKCMD_COUNTDOWN          0x7FFF
#define LINKCMD_CONT_BLOCK         0x8888
#define LINKCMD_0xAAAA             0xAAAA
#define LINKCMD_0xAAAB             0xAAAB
#define LINKCMD_INIT_BLOCK         0xBBBB
#define LINKCMD_SEND_HELD_KEYS_2   0xCAFE
#define LINKCMD_0xCCCC             0xCCCC

#define LINKTYPE_TRADE              0x1111  // trade
#define LINKTYPE_0x1122              0x1122  // trade
#define LINKTYPE_0x1133              0x1133  // trade
#define LINKTYPE_0x1144              0x1144  // trade
#define LINKTYPE_BATTLE              0x2211
#define LINKTYPE_0x2222              0x2222  // unused battle?
#define LINKTYPE_SINGLE_BATTLE       0x2233
#define LINKTYPE_DOUBLE_BATTLE       0x2244
#define LINKTYPE_MULTI_BATTLE        0x2255
#define LINKTYPE_BATTLE_TOWER_50     0x2266
#define LINKTYPE_BATTLE_TOWER_OPEN   0x2277
#define LINKTYPE_BATTLE_TOWER        0x2288
#define LINKTYPE_0x3311              0x3311
#define LINKTYPE_0x3322              0x3322
#define LINKTYPE_BERRY_BLENDER_SETUP 0x4411
#define LINKTYPE_CONTEST_GMODE       0x6601

#define MASTER_HANDSHAKE 0x8FFF
#define SLAVE_HANDSHAKE  0xB9A0

#define IDLE	0x0011
#define DOWN	0x0012
#define UP		0x0013
#define LEFT	0x0014
#define RIGHT	0x0015

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define LED_ON GPIOC->BRR = GPIO_PIN_13;
#define LED_OFF GPIOC->BSRR = GPIO_PIN_13;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
const uint16_t ticksPerBit = 625;
uint16_t zeroPackets = 0;
uint8_t timerPacketFlag = 0;
uint8_t flags = 0;	// Bit 0: received linkstart / Bit 1: Received zero block init 1 / Bit 2: Received zero block init 2
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void send16Bit(uint16_t v);
uint16_t receive16Bit();
void initLink();
void waitForUnionRoom();
void waitForTeamData();
void sendBlock(uint16_t* blockdata);
void sendZeroBlock();
void sendZero();
void sendContBlock(uint16_t* blockdata);
void copyBlock();
void handleUnionRoomMovement();
void sendMoveBlock(uint16_t dir);
void setPB5Output();
void setPB5Input();

void loopOfDespair();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
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
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*
	  //--------------------------------DON'T TOUCH! WORKS!------------------------
	  GPIOA->BSRR = DataOutPin;
	  while(receive16Bit() != 0xB9A0 || 0x8FFF);
	  GPIOB->BSRR = SC_Pin;
	  send16Bit(0xB9A0);
	  GPIOB->BRR = SC_Pin;
	  //--------------------------------DON'T TOUCH! WORKS!------------------------
	   */
	  GPIOC->BSRR = GPIO_PIN_13;
	  GPIOB->BRR = GPIO_PIN_6;
	  HAL_Delay(1000);
	  initLink();
	  //Block 1
	  sendZeroBlock();
	  //Block 2
	  uint16_t tempBlockData1[] = {0x0000,0x2222,0x1133,0x0000,0x0000,0x0000,0x0000};
	  sendBlock(tempBlockData1);
	  //Block 3
	  sendZeroBlock();
	  //Block 4
	  sendZeroBlock();
	  //Block 5
	  uint16_t tempBlockData2[] = {0x0000,0xBBBB,0x003C,0x0081,0x0000,0x0000,0x0000,0x0000,0x0000};
	  sendBlock(tempBlockData2);
	  //Block 6
	  sendZeroBlock();
	  //Block 7
	  sendZeroBlock();
	  //Block 8
	  uint16_t tempBlockData3[] = {0x6147,0x656D,0x7246,0x6165,0x206B,0x6E69,0x2E63};
	  sendContBlock(tempBlockData3);
	  //Block 9
	  uint16_t tempBlockData4[] = {0x0000,0x4004,0x8000,0xDD12,0xA92C,0xC6C0,0xCCC9};
	  sendContBlock(tempBlockData4);
	  //Block 10
	  uint16_t tempBlockData5[] = {0xBBC3,0xFFC8,0x0011,0x0000,0x1133,0x0000,0x0000};
	  sendContBlock(tempBlockData5);
	  //Block 11
	  uint16_t tempBlockData6[] = {0x0005,0x6147,0x656D,0x7246,0x6165,0x206B,0x6E69};
	  sendContBlock(tempBlockData6);
	  //Block 12
	  uint16_t tempBlockData7[] = {0x2E63,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
	  sendContBlock(tempBlockData7);
	  //Block 13
	  sendZeroBlock();
	  //Block 14
	  sendZeroBlock();
	  //Block 15
	  sendZeroBlock();
	  //Block 16
	  sendZero();
	  //Block 17
	  sendZeroBlock();
	  //Block 18
	  sendZeroBlock();
	  //Block 19
	  uint16_t tempBlockData8[] = {0x0064,0x0081,0x0000,0x0000,0x0000,0x0000,0x0000};
	  sendContBlock(tempBlockData8);
	  //Block 20
	  sendZeroBlock();
	  //Block 21
	  sendZeroBlock();
	  //Block 22
	  uint16_t tempBlockData9[] = {0x0200,0x0101,0x7200,0x0081,0x0011,0x0036,0x016F};
	  sendContBlock(tempBlockData9);
	  //Block 23
	  uint16_t tempBlockData10[] = {0xDD12,0x03E7,0x003B,0x0004,0x000B,0x0000,0x0000};
	  sendContBlock(tempBlockData10);
	  //Block 24
	  uint16_t tempBlockData11[] = {0x0000,0x0000,0x0154,0x0000,0x0714,0x0001,0x0A01};
	  sendContBlock(tempBlockData11);
	  //Block 25
	  uint16_t tempBlockData12[] = {0x0829,0x2A96,0x0075,0xC6C0,0xCCC9,0xBBC3,0xFFC8};
	  sendContBlock(tempBlockData12);
	  //Block 26
	  uint16_t tempBlockData13[] = {0x2E04,0x0000,0x000B,0x0000,0x0003,0x0000,0x000D};
	  sendContBlock(tempBlockData13);
	  //Block 27
	  uint16_t tempBlockData14[] = {0x0000,0x0000,0x0000,0x0001,0x5C00,0x0101,0x0001};
	  sendContBlock(tempBlockData14);
	  //Block 28
	  uint16_t tempBlockData15[] = {0x0097,0x0096,0x019A,0x00F3,0x00F5,0x00FB,0x0000};
	  sendContBlock(tempBlockData15);
	  //Block 29
	  sendZeroBlock();
	  //Block 30
	  sendZeroBlock();
	  //Block 31
	  sendZeroBlock();
	  //Block 32
	  waitForUnionRoom();

	  //--------------------------------------------------------------------------------------------------------------------------
	  //-----------------------------------------------Inside Union Room----------------------------------------------------------
	  //--------------------------------------------------------------------------------------------------------------------------

	  handleUnionRoomMovement();

	  //--------------------------------------------------------------------------------------------------------------------------
	  //--------------------------------------------------Trade Screen------------------------------------------------------------
	  //--------------------------------------------------------------------------------------------------------------------------
	  LED_ON
	  //waitForTeamData();

	  //TODO Neue Funktion "waitTill" mit wert und position im block als parameter

	  while(1)
	  {
		  copyBlock();
		  struct PokemonSubstruct0
		  {
		      u16 species;
		      u16 heldItem;
		      u32 experience;
		      u8 ppBonuses;
		      u8 friendship;
		  };

		  struct PokemonSubstruct1
		  {
		      u16 moves[4];
		      u8 pp[4];
		  };

		  struct PokemonSubstruct2
		  {
		      u8 hpEV;
		      u8 attackEV;
		      u8 defenseEV;
		      u8 speedEV;
		      u8 spAttackEV;
		      u8 spDefenseEV;
		      u8 cool;
		      u8 beauty;
		      u8 cute;
		      u8 smart;
		      u8 tough;
		      u8 sheen;
		  };

		  struct PokemonSubstruct3
		  {
		   /* 0x00 */ u8 pokerus;
		   /* 0x01 */ u8 metLocation;

		   /* 0x02 */ u16 metLevel:7;
		   /* 0x02 */ u16 metGame:4;
		   /* 0x03 */ u16 pokeball:4;
		   /* 0x03 */ u16 otGender:1;

		   /* 0x04 */ u32 hpIV:5;
		   /* 0x04 */ u32 attackIV:5;
		   /* 0x05 */ u32 defenseIV:5;
		   /* 0x05 */ u32 speedIV:5;
		   /* 0x05 */ u32 spAttackIV:5;
		   /* 0x06 */ u32 spDefenseIV:5;
		   /* 0x07 */ u32 isEgg:1;
		   /* 0x07 */ u32 abilityNum:1;

		   /* 0x08 */ u32 coolRibbon:3;
		   /* 0x08 */ u32 beautyRibbon:3;
		   /* 0x08 */ u32 cuteRibbon:3;
		   /* 0x09 */ u32 smartRibbon:3;
		   /* 0x09 */ u32 toughRibbon:3;
		   /* 0x09 */ u32 championRibbon:1;
		   /* 0x0A */ u32 winningRibbon:1;
		   /* 0x0A */ u32 victoryRibbon:1;
		   /* 0x0A */ u32 artistRibbon:1;
		   /* 0x0A */ u32 effortRibbon:1;
		   /* 0x0A */ u32 marineRibbon:1; // never distributed
		   /* 0x0A */ u32 landRibbon:1; // never distributed
		   /* 0x0A */ u32 skyRibbon:1; // never distributed
		   /* 0x0A */ u32 countryRibbon:1; // distributed during Pokémon Festa '04 and '05 to tournament winners
		   /* 0x0B */ u32 nationalRibbon:1;
		   /* 0x0B */ u32 earthRibbon:1;
		   /* 0x0B */ u32 worldRibbon:1; // distributed during Pokémon Festa '04 and '05 to tournament winners
		   /* 0x0B */ u32 filler:4;
		   /* 0x0B */ u32 eventLegal:1; // controls Mew & Deoxys obedience; if set, Pokémon is a fateful encounter in FRLG & Gen 4+ summary screens; set for in-game event island legendaries, some distributed events, and Pokémon from XD: Gale of Darkness.
		  };

		  union PokemonSubstruct
		  {
		      struct PokemonSubstruct0 type0;
		      struct PokemonSubstruct1 type1;
		      struct PokemonSubstruct2 type2;
		      struct PokemonSubstruct3 type3;
		      u16 raw[6];
		  };
	  }
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  /*
  			case 0x2222: send16Bit(0x0000); break;
  			case 0x8888: send16Bit(0x8888); break;
  			case 0x1133: send16Bit(0x0000); break;
  			case 0x3355: send16Bit(0x3355); zeroPackets = 0; flags |= 0b0010; break;
  			case 0xBC78: send16Bit(0xBC78); break;
  			case 0xBBBB: send16Bit(0x0000); break;
  			case 0x003C: send16Bit(0x0000); break;
  			case 0x0080: send16Bit(0x0000); break;
  			case 0xBC77: send16Bit(0xBC77); zeroPackets = 0; flags |= 0b0100; break;
  			case 0xE01E: send16Bit(0xE01E); break;
  			case 0xCAFE: send16Bit(0xCAFE); break;
  			*/
  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_Out_GPIO_Port, SD_Out_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|SO_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_Out_Pin */
  GPIO_InitStruct.Pin = SD_Out_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SD_Out_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_In_Pin */
  GPIO_InitStruct.Pin = SD_In_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SD_In_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SO_Pin */
  GPIO_InitStruct.Pin = SO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SO_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void setPB5Input()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	  GPIO_InitStruct.Pin = GPIO_PIN_5;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void setPB5Output()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	  GPIO_InitStruct.Pin = GPIO_PIN_5;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void send16Bit(uint16_t v)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	GPIOA->BRR = DataOutPin;
	while(__HAL_TIM_GET_COUNTER(&htim1) < ticksPerBit);
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	for(uint8_t i = 0; i < 16; i++)
	{
		if(1 & v)
		{
			GPIOA->BSRR = DataOutPin;
		}
		else
		{
			GPIOA->BRR = DataOutPin;
		}
		v >>= 1;
		while(__HAL_TIM_GET_COUNTER(&htim1) < ticksPerBit*(i+1));
	}
	GPIOA->BSRR = DataOutPin;
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while(__HAL_TIM_GET_COUNTER(&htim1) < 4000);
}

uint16_t receive16Bit()
{
	uint16_t inBytes = 0xFFFF;
	//while(inBytes == 0xFFFF)
	//{
		while(GPIOB->IDR & DataInPin);
		__HAL_TIM_SET_COUNTER(&htim1, 0);
		while(__HAL_TIM_GET_COUNTER(&htim1) < 939);
		__HAL_TIM_SET_COUNTER(&htim1, 0);
		for(uint8_t i = 0;i < 16; i++)
		{
			GPIOB->ODR ^= GPIO_PIN_5;
			inBytes >>= 1;
			if(GPIOB->IDR & DataInPin)
			{
				inBytes = inBytes | 0x8000;
			}
			while(__HAL_TIM_GET_COUNTER(&htim1) < ticksPerBit*(i+1));
		}
	//}
	return inBytes;
}

void initLink()
{
	while(flags != 1)
	{
		switch(receive16Bit())
		{
			case SLAVE_HANDSHAKE: send16Bit(SLAVE_HANDSHAKE); break;
			case MASTER_HANDSHAKE: send16Bit(SLAVE_HANDSHAKE); flags = 1; break;
			case 0x0000: send16Bit(0x0000); break;
		}
	}
}

void waitForUnionRoom()
{
	uint8_t escapeFlag = 0;
	while(!escapeFlag)
	{
		switch (receive16Bit()) {
			case 0x0000	:	send16Bit(0x0000); break;
			case 0x2FFE :	send16Bit(0x2FFE); escapeFlag = 1; break;
			default		:	break;
		}
	}
	for(uint8_t i = 0; i < 7; i++)
	{
		receive16Bit();
		send16Bit(0x0000);
	}

	for(uint8_t i = 0; i < 30; i++)
	{
		copyBlock();
	}
}

void sendBlock(uint16_t* blockdata)
{
	for(uint8_t i = 0; i != 9; i++)
	{
		receive16Bit();
		send16Bit(blockdata[i]);
	}
}

void waitForTeamData()
{
	  uint16_t temp = 0;
	  while(temp != 0x8888)
	  {
		  temp = receive16Bit();
		  copyBlock();
	  }
}

void sendZeroBlock()
{
	uint16_t temp = receive16Bit();
	send16Bit(temp);
	temp = receive16Bit();
	send16Bit(temp);
	for(uint8_t i = 0; i != 7; i++)
	{
		receive16Bit();
		send16Bit(0x0000);
	}
}

void sendZero()
{
		for(uint8_t i = 0; i != 9; i++)
		{
			receive16Bit();
			send16Bit(0x0000);
		}
}

void sendContBlock(uint16_t* blockdata)
{
	uint16_t temp = receive16Bit();
	send16Bit(temp);
	temp = receive16Bit();
	send16Bit(temp);
	for(uint8_t i = 0; i != 7; i++)
	{
		receive16Bit();
		send16Bit(blockdata[i]);
	}
}

void copyBlock()
{
	uint16_t temp = 0;
	for(uint8_t i = 0; i < 9; i++)
	{
		temp = receive16Bit();
		send16Bit(temp);
	}
}

void handleUnionRoomMovement()
{
	sendMoveBlock(UP);
	sendMoveBlock(UP);
	sendMoveBlock(RIGHT);
	sendMoveBlock(UP);
}

void sendMoveBlock(uint16_t dir)
{
	switch (dir) {
		case IDLE	:	break;
		case UP		:	break;
		case DOWN	:	break;
		case LEFT	:	break;
		case RIGHT	:	break;
		default		:	copyBlock(); return;
	}
	uint16_t temp = 0;
	for(uint8_t i = 0; i < 10; i++)
	{
		temp = receive16Bit();
		send16Bit(temp);
		receive16Bit();
		send16Bit(0xCAFE);
		receive16Bit();
		send16Bit(dir);
		receive16Bit();
		send16Bit(0x0000);
		receive16Bit();
		send16Bit(0x0000);
		receive16Bit();
		send16Bit(0x0000);
		receive16Bit();
		send16Bit(0x0000);
		receive16Bit();
		send16Bit(0x0000);
		receive16Bit();
		send16Bit(0x0000);
	}
	for(uint8_t i = 0; i < 4; i++)
	{
	temp = receive16Bit();
	send16Bit(temp);
	receive16Bit();
	send16Bit(0xCAFE);
	receive16Bit();
	send16Bit(0x0011);
	receive16Bit();
	send16Bit(0x0000);
	receive16Bit();
	send16Bit(0x0000);
	receive16Bit();
	send16Bit(0x0000);
	receive16Bit();
	send16Bit(0x0000);
	receive16Bit();
	send16Bit(0x0000);
	receive16Bit();
	send16Bit(0x0000);
	}
}

void loopOfDespair()
{
	sendMoveBlock(UP);
	while(1)
	{
	sendMoveBlock(RIGHT);
	sendMoveBlock(IDLE);
	sendMoveBlock(RIGHT);
	sendMoveBlock(IDLE);
	sendMoveBlock(RIGHT);
	sendMoveBlock(IDLE);
	sendMoveBlock(RIGHT);
	sendMoveBlock(IDLE);
	sendMoveBlock(UP);
	sendMoveBlock(IDLE);
	sendMoveBlock(UP);
	sendMoveBlock(IDLE);
	sendMoveBlock(UP);
	sendMoveBlock(IDLE);
	sendMoveBlock(UP);
	sendMoveBlock(IDLE);
	sendMoveBlock(LEFT);
	sendMoveBlock(IDLE);
	sendMoveBlock(LEFT);
	sendMoveBlock(IDLE);
	sendMoveBlock(LEFT);
	sendMoveBlock(IDLE);
	sendMoveBlock(LEFT);
	sendMoveBlock(IDLE);
	sendMoveBlock(LEFT);
	sendMoveBlock(IDLE);
	sendMoveBlock(LEFT);
	sendMoveBlock(IDLE);
	sendMoveBlock(LEFT);
	sendMoveBlock(IDLE);
	sendMoveBlock(LEFT);
	sendMoveBlock(IDLE);
	sendMoveBlock(LEFT);
	sendMoveBlock(IDLE);
	sendMoveBlock(DOWN);
	sendMoveBlock(IDLE);
	sendMoveBlock(DOWN);
	sendMoveBlock(IDLE);
	sendMoveBlock(DOWN);
	sendMoveBlock(IDLE);
	sendMoveBlock(DOWN);
	sendMoveBlock(IDLE);
	sendMoveBlock(RIGHT);
	sendMoveBlock(IDLE);
	sendMoveBlock(RIGHT);
	sendMoveBlock(IDLE);
	sendMoveBlock(RIGHT);
	sendMoveBlock(IDLE);
	sendMoveBlock(RIGHT);
	sendMoveBlock(IDLE);
	sendMoveBlock(RIGHT);
	sendMoveBlock(IDLE);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
