#include <SPI.h>          
#include "nRF24L01.h"     
#include "RF24.h"         

RF24 radio(9, 53);


byte address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" };
int transmit_data[2];

int left1 = A1;
int right1 = A2;
int res[2];
int left11;
int right11;

const int SENSOR = 150;



void setup() {



	Serial.begin(9600);

	radio.begin();
	radio.setAutoAck(1);
	radio.setRetries(0, 15);
	radio.enableAckPayload();
	radio.setPayloadSize(32);

	radio.openWritingPipe(address[0]);
	radio.setChannel(0x60);

	radio.setPALevel(RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
	radio.setDataRate(RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS

	radio.powerUp();
	radio.stopListening();
}

void loop() {


	while (transmit_data[0] == 0 && transmit_data[1] == 0) {
		left11 = analogRead(left1);
		right11 = analogRead(right1);

		if (left11 < SENSOR && right11 > SENSOR) {
			transmit_data[0] = 1;
			unsigned long last_time = micros();

			while (transmit_data[1] == 0) {
				analogRead(right1);
				if (analogRead(right1) < SENSOR) {
					transmit_data[1] = ((micros() - last_time) / 10000);
				}
			}
		}

		if (left11 > SENSOR && right11 < SENSOR) {
			transmit_data[1] = 1;
			unsigned long last_time = micros();

			while (transmit_data[0] == 0) {
				analogRead(left1);
				if (analogRead(left1) < SENSOR) {
					transmit_data[0] = ((micros() - last_time) / 10000);
				}
			}
		}



	}









	radio.write(&transmit_data, sizeof(transmit_data));

	Serial.print("Left: "); Serial.println(transmit_data[0]);
	Serial.print("Right: "); Serial.println(transmit_data[1]);
	left11 = 0;
	right11 = 0;
	transmit_data[0] = 0;
	transmit_data[1] = 0;

	delay(1000);
}