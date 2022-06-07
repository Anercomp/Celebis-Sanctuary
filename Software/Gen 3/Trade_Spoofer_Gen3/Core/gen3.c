/*
 * gen3.c
 *
 *  Created on: 03.06.2022
 *      Author: Anercomp
 */

#include "gen3.h"
#include "stm32f1xx.h"
#include "hardware.h"
#include "stm32f1xx_hal.h"
#include "main.h"

const uint16_t ticksPerBit = 625;
uint16_t zeroPackets = 0;
uint8_t timerPacketFlag = 0;
uint8_t flags = 0;

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

void sendUnionRoomData()
{
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
}

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

struct PokemonSubstruct
{
	struct PokemonSubstruct0 type0;
	struct PokemonSubstruct1 type1;
	struct PokemonSubstruct2 type2;
	struct PokemonSubstruct3 type3;
	u16 raw[6];
};
