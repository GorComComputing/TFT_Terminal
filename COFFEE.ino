void COFFEE(){ 
  bool isRed;
  bool isPump;
  bool isTrack;
  bool isFun;
  bool isRelay;

  if(WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverDOORS);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "cmd=curl_get%20http://192.168.0.12/get?cmd=state";
    int httpResponseCode = http.POST(httpRequestData);
    if (httpResponseCode > 0) {
      MyText = http.getString();
      http.end();
      StaticJsonDocument<300> parsed;   //Пул памяти
      DeserializationError error = deserializeJson(parsed, MyText.c_str());
      if (error) {  // Проверьте, удастся ли выполнить синтаксический анализ
        MyText = "error JSON";
      } else {   //Вывести если ошибок нет
        const char* Red = parsed["Red"];
        const char* Pump = parsed["Pump"];
        const char* Track = parsed["Track"];
        const char* Fun = parsed["Fun"];
        const char* Relay = parsed["Relay"];
        const char* ServoX = parsed["ServoX"];
        const char* ServoY = parsed["ServoY"];
        isRed = strcmp(Red, "ON") ? false : true;
        isPump = strcmp(Pump, "ON") ? false : true;
        isTrack = strcmp(Track, "ON") ? false : true;
        isFun = strcmp(Fun, "ON") ? false : true;
        isRelay = strcmp(Relay, "ON") ? false : true;


  delay(100);
  tft.fillScreen(BackColor); 
  startX = 19; 
  startY = 2;  
  MyText = isRed ? "Red ON" : "Red OFF"; 
  event();
  startX = 19; 
  startY = 24; 
  MyText = isPump ? "Насос ON" : "Насос OFF"; 
  event();
  startX = 19; 
  startY = 46; 
  MyText = isTrack ? "Конвейер ON" : "Конвейер OFF";
  event();
  startX = 19; 
  startY = 68; 
  MyText = isFun ? "Вентилятор ON" : "Вентилятор OFF"; 
  event();
  startX = 19; 
  startY = 92; 
  MyText = isRelay ? "Гирлянда ON" : "Гирлянда OFF"; 
  event();
  startX = 19; 
  startY = 116; 
  MyText = "X = " + String(ServoX); 
  event();
  startX = 19; 
  startY = 140; 
  MyText = "Y = " + String(ServoY); 
  event();
  rectMenuX = 0; 
  rectMenuY = 0;//CURSOR
  tft.fillRect(rectMenuX, rectMenuY + 3, 17, 22, color565(255,255,255));//FIRST 
  if(WiFi.status() == WL_CONNECTED) {
    tft.drawBitmap(214, 0, NET_PIC, 25, 21, WHITE);
  }
  double pau = millis();
  PressedButton(0);
  while(numButton0 != 14) { 
    PressedButton(0); 
    if(numButton2 == 8) {return;}
    if((numButton0 == 10 || numButton2 == 4) && rectMenuY < 90 && millis() - pau > 100) {
      tft.fillRect(0, 0, 17, 240, BackColor); //TO BACKGROUND COLOR 3 ROWS
      rectMenuY += 22; 
      tft.fillRect(rectMenuX, rectMenuY + 3, 17, 22, color565(255,255,255));
      pau = millis();
    }//NEXT    
    if((numButton0 == 9 || numButton2 == 3) && rectMenuY > 10 && millis() - pau > 100) {
      tft.fillRect(0, 0, 17, 240, BackColor); //TO BACKGROUND COLOR 3 ROWS
      rectMenuY -= 22; tft.fillRect(rectMenuX, rectMenuY + 3, 17, 22, color565(255,255,255));
      pau = millis();
    }//PREV 

    //IF ENTER KEY PRESSED
    if(numButton2 == 1 && rectMenuY == 0) { 
      if(WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverDOORS);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String cmd = isRed ? "off" : "on";
        String httpRequestData = "cmd=curl_get%20http://192.168.0.12/get?cmd=red%20" + cmd;
        int httpResponseCode = http.POST(httpRequestData);
        if (httpResponseCode > 0) {
          String payload = http.getString();
          isRed = !isRed;
          MyText = isRed ? "Red ON" : "Red OFF"; 
        } else {
          MyText = "error"; 
        }
        http.end();
      } else {
        MyText = "WiFi disconnect"; 
      }
      startX = 19; 
      startY = 2;
      tft.fillRect(startX, startY, 220, 22, BackColor);  
      event();
      if(WiFi.status() == WL_CONNECTED) {
        tft.drawBitmap(214, 0, NET_PIC, 25, 21, WHITE);
      }
      delay(100);
    } // RED
    if(numButton2 == 1 && rectMenuY == 22) {
      if(WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverDOORS);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String cmd = isPump ? "off" : "on";
        String httpRequestData = "cmd=curl_get%20http://192.168.0.12/get?cmd=pump%20" + cmd;
        int httpResponseCode = http.POST(httpRequestData);
        if (httpResponseCode > 0) {
          String payload = http.getString();
          isPump = !isPump;
          MyText = isPump ? "Насос ON" : "Насос OFF"; 
        } else {
          MyText = "error"; 
        }
        http.end();
      } else {
        MyText = "WiFi disconnect"; 
      }
      startX = 19; 
      startY = 24;
      tft.fillRect(startX, startY, 220, 22, BackColor);   
      event();
      delay(100);
    } // PUMP
    if(numButton2 == 1 && rectMenuY == 44) {
      if(WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverDOORS);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String cmd = isTrack ? "off" : "on";
        String httpRequestData = "cmd=curl_get%20http://192.168.0.12/get?cmd=track%20" + cmd;
        int httpResponseCode = http.POST(httpRequestData);
        if (httpResponseCode > 0) {
          String payload = http.getString();
          isTrack = !isTrack;
          MyText = isTrack ? "Конвейер ON" : "Конвейер OFF"; 
        } else {
          MyText = "error"; 
        }
        http.end();
      } else {
        MyText = "WiFi disconnect"; 
      }
      startX = 19; 
      startY = 46; 
      tft.fillRect(startX, startY, 220, 22, BackColor);  
      event();
      delay(100);
    }// TRACK
    if(numButton2 == 1 && rectMenuY == 66) {
      if(WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverDOORS);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String cmd = isFun ? "off" : "on";
        String httpRequestData = "cmd=curl_get%20http://192.168.0.12/get?cmd=fun%20" + cmd;
        int httpResponseCode = http.POST(httpRequestData);
        if (httpResponseCode > 0) {
          String payload = http.getString();
          isFun = !isFun;
          MyText = isFun ? "Вентилятор ON" : "Вентилятор OFF";  
        } else {
          MyText = "error"; 
        }
        http.end();
      } else {
        MyText = "WiFi disconnect"; 
      }
      startX = 19; 
      startY = 68; 
      tft.fillRect(startX, startY, 220, 22, BackColor);  
      event();
      delay(100);
    }// FUN  
    if(numButton2 == 1 && rectMenuY == 88) {
      if(WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverDOORS);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String cmd = isRelay ? "off" : "on";
        String httpRequestData = "cmd=curl_get%20http://192.168.0.12/get?cmd=relay%20" + cmd;
        int httpResponseCode = http.POST(httpRequestData);
        if (httpResponseCode > 0) {
          String payload = http.getString();
          isRelay = !isRelay;
          MyText = isRelay ? "Гирлянда ON" : "Гирлянда OFF"; 
        } else {
          MyText = "error"; 
        }
        http.end();
      } else {
        MyText = "WiFi disconnect"; 
      }
      startX = 19; 
      startY = 90;
      tft.fillRect(startX, startY, 220, 22, BackColor);   
      event();
      delay(100);
    }// RELAY                                              
  }//END WHILE 
  //InMenu = 0;



      }
    } else {
      MyText = "error";
      delay(100);
      tft.fillScreen(BackColor);
      scaleX = 1; 
      scaleY = 1; 
      startX = 32; 
      startY = 12; 
      event();
      PressedButton(0);
      while(numButton0 != 14 && numButton2 != 1) {
        PressedButton(0); 
        if(numButton2 == 8) {
          numButton0 = -1; 
          numButton2 = -1; 
          delay(100);
          return;
        }
      } 
    }
    http.end();
  } else {
    MyText = "WiFi disconnect";
    delay(100);
    tft.fillScreen(BackColor);
    scaleX = 1; 
    scaleY = 1; 
    startX = 32; 
    startY = 12; 
    event();
    PressedButton(0);
    while(numButton0 != 14 && numButton2 != 1) {
      PressedButton(0); 
      if(numButton2 == 8) {
        numButton0 = -1; 
        numButton2 = -1; 
        delay(100);
        return;
      }
    }
  }
}