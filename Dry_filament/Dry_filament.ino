#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"

/***********************
Next time bisa upgrade pake I2C untuk LCD-nya
***********************/
#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);

// tombol right,middle,left
#define swr 8
#define swm 9
#define swl 10

// dht setup
#define DHTPIN 11
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// relay heater
#define rel 12
// blower exthaus
#define blow 13

// value ADC button right,middle,left
int valuer,valuem,valuel;
// default temperature dan kelembaban
int settemp = 37;
int sethumy = 50;
// kelembaban dan suhu
int h,t;
// counter click button
byte klik =0;

// Custom character hexadecimal
byte part3_1[8]={0x00,0x0f,0x0f,0x00,
                 0x00,0x00,0x00,0x03};
byte part3_2[8]={0x00,0x1e,0x1e,0x00,
                 0x00,0x00,0x00,0x1e};
byte part3_3[8]={0x03,0x00,0x00,0x00,
                 0x00,0x0f,0x0f,0x00};
byte part3_4[8]={0x1e,0x00,0x00,0x00,
                 0x00,0x1e,0x1e,0x00};
byte partd_1[8]={0x01,0x02,0x04,0x08,
                 0x18,0x0c,0x0a,0x09};
byte partd_2[8]={0x10,0x08,0x04,0x02,
                 0x03,0x06,0x0a,0x12};
byte partd_3[8]={0x09,0x09,0x09,0x09,
                 0x05,0x03,0x01,0x00};
byte partd_4[8]={0x12,0x12,0x12,0x14,
                 0x18,0x10,0x00,0x10};


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.createChar(1, part3_1);
  lcd.createChar(2, part3_2);
  lcd.createChar(3, part3_3);
  lcd.createChar(4, part3_4);

  lcd.createChar(5, partd_1);
  lcd.createChar(6, partd_2);
  lcd.createChar(7, partd_3);
  lcd.createChar(8, partd_4);

  dht.begin();
  
  pinMode(swr, INPUT);
  pinMode(swm, INPUT);
  pinMode(swl, INPUT);

  pinMode(rel, OUTPUT);
  pinMode(blow, OUTPUT);

// Print custom character 
  lcd.setCursor(1,0);
  lcd.print("\x7c");
  lcd.print("\xa4");
  lcd.print("\x2f");
  lcd.print("\x7c");
  lcd.print("     ");
  lcd.write(1);
  lcd.write(2);
  lcd.write(5);
  lcd.write(6);
  lcd.setCursor(1,1);
  lcd.print("|  |");
  lcd.print("ESIN");
  lcd.print(" ");
  lcd.write(3);
  lcd.write(4);
  lcd.write(7);
  lcd.write(8);
  delay(10000);
  lcd.clear();
}

// Loop menu setting temperature
void mtombol() {
  if (valuem == 1) {
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("SETTING");
    lcd.setCursor(3,1);
    lcd.print("TEMPERATUR");
    delay(1500);
    lcd.clear();

    do {
      if(valuem == 1) {
        klik++;
        delay(1000);
      }
      else if(valuer == 1) {
        if(settemp < 71){
          settemp = settemp+1;
          delay(500);
        }
      }
      else if(valuel == 1) {
        if(settemp > 36){
          settemp = settemp-1;
          delay(500);
        }
      }
      lcd.setCursor(0,0);
      lcd.print("Set Temperatur");
      lcd.setCursor(0,1);
      lcd.print(settemp);
      lcd.print("\xdf");
      lcd.print("C   ");
      
      valuer=digitalRead(swr);
      valuem=digitalRead(swm);
      valuel=digitalRead(swl);
    }
    while(klik <= 1);
    klik = 0;
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("TEMPERATUR");
    lcd.setCursor(5,1);
    lcd.print("SAVED");
    delay(1000);
    lcd.clear();

  }
}

// Loop menu setting kelembaban
void htombol() {
  if (valuel == 1 && valuer == 1) {
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("SETTING");
    lcd.setCursor(4,1);
    lcd.print("HUMIDITY");
    delay(1500);
    lcd.clear();

    do {
      if(valuem == 1) {
        klik++;
        delay(1000);
      }
      else if(valuer == 1) {
        if(sethumy < 100){
          sethumy = sethumy+1;
          delay(500);
        }
      }
      else if(valuel == 1) {
        if(sethumy > 5){
          sethumy = sethumy-1;
          delay(500);
        }
      }
      lcd.setCursor(2,0);
      lcd.print("Set Humidity");
      lcd.setCursor(0,1);
      lcd.print(sethumy);
      lcd.print("%");
      lcd.print("    ");
      
      valuer=digitalRead(swr);
      valuem=digitalRead(swm);
      valuel=digitalRead(swl);
    }
    while(klik <= 1);
    klik = 0;
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("HUMIDITY");
    lcd.setCursor(5,1);
    lcd.print("SAVED");
    delay(1000);
    lcd.clear();

  }
}

// Range temperature
void controlt() {
  if(t >= settemp) {
    digitalWrite(rel, HIGH);
  }
  else if(t < settemp) {
    digitalWrite(rel, LOW);
  }
}

// Range kelembaban
void controlh() {
  if(h >= sethumy) {
    digitalWrite(blow, HIGH);
  }
  else if(h < sethumy) {
    digitalWrite(blow, LOW);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  valuer=digitalRead(swr);
  valuem=digitalRead(swm);
  valuel=digitalRead(swl);

  h = dht.readHumidity();
  t = dht.readTemperature();

  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("\xdf");
  lcd.print("C/");
  lcd.print(settemp);
  lcd.print("\xdf");
  lcd.print("C");
  
  lcd.setCursor(0,1);
  lcd.print("Humy: ");
  lcd.print(h);
  lcd.print("%/");
  lcd.print(sethumy);
  lcd.print("%");

  controlt();
  controlh();

  mtombol();
  htombol();
  delay(1000);
}
