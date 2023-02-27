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
	for (long adr = 0L; adr < 65536L; adr++)
	{
		bool negative = adr >= 32768;
		long i = negative ? adr - 32768 : adr;
	
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

		// Setting all inputs as '1', because float display is common anode.
		for (int pin = I0; pin <= I7; pin++)
			digitalWrite(pin, HIGH);
		
		float exponent = (float)((i >> 10L) & 31L);
		double value;
    
		if (i == 0L) // zero
		{
			selectLEDs('0'); // units
			//if (negative) digitalWrite(I6, LOW); // tenths
		}
		else if (exponent < 31.0) // Valid exponent
		{
			if (exponent) value = pow(2.0, exponent - 15.0) * (1.0 + ((double)(i & 1023L)) / 1024.0);
			else          value = pow(2.0, -14.0          ) * (      ((double)(i & 1023L)) / 1024.0);
      
			char numStr[10];
      
			if (i < 8479L)
			{	// Numbers whose exponent in base 10 is -3 or less (numbers smaller than 0.01) (will be displayed in scientific notation).
				// 8479 in binary corresponds to float 0.01000213623046875, which will be displayed as 0.01.
				// The previous number (8478) corresponds to 0.0099945068359375, which will be displayed in scientific notation as 9.995e-003.
				sprintf(numStr, "%.3e", value);

				selectLEDs(numStr[9]); // units (exponent)

				// tenths (letra E)
				//digitalWrite(I0, LOW);
				//digitalWrite(I3, LOW);
				//digitalWrite(I4, LOW);
				//digitalWrite(I5, LOW);
				//digitalWrite(I6, LOW);

				if (numStr[4] != '0')
				{
					//selectLEDs(numStr[4]); // hundreds
					//selectLEDs(numStr[3]); // thousands
					//selectLEDs(numStr[2]); // tens of thousands
					//selectLEDs(numStr[0]); // hundreds of thousands
					//digitalWrite(I7, LOW); // hundreds of thousands (decimal point)
					//if (negative) digitalWrite(I6, LOW); // millions (signal)
				}
				else if (numStr[3] != '0')
				{
					//selectLEDs(numStr[3]); // hundreds
					//selectLEDs(numStr[2]); // thousands
					//selectLEDs(numStr[0]); // tens of thousands
					//digitalWrite(I7, LOW); // tens of thousands (decimal point)
					//if (negative) digitalWrite(I6, LOW); // hundreds of thousands (signal)
				}
				else
				{
					//selectLEDs(numStr[2]); // hundreds
					//selectLEDs(numStr[0]); // thousands
					//digitalWrite(I7, LOW); // thousands (decimal point)
					//if (negative) digitalWrite(I6, LOW); // tens of thousands (signal)
				}
        
			}
			else { // Numbers that fit entirely on the display.

				if (value < 10.0) sprintf(numStr, "%.5lf", value);
				else if (value < 100.0) sprintf(numStr, "%.4lf", value);
				else if (value < 1000.0) sprintf(numStr, "%.3lf", value);
				else if (value < 10000.0) sprintf(numStr, "%.2lf", value);
				else sprintf(numStr, "%.1lf", value);

				int l = 1; // units
				//int l = 2; // tenths
				//int l = 3; // hundreds
				//int l = 4; // thousands
				//int l = 5; // tens of thousands
				//int l = 6; // hundreds of thousands
				//int l = 7; // millions

				int pointer = 7;
				bool foundNonZero = false;
				bool hasPoint = true;
				bool validDigit = true;
				do
				{
					pointer--;
					if (pointer < 0)
					{
						validDigit = false;
						break;
					}
					if (!foundNonZero)
					{
						if (numStr[pointer] != '0')
						{
							foundNonZero = true;
							if (numStr[pointer] != '.') l--;
							else hasPoint = false;
						}
					}
					else if (numStr[pointer] != '.') l--;
				} while (l > 0);

				if (validDigit)
				{
					selectLEDs(numStr[pointer]);
					if (hasPoint && (numStr[pointer + 1] == '.')) digitalWrite(I7, LOW);
				}
				else if (negative && (l == 1)) digitalWrite(I6, LOW); // signal

			}
		}
		else // Invalid exponent
		{
			if (i & 1023L) // NaN
			{
				// units
				digitalWrite(I4, LOW);
				digitalWrite(I6, LOW);
        
				// tenths
				//digitalWrite(I2, LOW);
				//digitalWrite(I3, LOW);
				//digitalWrite(I4, LOW);
				//digitalWrite(I6, LOW);
        
				// hundreds
				//digitalWrite(I4, LOW);
				//digitalWrite(I6, LOW);

				// thousands
				//digitalWrite(I4, LOW);
				//digitalWrite(I6, LOW);

				// tens of thousands
				//digitalWrite(I0, LOW);
				//digitalWrite(I3, LOW);
				//digitalWrite(I4, LOW);
				//digitalWrite(I5, LOW);
				//digitalWrite(I6, LOW);
			}
			else // Infinity
			{
				// units
				digitalWrite(I0, LOW);
				digitalWrite(I4, LOW);
				digitalWrite(I5, LOW);
				digitalWrite(I6, LOW);
				digitalWrite(I7, LOW);

				// tenths
				//digitalWrite(I2, LOW);
				//digitalWrite(I4, LOW);
				//digitalWrite(I6, LOW);

				// hundreds
				//digitalWrite(I1, LOW);
				//digitalWrite(I2, LOW);

				// thousands
				//if (negative) digitalWrite(I6, LOW);
			}
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

void selectLEDs(char n)
{
	switch (n)
	{
		case 0:
			digitalWrite(I0, LOW);
			digitalWrite(I1, LOW);
			digitalWrite(I2, LOW);
			digitalWrite(I3, LOW);
			digitalWrite(I4, LOW);
			digitalWrite(I5, LOW);
			break;
		case 1:
			digitalWrite(I1, LOW);
			digitalWrite(I2, LOW);
			break;
		case 2:
			digitalWrite(I0, LOW);
			digitalWrite(I1, LOW);
			digitalWrite(I3, LOW);
			digitalWrite(I4, LOW);
			digitalWrite(I6, LOW);
			break;
		case 3:
			digitalWrite(I0, LOW);
			digitalWrite(I1, LOW);
			digitalWrite(I2, LOW);
			digitalWrite(I3, LOW);
			digitalWrite(I6, LOW);
			break;
		case 4:
			digitalWrite(I1, LOW);
			digitalWrite(I2, LOW);
			digitalWrite(I5, LOW);
			digitalWrite(I6, LOW);
			break;
		case 5:
			digitalWrite(I0, LOW);
			digitalWrite(I2, LOW);
			digitalWrite(I3, LOW);
			digitalWrite(I5, LOW);
			digitalWrite(I6, LOW);
			break;
		case 6:
			digitalWrite(I0, LOW);
			digitalWrite(I2, LOW);
			digitalWrite(I3, LOW);
			digitalWrite(I4, LOW);
			digitalWrite(I5, LOW);
			digitalWrite(I6, LOW);
			break;
		case 7:
			digitalWrite(I0, LOW);
			digitalWrite(I1, LOW);
			digitalWrite(I2, LOW);
			break;
		case 8:
			digitalWrite(I0, LOW);
			digitalWrite(I1, LOW);
			digitalWrite(I2, LOW);
			digitalWrite(I3, LOW);
			digitalWrite(I4, LOW);
			digitalWrite(I5, LOW);
			digitalWrite(I6, LOW);
			break;
		case 9:
			digitalWrite(I0, LOW);
			digitalWrite(I1, LOW);
			digitalWrite(I2, LOW);
			digitalWrite(I3, LOW);
			digitalWrite(I5, LOW);
			digitalWrite(I6, LOW);
			break;
		default:
			digitalWrite(I6, LOW);
			digitalWrite(I7, LOW);
			break;
	}
}

void loop()
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(200);
	digitalWrite(LED_BUILTIN, LOW);
	delay(200);
}
