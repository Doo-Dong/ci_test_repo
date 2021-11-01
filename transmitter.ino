// Load in the libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <SoftwareSerial.h>

#include <arduino-timer.h>
auto timer = timer_create_default();

bool state[6] = {false, false, false, false, false, false};
bool test = false;
int ackMessg[4];

int blueTx=2;   //Tx (보내는핀 설정)at
int blueRx=3;   //Rx (받는핀 설정)
SoftwareSerial mySerial(blueTx, blueRx);  //시리얼 통신을 위한 객체선언

// Set the CE & CSN pins
#define CE_PIN   9
#define CSN_PIN 10

#define TTP_1 8
#define TTP_2 7
#define TTP_3 6
#define TTP_4 5
 
// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe[5] = {0xF0F0F0F01111, 0xF0F0F0F01112, 0xF0F0F0F01113, 0xF0F0F0F01114, 0xF0F0F0F01115};
 
// Create a Radio
RF24 radio(CE_PIN, CSN_PIN);

const int OFF_MODE = 0;
const int LED_MODE = 1;
const int UV_MODE = 2;
const int DELAY_MODE_p = 3;
const int DELAY_MODE_m = 4;
const int INIT_MODE = 5;

const int DELAY_TIME = 500;
 
void setup() {
  pinMode(TTP_1, INPUT);
  pinMode(TTP_2, INPUT);
  pinMode(TTP_3, INPUT);
  pinMode(TTP_4, INPUT);
  
  // Start the serial
  Serial.begin(9600);
  while(!Serial);
  Serial.println("NRF24L01P Transmitter Starting...");
  mySerial.begin(9600); //블루투스 시리얼
  
  // Start the Radio!
  radio.begin();
  
  // Power setting
  radio.setPALevel(RF24_PA_MIN); // RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  
  // Slower data rate for better range
  radio.setDataRate( RF24_250KBPS ); // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.setAutoAck(false);
  //radio.enableAckPayload();
  //radio.enableDynamicAck();
  
  // Stop listening, so we can send!
  radio.stopListening();
  
  // Number of retries
  radio.setRetries(15, 15);
  
  for (int i=0; i < 5; i++) {
    radio.openWritingPipe(pipe[i]);
  }

  if (radio.isChipConnected()) {
    Serial.println("chip is connected");
  }
}
 
void loop() {
  timer.tick();
  while(mySerial.available())  //mySerial에 전송된 값이 있으면
  {
    String myChar = mySerial.readString();
    Serial.println("input value: "+myChar);

      // OFF MODE
      if (myChar.equals("0") ){
        myChar="";
        state[0] = !state[0];
        timer.in(DELAY_TIME, stopPulse);
      }

      // LED MODE
      if (myChar.equals("1") ){
        myChar="";
        state[1] = !state[1];
        timer.in(DELAY_TIME, stopPulse);
      }

      // UV MODE
      if (myChar.equals("2") ){
        myChar="";
        state[2] = !state[2];
        timer.in(DELAY_TIME, stopPulse);
      }

      // DELAY MODE +
      if (myChar.equals("3") ){
        myChar="";
        state[3] = !state[3];
        timer.in(DELAY_TIME, stopPulse);
      }

      // DELAY MODE -
      if (myChar.equals("4") ){
        myChar="";
        state[4] = !state[4];
        timer.in(DELAY_TIME, stopPulse);
      }

      // INIT MODE
      if (myChar.equals("5") ){
        myChar="";
        state[5] = !state[5];
        timer.in(DELAY_TIME, stopPulse);
      }
    delay(100);
  }

  if (state[0]) {
    radio.write(&OFF_MODE, 2);
    Serial.println(OFF_MODE);
  }

  if (state[1]) {
    radio.write(&LED_MODE, 2);
    Serial.println(LED_MODE);
  }

  if (state[2]) {
      radio.write(&UV_MODE, 2, false);
      Serial.println(UV_MODE);
  }

  if (state[3]) {
      radio.write(&DELAY_MODE_p, 2, false);
      Serial.println(DELAY_MODE_p);
  }

  if (state[4]) {
      radio.write(&DELAY_MODE_m, 2, false);
      Serial.println(DELAY_MODE_m);
  }

  if (state[5]) {
      radio.write(&INIT_MODE, 2, false);
      Serial.println(INIT_MODE);
  }

  //TTP ZONE
  if (digitalRead(TTP_1)) {
    radio.write(&LED_MODE, 2);
    Serial.println(LED_MODE);
  }
  
  if (digitalRead(TTP_2)) {
    radio.write(&UV_MODE, 2, false);
    Serial.println(UV_MODE);
  }
  
  if (digitalRead(TTP_3)) {
      radio.write(&DELAY_MODE_m, 2, false);
      Serial.println(DELAY_MODE_m);
  }
  
  if (digitalRead(TTP_4)) {
    radio.write(&DELAY_MODE_p, 2, false);
    Serial.println(DELAY_MODE_p);
  }
}

bool stopPulse() {
  for (int i=0; i < 10; i++) {
    state[i] = false;
  }
  
  Serial.println("stop");

  return false;
}
