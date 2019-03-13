#include <Wire.h>
#include <EEPROM.h>
int A_1 = 4;
int B_1 = 5;
int C_1 = 6;
int D_1 = 7;
int A_2 = 8;
int B_2 = 9;
int C_2 = 10;
int D_2 = 11;
int cen1 = 2;
int cen2 = 3;
int HV_en = 12;


//--------------- charging ----------
int checkBatCharging = 0;
int ccFill = 0;
int arrayFofill[3];
int timeCheck = 0;
float min_ = 4.21, max_ = 0.0;
////////////////////////////////////

//--------keyboard and cursor -------
byte readKey2, readKey;
byte readCur1, readCur2, readCur3, readCur4;
byte leftShift, rightShitf;
byte min_key = 0, min_key2 = 0;
byte cur_key1 = 0, cur_key2 = 0, cur_key3 = 0, cur_key3_5 = 0 , cur_key4 = 0, release_cersor = 0;
int sum_key = 0, sum_cursorkey1 = 0;
int stateReleaseHold = 0;
/////////////////////////////////////
boolean beep__ = false;
//-----------------beep -------------------
int time_ = 0;
byte toggleBeep = 0;
byte toggleBeepU = 0;
///////////////////////////////////////////
//--------------- hight Volt----------------
boolean hiV = 1;
unsigned long previousMillis = 0;
const long interval = 60000; //เวลาทีเช็ค sleep high vold ทุกๆ 1 นาที
const long interval2 = 3000; //charg high volt
////////////////////////////////////////////
void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  pinMode(A_1, INPUT_PULLUP);
  pinMode(B_1, INPUT_PULLUP);
  pinMode(C_1, INPUT_PULLUP);
  pinMode(D_1, INPUT_PULLUP);

  pinMode(A_2, INPUT_PULLUP);
  pinMode(B_2, INPUT_PULLUP);
  pinMode(C_2, INPUT_PULLUP);
  pinMode(D_2, INPUT_PULLUP);

  pinMode(cen1, INPUT_PULLUP); //ขวา
  pinMode(cen2, INPUT_PULLUP); //ซ้าย
  pinMode(HV_en, OUTPUT);
  digitalWrite(HV_en, hiV);
  pinMode(13, OUTPUT); // sound
  pinMode(A0, INPUT); // volt value
  pinMode(A2, INPUT); // chart state
 
  beep__ = ReadStatusBeep();
  startSound();
}
void errorSound2() {
  fr(100, 90, 100);
  delay(100);
  fr(100, 80, 80);
  delay(100);
}
void errorSound() {
  fr(100, 90, 100);
  delay(100);
  fr(100, 90, 100);
  delay(100);
}
void beep() {
  fr(400, 90, 100);
}
void errorBreak() {
  fr(100, 90, 100);
  delay(60);
  fr(100, 90, 100);
  delay(60);
  fr(600, 90, 100);
  delay(50);
}
void beepbeep() {
  fr(100, 600, 300);
  delay(100);
  fr(150, 600, 320);
  delay(100);
}
void startSound() {
  fr(300, 100, 200);
  delay(100);
  fr(500, 100, 200);
  delay(100);
}
//---------- beep gen----------
void fr(int gg, int cc, int fg) {
  int ab = gg;
  if (beep__) {
    while (ab) {
      digitalWrite(13, 1);
      delayMicroseconds(cc);
      analogWrite(13, 0);
      delayMicroseconds(fg);
      ab--;
    }
  }
}
//----------analog volt to percent------------
int voltV() {
  float vin;
  float av = 0.0;
  vin = analogRead(A0);
  vin = vin * 5.0;
  vin = (vin / 1023) * 3.0;
  time_++;
  if (time_ > 300) {
    time_ = 0;
    min_ = 4.22;
    max_ = 0.0;
  }
  if (min_ > vin)
    min_ = vin;
  if (max_ < vin)
    max_ = vin;
    
  av = (min_ + max_) / 2.0;
  //av = map(2.7,4.2,0,4.2);
  av = av - 2.75;
  float percent = av * 100.0 / (4.21 - 2.75);

  return ceil(percent);
}
//------- beep when charging and unplug------------
void chargStatBeepSound() {
  float gg = analogRead(A2);  //check charging
  //ccFill = 0;

  if (gg) {
    arrayFofill[ccFill] = gg;
    checkBatCharging++;
    if (checkBatCharging > 50) {
      checkBatCharging = 0;
      ccFill++;
    }
    if (ccFill >= 3) {
      //Serial.println(fillter(arrayFofill[0], arrayFofill[1], arrayFofill[2]));
      if (fillter(arrayFofill[0], arrayFofill[1], arrayFofill[2]) > 2) {
        if (toggleBeep == 0)
          toggleBeep = 1;
        //beep();
      }
      ccFill = 0;
      checkBatCharging = 0;
    }
  }
  if (!gg) {
    arrayFofill[ccFill] = gg;
    checkBatCharging++;
    if (checkBatCharging > 10) {
      checkBatCharging = 0;
      ccFill++;
    }
    if (ccFill >= 3) {
      //erial.println(fillter(arrayFofill[0], arrayFofill[1], arrayFofill[2]));
      if (fillter(arrayFofill[0], arrayFofill[1], arrayFofill[2]) == 0) {
        if (toggleBeep == 2) {
          toggleBeep = 0;
          beep();
          delay(100);
          beep();
          //Serial.println("B");
        }
      }
      ccFill = 0;
      checkBatCharging = 0;
    }
  }
  if (toggleBeep == 1) { //chart.
    //Serial.println("A");
    beep();
    toggleBeep = 2;
  }
}
//-----------------------beep manage-----------------------
//EEPROM address 0 use for beep status
//
//////////////////////////////////////////////////////////
int muteBeep(byte status__) { // status 1, 0
  // value = EEPROM.read(0);
  int ggBB = 0;
  if (status__ == 0) {
    beep__ = 1;
    beep();
    delay(50);
    beep();
  } else if (status__ == 1) {
    beep__ = 1;
    beep();
  }
  if (status__ == 1 || status__ == 0) {
    EEPROM.write(0, status__);
    beep__ = status__;
    ggBB = status__;
  }
  return ggBB;
}

int ReadStatusBeep() {
  int aaa = EEPROM.read(0);
  return aaa;
}
int fillter(int a, int b, int c) {
  return (a + b + c ) / 3;

}
//----------------command for stm32------------------------
void SerialComm() {
  if (Serial.available()) {
    byte data = Serial.read();
    if (data == 155) {
      errorSound();
    } else if (data == 154) {
      beep();
    } else if (data == 153) {
      beepbeep();
    } else if (data == 152) {
      errorSound2();
    } else if (data == 151) {
      errorBreak();
    } else if (data == 150) { //battery
      Serial.write(voltV());
      //Serial.write(0xFA);
    } else if (data == 149) {
      Serial.write(muteBeep(1));
    } else if (data == 148) {
      Serial.write(muteBeep(0));
    }else if (data == 147) {
      Serial.write(ReadStatusBeep());
    }
    data = 0;
  }
}
void loop()
{
  //top
  //Serial.println(voltV());
  //startSound();
  chargStatBeepSound();
  SerialComm();
  //****************hight volt*****************
  unsigned long currentMillis = millis();
  //---------------- check time for sleep hight volt---------------
  if (currentMillis - previousMillis >= interval) {
    hiV = 1;
    //Serial.println("HHH");
    if (currentMillis - previousMillis >= (interval + interval2)) {
      hiV = 0;
      //Serial.println("OO");
      //errorBreak();
      previousMillis = currentMillis;
    }
  }
  digitalWrite(HV_en, hiV);

  byte rightJoy = 0, leftJoy = 0;
  /* Serial.print(digitalRead(A_1)); //ขึ้น
    Serial.print(" ");
    Serial.print(digitalRead(B_1)); //ซ้าย
    Serial.print(" ");
    Serial.print(digitalRead(C_1)); //ลง
    Serial.print(" ");
    Serial.print(digitalRead(D_1)); //ขวา
    Serial.println();*/
  //----- check bit----
  byte j2_1 = digitalRead(A_1); //right joy
  byte j2_2 = digitalRead(B_1);
  byte j2_3 = digitalRead(C_1);
  byte j2_4 = digitalRead(D_1);
  byte push_r = digitalRead(cen1);
  j2_1 == LOW ? bitSet(rightJoy, 4) :  bitClear(rightJoy, 4);
  j2_2 == LOW ? bitSet(rightJoy, 2) :  bitClear(rightJoy, 2);
  j2_3 == LOW ? bitSet(rightJoy, 5) :  bitClear(rightJoy, 5);
  j2_4 == LOW ? bitSet(rightJoy, 3) :  bitClear(rightJoy, 3);
  push_r == LOW ? bitSet(rightJoy, 1) :  bitClear(rightJoy, 1);

  byte j1_1 = digitalRead(A_2); //left joy
  byte j1_2 = digitalRead(B_2);
  byte j1_3 = digitalRead(C_2);
  byte j1_4 = digitalRead(D_2);
  byte push_l = digitalRead(cen2);
  j1_1 == LOW ? bitSet(leftJoy, 7) :  bitClear(leftJoy, 7);
  j1_2 == LOW ? bitSet(leftJoy, 5) :  bitClear(leftJoy, 5);
  j1_3 == LOW ? bitSet(leftJoy, 0) :  bitClear(leftJoy, 0);
  j1_4 == LOW ? bitSet(leftJoy, 6) :  bitClear(leftJoy, 6);
  push_l == LOW ? bitSet(leftJoy, 4) :  bitClear(leftJoy, 4);
  if (j2_1 == 0 || j2_2 == 0 || j2_3 == 0 || j2_4 == 0 || push_r == 0    || j1_1 == 0 || j1_2 == 0 || j1_3 == 0 || j1_4 == 0 || push_l == 0) {
    timeCheck = 0;
    stateReleaseHold = 0;
    while (digitalRead(A_1) == 0 || digitalRead(B_1) == 0 || digitalRead(C_1) == 0 || digitalRead(D_1) == 0 || digitalRead(cen1) == 0) { //joystick right debounce
      timeCheck++;
      if (timeCheck > 140) {  // debounce switch
        timeCheck = 0;
        stateReleaseHold = 1;
        break;
      }
      stateReleaseHold = 2;
      delay(5);
    }
    while (digitalRead(A_2) == 0 || digitalRead(B_2) == 0 || digitalRead(C_2) == 0 || digitalRead(D_2) == 0 || digitalRead(cen2) == 0) { //joystick left debounce
      timeCheck++;
      if (timeCheck > 140) { // debounce switch
        timeCheck = 0;
        stateReleaseHold = 1;
        break;
      }
      stateReleaseHold = 2;
      delay(5);
    }
    if (stateReleaseHold) {
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xa6);
      Serial.write(0x03);
      Serial.write(00);
      if (leftJoy == 01) {
        Serial.write(rightJoy);
        Serial.write(leftJoy);
      } else {
        Serial.write(leftJoy);
        Serial.write(rightJoy);
      }
    } else if (stateReleaseHold != 2) {                                                                                                                                                                                                      
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xa6);
      Serial.write(0x03);
      Serial.write(00);
      if (leftJoy == 01) {
        Serial.write(rightJoy);
        Serial.write(leftJoy);
      } else {
        Serial.write(leftJoy);
        Serial.write(rightJoy);
      }
    }
    previousMillis = currentMillis;
    hiV = 1;

  }
  Wire.requestFrom(32, 1); //top button //key 1 - 8
  if (Wire.available())
  {
    readKey = bitMaping(~Wire.read());
    if (readKey >= min_key) {
      min_key = readKey;
    }//com
    //   Serial.println(data, HEX);
  }
  Wire.requestFrom(33, 1); //bottom buttons ex. [left button,right button,space bar left and right]
  if (Wire.available())
  {
    readKey2 = bitMap2(~Wire.read());
    if (readKey2 >= min_key2) {
      min_key2 = readKey2;
    }
  }


  //------------------------------------cursor key--------------------------
  Wire.requestFrom(34, 1);
  if (Wire.available())
  {
    readCur1 = (~Wire.read());
    if (readCur1 >= cur_key1) {
      cur_key1 = readCur1;
    }
  }


  Wire.requestFrom(35, 1);
  if (Wire.available())
  {
    readCur2 = (~Wire.read());
    if (readCur2 >= cur_key2) {
      cur_key2 = readCur2;
    }
  }

  Wire.requestFrom(36, 1);
  if (Wire.available())
  {
    readCur3 = (~Wire.read());
    if (readCur3 >= cur_key3_5) {
      cur_key3_5 = readCur3;
    }
  }

  Wire.requestFrom(37, 1);
  if (Wire.available())
  {
    readCur4 = (~Wire.read());
    if (readCur4 >= cur_key4) {
      cur_key4 = readCur4;
    }
  }
  if (cur_key2 >= 8) {
    cur_key3 = cur_key2 / 8; //chip key 2
    cur_key2 = 0;
  }

  //release cursor key
  sum_cursorkey1 = cur_key1 + cur_key2 * 32;
  if (readCur1 == 0 && readCur2 == 0 && sum_cursorkey1 != 0) { // set1 chip 1 and 2
    /* Serial.write(sum_cursorkey1);
      cur_key1 = 0;
       cur_key2 = 0;*/
    release_cersor = 1;
  }
  if (readCur2 == 0 && cur_key3 != 0) { //set 2 chip 2
    /* Serial.println(cur_key3, HEX);
      cur_key3 = 0;*/
    release_cersor = 1;

  }
  if (readCur4 == 0 && cur_key4 != 0) {
    if (cur_key4 == 1) {
      cur_key3_5 = 32;
      cur_key4 = 0;
    }
  }
  if (readCur3 == 0 && cur_key3_5 != 0) { //set 2 chip 3
    /*Serial.println(cur_key3_5 * 4, HEX);
      cur_key3_5 = 0;*/
    release_cersor = 1;
  }
  if (readCur4 == 0 && cur_key4 != 0 && cur_key4 != 1) { //set 3 chip 4
    /* Serial.println(cur_key4 / 2, HEX);
      cur_key4 = 0;*/
    release_cersor = 1;
  }
  if (release_cersor) {
    release_cersor = 0;
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xa4);
    Serial.write(0x03);
    Serial.write(sum_cursorkey1);
    Serial.write(cur_key3 + cur_key3_5 * 4);
    Serial.write(cur_key4 / 2);
    cur_key1 = 0;
    cur_key2 = 0;
    cur_key3 = 0;
    cur_key3_5 = 0;
    cur_key4 = 0;
    //--------- hight volt ---------
    previousMillis = currentMillis;
    hiV = 1;
    //------------------------------
  }
  //---sending or release key ---//
  sum_key = min_key2 + min_key;
  if (readKey2 == 0 && readKey == 0 && sum_key != 0) { // release two shift
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xa6);
    Serial.write(0x03);
    Serial.write(min_key);
    Serial.write(min_key2);
    Serial.write(00);
    min_key2 = 0;
    min_key = 0;
    sum_key = 0;
    //--------- hight volt ---------
    previousMillis = currentMillis;
    hiV = 1;
    //------------------------------
    // Serial.println("Max");
  }

}
//------------ bit mapping to braille unicode---------------
byte bitMaping(byte data) {
  byte DataOutput = 0;
  byte bit_0 = bitRead(data, 0);
  byte bit_1 = bitRead(data, 1);
  byte bit_2 = bitRead(data, 2);
  byte bit_3 = bitRead(data, 3);
  byte bit_4 = bitRead(data, 4);
  byte bit_5 = bitRead(data, 5);
  byte bit_6 = bitRead(data, 6);
  byte bit_7 = bitRead(data, 7);

  bit_3 == HIGH ? bitSet(DataOutput, 0) :  bitClear(DataOutput, 0);
  bit_2 == HIGH ? bitSet(DataOutput, 1) :  bitClear(DataOutput, 1);
  bit_1 == HIGH ? bitSet(DataOutput, 2) :  bitClear(DataOutput, 2);

  bit_4 == HIGH ? bitSet(DataOutput, 3) :  bitClear(DataOutput, 3);
  bit_5 == HIGH ? bitSet(DataOutput, 4) :  bitClear(DataOutput, 4);
  bit_6 == HIGH ? bitSet(DataOutput, 5) :  bitClear(DataOutput, 5);

  bit_0 == HIGH ? bitSet(DataOutput, 6) :  bitClear(DataOutput, 6);
  bit_7 == HIGH ? bitSet(DataOutput, 7) :  bitClear(DataOutput, 7);
  return DataOutput;
}
byte bitMap2(byte dataInput) {
  byte dataOutput = 0;
  byte bit_0 = bitRead(dataInput, 0);
  byte bit_1 = bitRead(dataInput, 1);
  byte bit_2 = bitRead(dataInput, 2);
  byte bit_3 = bitRead(dataInput, 3);

  bit_2 == HIGH ? bitSet(dataOutput, 0) :  bitClear(dataOutput, 0);
  bit_3 == HIGH ? bitSet(dataOutput, 1) :  bitClear(dataOutput, 1);

  bit_0 == HIGH ? bitSet(dataOutput, 3) :  bitClear(dataOutput, 3);
  bit_1 == HIGH ? bitSet(dataOutput, 2) :  bitClear(dataOutput, 2);
  //  bitSet(dataOutput, 0);
  return dataOutput;
}
