//*********************************************************
// pins
//*********************************************************
#define R1 A1 // IN1 (right)
#define R2 A2 // IN2 (right)
#define REN 5 // ENB (right)
#define L2 A4 // IN3 (left)
#define L1 A3 // IN4 (left)
#define LEN 6 // ENA (left)
#define servo 9   // for servo
#define MID 70    // for servo
#define R_MAX 105 // for servo
#define L_MAX 35  // for servo
#define CE A0     // SPI
#define CSN 10    // SPI
//SCK   13        // SPI
//MISO  12        // SPI
//MOSI  11        // SPI

#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>


RF24 radio(8, 7); // CE, CSN
Servo steering;


const byte address[6] = "00002";
int x = 0;
int y = 0;

//*********************************************************
// prototypes
//*********************************************************
void move_and_turn_test();
void move(int y);
void turn(int x);

//*********************************************************
// setup
//*********************************************************
void setup() {
  // put your setup code here, to run once:

  pinMode(REN, OUTPUT); // pwm
  pinMode(LEN, OUTPUT); // pwm
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);

  steering.attach(servo);
  delay(10);
  steering.write(MID);

  Serial.begin(9600);
  //  while (!Serial) { }
  //  Serial.println("BTLE advertisement receiver");

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  Serial.println("Listening");
  //  // this is super important (for the WiFi version)
  //  // this cant be stressed enough i had to add a second comment
  //  // default is 1000 - COMPLETE GARBAGE
  //  // 1ms too small, 2ms better by still too small, 3ms,,, thats hits the spot!
  //  Serial.setTimeout(3);


  //  move_and_turn_test();
}

//*********************************************************
// loop
//*********************************************************
void loop() {

  // serial communication
  //*********************

  //  if (Serial.available()) {
  //    String s = Serial.readString();
  //    s.replace("\r\n", "");
  //    if (s[0] == 's') {
  //      move(s.substring(1).toInt());
  //    }
  //    else if (s[0] == 't') {
  //      turn(s.substring(1).toInt());
  //    }
  //  }



  //  if (Serial.available()) {
  //        String sinp = Serial.readStringUntil('\n');
  //        int x = sinp.substring(0, sinp.indexOf(' ')).toInt();
  //        int y = sinp.substring(sinp.indexOf(' ')).toInt();
  //
  //        Serial.print(" x: ");
  //        Serial.print(x);
  //        Serial.print("        y: ");
  //        Serial.print(y);
  //
  //
  //
  //        turn(x);
  //        move(y);
  //    Serial.read();
  //  }

  // software serial communication
  //******************************

  //  if (esp.available()) {
  //    String sinp = esp.readStringUntil('\n');
  //
  //    int x = sinp.substring(0, sinp.indexOf(' ')).toInt();
  //    int y = sinp.substring(sinp.indexOf(' ')).toInt();
  //    Serial.println(sinp);
  //    Serial.print(" x: ");
  //    Serial.print(x);
  //    Serial.print("        y: ");
  //    Serial.print(y);



  // nrf24l01 communication (SPI bus)
  //*********************************

  //  Serial.print("Listening... ");
  //
  //  if (btle.listen()) {
  //    Serial.print("Got payload: ");
  //    for (uint8_t i = 0; i < (btle.buffer.pl_size) - 6; i++) {
  //      Serial.print(btle.buffer.payload[i], HEX);
  //      Serial.print(" ");
  //    }
  //  }
  //
  //  Serial.println("done.");
  //  btle.hopChannel();


  if (radio.available()) {
    //Serial.print("got message: ");
    int msg[2];
    //int msg = 0;
    radio.read(&msg, sizeof(msg));
    x = msg[0];
    y = msg[1];
    //    Serial.print(msg[0]);
    //    Serial.print(" ");
    //    Serial.println(msg[1]);
    //Serial.println(msg);
    turn(x);
    move(y);
  }
  //  Serial.print(x);
  //  Serial.print(" ");
  //  Serial.println(y);


  // motion functions
  //*****************



  // stability thing for serial/software serial
  //*******************************************

  //    esp.read();
  //  }
  delay(5);
  //*********************************************************
  // void loop end
  //*********************************************************

}

//*********************************************************
// functions
//*********************************************************

void move_and_turn_test() {
  // this function should be run in setup()
  move(30);
  while (true) {
    for (int i = L_MAX; i < R_MAX; i++) {
      steering.write(i);
      delay(20);
    }
    for (int i = R_MAX; i > L_MAX; i--) {
      steering.write(i);
      delay(20);
    }
  }
}

void move(int y) {
  if ((y <= 100) && (y >= -100) && (y != 0)) {
    if (y >= 0) {
      digitalWrite(L1, HIGH);
      digitalWrite(L2, LOW);
      digitalWrite(R1, HIGH);
      digitalWrite(R2, LOW);
    }
    else if (y < 0) {
      digitalWrite(L1, LOW);
      digitalWrite(L2, HIGH);
      digitalWrite(R1, LOW);
      digitalWrite(R2, HIGH);
    }
    else {
      ;
    }

    y = map(abs(y), 0, 100, 0, 255);
    analogWrite(LEN, y);
    analogWrite(REN, y);
  }
}

void turn(int x) {
  if ((x >= -100) && (x <= 100) && (x != 0)) {
    if (x <= 0) {
      x = map(x, -100, 0, L_MAX, MID);
      Serial.println(x);
    }
    else {
      x = map(x, 1, 100, MID, R_MAX);
      Serial.println(x);
    }
    steering.write(x);
  }
}
