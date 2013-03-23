/*
* WiFly.cpp
*
* Created: 28/1/2013 2:01:16 PM
*  Author: RavenXce
*/

#include "WiFly.h"

char buffer[RESPONSE_BUFFER_SIZE];

WiFly::WiFly(int rxPin, int txPin) : uart(rxPin, txPin)
{
	uart.begin(WIFLY_DEFAULT_BAUD_RATE);
	uart.listen();
	uart.flush();
}

bool WiFly::EnterCommandMode()
{				
	if (IsInCommandMode()) return true;
	
	// Enter command mode
	Serial.println("sending cmd symbol..");	
	uart.flush();
	uart.print("$$$");	
	delay(WIFLY_DEFAULT_DELAY);
	
	// Check for response
	GetResponse(buffer);
	return (buffer == "CMD") ? true : false;
}

bool WiFly::IsInCommandMode()
{	
	// Send command to test for command mode
	Serial.println("checking if already in cmd mode..");
	uart.flush();
	uart.println("get mac");
	delay(WIFLY_DEFAULT_DELAY);
	
	// Check for response
	//WaitForResponse("a",50000);
	GetResponse(buffer);
	return (buffer == "00:06:66:80ù:14:18") ? true : false;
}

void WiFly::GetResponse( char * buffer )
{
	char ch;
	int i=0;
	
	if (uart.available())
	{
		while (uart.available()){
			ch = uart.read();
			buffer[i] = ch;
			if (i < RESPONSE_BUFFER_SIZE)
				i++;
			else
				Serial.println("Buffer Overflow!");
		}
		buffer[i] = '\0';
		
		Serial.println(buffer);
	}
	
	else
	{
		Serial.println("no response.");
	}
	
	return;
}

bool WiFly::WaitForResponse(char* compareBuffer, int timeout){
	
	//Variables
	char* responseBuffer;
	boolean isReadComplete = false;
	boolean isCompareSuccess = false;
	
	int  bufferPosition = 0;

	//Reset the buffer
	responseBuffer = (char*) malloc(RESPONSE_BUFFER_SIZE);
	memset (responseBuffer, '\0', RESPONSE_BUFFER_SIZE - 1);

	//Fill the buffer
	unsigned long startTime = millis();
	char chResponse = 0;
	
	while(!isReadComplete)
	{		
		//Start getting values		
		if(uart.available()){
			chResponse = uart.read();			
			if(bufferPosition < RESPONSE_BUFFER_SIZE - 1){
				responseBuffer[bufferPosition]=chResponse;
				bufferPosition++;
			}
			else{
				Serial.println("Buffer overflow!");
				bufferPosition = 0;
			}
		}

		//Check for existence of the comparison string, or if timeout stop
		if(StringsMatch(responseBuffer,compareBuffer))
		{
			isCompareSuccess = true;
			isReadComplete = true;
		}
		else if((millis()-startTime)>timeout)
		{
			isCompareSuccess = false;
			isReadComplete = true;
		}
	}
	
	Serial.println(responseBuffer);
	uart.flush();
	
	return isCompareSuccess;
}

int WiFly::SetupAdHoc()
{
	// Enter command mode	
	while(!EnterCommandMode())
	{
		Serial.println("failed to enter cmd mode..");
	}
	
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

bool WiFly::StringsMatch(char* a,char* b){
	char * pch = NULL;
	pch = strstr (a,b);
	
	if(pch == NULL) return false;
	else return true;
}