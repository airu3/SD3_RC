//#define DEBUG_MODE
#define MINIZADE
#include "SD3/ALL.h"

void loop()
{

	//SOUND(0);

	Serial.print(F("Please R/C system connect\n"));
	while (port(BTN1))
	{
		update_port(STICK1);
		update_port(STICK2);
		print_port(0, 6);
	}
	SOUND(0);
	Serial.print(F("Motor safety unlocked!!!\n"));
	Drive();
}
