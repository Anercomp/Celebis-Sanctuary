/*
 * gen3.h
 *
 *  Created on: 03.06.2022
 *      Author: Anercomp
 */
#ifndef GEN3_H_
#define GEN3_H_

#include "stm32f1xx.h"
#include "gen3.h"

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

extern const uint16_t ticksPerBit;
extern uint16_t zeroPackets;
extern uint8_t timerPacketFlag;
extern uint8_t flags;

struct PokemonSubstruct0;
struct PokemonSubstruct1;
struct PokemonSubstruct2;
struct PokemonSubstruct3;
struct PokemonSubstruct;

void send16Bit(uint16_t v);
uint16_t receive16Bit();
void initLink();
void sendUnionRoomData();
void waitForUnionRoom();
void waitForTeamData();
void sendBlock(uint16_t* blockdata);
void sendZeroBlock();
void sendZero();
void sendContBlock(uint16_t* blockdata);
void copyBlock();
void handleUnionRoomMovement();
void sendMoveBlock(uint16_t dir);
void loopOfDespair();

#endif /* GEN3_H_ */
