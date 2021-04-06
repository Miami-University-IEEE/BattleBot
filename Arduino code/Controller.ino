#include <LiquidCrystal_I2C.h>
#include <EasyTransfer.h>

//EasyTransfer Transmit;
LiquidCrystal_I2C lcd(0x27, 20, 4);

EasyTransfer DataPacket;

//Pin defenitions
const int pinJoyA_y = A8; //Joy A y-axis
const int pinJoyA_x = A7; //Joy A x-axis
const int pinJoyA_b = 46; //Joy A pushbutton

const int pinJoyB_y = A10; //Joy B y-axis
const int pinJoyB_x = A9; //Joy B x-axis
const int pinJoyB_b = 48; //Joy B pushbutton

//Switches below LCD
const int pinSwitchA = 32;
const int pinSwitchB = 34;
const int pinSwitchC = 40;
const int pinSwitchD = 42;
const int pinSwitchE = 52;
const int pinSwitchF = 50;

/* 
 * Bottons in D-pad configuration
 * 
 *        - A -
 *        B C D
 *        - E -
 *        
 */
const int pinButtonA = 10;    
const int pinButtonA_led = 11;
const int pinButtonB = 8;
const int pinButtonB_led = 9;
const int pinButtonC = 6;
const int pinButtonC_led = 7;
const int pinButtonD = 4;
const int pinButtonD_led = 5;
const int pinButtonE = 2;
const int pinButtonE_led = 3;

//Button on bottom left next to potentiometer
const int pinButtonF = 12;
const int pinButtonF_led = 13;

const int pinPotA = A2;

//Loop variables
int currJoyA_y;
int currJoyA_x;
bool currJoyA_b;

int currJoyB_y;
int currJoyB_x;
bool currJoyB_b;

bool currSwitchA;
bool currSwitchB;
bool currSwitchC;
bool currSwitchD;
bool currSwitchE;
bool currSwitchF;

bool currButtonA;
bool prevButtonA;
bool currButtonB;
bool prevButtonB;
bool currButtonC;
bool prevButtonC;
bool currButtonD;
bool prevButtonD;
bool currButtonE;
bool prevButtonE;
bool currButtonF;
bool prevButtonF;

int currPotA;


struct DATA_STRUCT {
  int valueJoyA_y;
  int valueJoyA_x;
  bool valueJoyA_b;

  int valueJoyB_y;
  int valueJoyB_x;
  bool valueJoyB_b;

  bool valueSwitchA;
  bool valueSwitchB;
  bool valueSwitchC;
  bool valueSwitchD;
  bool valueSwitchE;
  bool valueSwitchF;

  bool valueButtonA;
  bool valueButtonB;
  bool valueButtonC;
  bool valueButtonD;
  bool valueButtonE;
  bool valueButtonF;

  int valuePotA;
};

DATA_STRUCT transmitStruct;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.noBlink();
  lcd.clear();

  lcd.home();
  lcd.print("BattleBot Controller");
  lcd.setCursor(1,2);
  lcd.print("Made by Miami IEEE");
  lcd.setCursor(5,3);
  lcd.print("2019/2020");

  pinModes();
  bootBlink(3, 100);

  //Initialize XBee on Serial 2
  Serial1.begin(9600);
  DataPacket.begin(details(transmitStruct), &Serial1);

  Serial.begin(9600);

  delay(3000);
  
  lcd.clear();
}

void loop() {
  handleIO();

  lcd.home();
  lcd.print("Motors ");
  currButtonA ? lcd.print("enabled") : lcd.print("disabled");

  lcd.setCursor(0, 1);
  lcd.print("y: " + pinJoyA_y);

  lcd.setCursor(0, 2);
  lcd.print("x: " + pinJoyA_x);

  pushData();

  printData();

  delay(100);
}

void pinModes() {
  pinMode(pinJoyA_y, INPUT);
  pinMode(pinJoyA_x, INPUT);
  pinMode(pinJoyA_b, INPUT_PULLUP);

  pinMode(pinJoyB_y, INPUT);
  pinMode(pinJoyB_x, INPUT);
  pinMode(pinJoyB_b, INPUT_PULLUP);

  pinMode(pinSwitchA, INPUT_PULLUP);
  pinMode(pinSwitchB, INPUT_PULLUP);
  pinMode(pinSwitchC, INPUT_PULLUP);
  pinMode(pinSwitchD, INPUT_PULLUP);
  pinMode(pinSwitchE, INPUT_PULLUP);
  pinMode(pinSwitchF, INPUT_PULLUP);

  pinMode(pinButtonA, INPUT_PULLUP);    
  pinMode(pinButtonA_led, OUTPUT);
  pinMode(pinButtonB, INPUT_PULLUP);
  pinMode(pinButtonB_led, OUTPUT);
  pinMode(pinButtonC, INPUT_PULLUP);
  pinMode(pinButtonC_led, OUTPUT);
  pinMode(pinButtonD, INPUT_PULLUP);
  pinMode(pinButtonD_led, OUTPUT);
  pinMode(pinButtonE, INPUT_PULLUP);
  pinMode(pinButtonE_led, OUTPUT);
  
  pinMode(pinButtonF, INPUT_PULLUP);
  pinMode(pinButtonF_led, OUTPUT);

  pinMode(pinPotA, INPUT_PULLUP);
}

void bootBlink(int n, int pause) {
  for(int i = 0; i < n; i++) {
    digitalWrite(pinButtonA_led, HIGH);
    digitalWrite(pinButtonB_led, HIGH);
    digitalWrite(pinButtonC_led, HIGH);
    digitalWrite(pinButtonD_led, HIGH);
    digitalWrite(pinButtonE_led, HIGH);
    digitalWrite(pinButtonF_led, HIGH);

    delay(pause);

    digitalWrite(pinButtonA_led, LOW);
    digitalWrite(pinButtonB_led, LOW);
    digitalWrite(pinButtonC_led, LOW);
    digitalWrite(pinButtonD_led, LOW);
    digitalWrite(pinButtonE_led, LOW);
    digitalWrite(pinButtonF_led, LOW);

    delay(pause);
  }
}

void handleIO() {
  currJoyA_y = analogRead(pinJoyA_y);
  currJoyA_x = analogRead(pinJoyA_x);
  currJoyA_b = digitalRead(pinJoyA_b);

  currJoyB_y = analogRead(pinJoyB_y);
  currJoyB_x = analogRead(pinJoyB_x);
  currJoyB_b = digitalRead(pinJoyB_b);

  currSwitchA = !digitalRead(pinSwitchA);
  currSwitchB = !digitalRead(pinSwitchB);
  currSwitchC = !digitalRead(pinSwitchC);
  currSwitchD = !digitalRead(pinSwitchD);
  currSwitchE = !digitalRead(pinSwitchE);
  currSwitchF = !digitalRead(pinSwitchF);

  if(digitalRead(pinButtonA) != prevButtonA) {
    currButtonA = !prevButtonA;
    digitalWrite(pinButtonA_led, currButtonA);
  }
  
  if(digitalRead(pinButtonB) != prevButtonB) {
    currButtonB = !prevButtonB;
    digitalWrite(pinButtonB_led, currButtonB);
  }
  
  if(digitalRead(pinButtonC) != prevButtonC) {
    currButtonC = !prevButtonC;
    digitalWrite(pinButtonC_led, currButtonC);
  }
  
  if(digitalRead(pinButtonD) != prevButtonD) {
    currButtonD = !prevButtonD;
    digitalWrite(pinButtonD_led, currButtonD);
  }
  
  if(digitalRead(pinButtonE) != prevButtonE) {
    currButtonE = !prevButtonE;
    digitalWrite(pinButtonE_led, currButtonE);
  }
  
  if(digitalRead(pinButtonF) != prevButtonF) {
    currButtonF = !prevButtonF;
    digitalWrite(pinButtonF_led, currButtonF);
  }
  
  prevButtonA = currButtonA;
  prevButtonB = currButtonB;
  prevButtonC = currButtonC;
  prevButtonD = currButtonD;
  prevButtonE = currButtonE;
  prevButtonF = currButtonF;

  currPotA = analogRead(pinPotA);
}

void pushData() {
  transmitStruct.valueJoyA_y = currJoyA_y;
  transmitStruct.valueJoyA_x = currJoyA_x;
  transmitStruct.valueJoyA_b = currJoyA_b;

  transmitStruct.valueJoyB_y = currJoyB_y;
  transmitStruct.valueJoyB_x = currJoyB_x;
  transmitStruct.valueJoyB_b = currJoyB_b;

  transmitStruct.valueSwitchA = currSwitchA;
  transmitStruct.valueSwitchB = currSwitchB;
  transmitStruct.valueSwitchC = currSwitchC;
  transmitStruct.valueSwitchD = currSwitchD;
  transmitStruct.valueSwitchE = currSwitchE;
  transmitStruct.valueSwitchF = currSwitchF;

  transmitStruct.valueButtonA = currButtonA;
  transmitStruct.valueButtonB = currButtonB;
  transmitStruct.valueButtonC = currButtonC;
  transmitStruct.valueButtonD = currButtonD;
  transmitStruct.valueButtonE = currButtonE;
  transmitStruct.valueButtonF = currButtonF;

  transmitStruct.valuePotA = currPotA;

  DataPacket.sendData();
}

void printData() {
  Serial.println("Switches");
  Serial.print(currSwitchA);
  Serial.print(currSwitchB);
  Serial.print(currSwitchC);
  Serial.print(currSwitchD);
  Serial.print(currSwitchE);
  Serial.println(currSwitchF);

  Serial.println();

  Serial.println("Buttons");
  Serial.print(currButtonA);
  Serial.print(currButtonB);
  Serial.print(currButtonC);
  Serial.print(currButtonD);
  Serial.print(currButtonE);
  Serial.println(currButtonF);

  Serial.println();

  /*
  Serial.println("Joy");
  Serial.print(currJoyA_y);
  Serial.println(currJoyA_x);
  Serial.print(currJoyB_y);
  Serial.println(currJoyB_x);
  */
  
}
