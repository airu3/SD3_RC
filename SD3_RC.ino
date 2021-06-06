#define DEBUG_MODE
#include "SD3/ALL.h"

void loop()
{
	Serial.print(F("Please R/C system connect\n"));
	while (!port(PB1))
	{
		// Serial.println(  pulseIn(STICK1, HIGH, 30000));
		print_port(0, 21);

		static unsigned long time;
		if (millis() - time > 1000)
		{
			SET_LED(1);
			time = millis();
		}
		else
		{
			SET_LED(0);
		}
	}
	SOUND(2);
	Serial.print(F("Motor safety unlocked!!!\n"));
	Drive();
}
