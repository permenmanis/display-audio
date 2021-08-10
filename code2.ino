#include <LiquidCrystal.h>
#define pot1              A2
#define pot2              A3
#define sensorSuhuCH1     A0
#define sensorSuhuCH2     A1
#define rs                7
#define en                6
#define d4                5
#define d5                4
#define d6                3
#define d7                2

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int dataSensorSuhuCH1, dataSensorSuhuCH2;
float R1          = 10000;
float logR2, R2;
float T, Tc;
float T1, Tc1;
float c1              = 1.009249522e-03, 
      c2              = 2.378405444e-04, 
      c3              = 2.019202697e-07;

int outPot1, outPot2;
int mapPot1, mapPot2;
int writeLCD          = 0xFF;
int lcdDisplay1, lcdDisplay2;


int dataTerakhir;
unsigned long previousMillis = 0;
const long interval         = 1000;
int flagWaktu, detik;
int flagData;
int filterDataSuhu1, filterDataSuhu2;
int filterPot1, filterPot2;
void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  //lcd.backlight();
}

void bacaSensor(){
  dataSensorSuhuCH1 = analogRead(sensorSuhuCH1);
  dataSensorSuhuCH2 = analogRead(sensorSuhuCH2);
  //////////////////////////////////////////////////////////////////////
  R2 = R1 * (1023.0 / (float)dataSensorSuhuCH1 - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  //////////////////////////////////////////////////////////////////////
  R2 = R1 * (1023.0 / (float)dataSensorSuhuCH2 - 1.0);
  logR2 = log(R2);
  T1 = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc1 = T1 - 273.15;
  ////////////////////////////////////////// 
  for(int x = 0; x < 5; x++){
    filterPot1 += analogRead(pot1);
    filterPot2 += analogRead(pot2);
  }
  outPot1 = filterPot1 / 5;
  outPot2 = filterPot2 / 5;
  if(outPot1 > 1000){
    outPot1 = 1000;
  }
  if(outPot2 > 1000){
    outPot2 = 1000;
  }
  mapPot1 = map(outPot1, 0, 1000, 0, 99);
  mapPot2 = map(outPot2, 0, 1000, 0, 99);
  lcdDisplay1 = map(mapPot1, 0, 99, 0, 11);
  lcdDisplay2 = map(mapPot2, 0, 99, 0, 11);
  
    filterPot1 = 0;
    filterPot2 = 0;
}

void display1(){
  if(mapPot1 < 10){
    lcd.setCursor(1,0);
    lcd.print(" ");
  }
  lcd.setCursor(0,0);
  lcd.print(mapPot1);
  if(mapPot2 < 10){
    lcd.setCursor(15,0);
  }
  else{
    lcd.setCursor(14,0);
  }
  
  lcd.print(mapPot2);
  lcd.setCursor(0,1);
  //lcd.print(Tc, 1);
  lcd.print("47.3");
  lcd.write(223);
  lcd.print("C");
  lcd.setCursor(10,1);
  //lcd.print(Tc, 1);
  lcd.print("47.3");
  lcd.write(223);
  lcd.print("C");
  //////////////////////////////////////////////
}

void display2(){
  /////////////////////////////////////////
  lcd.setCursor(0,0);
  lcd.print("L");
  lcd.setCursor(1 + lcdDisplay1,0);
  if(mapPot1 == 0){
    lcd.print(" ");
  }
  else{
    lcd.write(writeLCD);
    lcd.print(" ");
  }
  lcd.setCursor(14,0);
  if(mapPot1 < 10){
    lcd.print(" ");
  }
  lcd.print(mapPot1);
  //////////////////////////////////////////
  lcd.setCursor(0,1);
  lcd.print("R");
  lcd.setCursor(1 + lcdDisplay2,1);
  if(mapPot2 == 0){
    lcd.print(" ");
  }
  else{
    lcd.write(writeLCD);
    lcd.print(" ");
  }
  lcd.setCursor(14,1);
  if(mapPot2 < 10){
    lcd.print(" ");
  }
  lcd.print(mapPot2);
}
void waktu(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    detik++;
    if(detik >= 3){
      if(flagWaktu == 1){
        lcd.setCursor(0,0);
        lcd.print("               ");
        lcd.setCursor(0,1);
        lcd.print("               ");
      }
      flagWaktu = 0;
    }
    previousMillis = currentMillis;
  }
}
void loop() {
  
  waktu();
  for(int c = 0; c < 10; c++){
    bacaSensor();
    flagData += (mapPot1 + mapPot2);
  }
  flagData = flagData / 10;
  if(dataTerakhir != flagData && flagWaktu == 0){
    flagWaktu = 1;
    detik = 0;
    lcd.setCursor(0,0);
    lcd.print("               ");
    lcd.setCursor(0,1);
    lcd.print("               ");
    if(lcdDisplay1 > 0){
      for(int x = 0; x < lcdDisplay1; x++){
        lcd.setCursor(1 + x,0);
        lcd.write(writeLCD);
        lcd.print(" ");
      }
    }
    else{
      lcd.setCursor(0,0);
      lcd.print("               ");
    }
    if(lcdDisplay2 > 0){
      for(int x = 0; x <= lcdDisplay2; x++){
        lcd.setCursor(1 + x,1);
        lcd.write(writeLCD);
        lcd.print(" ");
      }
    }
    else{
      lcd.setCursor(0,1);
      lcd.print("               ");
    }
  }
  if(dataTerakhir != flagData && flagWaktu == 1){
    display2();
    detik = 0;
  }
  if(dataTerakhir == flagData && flagWaktu == 0){
    display1();
  }
    dataTerakhir = flagData;
    Serial.print(flagData);
    Serial.print("____");
    Serial.println(dataTerakhir);
}
