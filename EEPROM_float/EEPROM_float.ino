/*
 * Para programar o EEPROM:
 * Desconectar/conectar os EEPROM da placa quando o led estiver piscando, com:
 * 		todos os pinos de endereço ligados nos outputs dos shift registers,
 * 		todos os pinos de dados ligados nas saídas de I0 a I7 do arduino
 * 		o pino CE ligado na saída REC do arduino
 * Resetar o arduino para iniciar o processo
 * Enquanto o led estiver piscando, conectar o pino OE em 12V (>11.75 e <12.25)
 * Led vai piscar rápido, indicando que está prestes a iniciar a gravação
 * Led apaga enquanto o EEPROM está sendo programado
 * Ao terminar de programar, led acende continuamente por 3 segundos, e então volta a piscar
 * Um pouco mais que 15 segundos pra programar cada EEPROM
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

void setup() {

	// setup
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(CLK, OUTPUT);
	digitalWrite(CLK, LOW);
	pinMode(ADR, OUTPUT);
	pinMode(REC, OUTPUT);
	digitalWrite(REC, HIGH);
	for (int pin = I0; pin <= I7; pin++) pinMode(pin, OUTPUT);

	// led pisca 5 vezes, é o momento de ligar 12V onde precisa
	for (int i = 0; i < 6; i++) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(1000);
		digitalWrite(LED_BUILTIN, LOW);
		delay(1000);		
	}

	// pisca rápido porque está prestes a começar
	for (int i = 0; i < 6; i++) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(300);
		digitalWrite(LED_BUILTIN, LOW);
		delay(300);		
	}

	// loop que passa por todos os endereços
	for (long adr = 0L; adr < 65536L; adr++) {

		bool negative = adr >= 32768;
		long i = negative ? adr - 32768 : adr;
	
		// pegando cada bit do endereço atual e colocando nos shift registers
		for (long j = 0L; j < 16L; j++) {
			digitalWrite(ADR, (i & (1L << j)) ? HIGH : LOW);
			delayMicroseconds(10);
			digitalWrite(CLK, HIGH);
			delayMicroseconds(10);
			digitalWrite(CLK, LOW);
			delayMicroseconds(10);
		}

		// colocando todos os inputs como '1', pois o display de float é ânodo comum
		for (int pin = I0; pin <= I7; pin++) digitalWrite(pin, HIGH);
		
		float exponent = (float)((i >> 10L) & 31L);
		double value;
    
		if (i == 0L) { // zero
			selectLEDs('0'); // unidade
			//if (negative) digitalWrite(I6, LOW); // dezena
		}
		else if (exponent < 31.0) { // expoente válido
      
			if (exponent) value = pow(2.0, exponent - 15.0) * (1.0 + ((double)(i & 1023L)) / 1024.0);
			else          value = pow(2.0, -14.0          ) * (      ((double)(i & 1023L)) / 1024.0);
      
			char numStr[10];
      
			if (i < 8479L) { // números cujo expoente em base 10 é -3 ou menos (< 0.01) (colocados em notação científica no mostrador)
							 // 8479 em binário corresponde ao float 0.01000213623046875, que será representado no mostrador como 0.01
							 // O número logo abaixo (8478) corresponde a 0.0099945068359375, que será representado em notação científica como 9.995e-003
				sprintf(numStr, "%.3e", value);

				selectLEDs(numStr[9]); // unidade (expoente)

				// dezena (letra E)
				//digitalWrite(I0, LOW);
				//digitalWrite(I3, LOW);
				//digitalWrite(I4, LOW);
				//digitalWrite(I5, LOW);
				//digitalWrite(I6, LOW);

				if (numStr[4] != '0') {
					//selectLEDs(numStr[4]); // centena
					//selectLEDs(numStr[3]); // milhar
					//selectLEDs(numStr[2]); // dezena de milhar
					//selectLEDs(numStr[0]); // centena de milhar
					//digitalWrite(I7, LOW); // centena de milhar (ponto decimal)
					//if (negative) digitalWrite(I6, LOW); // milhão (sinal)
				}
				else if (numStr[3] != '0') {
					//selectLEDs(numStr[3]); // centena
					//selectLEDs(numStr[2]); // milhar
					//selectLEDs(numStr[0]); // dezena de milhar
					//digitalWrite(I7, LOW); // dezena de milhar (ponto decimal)
					//if (negative) digitalWrite(I6, LOW); // centena de milhar (sinal)
				}
				else {
					//selectLEDs(numStr[2]); // centena
					//selectLEDs(numStr[0]); // milhar
					//digitalWrite(I7, LOW); // milhar (ponto decimal)
					//if (negative) digitalWrite(I6, LOW); // dezena de milhar (sinal)
				}
        
			}
			else { // números que cabem inteiros no mostrador

				if (value < 10.0) sprintf(numStr, "%.5lf", value);
				else if (value < 100.0) sprintf(numStr, "%.4lf", value);
				else if (value < 1000.0) sprintf(numStr, "%.3lf", value);
				else if (value < 10000.0) sprintf(numStr, "%.2lf", value);
				else sprintf(numStr, "%.1lf", value);

				int l = 1; // unidade
				//int l = 2; // dezena
				//int l = 3; // centena
				//int l = 4; // milhar
				//int l = 5; // dezena de milhar
				//int l = 6; // centena de milhar
				//int l = 7; // milhão

				int pointer = 7;
				bool foundNonZero = false;
				bool hasPoint = true;
				bool validDigit = true;
				do {
					pointer--;
					if (pointer < 0) {
						validDigit = false;
						break;
					}
					if (!foundNonZero) {
						if (numStr[pointer] != '0') {
							foundNonZero = true;
							if (numStr[pointer] != '.') l--;
							else hasPoint = false;
						}
					}
					else if (numStr[pointer] != '.') l--;
				} while (l > 0);

				if (validDigit) {
					selectLEDs(numStr[pointer]);
					if (hasPoint && (numStr[pointer + 1] == '.')) {
						digitalWrite(I7, LOW);
					}
				}
				else if (negative && (l == 1)) digitalWrite(I6, LOW); // sinal

			}
		}
		else { // expoente inválido
			if (i & 1023L) { // NaN
				
				// unidade
				digitalWrite(I4, LOW);
				digitalWrite(I6, LOW);
        
				// dezena
				//digitalWrite(I2, LOW);
				//digitalWrite(I3, LOW);
				//digitalWrite(I4, LOW);
				//digitalWrite(I6, LOW);
        
				// centena
				//digitalWrite(I4, LOW);
				//digitalWrite(I6, LOW);

				// milhar
				//digitalWrite(I4, LOW);
				//digitalWrite(I6, LOW);

				// dezena de milhar
				//digitalWrite(I0, LOW);
				//digitalWrite(I3, LOW);
				//digitalWrite(I4, LOW);
				//digitalWrite(I5, LOW);
				//digitalWrite(I6, LOW);
			}
			else { // infinito
				// unidade
				digitalWrite(I0, LOW);
				digitalWrite(I4, LOW);
				digitalWrite(I5, LOW);
				digitalWrite(I6, LOW);
				digitalWrite(I7, LOW);

				// dezena
				//digitalWrite(I2, LOW);
				//digitalWrite(I4, LOW);
				//digitalWrite(I6, LOW);

				// centena
				//digitalWrite(I1, LOW);
				//digitalWrite(I2, LOW);

				// milhar
				//if (negative) digitalWrite(I6, LOW);
			}
		}

		// pulso de gravação
		delayMicroseconds(50);
		digitalWrite(REC, LOW);
		delayMicroseconds(100); // 100 uS pulse
		digitalWrite(REC, HIGH);
		delayMicroseconds(50);
	}

	// acende o led continuamente por 3 segundos pra indicar que terminou
	digitalWrite(LED_BUILTIN, HIGH);
	delay(3000);
}

void selectLEDs(char n) {
	switch (n) {
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

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);
	delay(200);
	digitalWrite(LED_BUILTIN, LOW);
	delay(200);
}
