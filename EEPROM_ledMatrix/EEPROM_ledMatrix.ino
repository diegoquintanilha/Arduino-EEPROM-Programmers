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
	for (int pin = I1; pin <= I7; pin++) pinMode(pin, OUTPUT);

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
	for (long i = 0L; i < 65536L; i++) {

		// pegando cada bit do endereço atual e colocando nos shift registers
		for (long j = 0L; j < 16L; j++) {
			digitalWrite(ADR, (i & (1L << j)) ? HIGH : LOW);
			delayMicroseconds(10);
			digitalWrite(CLK, HIGH);
			delayMicroseconds(10);
			digitalWrite(CLK, LOW);
			delayMicroseconds(10);
		}
	
		// colocando todos os inputs como '0'
		for (int pin = I1; pin <= I7; pin++) digitalWrite(pin, LOW);
	
		// CÓDIGO AQUI
		//digitalWrite(?, HIGH);

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

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);
	delay(200);
	digitalWrite(LED_BUILTIN, LOW);
	delay(200);
}

int charMat[] = {

	//--------------------------------------------------------
	// 1st HALF
	//--------------------------------------------------------

	0b00000000, // character 0x20 column 1 ........
	0b00000000, // character 0x20 column 2 ........
	0b00000000, // character 0x20 column 3 ........
	0b00000000, // character 0x20 column 4 ........
	0b00000000, // character 0x20 column 5 ........
	
	0b00000000, // character 0x21 column 1 ........
	0b00000000, // character 0x21 column 2 ........
	0b01011111, // character 0x21 column 3 .*.*****
	0b00000000, // character 0x21 column 4 ........
	0b00000000, // character 0x21 column 5 ........
	
	0b00000000, // character 0x22 column 1 ........
	0b00000111, // character 0x22 column 2 .....***
	0b00000000, // character 0x22 column 3 ........
	0b00000111, // character 0x22 column 4 .....***
	0b00000000, // character 0x22 column 5 ........
	
	0b00010100, // character 0x23 column 1 ...*.*..
	0b01111111, // character 0x23 column 2 .*******
	0b00010100, // character 0x23 column 3 ...*.*..
	0b01111111, // character 0x23 column 4 .*******
	0b00010100, // character 0x23 column 5 ...*.*..
	
	0b00100100, // character 0x24 column 1 ..*..*..
	0b00101010, // character 0x24 column 2 ..*.*.*.
	0b01111111, // character 0x24 column 3 .*******
	0b00101010, // character 0x24 column 4 ..*.*.*.
	0b00010010, // character 0x24 column 5 ...*..*.
	
	0b00100011, // character 0x25 column 1 ..*...**
	0b00010011, // character 0x25 column 2 ...*..**
	0b00001000, // character 0x25 column 3 ....*...
	0b01100100, // character 0x25 column 4 .**..*..
	0b01100010, // character 0x25 column 5 .**...*.
	
	0b00110110, // character 0x26 column 1 ..**.**.
	0b01001001, // character 0x26 column 2 .*..*..*
	0b01010101, // character 0x26 column 3 .*.*.*.*
	0b00100010, // character 0x26 column 4 ..*...*.
	0b01010000, // character 0x26 column 5 .*.*....
	
	0b00000000, // character 0x27 column 1 ........
	0b00000101, // character 0x27 column 2 .....*.*
	0b00000011, // character 0x27 column 3 ......**
	0b00000000, // character 0x27 column 4 ........
	0b00000000, // character 0x27 column 5 ........
	
	0b00000000, // character 0x28 column 1 ........
	0b00011100, // character 0x28 column 2 ...***..
	0b00100010, // character 0x28 column 3 ..*...*.
	0b01000001, // character 0x28 column 4 .*.....*
	0b00000000, // character 0x28 column 5 ........
	
	0b00000000, // character 0x29 column 1 ........
	0b01000001, // character 0x29 column 2 .*.....*
	0b00100010, // character 0x29 column 3 ..*...*.
	0b00011100, // character 0x29 column 4 ...***..
	0b00000000, // character 0x29 column 5 ........
	
	0b00001000, // character 0x2A column 1 ....*...
	0b00101010, // character 0x2A column 2 ..*.*.*.
	0b00011100, // character 0x2A column 3 ...***..
	0b00101010, // character 0x2A column 4 ..*.*.*.
	0b00001000, // character 0x2A column 5 ....*...
	
	0b00001000, // character 0x2B column 1 ....*...
	0b00001000, // character 0x2B column 2 ....*...
	0b00111110, // character 0x2B column 3 ..*****.
	0b00001000, // character 0x2B column 4 ....*...
	0b00001000, // character 0x2B column 5 ....*...
	
	0b00000000, // character 0x2C column 1 ........
	0b01010000, // character 0x2C column 2 .*.*....
	0b00110000, // character 0x2C column 3 ..**....
	0b00000000, // character 0x2C column 4 ........
	0b00000000, // character 0x2C column 5 ........
	
	0b00001000, // character 0x2D column 1 ....*...
	0b00001000, // character 0x2D column 2 ....*...
	0b00001000, // character 0x2D column 3 ....*...
	0b00001000, // character 0x2D column 4 ....*...
	0b00001000, // character 0x2D column 5 ....*...
	
	0b00000000, // character 0x2E column 1 ........
	0b01100000, // character 0x2E column 2 .**.....
	0b01100000, // character 0x2E column 3 .**.....
	0b00000000, // character 0x2E column 4 ........
	0b00000000, // character 0x2E column 5 ........
	
	0b00100000, // character 0x2F column 1 ..*.....
	0b00010000, // character 0x2F column 2 ...*....
	0b00001000, // character 0x2F column 3 ....*...
	0b00000100, // character 0x2F column 4 .....*..
	0b00000010, // character 0x2F column 5 ......*.
	
	0b00111110, // character 0x30 column 1 ..*****.
	0b01010001, // character 0x30 column 2 .*.*...*
	0b01001001, // character 0x30 column 3 .*..*..*
	0b01000101, // character 0x30 column 4 .*...*.*
	0b00111110, // character 0x30 column 5 ..*****.
	
	0b00000000, // character 0x31 column 1 ........
	0b01000010, // character 0x31 column 2 .*....*.
	0b01111111, // character 0x31 column 3 .*******
	0b01000000, // character 0x31 column 4 .*......
	0b00000000, // character 0x31 column 5 ........
	
	0b01000010, // character 0x32 column 1 .*....*.
	0b01100001, // character 0x32 column 2 .**....*
	0b01010001, // character 0x32 column 3 .*.*...*
	0b01001001, // character 0x32 column 4 .*..*..*
	0b01000110, // character 0x32 column 5 .*...**.
	
	0b00100001, // character 0x33 column 1 ..*....*
	0b01000001, // character 0x33 column 2 .*.....*
	0b01000101, // character 0x33 column 3 .*...*.*
	0b01001011, // character 0x33 column 4 .*..*.**
	0b00110001, // character 0x33 column 5 ..**...*
	
	0b00011000, // character 0x34 column 1 ...**...
	0b00010100, // character 0x34 column 2 ...*.*..
	0b00010010, // character 0x34 column 3 ...*..*.
	0b01111111, // character 0x34 column 4 .*******
	0b00010000, // character 0x34 column 5 ...*....
	
	0b00100111, // character 0x35 column 1 ..*..***
	0b01000101, // character 0x35 column 2 .*...*.*
	0b01000101, // character 0x35 column 3 .*...*.*
	0b01000101, // character 0x35 column 4 .*...*.*
	0b00111001, // character 0x35 column 5 ..***..*
	
	0b00111100, // character 0x36 column 1 ..****..
	0b01001010, // character 0x36 column 2 .*..*.*.
	0b01001001, // character 0x36 column 3 .*..*..*
	0b01001001, // character 0x36 column 4 .*..*..*
	0b00110000, // character 0x36 column 5 ..**....
	
	0b00000001, // character 0x37 column 1 .......*
	0b01110001, // character 0x37 column 2 .***...*
	0b00001001, // character 0x37 column 3 ....*..*
	0b00000101, // character 0x37 column 4 .....*.*
	0b00000011, // character 0x37 column 5 ......**
	
	0b00110110, // character 0x38 column 1 ..**.**.
	0b01001001, // character 0x38 column 2 .*..*..*
	0b01001001, // character 0x38 column 3 .*..*..*
	0b01001001, // character 0x38 column 4 .*..*..*
	0b00110110, // character 0x38 column 5 ..**.**.
	
	0b00000110, // character 0x39 column 1 .....**.
	0b01001001, // character 0x39 column 2 .*..*..*
	0b01001001, // character 0x39 column 3 .*..*..*
	0b00101001, // character 0x39 column 4 ..*.*..*
	0b00011110, // character 0x39 column 5 ...****.
	
	0b00000000, // character 0x3A column 1 ........
	0b00110110, // character 0x3A column 2 ..**.**.
	0b00110110, // character 0x3A column 3 ..**.**.
	0b00000000, // character 0x3A column 4 ........
	0b00000000, // character 0x3A column 5 ........
	
	0b00000000, // character 0x3B column 1 ........
	0b01010110, // character 0x3B column 2 .*.*.**.
	0b00110110, // character 0x3B column 3 ..**.**.
	0b00000000, // character 0x3B column 4 ........
	0b00000000, // character 0x3B column 5 ........
	
	0b00001000, // character 0x3C column 1 ....*...
	0b00010100, // character 0x3C column 2 ...*.*..
	0b00100010, // character 0x3C column 3 ..*...*.
	0b01000001, // character 0x3C column 4 .*.....*
	0b00000000, // character 0x3C column 5 ........
	
	0b00010100, // character 0x3D column 1 ...*.*..
	0b00010100, // character 0x3D column 2 ...*.*..
	0b00010100, // character 0x3D column 3 ...*.*..
	0b00010100, // character 0x3D column 4 ...*.*..
	0b00010100, // character 0x3D column 5 ...*.*..
	
	0b00000000, // character 0x3E column 1 ........
	0b01000001, // character 0x3E column 2 .*.....*
	0b00100010, // character 0x3E column 3 ..*...*.
	0b00010100, // character 0x3E column 4 ...*.*..
	0b00001000, // character 0x3E column 5 ....*...
	
	0b00000010, // character 0x3F column 1 ......*.
	0b00000001, // character 0x3F column 2 .......*
	0b01010001, // character 0x3F column 3 .*.*...*
	0b00001001, // character 0x3F column 4 ....*..*
	0b00000110, // character 0x3F column 5 .....**.
	
	0b00110010, // character 0x40 column 1 ..**..*.
	0b01001001, // character 0x40 column 2 .*..*..*
	0b01111001, // character 0x40 column 3 .****..*
	0b01000001, // character 0x40 column 4 .*.....*
	0b00111110, // character 0x40 column 5 ..*****.
	
	0b01111110, // character 0x41 column 1 .******.
	0b00010001, // character 0x41 column 2 ...*...*
	0b00010001, // character 0x41 column 3 ...*...*
	0b00010001, // character 0x41 column 4 ...*...*
	0b01111110, // character 0x41 column 5 .******.
	
	0b01111111, // character 0x42 column 1 .*******
	0b01001001, // character 0x42 column 2 .*..*..*
	0b01001001, // character 0x42 column 3 .*..*..*
	0b01001001, // character 0x42 column 4 .*..*..*
	0b00110110, // character 0x42 column 5 ..**.**.
	
	0b00111110, // character 0x43 column 1 ..*****.
	0b01000001, // character 0x43 column 2 .*.....*
	0b01000001, // character 0x43 column 3 .*.....*
	0b01000001, // character 0x43 column 4 .*.....*
	0b00100010, // character 0x43 column 5 ..*...*.
	
	0b01111111, // character 0x44 column 1 .*******
	0b01000001, // character 0x44 column 2 .*.....*
	0b01000001, // character 0x44 column 3 .*.....*
	0b00100010, // character 0x44 column 4 ..*...*.
	0b00011100, // character 0x44 column 5 ...***..
	
	0b01111111, // character 0x45 column 1 .*******
	0b01001001, // character 0x45 column 2 .*..*..*
	0b01001001, // character 0x45 column 3 .*..*..*
	0b01001001, // character 0x45 column 4 .*..*..*
	0b01000001, // character 0x45 column 5 .*.....*
	
	0b01111111, // character 0x46 column 1 .*******
	0b00001001, // character 0x46 column 2 ....*..*
	0b00001001, // character 0x46 column 3 ....*..*
	0b00001001, // character 0x46 column 4 ....*..*
	0b00000001, // character 0x46 column 5 .......*
	
	0b00111110, // character 0x47 column 1 ..*****.
	0b01000001, // character 0x47 column 2 .*.....*
	0b01001001, // character 0x47 column 3 .*..*..*
	0b01001001, // character 0x47 column 4 .*..*..*
	0b01111010, // character 0x47 column 5 .****.*.
	
	0b01111111, // character 0x48 column 1 .*******
	0b00001000, // character 0x48 column 2 ....*...
	0b00001000, // character 0x48 column 3 ....*...
	0b00001000, // character 0x48 column 4 ....*...
	0b01111111, // character 0x48 column 5 .*******
	
	0b00000000, // character 0x49 column 1 ........
	0b01000001, // character 0x49 column 2 .*.....*
	0b01111111, // character 0x49 column 3 .*******
	0b01000001, // character 0x49 column 4 .*.....*
	0b00000000, // character 0x49 column 5 ........
	
	0b00100000, // character 0x4A column 1 ..*.....
	0b01000000, // character 0x4A column 2 .*......
	0b01000001, // character 0x4A column 3 .*.....*
	0b00111111, // character 0x4A column 4 ..******
	0b00000001, // character 0x4A column 5 .......*
	
	0b01111111, // character 0x4B column 1 .*******
	0b00001000, // character 0x4B column 2 ....*...
	0b00010100, // character 0x4B column 3 ...*.*..
	0b00100010, // character 0x4B column 4 ..*...*.
	0b01000001, // character 0x4B column 5 .*.....*
	
	0b01111111, // character 0x4C column 1 .*******
	0b01000000, // character 0x4C column 2 .*......
	0b01000000, // character 0x4C column 3 .*......
	0b01000000, // character 0x4C column 4 .*......
	0b01000000, // character 0x4C column 5 .*......
	
	0b01111111, // character 0x4D column 1 .*******
	0b00000010, // character 0x4D column 2 ......*.
	0b00001100, // character 0x4D column 3 ....**..
	0b00000010, // character 0x4D column 4 ......*.
	0b01111111, // character 0x4D column 5 .*******
	
	0b01111111, // character 0x4E column 1 .*******
	0b00000100, // character 0x4E column 2 .....*..
	0b00001000, // character 0x4E column 3 ....*...
	0b00010000, // character 0x4E column 4 ...*....
	0b01111111, // character 0x4E column 5 .*******
	
	0b00111110, // character 0x4F column 1 ..*****.
	0b01000001, // character 0x4F column 2 .*.....*
	0b01000001, // character 0x4F column 3 .*.....*
	0b01000001, // character 0x4F column 4 .*.....*
	0b00111110, // character 0x4F column 5 ..*****.
	
	0b01111111, // character 0x50 column 1 .*******
	0b00001001, // character 0x50 column 2 ....*..*
	0b00001001, // character 0x50 column 3 ....*..*
	0b00001001, // character 0x50 column 4 ....*..*
	0b00000110, // character 0x50 column 5 .....**.
	
	0b00111110, // character 0x51 column 1 ..*****.
	0b01000001, // character 0x51 column 2 .*.....*
	0b01010001, // character 0x51 column 3 .*.*...*
	0b00100001, // character 0x51 column 4 ..*....*
	0b01011110, // character 0x51 column 5 .*.****.
	
	0b01111111, // character 0x52 column 1 .*******
	0b00001001, // character 0x52 column 2 ....*..*
	0b00011001, // character 0x52 column 3 ...**..*
	0b00101001, // character 0x52 column 4 ..*.*..*
	0b01000110, // character 0x52 column 5 .*...**.
	
	0b01000110, // character 0x53 column 1 .*...**.
	0b01001001, // character 0x53 column 2 .*..*..*
	0b01001001, // character 0x53 column 3 .*..*..*
	0b01001001, // character 0x53 column 4 .*..*..*
	0b00110001, // character 0x53 column 5 ..**...*
	
	0b00000001, // character 0x54 column 1 .......*
	0b00000001, // character 0x54 column 2 .......*
	0b01111111, // character 0x54 column 3 .*******
	0b00000001, // character 0x54 column 4 .......*
	0b00000001, // character 0x54 column 5 .......*
	
	0b00111111, // character 0x55 column 1 ..******
	0b01000000, // character 0x55 column 2 .*......
	0b01000000, // character 0x55 column 3 .*......
	0b01000000, // character 0x55 column 4 .*......
	0b00111111, // character 0x55 column 5 ..******
	
	0b00011111, // character 0x56 column 1 ...*****
	0b00100000, // character 0x56 column 2 ..*.....
	0b01000000, // character 0x56 column 3 .*......
	0b00100000, // character 0x56 column 4 ..*.....
	0b00011111, // character 0x56 column 5 ...*****
	
	0b00111111, // character 0x57 column 1 ..******
	0b01000000, // character 0x57 column 2 .*......
	0b00111000, // character 0x57 column 3 ..***...
	0b01000000, // character 0x57 column 4 .*......
	0b00111111, // character 0x57 column 5 ..******
	
	0b01100011, // character 0x58 column 1 .**...**
	0b00010100, // character 0x58 column 2 ...*.*..
	0b00001000, // character 0x58 column 3 ....*...
	0b00010100, // character 0x58 column 4 ...*.*..
	0b01100011, // character 0x58 column 5 .**...**
	
	0b00000111, // character 0x59 column 1 .....***
	0b00001000, // character 0x59 column 2 ....*...
	0b01110000, // character 0x59 column 3 .***....
	0b00001000, // character 0x59 column 4 ....*...
	0b00000111, // character 0x59 column 5 .....***
	
	0b01100001, // character 0x5A column 1 .**....*
	0b01010001, // character 0x5A column 2 .*.*...*
	0b01001001, // character 0x5A column 3 .*..*..*
	0b01000101, // character 0x5A column 4 .*...*.*
	0b01000011, // character 0x5A column 5 .*....**
	
	0b00000000, // character 0x5B column 1 ........
	0b01111111, // character 0x5B column 2 .*******
	0b01000001, // character 0x5B column 3 .*.....*
	0b01000001, // character 0x5B column 4 .*.....*
	0b00000000, // character 0x5B column 5 ........
	
	0b00010101, // character 0x5C column 1 ...*.*.*
	0b00010110, // character 0x5C column 2 ...*.**.
	0b01111100, // character 0x5C column 3 .*****..
	0b00010110, // character 0x5C column 4 ...*.**.
	0b00010101, // character 0x5C column 5 ...*.*.*
	
	0b00000000, // character 0x5D column 1 ........
	0b01000001, // character 0x5D column 2 .*.....*
	0b01000001, // character 0x5D column 3 .*.....*
	0b01111111, // character 0x5D column 4 .*******
	0b00000000, // character 0x5D column 5 ........
	
	0b00000100, // character 0x5E column 1 .....*..
	0b00000010, // character 0x5E column 2 ......*.
	0b00000001, // character 0x5E column 3 .......*
	0b00000010, // character 0x5E column 4 ......*.
	0b00000100, // character 0x5E column 5 .....*..
	
	0b01000000, // character 0x5F column 1 .*......
	0b01000000, // character 0x5F column 2 .*......
	0b01000000, // character 0x5F column 3 .*......
	0b01000000, // character 0x5F column 4 .*......
	0b01000000, // character 0x5F column 5 .*......
	
	0b00000000, // character 0x60 column 1 ........
	0b00000001, // character 0x60 column 2 .......*
	0b00000010, // character 0x60 column 3 ......*.
	0b00000100, // character 0x60 column 4 .....*..
	0b00000000, // character 0x60 column 5 ........
	
	0b00100000, // character 0x61 column 1 ..*.....
	0b01010100, // character 0x61 column 2 .*.*.*..
	0b01010100, // character 0x61 column 3 .*.*.*..
	0b01010100, // character 0x61 column 4 .*.*.*..
	0b01111000, // character 0x61 column 5 .****...
	
	0b01111111, // character 0x62 column 1 .*******
	0b01001000, // character 0x62 column 2 .*..*...
	0b01000100, // character 0x62 column 3 .*...*..
	0b01000100, // character 0x62 column 4 .*...*..
	0b00111000, // character 0x62 column 5 ..***...
	
	0b00111000, // character 0x63 column 1 ..***...
	0b01000100, // character 0x63 column 2 .*...*..
	0b01000100, // character 0x63 column 3 .*...*..
	0b01000100, // character 0x63 column 4 .*...*..
	0b00100000, // character 0x63 column 5 ..*.....
	
	0b00111000, // character 0x64 column 1 ..***...
	0b01000100, // character 0x64 column 2 .*...*..
	0b01000100, // character 0x64 column 3 .*...*..
	0b01001000, // character 0x64 column 4 .*..*...
	0b01111111, // character 0x64 column 5 .*******
	
	0b00111000, // character 0x65 column 1 ..***...
	0b01010100, // character 0x65 column 2 .*.*.*..
	0b01010100, // character 0x65 column 3 .*.*.*..
	0b01010100, // character 0x65 column 4 .*.*.*..
	0b00011000, // character 0x65 column 5 ...**...
	
	0b00001000, // character 0x66 column 1 ....*...
	0b01111110, // character 0x66 column 2 .******.
	0b00001001, // character 0x66 column 3 ....*..*
	0b00000001, // character 0x66 column 4 .......*
	0b00000010, // character 0x66 column 5 ......*.
	
	0b00001100, // character 0x67 column 1 ....**..
	0b01010010, // character 0x67 column 2 .*.*..*.
	0b01010010, // character 0x67 column 3 .*.*..*.
	0b01010010, // character 0x67 column 4 .*.*..*.
	0b00111110, // character 0x67 column 5 ..*****.
	
	0b01111111, // character 0x68 column 1 .*******
	0b00001000, // character 0x68 column 2 ....*...
	0b00000100, // character 0x68 column 3 .....*..
	0b00000100, // character 0x68 column 4 .....*..
	0b01111000, // character 0x68 column 5 .****...
	
	0b00000000, // character 0x69 column 1 ........
	0b01000100, // character 0x69 column 2 .*...*..
	0b01111101, // character 0x69 column 3 .*****.*
	0b01000000, // character 0x69 column 4 .*......
	0b00000000, // character 0x69 column 5 ........
	
	0b00100000, // character 0x6A column 1 ..*.....
	0b01000000, // character 0x6A column 2 .*......
	0b01000100, // character 0x6A column 3 .*...*..
	0b00111101, // character 0x6A column 4 ..****.*
	0b00000000, // character 0x6A column 5 ........
	
	0b00000000, // character 0x6B column 1 ........
	0b01111111, // character 0x6B column 2 .*******
	0b00010000, // character 0x6B column 3 ...*....
	0b00101000, // character 0x6B column 4 ..*.*...
	0b01000100, // character 0x6B column 5 .*...*..
	
	0b00000000, // character 0x6C column 1 ........
	0b01000001, // character 0x6C column 2 .*.....*
	0b01111111, // character 0x6C column 3 .*******
	0b01000000, // character 0x6C column 4 .*......
	0b00000000, // character 0x6C column 5 ........
	
	0b01111100, // character 0x6D column 1 .*****..
	0b00000100, // character 0x6D column 2 .....*..
	0b00011000, // character 0x6D column 3 ...**...
	0b00000100, // character 0x6D column 4 .....*..
	0b01111000, // character 0x6D column 5 .****...
	
	0b01111100, // character 0x6E column 1 .*****..
	0b00001000, // character 0x6E column 2 ....*...
	0b00000100, // character 0x6E column 3 .....*..
	0b00000100, // character 0x6E column 4 .....*..
	0b01111000, // character 0x6E column 5 .****...
	
	0b00111000, // character 0x6F column 1 ..***...
	0b01000100, // character 0x6F column 2 .*...*..
	0b01000100, // character 0x6F column 3 .*...*..
	0b01000100, // character 0x6F column 4 .*...*..
	0b00111000, // character 0x6F column 5 ..***...
	
	0b01111100, // character 0x70 column 1 .*****..
	0b00010100, // character 0x70 column 2 ...*.*..
	0b00010100, // character 0x70 column 3 ...*.*..
	0b00010100, // character 0x70 column 4 ...*.*..
	0b00001000, // character 0x70 column 5 ....*...
	
	0b00001000, // character 0x71 column 1 ....*...
	0b00010100, // character 0x71 column 2 ...*.*..
	0b00010100, // character 0x71 column 3 ...*.*..
	0b00011000, // character 0x71 column 4 ...**...
	0b01111100, // character 0x71 column 5 .*****..
	
	0b01111100, // character 0x72 column 1 .*****..
	0b00001000, // character 0x72 column 2 ....*...
	0b00000100, // character 0x72 column 3 .....*..
	0b00000100, // character 0x72 column 4 .....*..
	0b00001000, // character 0x72 column 5 ....*...
	
	0b01001000, // character 0x73 column 1 .*..*...
	0b01010100, // character 0x73 column 2 .*.*.*..
	0b01010100, // character 0x73 column 3 .*.*.*..
	0b01010100, // character 0x73 column 4 .*.*.*..
	0b00100000, // character 0x73 column 5 ..*.....
	
	0b00000100, // character 0x74 column 1 .....*..
	0b00111111, // character 0x74 column 2 ..******
	0b01000100, // character 0x74 column 3 .*...*..
	0b01000000, // character 0x74 column 4 .*......
	0b00100000, // character 0x74 column 5 ..*.....
	
	0b00111100, // character 0x75 column 1 ..****..
	0b01000000, // character 0x75 column 2 .*......
	0b01000000, // character 0x75 column 3 .*......
	0b00100000, // character 0x75 column 4 ..*.....
	0b01111100, // character 0x75 column 5 .*****..
	
	0b00011100, // character 0x76 column 1 ...***..
	0b00100000, // character 0x76 column 2 ..*.....
	0b01000000, // character 0x76 column 3 .*......
	0b00100000, // character 0x76 column 4 ..*.....
	0b00011100, // character 0x76 column 5 ...***..
	
	0b00111100, // character 0x77 column 1 ..****..
	0b01000000, // character 0x77 column 2 .*......
	0b00110000, // character 0x77 column 3 ..**....
	0b01000000, // character 0x77 column 4 .*......
	0b00111100, // character 0x77 column 5 ..****..
	
	0b01000100, // character 0x78 column 1 .*...*..
	0b00101000, // character 0x78 column 2 ..*.*...
	0b00010000, // character 0x78 column 3 ...*....
	0b00101000, // character 0x78 column 4 ..*.*...
	0b01000100, // character 0x78 column 5 .*...*..
	
	0b00001100, // character 0x79 column 1 ....**..
	0b01010000, // character 0x79 column 2 .*.*....
	0b01010000, // character 0x79 column 3 .*.*....
	0b01010000, // character 0x79 column 4 .*.*....
	0b00111100, // character 0x79 column 5 ..****..
	
	0b01000100, // character 0x7A column 1 .*...*..
	0b01100100, // character 0x7A column 2 .**..*..
	0b01010100, // character 0x7A column 3 .*.*.*..
	0b01001100, // character 0x7A column 4 .*..**..
	0b01000100, // character 0x7A column 5 .*...*..
	
	0b00000000, // character 0x7B column 1 ........
	0b00001000, // character 0x7B column 2 ....*...
	0b00110110, // character 0x7B column 3 ..**.**.
	0b01000001, // character 0x7B column 4 .*.....*
	0b00000000, // character 0x7B column 5 ........
	
	0b00000000, // character 0x7C column 1 ........
	0b00000000, // character 0x7C column 2 ........
	0b01111111, // character 0x7C column 3 .*******
	0b00000000, // character 0x7C column 4 ........
	0b00000000, // character 0x7C column 5 ........
	
	0b00000000, // character 0x7D column 1 ........
	0b01000001, // character 0x7D column 2 .*.....*
	0b00110110, // character 0x7D column 3 ..**.**.
	0b00001000, // character 0x7D column 4 ....*...
	0b00000000, // character 0x7D column 5 ........
	
	0b00001000, // character 0x7E column 1 ....*...
	0b00001000, // character 0x7E column 2 ....*...
	0b00101010, // character 0x7E column 3 ..*.*.*.
	0b00011100, // character 0x7E column 4 ...***..
	0b00001000, // character 0x7E column 5 ....*...
	
	0b00001000, // character 0x7F column 1 ....*...
	0b00011100, // character 0x7F column 2 ...***..
	0b00101010, // character 0x7F column 3 ..*.*.*.
	0b00001000, // character 0x7F column 4 ....*...
	0b00001000, // character 0x7F column 5 ....*...

	//--------------------------------------------------------
	// 2nd HALF
	//--------------------------------------------------------
	
	0b00000000, // character 0xA0 column 1 ........
	0b00000000, // character 0xA0 column 2 ........
	0b00000000, // character 0xA0 column 3 ........
	0b00000000, // character 0xA0 column 4 ........
	0b00000000, // character 0xA0 column 5 ........
	   
	0b01110000, // character 0xA1 column 1 .***....
	0b01010000, // character 0xA1 column 2 .*.*....
	0b01110000, // character 0xA1 column 3 .***....
	0b00000000, // character 0xA1 column 4 ........
	0b00000000, // character 0xA1 column 5 ........
	   
	0b00000000, // character 0xA2 column 1 ........
	0b00000000, // character 0xA2 column 2 ........
	0b00001111, // character 0xA2 column 3 ....****
	0b00000001, // character 0xA2 column 4 .......*
	0b00000001, // character 0xA2 column 5 .......*
	   
	0b01000000, // character 0xA3 column 1 .*......
	0b01000000, // character 0xA3 column 2 .*......
	0b01111000, // character 0xA3 column 3 .****...
	0b00000000, // character 0xA3 column 4 ........
	0b00000000, // character 0xA3 column 5 ........
	   
	0b00010000, // character 0xA4 column 1 ...*....
	0b00100000, // character 0xA4 column 2 ..*.....
	0b01000000, // character 0xA4 column 3 .*......
	0b00000000, // character 0xA4 column 4 ........
	0b00000000, // character 0xA4 column 5 ........
	   
	0b00000000, // character 0xA5 column 1 ........
	0b00110000, // character 0xA5 column 2 ..**....
	0b00110000, // character 0xA5 column 3 ..**....
	0b00000000, // character 0xA5 column 4 ........
	0b00000000, // character 0xA5 column 5 ........
	   
	0b00000101, // character 0xA6 column 1 .....*.*
	0b00000101, // character 0xA6 column 2 .....*.*
	0b01000101, // character 0xA6 column 3 .*...*.*
	0b00100101, // character 0xA6 column 4 ..*..*.*
	0b00011111, // character 0xA6 column 5 ...*****
	   
	0b00000100, // character 0xA7 column 1 .....*..
	0b01000100, // character 0xA7 column 2 .*...*..
	0b00110100, // character 0xA7 column 3 ..**.*..
	0b00010100, // character 0xA7 column 4 ...*.*..
	0b00001100, // character 0xA7 column 5 ....**..
	   
	0b00100000, // character 0xA8 column 1 ..*.....
	0b00010000, // character 0xA8 column 2 ...*....
	0b01111000, // character 0xA8 column 3 .****...
	0b00000100, // character 0xA8 column 4 .....*..
	0b00000000, // character 0xA8 column 5 ........
	   
	0b00011000, // character 0xA9 column 1 ...**...
	0b00001000, // character 0xA9 column 2 ....*...
	0b01001100, // character 0xA9 column 3 .*..**..
	0b00111000, // character 0xA9 column 4 ..***...
	0b00000000, // character 0xA9 column 5 ........
	
	0b01001000, // character 0xAA column 1 .*..*...
	0b01001000, // character 0xAA column 2 .*..*...
	0b01111000, // character 0xAA column 3 .****...
	0b01001000, // character 0xAA column 4 .*..*...
	0b01001000, // character 0xAA column 5 .*..*...
	
	0b01001000, // character 0xAB column 1 .*..*...
	0b00101000, // character 0xAB column 2 ..*.*...
	0b00011000, // character 0xAB column 3 ...**...
	0b01111100, // character 0xAB column 4 .*****..
	0b00001000, // character 0xAB column 5 ....*...
	   
	0b00001000, // character 0xAC column 1 ....*...
	0b01111100, // character 0xAC column 2 .*****..
	0b00001000, // character 0xAC column 3 ....*...
	0b00101000, // character 0xAC column 4 ..*.*...
	0b00011000, // character 0xAC column 5 ...**...
	   
	0b01000000, // character 0xAD column 1 .*......
	0b01001000, // character 0xAD column 2 .*..*...
	0b01001000, // character 0xAD column 3 .*..*...
	0b01111000, // character 0xAD column 4 .****...
	0b01000000, // character 0xAD column 5 .*......
	   
	0b01010100, // character 0xAE column 1 .*.*.*..
	0b01010100, // character 0xAE column 2 .*.*.*..
	0b01010100, // character 0xAE column 3 .*.*.*..
	0b01111100, // character 0xAE column 4 .*****..
	0b00000000, // character 0xAE column 5 ........
	   
	0b00011000, // character 0xAF column 1 ...**...
	0b00000000, // character 0xAF column 2 ........
	0b01011000, // character 0xAF column 3 .*.**...
	0b01000000, // character 0xAF column 4 .*......
	0b00111000, // character 0xAF column 5 ..***...
	
	0b00001000, // character 0xB0 column 1 ....*...
	0b00001000, // character 0xB0 column 2 ....*...
	0b00001000, // character 0xB0 column 3 ....*...
	0b00001000, // character 0xB0 column 4 ....*...
	0b00001000, // character 0xB0 column 5 ....*...
	   
	0b00000001, // character 0xB1 column 1 .......*
	0b01000001, // character 0xB1 column 2 .*.....*
	0b00111101, // character 0xB1 column 3 ..****.*
	0b00001001, // character 0xB1 column 4 ....*..*
	0b00000111, // character 0xB1 column 5 .....***
	   
	0b00100000, // character 0xB2 column 1 ..*.....
	0b00010000, // character 0xB2 column 2 ...*....
	0b11111000, // character 0xB2 column 3 *****...
	0b00000100, // character 0xB2 column 4 .....*..
	0b00000010, // character 0xB2 column 5 ......*.
	   
	0b00001110, // character 0xB3 column 1 ....***.
	0b00000010, // character 0xB3 column 2 ......*.
	0b01000011, // character 0xB3 column 3 .*....**
	0b00100010, // character 0xB3 column 4 ..*...*.
	0b00011110, // character 0xB3 column 5 ...****.
	   
	0b01000010, // character 0xB4 column 1 .*....*.
	0b01000010, // character 0xB4 column 2 .*....*.
	0b01111110, // character 0xB4 column 3 .******.
	0b01000010, // character 0xB4 column 4 .*....*.
	0b01000010, // character 0xB4 column 5 .*....*.
	   
	0b00100010, // character 0xB5 column 1 ..*...*.
	0b00010010, // character 0xB5 column 2 ...*..*.
	0b00001010, // character 0xB5 column 3 ....*.*.
	0b01111111, // character 0xB5 column 4 .*******
	0b00000010, // character 0xB5 column 5 ......*.
	   
	0b01000010, // character 0xB6 column 1 .*....*.
	0b00111111, // character 0xB6 column 2 ..******
	0b00000010, // character 0xB6 column 3 ......*.
	0b01000010, // character 0xB6 column 4 .*....*.
	0b00111110, // character 0xB6 column 5 ..*****.
	   
	0b00001010, // character 0xB7 column 1 ....*.*.
	0b00001010, // character 0xB7 column 2 ....*.*.
	0b01111111, // character 0xB7 column 3 .*******
	0b00001010, // character 0xB7 column 4 ....*.*.
	0b00001010, // character 0xB7 column 5 ....*.*.
	   
	0b00001000, // character 0xB8 column 1 ....*...
	0b01000110, // character 0xB8 column 2 .*...**.
	0b01000010, // character 0xB8 column 3 .*....*.
	0b00100010, // character 0xB8 column 4 ..*...*.
	0b00011110, // character 0xB8 column 5 ...****.
	   
	0b00000100, // character 0xB9 column 1 .....*..
	0b00000011, // character 0xB9 column 2 ......**
	0b01000010, // character 0xB9 column 3 .*....*.
	0b00111110, // character 0xB9 column 4 ..*****.
	0b00000010, // character 0xB9 column 5 ......*.
	   
	0b01000010, // character 0xBA column 1 .*....*.
	0b01000010, // character 0xBA column 2 .*....*.
	0b01000010, // character 0xBA column 3 .*....*.
	0b01000010, // character 0xBA column 4 .*....*.
	0b01111110, // character 0xBA column 5 .******.
	   
	0b00000010, // character 0xBB column 1 ......*.
	0b01001111, // character 0xBB column 2 .*..****
	0b00100010, // character 0xBB column 3 ..*...*.
	0b00011111, // character 0xBB column 4 ...*****
	0b00000010, // character 0xBB column 5 ......*.
	   
	0b01001010, // character 0xBC column 1 .*..*.*.
	0b01001010, // character 0xBC column 2 .*..*.*.
	0b01000000, // character 0xBC column 3 .*......
	0b00100000, // character 0xBC column 4 ..*.....
	0b00011100, // character 0xBC column 5 ...***..
	   
	0b01000010, // character 0xBD column 1 .*....*.
	0b00100010, // character 0xBD column 2 ..*...*.
	0b00010010, // character 0xBD column 3 ...*..*.
	0b00101010, // character 0xBD column 4 ..*.*.*.
	0b01000110, // character 0xBD column 5 .*...**.
	   
	0b00000010, // character 0xBE column 1 ......*.
	0b00111111, // character 0xBE column 2 ..******
	0b01000010, // character 0xBE column 3 .*....*.
	0b01001010, // character 0xBE column 4 .*..*.*.
	0b01000110, // character 0xBE column 5 .*...**.
	   
	0b00000110, // character 0xBF column 1 .....**.
	0b01001000, // character 0xBF column 2 .*..*...
	0b01000000, // character 0xBF column 3 .*......
	0b00100000, // character 0xBF column 4 ..*.....
	0b00011110, // character 0xBF column 5 ...****.
	
	0b00001000, // character 0xC0 column 1 ....*...
	0b01000110, // character 0xC0 column 2 .*...**.
	0b01001010, // character 0xC0 column 3 .*..*.*.
	0b00110010, // character 0xC0 column 4 ..**..*.
	0b00011110, // character 0xC0 column 5 ...****.
	   
	0b00001010, // character 0xC1 column 1 ....*.*.
	0b01001010, // character 0xC1 column 2 .*..*.*.
	0b00111110, // character 0xC1 column 3 ..*****.
	0b00001001, // character 0xC1 column 4 ....*..*
	0b00001000, // character 0xC1 column 5 ....*...
	   
	0b00001110, // character 0xC2 column 1 ....***.
	0b00000000, // character 0xC2 column 2 ........
	0b01001110, // character 0xC2 column 3 .*..***.
	0b00100000, // character 0xC2 column 4 ..*.....
	0b00011110, // character 0xC2 column 5 ...****.
	   
	0b00000100, // character 0xC3 column 1 .....*..
	0b01000101, // character 0xC3 column 2 .*...*.*
	0b00111101, // character 0xC3 column 3 ..****.*
	0b00000101, // character 0xC3 column 4 .....*.*
	0b00000100, // character 0xC3 column 5 .....*..
	   
	0b00000000, // character 0xC4 column 1 ........
	0b01111111, // character 0xC4 column 2 .*******
	0b00001000, // character 0xC4 column 3 ....*...
	0b00010000, // character 0xC4 column 4 ...*....
	0b00000000, // character 0xC4 column 5 ........
	   
	0b01000100, // character 0xC5 column 1 .*...*..
	0b00100100, // character 0xC5 column 2 ..*..*..
	0b00011111, // character 0xC5 column 3 ...*****
	0b00000100, // character 0xC5 column 4 .....*..
	0b00000100, // character 0xC5 column 5 .....*..
	   
	0b01000000, // character 0xC6 column 1 .*......
	0b01000010, // character 0xC6 column 2 .*....*.
	0b01000010, // character 0xC6 column 3 .*....*.
	0b01000010, // character 0xC6 column 4 .*....*.
	0b01000000, // character 0xC6 column 5 .*......
	   
	0b01000010, // character 0xC7 column 1 .*....*.
	0b00101010, // character 0xC7 column 2 ..*.*.*.
	0b00010010, // character 0xC7 column 3 ...*..*.
	0b00101010, // character 0xC7 column 4 ..*.*.*.
	0b00000110, // character 0xC7 column 5 .....**.
	   
	0b00100010, // character 0xC8 column 1 ..*...*.
	0b00010010, // character 0xC8 column 2 ...*..*.
	0b01111011, // character 0xC8 column 3 .****.**
	0b00010110, // character 0xC8 column 4 ...*.**.
	0b00100010, // character 0xC8 column 5 ..*...*.
	   
	0b00000000, // character 0xC9 column 1 ........
	0b01000000, // character 0xC9 column 2 .*......
	0b00100000, // character 0xC9 column 3 ..*.....
	0b00011111, // character 0xC9 column 4 ...*****
	0b00000000, // character 0xC9 column 5 ........
	   
	0b01111000, // character 0xCA column 1 .****...
	0b00000000, // character 0xCA column 2 ........
	0b00000010, // character 0xCA column 3 ......*.
	0b00000100, // character 0xCA column 4 .....*..
	0b01111000, // character 0xCA column 5 .****...
	   
	0b00111111, // character 0xCB column 1 ..******
	0b01000100, // character 0xCB column 2 .*...*..
	0b01000100, // character 0xCB column 3 .*...*..
	0b01000100, // character 0xCB column 4 .*...*..
	0b01000100, // character 0xCB column 5 .*...*..
	   
	0b00000010, // character 0xCC column 1 ......*.
	0b01000010, // character 0xCC column 2 .*....*.
	0b01000010, // character 0xCC column 3 .*....*.
	0b00100010, // character 0xCC column 4 ..*...*.
	0b00011110, // character 0xCC column 5 ...****.
	   
	0b00000100, // character 0xCD column 1 .....*..
	0b00000010, // character 0xCD column 2 ......*.
	0b00000100, // character 0xCD column 3 .....*..
	0b00001000, // character 0xCD column 4 ....*...
	0b00110000, // character 0xCD column 5 ..**....
	   
	0b00110010, // character 0xCE column 1 ..**..*.
	0b00000010, // character 0xCE column 2 ......*.
	0b01111111, // character 0xCE column 3 .*******
	0b00000010, // character 0xCE column 4 ......*.
	0b00110010, // character 0xCE column 5 ..**..*.
	   
	0b00000010, // character 0xCF column 1 ......*.
	0b00010010, // character 0xCF column 2 ...*..*.
	0b00100010, // character 0xCF column 3 ..*...*.
	0b01010010, // character 0xCF column 4 .*.*..*.
	0b00001110, // character 0xCF column 5 ....***.
	
	0b00000000, // character 0xD0 column 1 ........
	0b00101010, // character 0xD0 column 2 ..*.*.*.
	0b00101010, // character 0xD0 column 3 ..*.*.*.
	0b00101010, // character 0xD0 column 4 ..*.*.*.
	0b01000000, // character 0xD0 column 5 .*......
	   
	0b00111000, // character 0xD1 column 1 ..***...
	0b00100100, // character 0xD1 column 2 ..*..*..
	0b00100010, // character 0xD1 column 3 ..*...*.
	0b00100000, // character 0xD1 column 4 ..*.....
	0b01110000, // character 0xD1 column 5 .***....
	   
	0b01000000, // character 0xD2 column 1 .*......
	0b00101000, // character 0xD2 column 2 ..*.*...
	0b00010000, // character 0xD2 column 3 ...*....
	0b00101000, // character 0xD2 column 4 ..*.*...
	0b00000110, // character 0xD2 column 5 .....**.
	   
	0b00000010, // character 0xD3 column 1 ......*.
	0b00111110, // character 0xD3 column 2 ..*****.
	0b01001010, // character 0xD3 column 3 .*..*.*.
	0b01001010, // character 0xD3 column 4 .*..*.*.
	0b01001010, // character 0xD3 column 5 .*..*.*.
	   
	0b00000100, // character 0xD4 column 1 .....*..
	0b01111111, // character 0xD4 column 2 .*******
	0b00000100, // character 0xD4 column 3 .....*..
	0b00010100, // character 0xD4 column 4 ...*.*..
	0b00001100, // character 0xD4 column 5 ....**..
	   
	0b01000000, // character 0xD5 column 1 .*......
	0b01000010, // character 0xD5 column 2 .*....*.
	0b01000010, // character 0xD5 column 3 .*....*.
	0b01111110, // character 0xD5 column 4 .******.
	0b01000000, // character 0xD5 column 5 .*......
	   
	0b01001010, // character 0xD6 column 1 .*..*.*.
	0b01001010, // character 0xD6 column 2 .*..*.*.
	0b01001010, // character 0xD6 column 3 .*..*.*.
	0b01001010, // character 0xD6 column 4 .*..*.*.
	0b01111110, // character 0xD6 column 5 .******.
	   
	0b00000100, // character 0xD7 column 1 .....*..
	0b00000101, // character 0xD7 column 2 .....*.*
	0b01000101, // character 0xD7 column 3 .*...*.*
	0b00100101, // character 0xD7 column 4 ..*..*.*
	0b00011100, // character 0xD7 column 5 ...***..
	   
	0b00001111, // character 0xD8 column 1 ....****
	0b01000000, // character 0xD8 column 2 .*......
	0b00100000, // character 0xD8 column 3 ..*.....
	0b00011111, // character 0xD8 column 4 ...*****
	0b00000000, // character 0xD8 column 5 ........
	   
	0b01111100, // character 0xD9 column 1 .*****..
	0b00000000, // character 0xD9 column 2 ........
	0b01111110, // character 0xD9 column 3 .******.
	0b01000000, // character 0xD9 column 4 .*......
	0b00110000, // character 0xD9 column 5 ..**....
	   
	0b01111110, // character 0xDA column 1 .******.
	0b01000000, // character 0xDA column 2 .*......
	0b00100000, // character 0xDA column 3 ..*.....
	0b00010000, // character 0xDA column 4 ...*....
	0b00001000, // character 0xDA column 5 ....*...
	   
	0b01111110, // character 0xDB column 1 .******.
	0b01000010, // character 0xDB column 2 .*....*.
	0b01000010, // character 0xDB column 3 .*....*.
	0b01000010, // character 0xDB column 4 .*....*.
	0b01111110, // character 0xDB column 5 .******.
	   
	0b00001110, // character 0xDC column 1 ....***.
	0b00000010, // character 0xDC column 2 ......*.
	0b01000010, // character 0xDC column 3 .*....*.
	0b00100010, // character 0xDC column 4 ..*...*.
	0b00011110, // character 0xDC column 5 ...****.
	   
	0b01000010, // character 0xDD column 1 .*....*.
	0b01000010, // character 0xDD column 2 .*....*.
	0b01000000, // character 0xDD column 3 .*......
	0b00100000, // character 0xDD column 4 ..*.....
	0b00011000, // character 0xDD column 5 ...**...
	   
	0b00000010, // character 0xDE column 1 ......*.
	0b00000100, // character 0xDE column 2 .....*..
	0b00000001, // character 0xDE column 3 .......*
	0b00000010, // character 0xDE column 4 ......*.
	0b00000000, // character 0xDE column 5 ........
	   
	0b00000111, // character 0xDF column 1 .....***
	0b00000101, // character 0xDF column 2 .....*.*
	0b00000111, // character 0xDF column 3 .....***
	0b00000000, // character 0xDF column 4 ........
	0b00000000, // character 0xDF column 5 ........
	
	0b00111000, // character 0xE0 column 1 ..***...
	0b01000100, // character 0xE0 column 2 .*...*..
	0b01001000, // character 0xE0 column 3 .*..*...
	0b00110000, // character 0xE0 column 4 ..**....
	0b01001100, // character 0xE0 column 5 .*..**..
	   
	0b00100000, // character 0xE1 column 1 ..*.....
	0b01010101, // character 0xE1 column 2 .*.*.*.*
	0b01010100, // character 0xE1 column 3 .*.*.*..
	0b01010101, // character 0xE1 column 4 .*.*.*.*
	0b01111000, // character 0xE1 column 5 .****...
	   
	0b01111110, // character 0xE2 column 1 .******.
	0b00010101, // character 0xE2 column 2 ...*.*.*
	0b00010101, // character 0xE2 column 3 ...*.*.*
	0b00010101, // character 0xE2 column 4 ...*.*.*
	0b00001010, // character 0xE2 column 5 ....*.*.
	   
	0b00101000, // character 0xE3 column 1 ..*.*...
	0b01010100, // character 0xE3 column 2 .*.*.*..
	0b01010100, // character 0xE3 column 3 .*.*.*..
	0b01000100, // character 0xE3 column 4 .*...*..
	0b00100000, // character 0xE3 column 5 ..*.....
	   
	0b01111111, // character 0xE4 column 1 .*******
	0b00010000, // character 0xE4 column 2 ...*....
	0b00010000, // character 0xE4 column 3 ...*....
	0b00001000, // character 0xE4 column 4 ....*...
	0b00011111, // character 0xE4 column 5 ...*****
	   
	0b00111000, // character 0xE5 column 1 ..***...
	0b01000100, // character 0xE5 column 2 .*...*..
	0b01001100, // character 0xE5 column 3 .*..**..
	0b01010100, // character 0xE5 column 4 .*.*.*..
	0b00100100, // character 0xE5 column 5 ..*..*..
	   
	0b01111100, // character 0xE6 column 1 .*****..
	0b00010010, // character 0xE6 column 2 ...*..*.
	0b00010001, // character 0xE6 column 3 ...*...*
	0b00010001, // character 0xE6 column 4 ...*...*
	0b00001110, // character 0xE6 column 5 ....***.
	   
	0b00000110, // character 0xE7 column 1 .....**.
	0b01001001, // character 0xE7 column 2 .*..*..*
	0b01001001, // character 0xE7 column 3 .*..*..*
	0b01001001, // character 0xE7 column 4 .*..*..*
	0b00111111, // character 0xE7 column 5 ..******
	   
	0b00100000, // character 0xE8 column 1 ..*.....
	0b01000000, // character 0xE8 column 2 .*......
	0b00111100, // character 0xE8 column 3 ..****..
	0b00000100, // character 0xE8 column 4 .....*..
	0b00000100, // character 0xE8 column 5 .....*..
	   
	0b00000100, // character 0xE9 column 1 .....*..
	0b00000100, // character 0xE9 column 2 .....*..
	0b00000000, // character 0xE9 column 3 ........
	0b00001110, // character 0xE9 column 4 ....***.
	0b00000000, // character 0xE9 column 5 ........
	   
	0b00100000, // character 0xEA column 1 ..*.....
	0b01000000, // character 0xEA column 2 .*......
	0b01000100, // character 0xEA column 3 .*...*..
	0b00111101, // character 0xEA column 4 ..****.*
	0b00000000, // character 0xEA column 5 ........
	   
	0b00001010, // character 0xEB column 1 ....*.*.
	0b00000100, // character 0xEB column 2 .....*..
	0b00001010, // character 0xEB column 3 ....*.*.
	0b00000000, // character 0xEB column 4 ........
	0b00000000, // character 0xEB column 5 ........
	   
	0b00011000, // character 0xEC column 1 ...**...
	0b00100100, // character 0xEC column 2 ..*..*..
	0b01111110, // character 0xEC column 3 .******.
	0b00100100, // character 0xEC column 4 ..*..*..
	0b00010000, // character 0xEC column 5 ...*....
	   
	0b00010100, // character 0xED column 1 ...*.*..
	0b01111111, // character 0xED column 2 .*******
	0b01010100, // character 0xED column 3 .*.*.*..
	0b01000000, // character 0xED column 4 .*......
	0b01000000, // character 0xED column 5 .*......
	   
	0b01111100, // character 0xEE column 1 .*****..
	0b00001001, // character 0xEE column 2 ....*..*
	0b00000101, // character 0xEE column 3 .....*.*
	0b00000101, // character 0xEE column 4 .....*.*
	0b01111000, // character 0xEE column 5 .****...
	   
	0b00111000, // character 0xEF column 1 ..***...
	0b01000101, // character 0xEF column 2 .*...*.*
	0b01000100, // character 0xEF column 3 .*...*..
	0b01000101, // character 0xEF column 4 .*...*.*
	0b00111000, // character 0xEF column 5 ..***...
	
	0b01111111, // character 0xF0 column 1 .*******
	0b00010010, // character 0xF0 column 2 ...*..*.
	0b00010001, // character 0xF0 column 3 ...*...*
	0b00010001, // character 0xF0 column 4 ...*...*
	0b00001110, // character 0xF0 column 5 ....***.
	   
	0b00001110, // character 0xF1 column 1 ....***.
	0b00010001, // character 0xF1 column 2 ...*...*
	0b00010001, // character 0xF1 column 3 ...*...*
	0b00010010, // character 0xF1 column 4 ...*..*.
	0b01111111, // character 0xF1 column 5 .*******
	   
	0b00111100, // character 0xF2 column 1 ..****..
	0b01001010, // character 0xF2 column 2 .*..*.*.
	0b01001010, // character 0xF2 column 3 .*..*.*.
	0b01001010, // character 0xF2 column 4 .*..*.*.
	0b00111100, // character 0xF2 column 5 ..****..
	   
	0b00110000, // character 0xF3 column 1 ..**....
	0b00101000, // character 0xF3 column 2 ..*.*...
	0b00010000, // character 0xF3 column 3 ...*....
	0b00101000, // character 0xF3 column 4 ..*.*...
	0b00011000, // character 0xF3 column 5 ...**...
	   
	0b01011000, // character 0xF4 column 1 .*.**...
	0b01100100, // character 0xF4 column 2 .**..*..
	0b00000100, // character 0xF4 column 3 .....*..
	0b01100100, // character 0xF4 column 4 .**..*..
	0b01011000, // character 0xF4 column 5 .*.**...
	   
	0b00111100, // character 0xF5 column 1 ..****..
	0b01000001, // character 0xF5 column 2 .*.....*
	0b01000000, // character 0xF5 column 3 .*......
	0b00100001, // character 0xF5 column 4 ..*....*
	0b01111100, // character 0xF5 column 5 .*****..
	   
	0b01100011, // character 0xF6 column 1 .**...**
	0b01010101, // character 0xF6 column 2 .*.*.*.*
	0b01001001, // character 0xF6 column 3 .*..*..*
	0b01000001, // character 0xF6 column 4 .*.....*
	0b01000001, // character 0xF6 column 5 .*.....*
	   
	0b01000100, // character 0xF7 column 1 .*...*..
	0b00111100, // character 0xF7 column 2 ..****..
	0b00000100, // character 0xF7 column 3 .....*..
	0b01111100, // character 0xF7 column 4 .*****..
	0b01000100, // character 0xF7 column 5 .*...*..
	   
	0b01000101, // character 0xF8 column 1 .*...*.*
	0b00101001, // character 0xF8 column 2 ..*.*..*
	0b00010001, // character 0xF8 column 3 ...*...*
	0b00101001, // character 0xF8 column 4 ..*.*..*
	0b01000101, // character 0xF8 column 5 .*...*.*
	   
	0b00000111, // character 0xF9 column 1 .....***
	0b01001000, // character 0xF9 column 2 .*..*...
	0b01001000, // character 0xF9 column 3 .*..*...
	0b01001000, // character 0xF9 column 4 .*..*...
	0b00111111, // character 0xF9 column 5 ..******
	   
	0b00010100, // character 0xFA column 1 ...*.*..
	0b00010100, // character 0xFA column 2 ...*.*..
	0b01111100, // character 0xFA column 3 .*****..
	0b00010100, // character 0xFA column 4 ...*.*..
	0b00010010, // character 0xFA column 5 ...*..*.
	   
	0b01000100, // character 0xFB column 1 .*...*..
	0b00111100, // character 0xFB column 2 ..****..
	0b00010100, // character 0xFB column 3 ...*.*..
	0b00010100, // character 0xFB column 4 ...*.*..
	0b01110100, // character 0xFB column 5 .***.*..
	   
	0b01111100, // character 0xFC column 1 .*****..
	0b00010100, // character 0xFC column 2 ...*.*..
	0b00011100, // character 0xFC column 3 ...***..
	0b00010100, // character 0xFC column 4 ...*.*..
	0b01111100, // character 0xFC column 5 .*****..
	   
	0b00001000, // character 0xFD column 1 ....*...
	0b00001000, // character 0xFD column 2 ....*...
	0b00101010, // character 0xFD column 3 ..*.*.*.
	0b00001000, // character 0xFD column 4 ....*...
	0b00001000, // character 0xFD column 5 ....*...
	   
	0b00000000, // character 0xFE column 1 ........
	0b00000000, // character 0xFE column 2 ........
	0b00000000, // character 0xFE column 3 ........
	0b00000000, // character 0xFE column 4 ........
	0b00000000, // character 0xFE column 5 ........
	   
	0b01111111, // character 0xFF column 1 .*******
	0b01111111, // character 0xFF column 2 .*******
	0b01111111, // character 0xFF column 3 .*******
	0b01111111, // character 0xFF column 4 .*******
	0b01111111  // character 0xFF column 5 .*******
};
