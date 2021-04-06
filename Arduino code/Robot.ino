#include <EasyTransfer.h>
#include <SoftwareSerial.h>
#include <Sabertooth.h>

EasyTransfer DataPacket;
SoftwareSerial SWSerial(NOT_A_PIN, 2);
Sabertooth Saber(128, SWSerial);

//Loop variables
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

DATA_STRUCT pushedData;

void setup() {
   //XBee
  Serial3.begin(9600);

  //Sabertooth
  SWSerial.begin(9600);

  //Computer
  Serial.begin(9600);

  DataPacket.begin(details(pushedData), &Serial3);

  Saber.drive(0);
  Saber.turn(0);
}

void loop() {
  if(DataPacket.receiveData()) {
    getNewData();
    
  }

  if(valueSwitchA == 1) {

    Serial.print(map(valueJoyA_y, 0, 1023, -127, 127));
    Serial.print(", ");
    Serial.println(map(valueJoyA_x, 0, 1023, -127, 127));
    
    Saber.drive(map(valueJoyA_y, 0, 1023, -127, 127));
    Saber.turn(map(valueJoyA_x, 0, 1023, -127, 127));
  }
  
  delay(100);
}

void getNewData() {
  valueJoyA_y = pushedData.valueJoyA_y;
  valueJoyA_x = pushedData.valueJoyA_x;
  valueJoyA_b = pushedData.valueJoyA_b;

  valueJoyB_y = pushedData.valueJoyB_y;
  valueJoyB_x = pushedData.valueJoyB_x;
  valueJoyB_b = pushedData.valueJoyB_b;

  valueSwitchA = pushedData.valueSwitchA;
  valueSwitchB = pushedData.valueSwitchB;
  valueSwitchC = pushedData.valueSwitchC;
  valueSwitchD = pushedData.valueSwitchD;
  valueSwitchE = pushedData.valueSwitchE;
  valueSwitchF = pushedData.valueSwitchF;

  valueButtonA = pushedData.valueButtonA;
  valueButtonB = pushedData.valueButtonB;
  valueButtonC = pushedData.valueButtonD;
  valueButtonD = pushedData.valueButtonE;
  valueButtonE = pushedData.valueButtonE;
  valueButtonF = pushedData.valueButtonF;

  valuePotA = pushedData.valuePotA;
      
}
