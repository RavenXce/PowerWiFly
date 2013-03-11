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
	while(!EnterCommandMode())
	{
		Serial.println("failed to enter cmd mode..");
	}
}

bool WiFly::EnterCommandMode()
{				
	if (IsInCommandMode()) return true;
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
			if (i < 500)
				i++;
			else
				Serial.println("Buffer Overflow!");
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
	uart.println("save");
	uart.println("exit");
	
	return 1;
}

void WiFly::SetupHTTP(){
	int delayW=500;
	
	while(!EnterCommandMode())
	{
		Serial.println("failed to enter cmd mode..");
	}		
	
	delay(delayW); uart.flush();
	uart.println("set ip proto 18");
	delay(delayW); uart.flush();
	uart.println("set dns name www.raaj.homeip.net");
	delay(delayW); uart.flush();
	uart.println("set ip address 0");
	delay(delayW); uart.flush();
	uart.println("set com remote 0");
	delay(delayW); uart.flush();
	
	uart.println("save"); 	
	delay(delayW); uart.flush();
	uart.println("exit"); delay(delayW);
	delay(2000);
	
	Serial.println("Done SetHTTP");
	uart.flush();
}


void WiFly::SendHTTPResponse(char* value){
	
	int delayW=500;
	bool success = false;
	char serverBuffer[500];	
	char responseArr[128];
	
	Serial.println("sendreponse");
	
	while(!EnterCommandMode())
	{
		Serial.println("failed to enter cmd mode..");
	}
	
	delay(delayW);
	Serial.println("open www.raaj.homeip.net 80"); uart.flush();
	uart.println("open www.raaj.homeip.net 80"); //delay(delayW); //getBufferResponse();
	//uart.flush();
	
	// wait for open => TODO: loop and set timeout	
	unsigned long startTime = millis();
	while (millis() - startTime < 6000 || success){
		GetResponse(serverBuffer);
		if(serverBuffer == "*OPEN*") success = true;
		else delay(WIFLY_DEFAULT_DELAY);
	}

	if(success = false){
		Serial.println("timed out on opening server port..");
		return;
	}
	
	Serial.println("port open");
	delay(2000);
	
	char* response="GET /addtoDB.php?count=";
	responseArr[0] = '\0';
	strcat(responseArr, response);
	strcat(responseArr, value);
	uart.print(responseArr);

	success = false;
	while (millis() - startTime < 8000 || success){
			GetResponse(serverBuffer);
			if(serverBuffer[0] == '(') success = true;
			else delay(WIFLY_DEFAULT_DELAY);
	}
	
	if(success = false){
		Serial.println("failed to get '(' character..");
		return;
	}
	
	Serial.println("test success!");
	
	uart.println("exit");
}