/*
 * In order to erase EEPROM:
 * Disconnect the previous EEPROM from the board and connect the next one while the LED is blinking. 
 * 		- All address pins must be LOW (except A9).
 * 		- All data pins (outputs) must be HIGH. 
 * 		- The EEPROM CE pin must be connected to the arduino output labeled ERASE.
 * Reset the arduino to start the process.
 * While the LED is blinking, connect the EEPROM OE pin on 12V (11.75 < voltage <12.25).
 * LED will blink faster, indicating that the erase pulse is about to be emitted.
 * LED will turn off while EEPROM is being erased.
 * When erasing is finished, LED will turn on continuously for 3 seconds, and then will go back to blinking.
*/

#define ERASE 12

void setup()
{	
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(ERASE, OUTPUT);

	digitalWrite(ERASE, HIGH);
	
	// LED blinks 5 times, time to connect 12V where needed.
	for (int i = 0; i < 6; i++)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(1000);
		digitalWrite(LED_BUILTIN, LOW);
		delay(1000);		
	}

	// Blinks fast to indicate it is about to begin.
	for (int i = 0; i < 6; i++)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(300);
		digitalWrite(LED_BUILTIN, LOW);
		delay(300);		
	}

	// Erase pulse
	digitalWrite(ERASE, LOW);
	delay(100); // 100 mS pulse
	digitalWrite(ERASE, HIGH);

	delay(500);

	// Turns LED on continuously for 3 seconds to indicate that erasing is finished.
	digitalWrite(LED_BUILTIN, HIGH);
	delay(3000);
}

void loop()
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(200);
	digitalWrite(LED_BUILTIN, LOW);
	delay(200);
}
