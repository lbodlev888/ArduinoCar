#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
unsigned data = A0, stcp = A1, shcp = A2;
const unsigned short en_pins[] = {3, 5, 6, 9};

void setup() {
  for(int i = A0; i <= A2; i++) pinMode(i, OUTPUT);
  for(int i = 0; i < 4; i++) pinMode(i, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    int var[9];
    radio.read(&var, sizeof(var));
    if(var[8]) tone(2, 800);
    else noTone(2);
    digitalWrite(10, var[7]);
    if(var[2] && !var[4]) forward(var[1]);
    else if(var[4] && !var[2]) backward(var[1]);
    else if(var[3] && !var[5]) right(var[1]);
    else if(var[5] && !var[3]) left(var[1]);
    else if(var[0] > 528) omni_right(var[0]);
    else if(var[0] < 524) omni_left(var[0]);
    else stop();
    for(int i = 0; i < 9; i++)
    {
      Serial.print(var[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
  else {
    Serial.println("Semnal invalid");
    noTone(2);
    stop();
  }
}

void stop()
{
  for(int i = 0; i < 4; i++) analogWrite(en_pins[i], 0);
  digitalWrite(stcp, LOW);
  shiftOut(data, shcp, LSBFIRST, 0);
  digitalWrite(stcp, HIGH);
}

void forward(unsigned speed)
{
  speed = map(speed, 0, 1023, 0, 255);
  for(int i = 0; i < 4; i++) analogWrite(en_pins[i], speed);
  digitalWrite(stcp, LOW);
  shiftOut(data, shcp, MSBFIRST, 0B01010101);
  digitalWrite(stcp, HIGH);
}

void backward(unsigned speed)
{
  speed = map(speed, 0, 1023, 0, 255);
  for(int i = 0; i < 4; i++) analogWrite(en_pins[i], speed);
  digitalWrite(stcp, LOW);
  shiftOut(data, shcp, LSBFIRST, 0B01010101);
  digitalWrite(stcp, HIGH);
}

void left(unsigned speed)
{
  speed = map(speed, 0, 1023, 0, 255);
  for(int i = 0; i < 4; i++) analogWrite(en_pins[i], speed);
  digitalWrite(stcp, LOW);
  shiftOut(data, shcp, LSBFIRST, 0B10100101);
  digitalWrite(stcp, HIGH);
}

void right(unsigned speed)
{
  speed = map(speed, 0, 1023, 0, 255);
  for(int i = 0; i < 4; i++) analogWrite(en_pins[i], speed);
  digitalWrite(stcp, LOW);
  shiftOut(data, shcp, LSBFIRST, 0B01011010);
  digitalWrite(stcp, HIGH);
}

void omni_left(unsigned speed)
{
  speed = map(speed, 524, 0, 0, 255);
  for(int i = 0; i < 4; i++) analogWrite(en_pins[i], speed);
  digitalWrite(stcp, LOW);
  shiftOut(data, shcp, MSBFIRST, 0B01100110);
  digitalWrite(stcp, HIGH);
}

void omni_right(unsigned speed)
{
  speed = map(speed, 526, 1023, 0, 255);
  for(int i = 0; i < 4; i++) analogWrite(en_pins[i], speed);
  digitalWrite(stcp, LOW);
  shiftOut(data, shcp, MSBFIRST, 0B10011001);
  digitalWrite(stcp, HIGH);
}