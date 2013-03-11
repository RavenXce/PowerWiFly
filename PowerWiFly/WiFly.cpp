/*
* WiFly.cpp
*
* Created: 28/1/2013 2:01:16 PM
*  Author: RavenXce
*/

#include "WiFly.h"

char buffer[500];

WiFly::WiFly(int rxPin, int txPin) : uart(rxPin, txPin)
{
	uart.begin(WIFLY_DEFAULT_BAUD_RATE);
	uart.listen();
	uart.flush();
	if(!IsInCommandMode()){
		while(!EnterCommandMode())
		{
			Serial.println("failed to enter cmd mode..");
		}
	}	
}

bool WiFly::EnterCommandMode()
{				
	// Enter command mode
	Serial.println("sending cmd symbol..");
	
	uart.print("$$$");
	
	delay(WIFLY_DEFAULT_DELAY);
			
	// Check for response
	GetResponse(buffer);
	return (buffer == "CMD") ? true : false;
}

bool WiFly::IsInCommandMode()
{
	// Send command to test for command mode
	uart.println("get mac");
	
	// Check for response
	GetResponse(buffer);	
	return (buffer == "MACADDRESS") ? true : false;
}

void WiFly::GetResponse( char * buffer )
{
	if (uart.available())
	{
		int i=0;
		while (uart.available()){
			buffer[i] = (char)uart.read();
			i++;
		}
		buffer[i] = '\0';
	}
	
	Serial.println(buffer);
	
	return;
}

int WiFly::SetupAdHoc()
{
	// Setup adhoc network
	uart.println("set ip address 169.254.1.1");
	uart.println("set ip netmask 255.255.0.0");
	uart.println("set ip dhcp 0");
	uart.println("set wlan ssid WiFly-GSX-XX");
	uart.println("set wlan channel 1");
	
	// Create adhoc network
	uart.println("set wlan join 4");
	
	return 1;
}