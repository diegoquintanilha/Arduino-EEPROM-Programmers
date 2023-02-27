/*
 * In order to program the EEPROM:
 * Disconnect the previous EEPROM from the board and connect the next one while the LED is blinking. 
 * 		- All address pins must be connected to the shift register outputs.
 * 		- All data pins must be connected to the arduino output pins labeled I0 to I7.
 * 		- The EEPROM CE pin must be connected to the arduino output labeled REC.
 * Reset the arduino to start the process.
 * While the LED is blinking, connect the EEPROM OE pin on 12V (11.75 < voltage <12.25).
 * LED will blink faster, indicating that the programming is imminent.
 * LED will turn off while EEPROM is being programmed.
 * When programming is finished, LED will turn on continuously for 3 seconds, and then will go back to blinking.
 * About 15 seconds total for each EEPROM to be fully programmed.
*/

#define CLK 2
#define ADR 3
#define REC 12
#define I0 4
#define I1 5
#define I2 6
#define I3 7
#define I4 8
#define I5 9
#define I6 10
#define I7 11

void setup()
{
	// Setup
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(CLK, OUTPUT);
	digitalWrite(CLK, LOW);
	pinMode(ADR, OUTPUT);
	pinMode(REC, OUTPUT);
	digitalWrite(REC, HIGH);
	for (int pin = I0; pin <= I7; pin++) pinMode(pin, OUTPUT);

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
	
	// Loop that goes through every address.
	for (long i = 0L; i < 65536L; i++)
	{
		// Get every bit of current address and put on shift registers.
		for (long j = 0L; j < 16L; j++)
		{
			digitalWrite(ADR, (i & (1L << j)) ? HIGH : LOW);
			delayMicroseconds(10);
			digitalWrite(CLK, HIGH);
			delayMicroseconds(10);
			digitalWrite(CLK, LOW);
			delayMicroseconds(10);
		}
	
		// Setting all inputs to '0'.
		for (int pin = I0; pin <= I7; pin++)
			digitalWrite(pin, LOW);
	
		// Selecting the digit corresponding to this EEPROM and setting the inputs relative to this digit to '1'.
		switch ((int)(i % 10L)) // units
		//switch ((int)((i % 100L)) / 10L) // tens
		//switch ((int)((i % 1000L)) / 100L) // hundreds
		//switch ((int)((i % 10000L)) / 1000L) // thousands
		//switch ((int)(i / 10000L)) // tens of thousands
		{
			case 0:
				// no 'if' statement if it is units
				//if (i > 9L) { // tens
				//if (i > 99L) { // hundreds
				//if (i > 999L) { // thousands
				// there will never be a zero on the tens of thousands place (no zeros to the left).
					digitalWrite(I0, HIGH);
					digitalWrite(I1, HIGH);
					digitalWrite(I2, HIGH);
					digitalWrite(I3, HIGH);
					digitalWrite(I4, HIGH);
					digitalWrite(I5, HIGH);
				//}
				break;
			case 1:
				digitalWrite(I1, HIGH);
				digitalWrite(I2, HIGH);
				break;
			case 2:
				digitalWrite(I0, HIGH);
				digitalWrite(I1, HIGH);
				digitalWrite(I3, HIGH);
				digitalWrite(I4, HIGH);
				digitalWrite(I6, HIGH);
				break;
			case 3:
				digitalWrite(I0, HIGH);
				digitalWrite(I1, HIGH);
				digitalWrite(I2, HIGH);
				digitalWrite(I3, HIGH);
				digitalWrite(I6, HIGH);
				break;
			case 4:
				digitalWrite(I1, HIGH);
				digitalWrite(I2, HIGH);
				digitalWrite(I5, HIGH);
				digitalWrite(I6, HIGH);
				break;
			case 5:
				digitalWrite(I0, HIGH);
				digitalWrite(I2, HIGH);
				digitalWrite(I3, HIGH);
				digitalWrite(I5, HIGH);
				digitalWrite(I6, HIGH);
				break;
			case 6:
				digitalWrite(I0, HIGH);
				digitalWrite(I2, HIGH);
				digitalWrite(I3, HIGH);
				digitalWrite(I4, HIGH);
				digitalWrite(I5, HIGH);
				digitalWrite(I6, HIGH);
				break;
			case 7:
				digitalWrite(I0, HIGH);
				digitalWrite(I1, HIGH);
				digitalWrite(I2, HIGH);
				break;
			case 8:
				digitalWrite(I0, HIGH);
				digitalWrite(I1, HIGH);
				digitalWrite(I2, HIGH);
				digitalWrite(I3, HIGH);
				digitalWrite(I4, HIGH);
				digitalWrite(I5, HIGH);
				digitalWrite(I6, HIGH);
				break;
			case 9:
				digitalWrite(I0, HIGH);
				digitalWrite(I1, HIGH);
				digitalWrite(I2, HIGH);
				digitalWrite(I3, HIGH);
				digitalWrite(I5, HIGH);
				digitalWrite(I6, HIGH);
				break;
			default:
				digitalWrite(I6, HIGH);
				digitalWrite(I7, HIGH);
				break;
		}

		// Recording pulse.
		delayMicroseconds(50);
		digitalWrite(REC, LOW);
		delayMicroseconds(100); // 100 uS pulse.
		digitalWrite(REC, HIGH);
		delayMicroseconds(50);
	}

	// Turns LED on continuously for 3 seconds to indicate that programming is finished.
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
