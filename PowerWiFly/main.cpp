/*
* main.cpp
*
* Created: 27/1/2013 3:35:12 PM
*  Author: RavenXce
*/

#include "main.h"

bool sync;
bool state[4];
MODE mode;
WiFly wifly(WIFLY_RX_PIN, WIFLY_TX_PIN);
AuthTable authTable;

void setup(){
	DDRD = 0b11111000;
	state[0] = state[1] = state[2] = state[3] = sync = false;
	mode = ADHOC;
	
	sei();
	
	Serial.begin(WIFLY_DEFAULT_BAUD_RATE);
	Serial.println("Beginning..");
		
	Serial.println("Setting up adhoc mode");
	wifly.SetupAdHoc();
	// wifly.SetupHTTP();
}

int loopcount = 0;
void loop()
{
	char buffer[500];
	// sleep
	delay(WIFLY_DEFAULT_DELAY);
	
	if(mode == NETWORKED && loopcount % 10) DoSlowLoop(); // ping internet server for commands.
	
	// FAST LOOP => read direct buffer
	wifly.GetResponse(buffer);
	
	if (mode == ADHOC)
	{
		if (sync == false)
		DecryptBuffer(buffer);
		else if (CompareCommand(buffer,KEYWORD_SYNC))
		{
			authTable.AddDevice(buffer);
		}
	}
	
	if (CompareCommand(buffer,KEYWORD_REQUEST_STATE))
	{
		// send power relay state, error msgs and auth info if adhoc
	}
	else if (CompareCommand(buffer,KEYWORD_TOGGLE_MODES))
	{
		// if adhoc parse network info => try connect => save error and restart adhoc if fail
		//
		// clear wifly buffer!
	}
	else if (CompareCommand(buffer,KEYWORD_TOGGLE_SWITCHES))
	{
		// get switch number
		// toggle and save state => make atomic!!
		switch(buffer[0])
		{
			case 'A':
			// switch A to buffer[1].
			break;
			case 'B':
			break;
			case 'C':
			break;
			case 'D':
			break;
			default:
			// invalid cmd, clear buffer
			break;
		}
	}
	
	// possible cmds:
	// send state
	// toggle modes
	// toggle power switches => check for consistancy with sender.
	// set a timer
	// sync:
	// check for sync (pushbutton). => generate encrypted pw seed and send.
	// phone app will decrypt and use seed to hash(encrypt) all cmds.
}

bool CompareCommand( char* buffer, char* keyword)
{
	int length = strlen(keyword);
	if (strncmp(buffer,keyword,length) != 0)
	return false;
	TrimBuffer(buffer, length);
	return true;
}

void DecryptBuffer( char* buffer )
{
	// search the map for mac address of sender. use hash.
	return;
}

void TrimBuffer( char* buffer, int count)
{
	int i = 0;
	for (int j = count; buffer[j-1]!= '\0'; j++)
	{
		buffer[i] = buffer[j];
		i++;
	}
}

void AddDeviceToAuthTable()
{
	return;
}

void DoSlowLoop()
{
	return;
}