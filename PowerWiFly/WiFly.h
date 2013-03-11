/*
 * WiFly.h
 *
 * Created: 28/1/2013 2:06:53 PM
 *  Author: RavenXce
 */ 

#ifndef WIFLY_H_
#define WIFLY_H_

#include <Arduino.h>
#include <Wstring.h>
#include <util/delay.h>
#include "SoftwareSerial.h"

#define WIFLY_DEFAULT_BAUD_RATE 9600
#define WIFLY_DEFAULT_DELAY 1000

class WiFly{
	
	public:
		WiFly(int, int);
		int SetupAdHoc();
		void GetResponse( char *);
	
	private:
		SoftwareSerial uart;
		bool EnterCommandMode();
		bool IsInCommandMode();

};
#endif /* WIFLY_H_ */