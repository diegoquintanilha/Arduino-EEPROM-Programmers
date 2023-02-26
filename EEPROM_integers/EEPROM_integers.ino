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
		for (int pin = I0; pin <= I7; pin++) digitalWrite(pin, LOW);
	
		// selecionando o dígito correspondente a este EEPROM e colocando os inputs relativos ao dígito como '1'
		switch ((int)(i % 10L)) { // unidade
		//switch ((int)((i % 100L)) / 10L) { // dezena
		//switch ((int)((i % 1000L)) / 100L) { // centena
		//switch ((int)((i % 10000L)) / 1000L) { // milhar
		//switch ((int)(i / 10000L)) { // dezena de milhar

			case 0:
				// nenhum if se for unidade
				//if (i > 9L) { // dezena
				//if (i > 99L) { // centena
				//if (i > 999L) { // milhar
				// nunca vai ter zero na dezena de milhar
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
