#include <Adafruit_SH110X.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define OLED_MOSI   11 //blue
#define OLED_CLK   12 //yellow
#define OLED_DC    8 //green
#define OLED_CS    10 //orange
#define OLED_RESET 9 //white
#define HEATER 4
#define PUMP 2
#define BUZZER 3


float chemATemp,chemBTemp,waterTemp; 
int curMenu = 0;

float maxTempVariation = 0.4;
int i = 0 ;
bool reverse = false;
uint8_t sensor1[8] = {0x28, 0x80, 0xD6, 0x3A, 0x01, 0x21, 0x0A, 0xB5}; //sensor address
uint8_t sensor2[8] ={0x28, 0x28, 0x79, 0x43, 0x01, 0x21, 0x0A, 0x2F};
uint8_t sensor3[8] ={0x28, 0x45, 0xA6, 0x75, 0xD0, 0x01, 0x3C, 0xC6};
   


OneWire oneWire(7);
DallasTemperature sensors(&oneWire);

//startup logo
const unsigned char filmochem_bmp [] PROGMEM ={
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x20, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x20, 0x80,
0x00, 0x00, 0x00, 0x00, 0x02, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x20, 0x80,
0x00, 0x00, 0x00, 0x00, 0x01, 0x8f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x23, 0x80,
0x00, 0x00, 0x00, 0x00, 0x09, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x80,
0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0xe0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x80,
0x00, 0x00, 0x00, 0x00, 0x34, 0x01, 0xf0, 0x38, 0x00, 0x00, 0x00, 0x02, 0x00, 0xc0, 0x23, 0x80,
0x00, 0x00, 0x00, 0x00, 0x58, 0x00, 0xf8, 0x64, 0x00, 0x00, 0x00, 0x02, 0x00, 0xc0, 0x20, 0x80,
0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x7c, 0xe8, 0x00, 0x00, 0x00, 0x02, 0x00, 0xc0, 0x20, 0x80,
0x00, 0x00, 0x00, 0x01, 0x20, 0x00, 0x3f, 0x90, 0x00, 0x00, 0x00, 0x02, 0x00, 0xc0, 0x23, 0x80,
0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x1f, 0x60, 0x00, 0x00, 0x00, 0x02, 0x00, 0xc0, 0x20, 0x80,
0x00, 0x00, 0x00, 0x04, 0xc0, 0x00, 0x0e, 0x40, 0x00, 0x00, 0x00, 0x02, 0x00, 0xc0, 0x20, 0x80,
0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x0d, 0x80, 0x00, 0x00, 0x00, 0x02, 0x00, 0xc0, 0x23, 0x80,
0x00, 0x00, 0x00, 0x13, 0xe0, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0xc0, 0x20, 0x80,
0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0xc0, 0x20, 0x80,
0x00, 0x00, 0x00, 0x68, 0xf8, 0x00, 0x74, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0xc0, 0x23, 0x80,
0x00, 0x00, 0x00, 0xb0, 0x7c, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x40, 0x20, 0x80,
0x00, 0x00, 0x00, 0xa0, 0x3e, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x60, 0x20, 0x80,
0x00, 0x00, 0x02, 0x40, 0x1f, 0x03, 0x20, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x20, 0x21, 0x80,
0x00, 0x00, 0x03, 0x80, 0x0f, 0x87, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x30, 0x20, 0x80,
0x00, 0x00, 0x09, 0x00, 0x07, 0xcc, 0x80, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x30, 0x20, 0x80,
0x00, 0x00, 0x06, 0x00, 0x03, 0xfb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x10, 0x21, 0x80,
0x00, 0x00, 0x24, 0x00, 0x01, 0xf2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x18, 0x20, 0x80,
0x00, 0x00, 0x1c, 0x00, 0x00, 0xec, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x10, 0x08, 0x20, 0x80,
0x00, 0x00, 0xdc, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x0c, 0x21, 0x80,
0x00, 0x01, 0x7e, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x04, 0x04, 0x20, 0x80,
0x00, 0x03, 0x7f, 0x00, 0x01, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x04, 0x06, 0x20, 0x80,
0x00, 0x04, 0x8f, 0x80, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x04, 0x02, 0x21, 0x80,
0x00, 0x0d, 0x07, 0xc0, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xce, 0x07, 0xf2, 0x20, 0x80,
0x00, 0x12, 0x03, 0xe0, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9e, 0x0f, 0xf2, 0x20, 0x80,
0x00, 0x2c, 0x01, 0xf0, 0x14, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x01, 0x9f, 0xff, 0xf8, 0x20, 0x80,
0x00, 0x48, 0x00, 0xf8, 0x24, 0x7f, 0x03, 0xf0, 0x3e, 0x03, 0xe1, 0x3f, 0xf8, 0xf8, 0x20, 0x80,
0x00, 0xb0, 0x00, 0x7c, 0x58, 0xff, 0x07, 0xf0, 0x7f, 0x07, 0xf3, 0x3f, 0xf0, 0x7c, 0x3f, 0x80,
0x01, 0x20, 0x00, 0x3f, 0xd0, 0x87, 0x0e, 0x70, 0xc7, 0x0c, 0x62, 0x3f, 0xf0, 0x7c, 0x3f, 0x80,
0x02, 0xc0, 0x00, 0x1f, 0x20, 0x06, 0x1c, 0x71, 0xc6, 0x18, 0x66, 0x7f, 0xf0, 0x7e, 0x3f, 0x80,
0x06, 0xc0, 0x00, 0x0f, 0x00, 0x0e, 0x38, 0x63, 0x8e, 0x38, 0xe6, 0x7f, 0xf8, 0xfe, 0x3f, 0x80,
0x09, 0xc0, 0x00, 0x0c, 0x80, 0x0c, 0x30, 0xe3, 0x0e, 0x70, 0xe4, 0xff, 0xf7, 0xfe, 0x3f, 0x80,
0x0b, 0xe0, 0x00, 0x0c, 0x00, 0x1c, 0x70, 0xc7, 0x0c, 0x71, 0xcc, 0xff, 0xe3, 0xfe, 0x3f, 0x80,
0x27, 0xf0, 0x00, 0x12, 0x00, 0x38, 0x61, 0xce, 0x1c, 0xe1, 0xcc, 0xff, 0xe3, 0xfe, 0x3f, 0x80,
0x5c, 0xf8, 0x00, 0x28, 0x00, 0x70, 0xe1, 0x8e, 0x18, 0xe3, 0x88, 0xff, 0xf3, 0xfe, 0x3f, 0x80,
0x98, 0x7c, 0x00, 0x48, 0x00, 0xe0, 0xc3, 0x9c, 0x39, 0xc3, 0x8c, 0x7f, 0xff, 0xfe, 0x3f, 0x80,
0x70, 0x3e, 0x00, 0xb0, 0x01, 0xc1, 0xc3, 0x1c, 0x71, 0xc7, 0x0c, 0x00, 0x00, 0x00, 0x7f, 0xc0,
0x20, 0x1f, 0x01, 0xa0, 0x07, 0x01, 0x87, 0x18, 0x61, 0x86, 0x07, 0xff, 0xff, 0xf8, 0xff, 0xe0,
0x00, 0x0f, 0x82, 0xc0, 0x0e, 0x01, 0x8e, 0x38, 0xe3, 0x8e, 0x03, 0xff, 0xff, 0xf1, 0xff, 0xf0,
0x00, 0x07, 0xc6, 0x00, 0x1c, 0x03, 0x9c, 0x39, 0xc3, 0x9c, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf0,
0x00, 0x03, 0xe9, 0x00, 0x7f, 0xe3, 0xf8, 0x3f, 0x83, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf0,
0x00, 0x01, 0xf8, 0x00, 0x7f, 0xc3, 0xf0, 0x3f, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf8,
0x00, 0x00, 0xe4, 0x00, 0x00, 0x01, 0xc0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf8,
0x00, 0x00, 0xf0, 0x7e, 0xcc, 0x1c, 0x31, 0xf8, 0x3e, 0x61, 0x9f, 0xdc, 0x30, 0x03, 0xff, 0xf8,
0x00, 0x00, 0x90, 0x60, 0xcc, 0x1c, 0x73, 0x9c, 0xe3, 0x61, 0x98, 0x1c, 0x70, 0x03, 0xff, 0xf8,
0x00, 0x01, 0x60, 0x60, 0xcc, 0x1c, 0x73, 0x0c, 0xc0, 0x61, 0x98, 0x1c, 0x70, 0x01, 0xff, 0xf0,
0x00, 0x03, 0x00, 0x60, 0xcc, 0x1e, 0xf2, 0x06, 0xc0, 0x61, 0x98, 0x1e, 0xf0, 0x01, 0xff, 0xf0,
0x00, 0x05, 0x80, 0x7e, 0xcc, 0x1a, 0xf6, 0x07, 0x80, 0x7f, 0x9f, 0x9a, 0xf0, 0x00, 0xff, 0xe0,
0x00, 0x04, 0x00, 0x60, 0xcc, 0x1b, 0xb2, 0x06, 0xc0, 0x61, 0x98, 0x1b, 0xb0, 0x00, 0x7f, 0xc0,
0x00, 0x02, 0x00, 0x60, 0xcc, 0x1b, 0xb3, 0x0c, 0xc0, 0x61, 0x98, 0x1b, 0xb0, 0x00, 0x3f, 0x80,
0x00, 0x00, 0x00, 0x60, 0xcc, 0x18, 0x33, 0x9c, 0xe3, 0x61, 0x98, 0x18, 0x30, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x60, 0xcf, 0xd8, 0x31, 0xf8, 0x3e, 0x61, 0x9f, 0xd8, 0x30, 0x00, 0x00, 0x00};


  
Adafruit_SH1107 display = Adafruit_SH1107(64, 128,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


void setup() {

  Serial.begin(9600);
  Serial.println("START");
  pinMode(HEATER,OUTPUT);
  pinMode(PUMP,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  digitalWrite(HEATER,HIGH);
  digitalWrite(PUMP,HIGH);


  // Start OLED
  display.begin(0, true); // we dont use the i2c address but we will reset!
  display.clearDisplay();
  display.setRotation(3);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.drawBitmap(0, 0, filmochem_bmp, 128, 64, SH110X_WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color);
  display.display();
  
  
  delay(5000);
  displayProcess("   C-41");
  curMenu = 1;


}

void displayCurrentStatus(){
  display.clearDisplay();
  display.setCursor(0,0);
  
  if(!reverse && i<110){
    i=i+10;
  }
  else if(reverse && i<=110){
    i=i-10;
  }
  if(i == 110){
    reverse = true;
  }
  else if(i == 0){
    reverse = false;
  }
  
  if(waterTemp == -127){
    display.setTextSize(2);
    display.print("WATER \nSENSOR \nERROR!");
    display.invertDisplay(1);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    tone(BUZZER,900,100);
    delay(200);
    display.invertDisplay(0);
    digitalWrite(5,HIGH);
    digitalWrite(6,HIGH);
    tone(BUZZER,700,100);
  }
  else{
    display.setTextSize(1);
    display.print("--- Sensor values ---\n\nWater  temp:  ");
    display.print(waterTemp);
    display.print(" C \nChem A temp:  ");
    display.print(chemATemp);
    display.print(" C \nChem B temp:  ");
    display.print(chemBTemp);
    display.print(" C");
    display.setCursor(i,58);
    display.println("---");
  }
   display.display();

  
}
void displayProcess(char* process){
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Sel. process UP/DOWN\n\n");
  display.setTextSize(2);
  display.println(process);
  display.display();

  
}


void startWaterBath(int targetTemp){
  //tone(BUZZER,800,150);
  digitalWrite(5,HIGH);
  digitalWrite(PUMP,LOW);
  while(true){
      delay(500);
      sensors.requestTemperatures(); 
      waterTemp = sensors.getTempC(sensor3);
      chemATemp = sensors.getTempC(sensor1);
      chemBTemp = sensors.getTempC(sensor2);
      displayCurrentStatus();

      if(waterTemp!=-127 && (waterTemp < targetTemp - maxTempVariation)){
        digitalWrite(HEATER,LOW);
        digitalWrite(6,HIGH);
      }
  
      else if (waterTemp > targetTemp + maxTempVariation)
      {
        digitalWrite(HEATER,HIGH);
        digitalWrite(6,LOW);
      }
  }
}



void loop() {
  delay(100);
  int value = analogRead(A0);
  
  //down 239-249
  //up 415-425
  //left 695-705
  //right 814-824;
  //center 1010-1024


  if(value>239 && value<249){ //down
    if(curMenu==1) {
      displayProcess("   E-6");
      curMenu = 2;
      return;
    }
    displayProcess("   C-41");
    curMenu = 1;
    
  }
  
  else if(value>415 && value<425){ //up
    if(curMenu==1) {
      displayProcess("   E-6");
      curMenu = 2;
      return;
    }
    
    displayProcess("   C-41");
    curMenu = 1;
    
  }

  

   if(value>1010 && value<1024 && curMenu==1){ //center
    display.invertDisplay(1);
    delay(300);
    display.invertDisplay(0);
    startWaterBath(39);
    }

    
  

}
