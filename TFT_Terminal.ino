// Моя прошивка для ESP32C3 клавиатуры

void ads_config();
void UART_CHAT();
void WIFI_CONFIG();
void CONNECT_WIFI(int start_device);
void DISCONNECT_WIFI();
void DOORS();
void PressedButton(int which);
uint16_t color565(byte r, byte g, byte b);
void NOTE(int i);
void serialCheck();
void event();
void symbolDraw(int shift);
void chooseSymbol();
int16_t ads_read0();
int16_t ads_read1();
int16_t ads_read2();
int16_t ads_read3();
void startMenu();
void drawMenu(int numMenu);
void moveCursor();
void chooseMenu();

#include "TEMP_PIC.h"
#include "H_PIC.h"
#include "PHOTORESISTOR_PIC.h"
#include "PHOTORESISTOR_BIG_PIC.h"

// JSON
#include "ArduinoJson.h"

// DOORS
String serverDOORS = "http://85.113.55.171:80/api";

// SHT30 sensor
#include <SHT3x.h>
SHT3x Sensor;

// Wi-Fi for ESP32C3
#include <WiFi.h>
#include "NET_PIC.h"
#include <HTTPClient.h>

// DISPLAY 240x240
#include <Adafruit_GFX.h>    
#include <Arduino_ST7789.h> 
#include <SPI.h>
#define TFT_DC    1 
#define TFT_RST   0 
#define TFT_CS    A0 //VIRTUAL PIN?
#define TFT_MOSI  6   
#define TFT_SCLK  4   
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_CS);

// KEYBOARD
#include <Wire.h>
#include <ADS1115_lite.h>
#include "NOTO_FONT_ALL.h"
ADS1115_lite ads(ADS1115_DEFAULT_ADDRESS);

// PINS
#define RU_BRIGHTNESS 3
#define ENG_BRIGHTNESS 10
#define DISPLAY_BRIGHTNESS 7
#define PHOTORESISTOR 2
int photoresistor_val = 0; 

// EEPROM
#include "EEPROM.h"

// VARS
int h = 27-1;//HEIGHT IMAGE 
int w = 2450;//WIDTH IMAGE
int layout = 1;//ENG ON START
double timeCursor;
String apiC = "Samara";
String apiCountry = "ru";
String apiKey = "";
String InText = "";     
bool stringComplete = false; 
bool originalCol = false;
float raw0 = 0;
float raw1 = 0;
float raw2 = 0;
float raw3 = 0;
float raw0pre = 0;
float raw1pre = 0;
float raw2pre = 0;
float raw3pre = 0;
float top_resolution = 1;
double resolution = 0;
float result0;
float result1;
float result2;
float result3;
float button_space0 = 1;
float numButton0 = -1;
float button_space1 = 1;
float numButton1 = -1;
float button_space2 = 1;
float numButton2 = -1;
float button_space3 = 1;
float numButton3 = -1;
char previous_char;
float p = 3.1415926;
int startX = 2; 
int startY = 0;
int scaleX = 1; // >=1
int scaleY = 1; // >=1
String MyText = "";
String substrSpecific; 
String noteStr;
int rectMenuX = 0;
int rectMenuY = 0;
int selectMenuP = 0;
unsigned long timePast = 0;
int InMenu = 0;
int colorR = 180;
int colorG = 180;
int colorB = 250;
int selectColorProg = 2; //GREEN
uint16_t charColo = WHITE;
uint16_t BackColor;
uint16_t inactElem;
int BRIG = -1;
int BRIG_LAYOUT = -1;
int tempFine;
int humFine;
String StrToBLE;


void setup() {
  // EEPROM
  EEPROM.begin(1200);
  if(EEPROM.read(0) == 255)   {EEPROM.writeString(0, ""); EEPROM.commit();}//NOTE
  if(EEPROM.read(400) == 255) {EEPROM.writeString(400, ""); EEPROM.commit();}//SSID
  if(EEPROM.read(450) == 255) {EEPROM.writeString(450, ""); EEPROM.commit();}//PASSWORD
  if(EEPROM.read(490) == 255) {EEPROM.writeInt   (490, 125); EEPROM.commit();}//BRIGHTNESS  
  if(EEPROM.read(494) == 255) {EEPROM.writeInt   (494, 150); EEPROM.commit();}//BRIGHTNESS_LAYOUT
  if(EEPROM.read(500) == 255) {EEPROM.writeInt   (500, 0);  EEPROM.commit();}//FLAG TO CHANGE BACKGROUND//[0 OR 100]
  if(EEPROM.read(505) == 255) {EEPROM.writeInt   (505, 0);  EEPROM.commit();}//R
  if(EEPROM.read(506) == 255) {EEPROM.writeInt   (506, 0);  EEPROM.commit();}//G
  if(EEPROM.read(507) == 255) {EEPROM.writeInt   (507, 70); EEPROM.commit();}//B
  if(EEPROM.read(515) == 255) {EEPROM.writeInt   (515, 2);  EEPROM.commit();}//LAYOUT[1,2]
  if(EEPROM.read(520) == 255) {EEPROM.writeInt   (520, 0);  EEPROM.commit();}//SMART HOME CHANNEL[0,1,2,3]
  if(EEPROM.read(525) == 255) {EEPROM.writeInt   (525, 0);  EEPROM.commit();}//WI-FI AUTO CONNECT[0,1] 
  if(EEPROM.read(530) == 255) {EEPROM.writeString(530, apiC);       EEPROM.commit();}//CITY
  if(EEPROM.read(590) == 255) {EEPROM.writeString(590, apiCountry); EEPROM.commit();}//COUNTRY
  if(EEPROM.read(650) == 255) {EEPROM.writeString(650, apiKey);     EEPROM.commit();}//KEY FROM OPENWEATHERMAP.ORG
  if(EEPROM.read(800) == 255) {EEPROM.writeInt   (800, 9);  EEPROM.commit();}//TEMPFINE//"-1" = PROBLEM//9  = VIRTUAL 0
  if(EEPROM.read(804) == 255) {EEPROM.writeInt   (804, 9);  EEPROM.commit();}//HUMFINE//"-1"  = PROBLEM//9  = VIRTUAL 0
  if(EEPROM.read(808) == 255) {EEPROM.writeInt   (808, 12); EEPROM.commit();}//SavedUTC//"-1" = PROBLEM//12 = VIRTUAL 0
  if(EEPROM.read(820) == 255)  {EEPROM.writeString(820, "Hello world! Characters are sent at intervals for connection stability. String=1. ");                 EEPROM.commit();}//BLE STRING 1
  if(EEPROM.read(920) == 255)  {EEPROM.writeString(920, "This string can contain up to 98 characters. For example, to store the URL or password. String=2. "); EEPROM.commit();}//BLE STRING 2
  if(EEPROM.read(1020) == 255) {EEPROM.writeString(1020, "For bluetooth to work successfully, restart it on both devices after connecting. String=3. ");        EEPROM.commit();}//BLE STRING 3
 

  BRIG = EEPROM.readInt(490); 
  if(BRIG < 10) {
    BRIG = 110;        
    EEPROM.writeInt(490, BRIG);        
    EEPROM.commit();
  }//BRIGHTNESS FOR FIRST START
  BRIG_LAYOUT = EEPROM.readInt(494); 
  if(BRIG_LAYOUT < 10) {
    BRIG_LAYOUT = 140; 
    EEPROM.writeInt(494, BRIG_LAYOUT); 
    EEPROM.commit();
  }//BRIGHTNESS_LAYOUT FOR FIRST START
  if(EEPROM.readInt(500) == 100) {
    BackColor = color565(EEPROM.readInt(505), EEPROM.readInt(506), EEPROM.readInt(507));
  } //THEME FROM EEPROM//500 = FLAG 
  else {
    //BackColor = color565(0, 0, 70);
    BackColor = color565(0, 0, 170);
    charColo = color565(255, 255, 85);
    //charColo = color565(85, 255, 255);
  } //DEFAULT THEME

  // PINS
  pinMode(PHOTORESISTOR, INPUT);      
  pinMode(DISPLAY_BRIGHTNESS, OUTPUT);                                 
  analogWrite(DISPLAY_BRIGHTNESS, BRIG); 
  pinMode(RU_BRIGHTNESS, OUTPUT);  
  analogWrite(RU_BRIGHTNESS, BRIG_LAYOUT); 
  pinMode(ENG_BRIGHTNESS, OUTPUT); 
  analogWrite(ENG_BRIGHTNESS, BRIG_LAYOUT);

  // SHT30
  Sensor.Begin(); 

  // DISPLAY 240x240
  tft.setRotation(1);
  tft.init(240, 240); 
  uint16_t time = millis();
  inactElem = color565(180, 180, 180);
  ads_config();
  /*tft.fillScreen(color565(17,17,155));
  tft.fillRect(40,111, 40,17, color565(55,255,55));    
  tft.drawCircle(120,120, 119, color565(255,255,255)); 

  tft.setTextWrap(false);
  tft.setCursor(80, 20);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Hello World!");

  tft.setFont(&FreeMono12pt7b);
  tft.setTextWrap(true);
  tft.setCursor(40, 55);
  tft.setTextColor(color565(255,55,55));
  tft.setTextSize(1, 1);
  tft.print("Hello again!");*/

  // Serial
  Serial.begin(9600);
  while(!Serial);
  //Serial.println(""); 
  //Serial.println("Test");

  //BackColor = color565(0, 0, 70);

  if(EEPROM.readInt(525) == 1) {
    CONNECT_WIFI(1);
  }//(1) = RENURN HERE WITHOUT PRESS ESC

  startMenu();
}


void loop(void) {
  if(numButton0 == 14 || numButton2 == 8) {
    InMenu = 0; 
    startMenu(); //HERE AFTER EXIT FROM VOIDS WITHOUT RECHECK NUMBUTTON0==14(ESC KEY)// 8 = MENU KEY
    analogWrite(RU_BRIGHTNESS, BRIG_LAYOUT);   
    analogWrite(ENG_BRIGHTNESS, BRIG_LAYOUT);
  }
  numButton0 = -1; 
  numButton1 = -1; 
  numButton2 = -1; 
  numButton3 = -1;
  PressedButton(0); // 2 CIRCUITS
  if(numButton0 > -1 || numButton2 > -1) {
    if(InMenu < 0.5) {
      moveCursor(); 
    }
    if(numButton2 == 1 && selectMenuP < 12) {
      chooseMenu(); 
      InMenu = 0; 
      numButton0 = -1; 
      numButton2 = -1; 
      startMenu();
    }
  }//ENTER PRESSED AND AFTER RETURN
//PressedButton(1);//TEMPORARY ALL
}


void PressedButton(int which) {
  numButton0 = -1; numButton1 = -1; numButton2 = -1; numButton3 = -1;
//tft.fillRect(50,50,85,70,TFT_BLUE);//TEMPORARY
  
  int ix0 = 0, ix1 = 0, ix2 = 0, ix3 = 0;
  int times = 2; //HOW MANY
  int incdec = 700;
  int startPoint = 650; //800 = EDGE
  
  if(which == 0) {//CHECK ONLY 2 CIRCUITS 
  raw0=ads_read0(); raw1=ads_read1(); raw2=ads_read2(); raw3=ads_read3();
  top_resolution = max(max(raw0,raw1),max(raw2,raw3)); // 

if(top_resolution - raw0 > startPoint) {//TOP BUTTON FIX
    button_space0 = top_resolution / 14.6; //15 BUTTONS
    numButton0 = round((raw0-incdec) / button_space0);
  while(ix0 < times) {raw0=ads_read0();
                      if(round((raw0-incdec) / button_space0) != numButton0) {break;} ix0++; }   
                      if(ix0 < times || numButton0 > 14) {numButton0 = -1;} //15 BUTTONS
                                       } 

if(top_resolution - raw2 > startPoint) {//TOP BUTTON FIX
    button_space2 = top_resolution / 13.6; //14 BUTTONS
    numButton2 = round((raw2-incdec) / button_space2);                                
  while(ix2 < times) {raw2=ads_read2();
                      if(round((raw2-incdec) / button_space2) != numButton2) {break;} ix2++; }   
                      if(ix2 < times || numButton2 > 13) {numButton2 = -1;} //14 BUTTONS
                                       }                   
//startX = 52; startY = 52; MyText = numButton2; event();//TEMPORARY
//startX = 52; startY = 72; MyText = raw2-incdec; event();//TEMPORARY
//startX = 52; startY = 92; MyText = button_space2; event();//TEMPORARY                   
                 }
                 
                 else {//CHECK ALL KEYS WHICH CONNECTED TO ADS1115
                 
  raw0=ads_read0(); raw1=ads_read1(); raw2=ads_read2(); raw3=ads_read3();
  top_resolution = max(max(raw0,raw1),max(raw2,raw3)); // 

if(top_resolution - raw0 > startPoint) {//TOP BUTTON FIX
    button_space0 = top_resolution / 14.6; //15 BUTTONS
    numButton0 = round((raw0-incdec) / button_space0);
  while(ix0 < times) {raw0=ads_read0();
                      if(round((raw0-incdec) / button_space0) != numButton0) {break;} ix0++; }   
                      if(ix0 < times || numButton0 > 14) {numButton0 = -1;} //15 BUTTONS
                                       }

if(top_resolution - raw1 > startPoint) {//TOP BUTTON FIX
    button_space1 = top_resolution / 1.6; //2 BUTTONS
    numButton1 = round((raw1-incdec) / button_space1);
  while(ix1 < times) {raw1=ads_read1();
                      if(round((raw1-incdec) / button_space1) != numButton1) {break;} ix1++; }   
                      if(ix1 < times || numButton1 > 1) {numButton1 = -1;} //2 BUTTONS
                                       }

if(top_resolution - raw2 > startPoint) {//TOP BUTTON FIX
    button_space2 = top_resolution / 13.6; //14 BUTTONS
    numButton2 = round((raw2-incdec) / button_space2);                                
  while(ix2 < times) {raw2=ads_read2();
                      if(round((raw2-incdec) / button_space2) != numButton2) {break;} ix2++; }   
                      if(ix2 < times || numButton2 > 13) {numButton2 = -1;} //14 BUTTONS
                                       }
if(top_resolution - raw3 > startPoint) {//TOP BUTTON FIX
    button_space3 = top_resolution / 14.6; //15 BUTTONS
    numButton3 = round((raw3-incdec) / button_space3);
  while(ix3 < times) {raw3=ads_read3();
                      if(round((raw3-incdec) / button_space3) != numButton3) {break;} ix3++; }   
                      if(ix3 < times || numButton3 > 14) {numButton3 = -1;} //15 BUTTONS
                                       } 
//startX = 52; startY = 52; MyText = numButton3; event();//TEMPORARY
//startX = 52; startY = 72; MyText = raw3-incdec; event();//TEMPORARY
//startX = 52; startY = 92; MyText = button_space3; event();//TEMPORARY                   
                      }     
}


uint16_t color565( byte r, byte g, byte b ) {
  return ( ( r & 0xF8 ) << 8 ) | ( ( g & 0xFC ) << 3 ) | ( ( b & 0xF8 ) >> 3 );
}


void NOTE(int i) { 
  numButton0 = -1; 
  numButton2 = -1; 
  delay(50);
  uint16_t wall; 
  if(i != 4) {
    wall = color565(100, 0, 30); 
    tft.fillScreen(wall);
  } //IF !UART CHAT && ...
  if(i == 8 || i == 9 || i == 10) {
    tft.fillRect(0, 148, 240, 92, color565(0, 0, 255)); 
    MyText = "ENT=напечатать[BT]English only. 'layout' сменит раскладку на 2м устр-ве."; 
    scaleX = 1; 
    scaleY = 1; 
    startX = 2; 
    startY = 148; 
    charColo = WHITE; 
    event();
    //if(!Keyboard.isConnected()) {Keyboard.begin(); Mouse.begin();}
    layout = 1;
  } //IF EDIT STRING FOR SENDING VIA BLE //ENGLISH ONLY
  uint16_t curCol = color565(0, 200, 250);//CURSOR
  int curW = 8, curH = 22;

/*    if(i != 8 && i != 9 && i != 10) {//IF EDIT STRING AND SEND VIA BLE LAYOUT WILL BE CHANGED ON ANOTHER DEVICE
    if(EEPROM.readInt(515) == 2) {layout = 2; analogWrite(3, BRIG_LAYOUT); //RU LED
                                              analogWrite(10,0);  }//ENG LED
                            else {layout = 1; analogWrite(3, 0);   //RU LED
                                              analogWrite(10,BRIG_LAYOUT);}//ENG LED
                                    }
*/                                                 
  startX = 2; 
  startY = 0; //scaleX = 3; scaleY = 3;
  int add = 0; 
  int prevX, prevY;//ADD = SHIFT Y
  noteStr = "";
  if(i == 1){
    noteStr = EEPROM.readString(0); //NOTE//LENGTH ?
    startX = 2; 
    startY = 169;   
    MyText = "ESC или ENTER > сохр., MENU > без сохр. UP > UART"; 
    charColo = color565(180,180,250); 
    event(); 
    charColo = WHITE; 
    startX = 2; 
    startY = 0;
  }
  if(i == 2){
    noteStr = EEPROM.readString(400);
  } //SSID
  if(i == 3){
    noteStr = EEPROM.readString(450);
  } //PASSWORD 
  if(i == 4){
    startY = 167; 
    add = 167; 
    wall = BLUE; 
    Serial.begin(9600);
  }//FOR UART CHAT
  if(i == 5){noteStr = EEPROM.readString(530);} //APICITY
  if(i == 6){noteStr = EEPROM.readString(590);} //APICOUNTRY
  if(i == 7){noteStr = EEPROM.readString(650); //APIKEY  
  startX = 2; 
  startY = 144;   
  MyText = "please visit      openweathermap.orgto register and   get API key"; 
  charColo = color565(180, 180, 250); 
  event(); 
  charColo = WHITE; 
  startX = 2; 
  startY = 0;} 
  if(i == 8)  {noteStr = EEPROM.readString(820); } //BLE STRING 1
  if(i == 9)  {noteStr = EEPROM.readString(920); } //BLE STRING 2 
  if(i == 10) {noteStr = EEPROM.readString(1020);} //BLE STRING 3 
  
  //if(i == 2) {for(int qa = 0; qa < 50; qa++) {ssid[qa] = EEPROM.read(200 + qa);} noteStr = ssid.c_str();}//SSID
  //if(i == 3) {for(int qb = 0; qb < 50; qb++) {password[qb] = EEPROM.read(250 + qb);} noteStr = password.c_str();}//PASSWORD
  MyText = noteStr; 
  event();//DRAW NOTE OR SSID OR PASSWORD OR...
  tft.fillRect(startX, startY + 2, 8, 22, curCol);
  numButton0 = -1; 
  numButton2 = -1; 
  timeCursor = millis();
  PressedButton(0);
 
  while(numButton0 != 14 && numButton2 != 1) {  
 
  if(i == 4 && Serial.available()) {
    serialCheck(); //FOR UART CHAT
    //if(stringComplete == true) {
    prevX = startX; 
    prevY = startY; 
    MyText = InText.substring(0, 108);//CUT INCOMING STRING 
    tft.fillRoundRect(-5, 77, 250, 73, 15, color565(255, 200, 200));
    startX = 0; 
    startY = 77; 
    charColo = BLACK; 
    event(); 
    charColo = WHITE;  
    startX = prevX; 
    startY = prevY; 
    InText = ""; 
    //stringComplete = false; 
  }    
                                                                          
    PressedButton(1); 
//    if(numButton2 == 1 && (i == 8 || i == 9 || i == 10)) {SEND_STRING_BLE(0); delay(300); numButton2 = -1; continue;} //USE 'ENTER'  IF BLE
//                      if(numButton2 == 10 && (i == 8 || i == 9 || i == 10)) {SEND_STRING_BLE(1); delay(300); numButton2 = -1; continue;}//USE 'LAYOUT' IF BLE
                      if(numButton2 == 8 && i != 8 && i != 9 && i != 10) {analogWrite(3,  BRIG_LAYOUT);         //RU LED 
                                                                          analogWrite(10, BRIG_LAYOUT); return;}//ENG LED //DISABLE 'MENU' IF BLE FOR SAVING ANYWAY VIA 'ESC'
                                         if(millis() - timeCursor < 350) {continue;}//PAUSE FOR COMFORT
                                         if(numButton0 == -1 &&
                                            numButton1 == -1 && 
                                            numButton2 == -1 &&
                                            numButton3 == -1   ) {continue;} //NOTHING PRESSED
                                         
                                         if((numButton0 != -1 || numButton2 != -1 || numButton3 != -1)) {timeCursor = millis();}//PAUSE ONLY IF SYMBOL KEY PRESSED
 
if((i == 4) && noteStr.length() > 106){
  goto forfaster;
}//LIMIT FOR UART CHAT  //54 RU AND 107 EN
if((i == 8 || i == 9 || i == 10) && noteStr.length() > 97) {goto forfaster;}//LIMIT FOR BLE STRING //49 RU AND 98  EN
if((numButton1 == 0 || numButton1 == 1) && (numButton0 == -1 && numButton2 == -1 && numButton3 == -1)) {goto forfaster;}//SHIFT OR EXTRA AND NOTHING ELSE
           //
if(layout == 1) {                        //SHIFT                             //EXTRA
    if(numButton0 == 11) {MyText = "1"; if(numButton1 == 1) {MyText = "1";} if(numButton1 == 0) {MyText = "`";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 12) {MyText = "q"; if(numButton1 == 1) {MyText = "Q";} if(numButton1 == 0) {MyText = "2";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 13) {MyText = "w"; if(numButton1 == 1) {MyText = "W";} if(numButton1 == 0) {MyText = "3";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 2)  {MyText = "e"; if(numButton1 == 1) {MyText = "E";} if(numButton1 == 0) {MyText = "4";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 1)  {MyText = "r"; if(numButton1 == 1) {MyText = "R";} if(numButton1 == 0) {MyText = "5";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 14) {MyText = "t"; if(numButton1 == 1) {MyText = "T";} if(numButton1 == 0) {MyText = "6";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton2 == 13) {MyText = "y"; if(numButton1 == 1) {MyText = "Y";} if(numButton1 == 0) {MyText = "7";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton2 == 12) {MyText = "u"; if(numButton1 == 1) {MyText = "U";} if(numButton1 == 0) {MyText = "8";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton2 == 11) {MyText = "i"; if(numButton1 == 1) {MyText = "I";} if(numButton1 == 0) {MyText = "9";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton2 == 9)  {MyText = "o"; if(numButton1 == 1) {MyText = "O";} if(numButton1 == 0) {MyText = "0";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton2 == 6)  {MyText = "p"; if(numButton1 == 1) {MyText = "P";} if(numButton1 == 0) {MyText = "-";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
        
    if(numButton0 == 8)  {MyText = "a"; if(numButton1 == 1) {MyText = "A";} if(numButton1 == 0) {MyText = "\134";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 7)  {MyText = "s"; if(numButton1 == 1) {MyText = "S";} if(numButton1 == 0) {MyText = "/";}    noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 6)  {MyText = "d"; if(numButton1 == 1) {MyText = "D";} if(numButton1 == 0) {MyText = "@";}    noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 3)  {MyText = "f"; if(numButton1 == 1) {MyText = "F";} if(numButton1 == 0) {MyText = "(";}    noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 0)  {MyText = "g"; if(numButton1 == 1) {MyText = "G";} if(numButton1 == 0) {MyText = ")";}    noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 12) {MyText = "h"; if(numButton1 == 1) {MyText = "H";} if(numButton1 == 0) {MyText = "%";}    noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 0)  {MyText = "j"; if(numButton1 == 1) {MyText = "J";} if(numButton1 == 0) {MyText = "*";}    noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 1)  {MyText = "k"; if(numButton1 == 1) {MyText = "K";} if(numButton1 == 0) {MyText = "\42";}  noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 2)  {MyText = "l"; if(numButton1 == 1) {MyText = "L";} if(numButton1 == 0) {MyText = ":";}    noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 3)  {MyText = ";"; if(numButton1 == 1) {MyText = ";";} if(numButton1 == 0) {MyText = ";";}    noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 4)  {MyText = "'"; if(numButton1 == 1) {MyText = "'";} if(numButton1 == 0) {MyText = "'";}    noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 

    if(numButton0 == 5)  {MyText = "z";   if(numButton1 == 1) {MyText = "Z";}   if(numButton1 == 0) {MyText = "{";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 4)  {MyText = "x";   if(numButton1 == 1) {MyText = "X";}   if(numButton1 == 0) {MyText = "}";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 13) {MyText = "c";   if(numButton1 == 1) {MyText = "C";}   if(numButton1 == 0) {MyText = "<";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 10) {MyText = "\40"; if(numButton1 == 1) {MyText = "_";}   if(numButton1 == 0) {MyText = "|";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 11) {MyText = "v";   if(numButton1 == 1) {MyText = "V";}   if(numButton1 == 0) {MyText = ">";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 9)  {MyText = "b";   if(numButton1 == 1) {MyText = "B";}   if(numButton1 == 0) {MyText = "&";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 8)  {MyText = "n";   if(numButton1 == 1) {MyText = "N";}   if(numButton1 == 0) {MyText = "?";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 7)  {MyText = "m";   if(numButton1 == 1) {MyText = "M";}   if(numButton1 == 0) {MyText = "!";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 6)  {MyText = ",";   if(numButton1 == 1) {MyText = ",";}   if(numButton1 == 0) {MyText = ",";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 5)  {MyText = ".";   if(numButton1 == 1) {MyText = ".";}   if(numButton1 == 0) {MyText = ".";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton2 == 7)  {MyText = "=";   if(numButton1 == 1) {MyText = "+";}   if(numButton1 == 0) {MyText = "+";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}  
    if(numButton2 == 5)  {MyText = "[";   if(numButton1 == 1) {MyText = "[";} if(numButton1 == 0) {MyText = "[";}   noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}                             
    if(numButton2 == 2)  {MyText = "]";   if(numButton1 == 1) {MyText = "]";} if(numButton1 == 0) {MyText = "]";}   noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}              
    if(numButton0 == 9) {Serial.begin(9600); Serial.println(noteStr); goto forfaster;}             
                }
if(layout == 2) {                       
    if(numButton0 == 11) {MyText = "ё"; if(numButton1 == 1) {MyText = "Ё";} if(numButton1 == 0) {MyText = "1";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 12) {MyText = "й"; if(numButton1 == 1) {MyText = "Й";} if(numButton1 == 0) {MyText = "2";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 13) {MyText = "ц"; if(numButton1 == 1) {MyText = "Ц";} if(numButton1 == 0) {MyText = "3";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 2)  {MyText = "у"; if(numButton1 == 1) {MyText = "У";} if(numButton1 == 0) {MyText = "4";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 1)  {MyText = "к"; if(numButton1 == 1) {MyText = "К";} if(numButton1 == 0) {MyText = "5";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 14) {MyText = "е"; if(numButton1 == 1) {MyText = "Е";} if(numButton1 == 0) {MyText = "6";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton2 == 13) {MyText = "н"; if(numButton1 == 1) {MyText = "Н";} if(numButton1 == 0) {MyText = "7";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton2 == 12) {MyText = "г"; if(numButton1 == 1) {MyText = "Г";} if(numButton1 == 0) {MyText = "8";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton2 == 11) {MyText = "ш"; if(numButton1 == 1) {MyText = "Ш";} if(numButton1 == 0) {MyText = "9";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton2 == 9)  {MyText = "щ"; if(numButton1 == 1) {MyText = "Щ";} if(numButton1 == 0) {MyText = "0";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton2 == 6)  {MyText = "з"; if(numButton1 == 1) {MyText = "З";} if(numButton1 == 0) {MyText = "-";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
        
    if(numButton0 == 8){MyText = "ф"; if(numButton1 == 1) {MyText = "Ф";} if(numButton1 == 0) {MyText = "\134";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 7) {MyText = "ы"; if(numButton1 == 1) {MyText = "Ы";} if(numButton1 == 0) {MyText = "/";}   noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 6) {MyText = "в"; if(numButton1 == 1) {MyText = "В";} if(numButton1 == 0) {MyText = "@";}   noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 3) {MyText = "а"; if(numButton1 == 1) {MyText = "А";} if(numButton1 == 0) {MyText = "(";}   noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 0) {MyText = "п"; if(numButton1 == 1) {MyText = "П";} if(numButton1 == 0) {MyText = ")";}   noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 12) {MyText = "р"; if(numButton1 == 1) {MyText = "Р";} if(numButton1 == 0) {MyText = "%";}  noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 0) {MyText = "о"; if(numButton1 == 1) {MyText = "О";} if(numButton1 == 0) {MyText = "*";}   noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 1) {MyText = "л"; if(numButton1 == 1) {MyText = "Л";} if(numButton1 == 0) {MyText = "\42";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 2) {MyText = "д"; if(numButton1 == 1) {MyText = "Д";} if(numButton1 == 0) {MyText = ":";}   noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 3) {MyText = "ж"; if(numButton1 == 1) {MyText = "Ж";} if(numButton1 == 0) {MyText = ";";}   noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 4) {MyText = "э"; if(numButton1 == 1) {MyText = "Э";} if(numButton1 == 0) {MyText = "'";}   noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 

    if(numButton0 == 5)  {MyText = "я";   if(numButton1 == 1) {MyText = "Я";} if(numButton1 == 0) {MyText = "{";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton0 == 4)  {MyText = "ч";   if(numButton1 == 1) {MyText = "Ч";} if(numButton1 == 0) {MyText = "}";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 13) {MyText = "с";   if(numButton1 == 1) {MyText = "С";} if(numButton1 == 0) {MyText = "<";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 10) {MyText = "\40"; if(numButton1 == 1) {MyText = "_";} if(numButton1 == 0) {MyText = "|";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 11) {MyText = "м";   if(numButton1 == 1) {MyText = "М";} if(numButton1 == 0) {MyText = ">";} noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 9) {MyText = "и";   if(numButton1 == 1) {MyText = "И";} if(numButton1 == 0) {MyText = "&";}  noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 8) {MyText = "т";   if(numButton1 == 1) {MyText = "Т";} if(numButton1 == 0) {MyText = "?";}  noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 7) {MyText = "ь";   if(numButton1 == 1) {MyText = "Ь";} if(numButton1 == 0) {MyText = "!";}  noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton3 == 6) {MyText = "б";   if(numButton1 == 1) {MyText = "Б";} if(numButton1 == 0) {MyText = ",";}  noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;} 
    if(numButton3 == 5) {MyText = "ю";   if(numButton1 == 1) {MyText = "Ю";} if(numButton1 == 0) {MyText = ".";}  noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}
    if(numButton2 == 7) {MyText = "=";   if(numButton1 == 1) {MyText = "+";} if(numButton1 == 0) {MyText = "+";}  noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}  
    if(numButton2 == 5) {MyText = "х";   if(numButton1 == 1) {MyText = "Х";} if(numButton1 == 0) {MyText = "[";}  noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}                             
    if(numButton2 == 2) {MyText = "ъ";   if(numButton1 == 1) {MyText = "Ъ";} if(numButton1 == 0) {MyText = "]";}  noteStr = noteStr + MyText; tft.fillRect(startX,startY+2,curW,curH,wall);  event(); tft.fillRect(startX,startY+2,curW,curH,curCol); goto forfaster;}              
    if(numButton0 == 9) {Serial.begin(9600); Serial.println(noteStr); goto forfaster;}            
                }
                forfaster:
           
    if(numButton2 == 0) {if(noteStr.charAt(noteStr.length()-2) == '\320' ||  //IF BACKSPACE
                            noteStr.charAt(noteStr.length()-2) == '\321'   ) { tft.fillRect(startX,startY+2,curW,curH,wall); //DELETE CURSOR                   
                             MyText = noteStr.substring(noteStr.length()-2), noteStr.substring(noteStr.length()-1); //RU CHAR
                                                                           charColo = wall;
                                                                           if(startX < 3 && startY > 1+add) {startX = 223; startY-=22; event();} //MOVE CURSOR TO CLEAR SYMBOL[BACKGROUND COLOR]
                                                                           else{if(startX < 3 && startY < 1+add) {startX = 2; startY = 0+add; event();}}
                                                                           if(startX > 2) {startX-=13; event();}
                                                                           if(startX < 3 && startY > 1+add) {startX = 223; startY-=22;         } //MOVE CURSOR AGAIN TO PRINT ANY NEW SYMBOL
                                                                           else{if(startX < 3 && startY < 1+add) {startX = 2;   startY = 0+add;         }}
                                                                           if(startX > 2) {startX-=13;         }
                             noteStr.remove(noteStr.length()-2,2); //REMOVE LAST RU CHAR
                                                                               tft.fillRect(startX,startY+2,curW,curH,curCol); //TFT_REDRAW CURSOR 
                                                                             } 
                                        /////////////////////////////////////////////////////////////                                                   
                                                                        else { tft.fillRect(startX,startY+2,curW,curH,wall); //DELETE CURSOR
                             MyText = noteStr.charAt(noteStr.length()-1);                                          //ENG CHAR OR SYMBOL
                                                                           charColo = wall;
                                                                           if(startX < 3 && startY > 1+add) { startX = 223; startY-=22; event();} //MOVE CURSOR TO CLEAR SYMBOL[BACKGROUND COLOR]
                                                                           else{if(startX < 3 && startY < 1+add) {startX = 2; startY = 0+add; event();}}
                                                                           if(startX > 2) {startX-=13; event();}
                                                                           if(startX < 3 && startY > 1+add) {startX = 223; startY-=22;         } //MOVE CURSOR AGAIN TO PRINT ANY NEW SYMBOL
                                                                           else{if(startX < 3 && startY < 1+add) {startX = 2;   startY = 0+add;         }}
                                                                           if(startX > 2) {startX-=13;         }                                             
                             noteStr.remove(noteStr.length()-1,1);   //REMOVE LAST OTHER CHAR  
                                                                                    tft.fillRect(startX,startY+2,curW,curH,curCol); //TFT_REDRAW CURSOR                                             
                                                                                  }
     MyText = noteStr; //event();
                        } charColo = WHITE;

 if(i != 8 && i != 9 && i != 10) {//IF EDIT STRING AND SEND VIA BLE LAYOUT WILL BE CHANGED ON ANOTHER DEVICE                   
    if(numButton2 == 10 && layout == 1) {analogWrite(3, BRIG_LAYOUT); analogWrite(10, 0);   layout = 2; delay(350); numButton2 = -1;
//                                        EEPROM.writeInt(515, 2); EEPROM.commit();
} //CHANGE TO RU //IN THE END BECAUSE FURTHER NO CHECK PRESSED BUTTONS
    if(numButton2 == 10 && layout == 2) {analogWrite(3, 0);   analogWrite(10, BRIG_LAYOUT); layout = 1; delay(350); numButton2 = -1;
//                                        EEPROM.writeInt(515, 1); EEPROM.commit();
                                        } //CHANGE TO ENG//IN THE END BECAUSE FURTHER NO CHECK PRESSED BUTTONS
                                 }
                       
                         }//END WHILE 
                             analogWrite(3,  BRIG_LAYOUT); //RU LED 
                             analogWrite(10, BRIG_LAYOUT); //ENG LED
                             if(i == 1) {EEPROM.writeString(0, noteStr);   EEPROM.commit(); InMenu = 0; numButton0 = -1; numButton2 = -1;}//startMenu();
                             if(i == 2) {EEPROM.writeString(400, noteStr); EEPROM.commit(); numButton0 = -1; numButton2 = -1;}//SSID//WIFI_CONFIG();
                             if(i == 3) {EEPROM.writeString(450, noteStr); EEPROM.commit(); numButton0 = -1; numButton2 = -1;}//PASSWORD//WIFI_CONFIG(); 
                             if(i == 5) {EEPROM.writeString(530, noteStr); EEPROM.commit(); numButton0 = -1; numButton2 = -1;}//APICITY
                             if(i == 6) {EEPROM.writeString(590, noteStr); EEPROM.commit(); numButton0 = -1; numButton2 = -1;}//APICOUNTRY
                             if(i == 7) {EEPROM.writeString(650, noteStr); EEPROM.commit(); numButton0 = -1; numButton2 = -1;}//APIKEY  
                             if(i == 8) {EEPROM.writeString(820,  noteStr); EEPROM.commit(); numButton0 = -1; numButton2 = -1;}//BLE STRING 1
                             if(i == 9) {EEPROM.writeString(920,  noteStr); EEPROM.commit(); numButton0 = -1; numButton2 = -1;}//BLE STRING 2
                             if(i == 10){EEPROM.writeString(1020, noteStr); EEPROM.commit(); numButton0 = -1; numButton2 = -1;}//BLE STRING 3 
                             Serial.end();//FOR UP+S                              
}//END NOTE();


void serialCheck(){
  InText = "";
  while (Serial.available()){
    char inChar = (char)Serial.read();
    InText += inChar;
    //if (inChar == '\n') {stringComplete = true;}
  }
}


void event() {
  int charByteStart, charByteEnd, dobByte, strLen = MyText.length();
  for (charByteStart=0; charByteStart < strLen + 0; charByteStart += dobByte) {
    if(MyText.charAt(charByteStart) == '\320' || MyText.charAt(charByteStart) == '\321'){
      dobByte=2;
    }//+BYTE IF RUSSIAN   
    else {
      dobByte=1;
    }    
    charByteEnd = charByteStart + dobByte;        
    substrSpecific = MyText.substring(charByteStart, charByteEnd);
    chooseSymbol();
  }
} 


void symbolDraw(int shift){
  int row = 0, col = 0, buffidx=0; int16_t colo; //int h = 27-1, w = 2436; 
  int symbolW = 14, symbolWidth = 1;
                                    if(startX > (240-(symbolW*scaleX))) {startX = 2; startY = startY+(h*scaleY)+(-4);}//-X =BETWEEN ROWS
  for (col=startY; col<h*scaleY+startY; col+=scaleY) {  symbolWidth = 1;                                                 
  for (row=startX; row<w*scaleX+startX; row+=scaleX) { 
      colo = pgm_read_word(NOTO_FONT_ALL + buffidx + shift*symbolW); //COLOR EVERY POINT OF BITMAP
                                                      symbolWidth++;
      if(colo != -1) {//-1 IF INT16_T //65535 IF UINT16_T //NOT DRAW WHITE PIXEL
                      //if (originalCol != true) {colo = charColo;}//IF YOU NEED A COLOR NOT EXISTING IN THE BITMAP, BUT WITHOUT HALFOTONES
                      tft.fillRect(row,col,scaleX,scaleY,charColo); 
                      //tft.drawPixel(row,col,charColo);
                     }
      buffidx++;
                      if(symbolWidth > (symbolW)) {buffidx = buffidx + (w - symbolW); break;}                                 
                                                     }    
                                                     }
                                                      startX = row; 
}


void chooseSymbol() {
//if(substrSpecific == "\40") {startX+=15; return;}
if(substrSpecific == "а") {symbolDraw(0); return;}
if(substrSpecific == "б") {symbolDraw(1); return;}
if(substrSpecific == "в") {symbolDraw(2); return;}
if(substrSpecific == "г") {symbolDraw(3); return;}
if(substrSpecific == "д") {symbolDraw(4); return;}
if(substrSpecific == "е") {symbolDraw(5); return;}
if(substrSpecific == "ё") {symbolDraw(6); return;}
if(substrSpecific == "ж") {symbolDraw(7); return;}
if(substrSpecific == "з") {symbolDraw(8); return;}
if(substrSpecific == "и") {symbolDraw(9); return;}
if(substrSpecific == "й") {symbolDraw(10); return;}
if(substrSpecific == "к") {symbolDraw(11); return;}
if(substrSpecific == "л") {symbolDraw(12); return;}
if(substrSpecific == "м") {symbolDraw(13); return;}
if(substrSpecific == "н") {symbolDraw(14); return;}
if(substrSpecific == "о") {symbolDraw(15); return;}
if(substrSpecific == "п") {symbolDraw(16); return;}
if(substrSpecific == "р") {symbolDraw(17); return;}
if(substrSpecific == "с") {symbolDraw(18); return;}
if(substrSpecific == "т") {symbolDraw(19); return;}
if(substrSpecific == "у") {symbolDraw(20); return;}
if(substrSpecific == "ф") {symbolDraw(21); return;}
if(substrSpecific == "х") {symbolDraw(22); return;}
if(substrSpecific == "ц") {symbolDraw(23); return;}
if(substrSpecific == "ч") {symbolDraw(24); return;}
if(substrSpecific == "ш") {symbolDraw(25); return;}
if(substrSpecific == "щ") {symbolDraw(26); return;}
if(substrSpecific == "ъ") {symbolDraw(27); return;}
if(substrSpecific == "ы") {symbolDraw(28); return;}
if(substrSpecific == "ь") {symbolDraw(29); return;}
if(substrSpecific == "э") {symbolDraw(30); return;}
if(substrSpecific == "ю") {symbolDraw(31); return;}
if(substrSpecific == "я") {symbolDraw(32); return;}

if(substrSpecific == "a") {symbolDraw(35); return;}
if(substrSpecific == "b") {symbolDraw(36); return;}
if(substrSpecific == "c") {symbolDraw(37); return;}
if(substrSpecific == "d") {symbolDraw(38); return;}
if(substrSpecific == "e") {symbolDraw(39); return;}
if(substrSpecific == "f") {symbolDraw(40); return;}
if(substrSpecific == "g") {symbolDraw(41); return;}
if(substrSpecific == "h") {symbolDraw(42); return;}
if(substrSpecific == "i") {symbolDraw(43); return;}
if(substrSpecific == "j") {symbolDraw(44); return;}
if(substrSpecific == "k") {symbolDraw(45); return;}
if(substrSpecific == "l") {symbolDraw(46); return;}
if(substrSpecific == "m") {symbolDraw(47); return;}
if(substrSpecific == "n") {symbolDraw(48); return;}
if(substrSpecific == "o") {symbolDraw(49); return;}
if(substrSpecific == "p") {symbolDraw(50); return;}
if(substrSpecific == "q") {symbolDraw(51); return;}
if(substrSpecific == "r") {symbolDraw(52); return;}
if(substrSpecific == "s") {symbolDraw(53); return;}
if(substrSpecific == "t") {symbolDraw(54); return;}
if(substrSpecific == "u") {symbolDraw(55); return;}
if(substrSpecific == "v") {symbolDraw(56); return;}
if(substrSpecific == "w") {symbolDraw(57); return;}
if(substrSpecific == "x") {symbolDraw(58); return;}
if(substrSpecific == "y") {symbolDraw(59); return;}
if(substrSpecific == "z") {symbolDraw(60); return;}

if(substrSpecific == "А") {symbolDraw(63); return;}
if(substrSpecific == "Б") {symbolDraw(64); return;}
if(substrSpecific == "В") {symbolDraw(65); return;}
if(substrSpecific == "Г") {symbolDraw(66); return;}
if(substrSpecific == "Д") {symbolDraw(67); return;}
if(substrSpecific == "Е") {symbolDraw(68); return;}
if(substrSpecific == "Ё") {symbolDraw(69); return;}
if(substrSpecific == "Ж") {symbolDraw(70); return;}
if(substrSpecific == "З") {symbolDraw(71); return;}
if(substrSpecific == "И") {symbolDraw(72); return;}
if(substrSpecific == "Й") {symbolDraw(73); return;}
if(substrSpecific == "К") {symbolDraw(74); return;}
if(substrSpecific == "Л") {symbolDraw(75); return;}
if(substrSpecific == "М") {symbolDraw(76); return;}
if(substrSpecific == "Н") {symbolDraw(77); return;}
if(substrSpecific == "О") {symbolDraw(78); return;}
if(substrSpecific == "П") {symbolDraw(79); return;}
if(substrSpecific == "Р") {symbolDraw(80); return;}
if(substrSpecific == "С") {symbolDraw(81); return;}
if(substrSpecific == "Т") {symbolDraw(82); return;}
if(substrSpecific == "У") {symbolDraw(83); return;}
if(substrSpecific == "Ф") {symbolDraw(84); return;}
if(substrSpecific == "Х") {symbolDraw(85); return;}
if(substrSpecific == "Ц") {symbolDraw(86); return;}
if(substrSpecific == "Ч") {symbolDraw(87); return;}
if(substrSpecific == "Ш") {symbolDraw(88); return;}
if(substrSpecific == "Щ") {symbolDraw(89); return;}
if(substrSpecific == "Ъ") {symbolDraw(90); return;}
if(substrSpecific == "Ы") {symbolDraw(91); return;}
if(substrSpecific == "Ь") {symbolDraw(92); return;}
if(substrSpecific == "Э") {symbolDraw(93); return;}
if(substrSpecific == "Ю") {symbolDraw(94); return;}
if(substrSpecific == "Я") {symbolDraw(95); return;}

if(substrSpecific == "A") {symbolDraw(98); return;}
if(substrSpecific == "B") {symbolDraw(99); return;}
if(substrSpecific == "C") {symbolDraw(100); return;}
if(substrSpecific == "D") {symbolDraw(101); return;}
if(substrSpecific == "E") {symbolDraw(102); return;}
if(substrSpecific == "F") {symbolDraw(103); return;}
if(substrSpecific == "G") {symbolDraw(104); return;}
if(substrSpecific == "H") {symbolDraw(105); return;}
if(substrSpecific == "I") {symbolDraw(106); return;}
if(substrSpecific == "J") {symbolDraw(107); return;}
if(substrSpecific == "K") {symbolDraw(108); return;}
if(substrSpecific == "L") {symbolDraw(109); return;}
if(substrSpecific == "M") {symbolDraw(110); return;}
if(substrSpecific == "N") {symbolDraw(111); return;}
if(substrSpecific == "O") {symbolDraw(112); return;}
if(substrSpecific == "P") {symbolDraw(113); return;}
if(substrSpecific == "Q") {symbolDraw(114); return;}
if(substrSpecific == "R") {symbolDraw(115); return;}
if(substrSpecific == "S") {symbolDraw(116); return;}
if(substrSpecific == "T") {symbolDraw(117); return;}
if(substrSpecific == "U") {symbolDraw(118); return;}
if(substrSpecific == "V") {symbolDraw(119); return;}
if(substrSpecific == "W") {symbolDraw(120); return;}
if(substrSpecific == "X") {symbolDraw(121); return;}
if(substrSpecific == "Y") {symbolDraw(122); return;}
if(substrSpecific == "Z") {symbolDraw(123); return;}

if(substrSpecific == "1") {symbolDraw(126); return;}
if(substrSpecific == "2") {symbolDraw(127); return;}
if(substrSpecific == "3") {symbolDraw(128); return;}
if(substrSpecific == "4") {symbolDraw(129); return;}
if(substrSpecific == "5") {symbolDraw(130); return;}
if(substrSpecific == "6") {symbolDraw(131); return;}
if(substrSpecific == "7") {symbolDraw(132); return;}
if(substrSpecific == "8") {symbolDraw(133); return;}
if(substrSpecific == "9") {symbolDraw(134); return;}
if(substrSpecific == "0") {symbolDraw(135); return;}

if(substrSpecific == ".") {symbolDraw(138); return;}
if(substrSpecific == ":") {symbolDraw(139); return;}
if(substrSpecific == ",") {symbolDraw(140); return;}
if(substrSpecific == ";") {symbolDraw(141); return;}
if(substrSpecific == " ") {symbolDraw(142); return;}
if(substrSpecific == "'") {symbolDraw(143); return;}
if(substrSpecific == "\42") {symbolDraw(144); return;}// "
if(substrSpecific == "(") {symbolDraw(145); return;}
if(substrSpecific == "!") {symbolDraw(146); return;}
if(substrSpecific == "?") {symbolDraw(147); return;}
if(substrSpecific == ")") {symbolDraw(148); return;}
if(substrSpecific == "+") {symbolDraw(149); return;}
if(substrSpecific == "-") {symbolDraw(150); return;}
if(substrSpecific == "*") {symbolDraw(151); return;}
if(substrSpecific == "/") {symbolDraw(152); return;}
if(substrSpecific == "=") {symbolDraw(153); return;}
if(substrSpecific == "@") {symbolDraw(154); return;}
if(substrSpecific == "#") {symbolDraw(155); return;}
if(substrSpecific == "|") {symbolDraw(156); return;}
if(substrSpecific == "<") {symbolDraw(157); return;}
if(substrSpecific == ">") {symbolDraw(158); return;}
if(substrSpecific == "[") {symbolDraw(159); return;}
if(substrSpecific == "]") {symbolDraw(160); return;}
if(substrSpecific == "{") {symbolDraw(161); return;}
if(substrSpecific == "}") {symbolDraw(162); return;}
if(substrSpecific == "&") {symbolDraw(163); return;}
if(substrSpecific == "%") {symbolDraw(164); return;}
if(substrSpecific == "\134") {symbolDraw(165); return;}
if(substrSpecific == "_") {symbolDraw(173); return;}
if(substrSpecific == "`") {symbolDraw(174); return;}
//166 t* | 167 LEFT | 168 RIGHT | 169 UP | 170 DOWN | 171 SMART_HOME | 172 HUMIDITY

return;
}


void ads_config(){
  ads.setGain(ADS1115_REG_CONFIG_PGA_4_096V);       // GAIN_ONE and resolution to ± 4.096V
  ads.setSampleRate(ADS1115_REG_CONFIG_DR_250SPS);  // Samples per sec//8 16 32 64 128 250 475 860
}


int16_t ads_read0(){ 
  ads.setMux(ADS1115_REG_CONFIG_MUX_SINGLE_0);  // Single mode input on AIN0 (AIN0 - GND)
  ads.triggerConversion();                      // Triggered mannually 
  return ads.getConversion();                   // returns int16_t value
}


int16_t ads_read1(){ 
  ads.setMux(ADS1115_REG_CONFIG_MUX_SINGLE_1);  // Single mode input on AIN1 (AIN0 - GND)
  ads.triggerConversion();                      // Triggered mannually 
  return ads.getConversion();                   // returns int16_t value
}


int16_t ads_read2(){ 
  ads.setMux(ADS1115_REG_CONFIG_MUX_SINGLE_2);  // Single mode input on AIN2 (AIN0 - GND)
  ads.triggerConversion();                      // Triggered mannually 
  return ads.getConversion();                   // returns int16_t value
}


int16_t ads_read3(){ 
  ads.setMux(ADS1115_REG_CONFIG_MUX_SINGLE_3);  // Single mode input on AIN3 (AIN0 - GND)
  ads.triggerConversion();                      // Triggered mannually 
  return ads.getConversion();                   // returns int16_t value
} 






