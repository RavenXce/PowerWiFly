/*
 * main.h
 *
 * Created: 11/3/2013 10:26:57 AM
 *  Author: RavenXce
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>
#include <Streaming.h>
#include "WiFly.h"
#include "AuthTable.h"

#define F_CPU 16000000
#define WIFLY_RX_PIN 2
#define WIFLY_TX_PIN 3
#define POWER_A_PIN 0b00010000; // 4
#define POWER_B_PIN 0b00100000; // 5
#define POWER_C_PIN 0b01000000; // 6
#define POWER_D_PIN 0b10000000; // 7

#define KEYWORD_SYNC "$SYNC$"
#define KEYWORD_REQUEST_STATE "$STATE$"
#define KEYWORD_TOGGLE_MODES "$MODE$"
#define KEYWORD_TOGGLE_SWITCHES "$SWITCH$"

enum MODE { ADHOC, NETWORKED };

bool CompareCommand(char*, char*);
void DecryptBuffer(char*);
void TrimBuffer(char*, int);
void DoSlowLoop();

#endif /* MAIN_H_ */