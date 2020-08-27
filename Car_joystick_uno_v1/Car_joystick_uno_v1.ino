#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00002";

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  Serial.println("Transmitting");
}

void loop() {

  int X = map(analogRead(A0), 0, 1023, -99, 99);
  int Y = map(analogRead(A1), 0, 1023, 99, -99);
        Serial.print(X);
        Serial.print(" ");
        Serial.println(Y);

  int payload[] = {X, Y};

  radio.write(&payload, sizeof(payload));

  delay(5);
}
