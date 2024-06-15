void PICO(){ 
  bool isRed;
  bool isYellow;
  bool isGreen;
  bool isLed;
  bool isLaser;

  if(WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverDOORS);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "cmd=curl_get%20http://192.168.0.14/get?cmd=state";
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
        const char* Yellow = parsed["Yellow"];
        const char* Green = parsed["Green"];
        const char* Led = parsed["Led"];
        const char* Laser = parsed["Laser"];
        const char* Servo = parsed["Servo"];
        const char* TempIn = parsed["TempIn"];
        const char* Hum = parsed["Hum"];
        const char* Light = parsed["Light"];
        isRed = strcmp(Red, "ON") ? false : true;
        isYellow = strcmp(Yellow, "ON") ? false : true;
        isGreen = strcmp(Green, "ON") ? false : true;
        isLed = strcmp(Led, "ON") ? false : true;
        isLaser = strcmp(Laser, "ON") ? false : true;


        delay(100);
        tft.fillScreen(BackColor); 
        startX = 19; 
        startY = 2;  
        MyText = isRed ? "Red ON" : "Red OFF"; 
        event();
        startX = 19; 
        startY = 24; 
        MyText = isYellow ? "Yellow ON" : "Yellow OFF"; 
        event();
        startX = 19; 
        startY = 46; 
        MyText = isGreen ? "Green ON" : "Green OFF";
        event();
        startX = 19; 
        startY = 68; 
        MyText = isLed ? "Led ON" : "Led OFF"; 
        event();
        startX = 19; 
        startY = 92; 
        MyText = isLaser ? "Laser ON" : "Laser OFF"; 
        event();
        startX = 19; 
        startY = 116; 
        MyText = "Servo = " + String(Servo); 
        event();
        startX = 19; 
        startY = 140; 
        MyText = "TempIn = " + String(TempIn); 
        event();
        startX = 19; 
        startY = 164; 
        MyText = "Hum = " + String(Hum); 
        event();
        startX = 19; 
        startY = 188; 
        MyText = "Light = " + String(Light); 
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
              String httpRequestData = "cmd=curl_get%20http://192.168.0.14/get?cmd=red%20" + cmd;
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
        String cmd = isYellow ? "off" : "on";
        String httpRequestData = "cmd=curl_get%20http://192.168.0.14/get?cmd=yellow%20" + cmd;
        int httpResponseCode = http.POST(httpRequestData);
        if (httpResponseCode > 0) {
          String payload = http.getString();
          isYellow = !isYellow;
          MyText = isYellow ? "Yellow ON" : "Yellow OFF"; 
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
    } // YELLOW
    if(numButton2 == 1 && rectMenuY == 44) {
      if(WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverDOORS);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String cmd = isGreen ? "off" : "on";
        String httpRequestData = "cmd=curl_get%20http://192.168.0.14/get?cmd=green%20" + cmd;
        int httpResponseCode = http.POST(httpRequestData);
        if (httpResponseCode > 0) {
          String payload = http.getString();
          isGreen = !isGreen;
          MyText = isGreen ? "Green ON" : "Green OFF"; 
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
    }// GREEN
    if(numButton2 == 1 && rectMenuY == 66) {
      if(WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverDOORS);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String cmd = isLed ? "off" : "on";
        String httpRequestData = "cmd=curl_get%20http://192.168.0.14/get?cmd=led%20" + cmd;
        int httpResponseCode = http.POST(httpRequestData);
        if (httpResponseCode > 0) {
          String payload = http.getString();
          isLed = !isLed;
          MyText = isLed ? "Led ON" : "Led OFF";  
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
    }// LED
    if(numButton2 == 1 && rectMenuY == 88) {
      if(WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverDOORS);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String cmd = isLaser ? "off" : "on";
        String httpRequestData = "cmd=curl_get%20http://192.168.0.14/get?cmd=laser%20" + cmd;
        int httpResponseCode = http.POST(httpRequestData);
        if (httpResponseCode > 0) {
          String payload = http.getString();
          isLaser = !isLaser;
          MyText = isLaser ? "Laser ON" : "Laser OFF"; 
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
    }// LASER                                              
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