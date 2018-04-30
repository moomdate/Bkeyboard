int count = 0;
int same = 0;
int tc1, tc2;
#include <Wire.h>
//const int J1X = A0;
//int J1X_Val = 0;
//const int J1Y = A1;
//int J1Y_Val = 0;
//
//const int J2X = A2;
//int J2X_Val = 0;
//const int J2Y = A3;
//int J2Y_Val = 0;
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

byte readKey2, readKey;
byte readCur1, readCur2, readCur3, readCur4;
byte leftShift, rightShitf;
byte min_key = 0, min_key2 = 0;
byte cur_key1 = 0, cur_key2 = 0, cur_key3 = 0, cur_key3_5 = 0 , cur_key4 = 0, release_cersor = 0;
int sum_key = 0, sum_cursorkey1 = 0;
int c1, c2, c3, c4, c5, c6;
int temp = 0;

boolean hiV = 1;
unsigned long previousMillis = 0;
const long interval = 60000;
const long interval2 = 3000;
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
  tc1 = 0x00;
  tc2 = 0x00;
  //Serial.println("ready");
}
void loop()
{
  //top

  //****************hight volt*****************
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
   // Serial.println("HIGHT VOLT ON 1 SEC");
    hiV = 1;
    if (currentMillis - previousMillis >= (interval + interval2)) {
     // Serial.println("HIGHT VOLT OFF");
      hiV = 0;
      previousMillis = currentMillis;
    }

  }
  digitalWrite(HV_en, hiV);
  /////////////////////////////////////////////
  byte rightJoy = 0, leftJoy = 0;
  /* Serial.print(digitalRead(A_1)); //ขึ้น
    Serial.print(" ");
    Serial.print(digitalRead(B_1)); //ซ้าย
    Serial.print(" ");
    Serial.print(digitalRead(C_1)); //ลง
    Serial.print(" ");
    Serial.print(digitalRead(D_1)); //ขวา
    Serial.println();*/
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
     previousMillis = currentMillis;
     hiV = 1;
    while (digitalRead(A_1) == 0 || digitalRead(B_1) == 0 || digitalRead(C_1) == 0 || digitalRead(D_1) == 0 || digitalRead(cen1) == 0) { //joystick right debounce
      delay(5);
    }
    while (digitalRead(A_2) == 0 || digitalRead(B_2) == 0 || digitalRead(C_2) == 0 || digitalRead(D_2) == 0 || digitalRead(cen2) == 0) { //joystick left debounce
      delay(5);
    }
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
  /*if ((readCur1 == 0 && readCur2 == 0  && readCur3 == 0  && readCur4 == 0) && sum_cursorkey1 != 0){
    Serial.println(sum_cursorkey1, HEX);
    cur_key1 = 0;
    cur_key2 = 0;
    }*/
  /*  if (readCur2 == 0 && cur_key2 != 0 && cur_key2 < 8) { //if chip 2 key 1-3 is true
      Serial.println(cur_key2 * 32, HEX);
      cur_key2 = 0;
    } else if (readCur2 == 0 && cur_key2 != 0 ) { //clear cur_key2
      cur_key2 = 0;
    }
  */

  /*if (readKey2 < min_key2 && readKey2 == 0) { //on release key   [left button,right button,space bar left and right]
    release1 = true;
    //  Serial.println(min_key2,HEX);
    }
    if (readKey < min_key && readKey == 0) { //on release key 1-8
    release2 = true;
    }*/
  //---sending---//
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
