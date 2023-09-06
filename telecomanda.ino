#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

const byte address[6] = "00001";
RF24 radio(9, 10);

void setup() {
  for(int i = 2; i <= 8; i++) pinMode(i, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  int var[9];
  var[0] = analogRead(A0);
  var[1] = analogRead(A1);
  for(int i = 2; i <= 8; i++) var[i] = !digitalRead(i);
  var[7] = !var[7];
  radio.write(&var, sizeof(var));
}