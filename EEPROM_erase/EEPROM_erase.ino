/*
 * Para apagar o EEPROM:
 * Desconectar/conectar os EEPROM da placa quando o led estiver piscando, com:
 * 		todos os pinos de endereço LOW (menos A9),
 * 		todos os pinos de dados (outputs) HIGH, e
 * 		o pino CE ligado na saída ERASE do arduino
 * Resetar o arduino para iniciar o processo
 * Enquanto o led estiver piscando, conectar os pinos OE e A9 em 14V (>13.75 e <14.25)
 * Led vai piscar rápido, indicando que está prestes a emitir o erase pulse
 * Led apaga enquanto o EEPROM está sendo apagado
 * Ao terminar de apagar, led acende continuamente por 3 segundos, e então volta a piscar
*/

#define ERASE 12

void setup() {
	
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(ERASE, OUTPUT);

	digitalWrite(ERASE, HIGH);
	
	// led pisca 5 vezes, é o momento de ligar 14V onde precisa
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

	// pulso de erase
	digitalWrite(ERASE, LOW);
	delay(100); // 100 mS pulse
	digitalWrite(ERASE, HIGH);

	delay(500);

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
