void WEATHER() {
  numButton0 = -1; 
  numButton2 = -1; 
  delay(100);
  tft.fillScreen(BackColor); 
  startX = 0; startY = 212;  MyText = "openweathermap.org"; charColo = color565(180, 180, 250); event(); charColo = WHITE;     
  startX = 19; startY = 2;   MyText = "Получить погоду"; charColo = WHITE; event(); charColo = WHITE;
  startX = 19; startY = 24;  MyText = "Город"; charColo = WHITE; event(); charColo = WHITE;
  startX = 19; startY = 46;  MyText = "Страна"; charColo = WHITE; event(); charColo = WHITE;
  startX = 19; startY = 68;  MyText = "API key"; charColo = WHITE; event(); charColo = WHITE;
     
  rectMenuX = 0; 
  rectMenuY = 0;//CURSOR
  tft.fillRect(rectMenuX, rectMenuY + 2, 17, 22, color565(255, 255, 255));//FIRST 
  if(WiFi.status() == WL_CONNECTED) {
    tft.drawBitmap(214, 0, NET_PIC, 25, 21, WHITE);
  }
  double pau = millis();
  PressedButton(0);                   
  while(numButton0 != 14) { 
    PressedButton(0); 
    if(numButton2 == 8) {return;}
    if((numButton0 == 10 || numButton2 == 4) && rectMenuY < 65 && millis() - pau > 100) {
      tft.fillRect(0, 0, 17, 94, BackColor); //TO BACKGROUND COLOR 4 ROWS
      rectMenuY += 22; 
      tft.fillRect(rectMenuX, rectMenuY + 2, 17, 22, color565(255, 255, 255));
      pau = millis();
    }//NEXT    
    if((numButton0 == 9 || numButton2 == 3) && rectMenuY > 10 && millis() - pau > 100) {
      tft.fillRect(0, 0, 17, 94, BackColor); //TO BACKGROUND COLOR 4 ROWS
      rectMenuY -= 22; 
      tft.fillRect(rectMenuX, rectMenuY + 2, 17, 22, color565(255, 255, 255));
      pau = millis();
    }//PREV 

    //IF ENTER KEY PRESSED
    if(numButton2 == 1 && rectMenuY == 0)   {numButton0 = -1; numButton2 = -1; delay(100); GET_WEATHER(); if(numButton2 == 8) {return;} PressedButton(0); WEATHER(); PressedButton(0); break;}
    if(numButton2 == 1 && rectMenuY == 22)  {numButton0 = -1; numButton2 = -1; delay(100); NOTE(5); if(numButton2 == 8) {return;} PressedButton(0); WEATHER(); PressedButton(0); break;} //APICITY
    if(numButton2 == 1 && rectMenuY == 44)  {numButton0 = -1; numButton2 = -1; delay(100); NOTE(6); if(numButton2 == 8) {return;} PressedButton(0); WEATHER(); PressedButton(0); break;} //APICOUNTRY
    if(numButton2 == 1 && rectMenuY == 66)  {numButton0 = -1; numButton2 = -1; delay(100); NOTE(7); if(numButton2 == 8) {return;} PressedButton(0); WEATHER(); PressedButton(0); break;} //APIKEY                                                                
  }//END WHILE 
  InMenu = 0;   
}


void GET_WEATHER() { 
  tft.fillScreen(BackColor); //Serial.begin(9600);
  startX = 73; 
  startY = 110; 
  MyText = "WAIT..."; 
  event();
  apiC =       EEPROM.readString(530);
  apiCountry = EEPROM.readString(590);
  apiKey =     EEPROM.readString(650);
  String apiW = "http://api.openweathermap.org/data/2.5/weather?q=" + apiC + "," + apiCountry + ",pt&APPID="; 
  HTTPClient http;
  http.begin(apiW + apiKey);
  int httpCode = http.GET();  
  if (httpCode > 0) {
    String payload = http.getString();
    //Serial.println(httpCode);
    //Serial.println(payload);
    handleReceivedMessage(payload);
    http.end();                                  
  }
}


void handleReceivedMessage(String message) {
  StaticJsonDocument<1500> doc;   
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    //Serial.print(F("deserializeJson() failed: "));
    //Serial.println(error.c_str());
    //return;
  }
  const char* name = doc["name"];
  ////////////////////////
  if(name != "") {tft.fillScreen(BackColor);} //ERASE "WAIT..."
  ////////////////////////
  startX = 0; startY = 0; MyText = " "; event(); MyText = name; event();
    
  float tempK = doc["main"]["temp"];                   
  float tempC = tempK - 273.15;   
  startX = 0; startY = 48; MyText = "Темп-а: "; event(); MyText = tempC; event(); MyText = " C"; event();

  float tempKFeel = doc["main"]["feels_like"];                   
  float tempCFeel = tempKFeel - 273.15;   
  startX = 0; startY = 72; MyText = "По ощ-ям: "; event(); MyText = tempCFeel; event(); MyText = " C"; event();

  int humidity = doc["main"]["humidity"];
  startX = 0; startY = 96; MyText = "Влаж-ть: "; event(); MyText = humidity; event(); MyText = " %"; event();

  String weather = doc["clouds"]["all"]; 
  startX = 0; startY = 120; MyText = "Облач-ть: "; event(); MyText = weather; event(); MyText = " %"; event();;

  int pressurehPa = doc["main"]["pressure"]; 
  float pressure = pressurehPa/1.333;
  startX = 0; startY = 144; MyText = "Дав-е: "; event(); MyText = pressure; event(); MyText = " mmHc"; event();

  float windspeed = doc["wind"]["speed"]; 
  startX = 0; startY = 168; MyText = "Ветер: "; event(); MyText = windspeed; event(); MyText = " m/s"; event();
      
//  int timezone = doc["timezone"];
//  startX = 0; startY = 26; MyText = "Timezone: "; event(); MyText = timezone; event();
    
//  int winddeg = doc["wind"]["deg"]; 

  PressedButton(0);
  while(numButton0 != 14 && numButton2 != 1) {
    PressedButton(0); 
    if(numButton2 == 8) {return;}
  }
}