void WIFI_CONFIG(){ 
  delay(100);
  tft.fillScreen(BackColor); 
  startX = 19; 
  startY = 2;  
  MyText = "Connect"; 
  event();
  startX = 19; 
  startY = 24; 
  MyText = "Disconnect"; 
  event();
  startX = 19; 
  startY = 46; 
  MyText = "SSID"; 
  event();
  startX = 19; 
  startY = 68; 
  MyText = "Password"; 
  event();
  rectMenuX = 0; 
  rectMenuY = 0;//CURSOR
  tft.fillRect(rectMenuX, rectMenuY + 3, 17, 22, color565(255, 255, 255));//FIRST 
  if(WiFi.status() == WL_CONNECTED) {
    tft.drawBitmap(214, 0, NET_PIC, 25, 21, WHITE);
  }
  double pau = millis();
  PressedButton(0);
  while(numButton0 != 14) { 
    PressedButton(0); 
    if(numButton2 == 8) {return;}
    if((numButton0 == 10 || numButton2 == 4) && rectMenuY < 62 && millis() - pau > 100) {
      tft.fillRect(0, 0, 17, 92, BackColor); //TO BACKGROUND COLOR 3 ROWS
      rectMenuY += 22; 
      tft.fillRect(rectMenuX, rectMenuY + 3, 17, 22, color565(255,255,255));
      pau = millis();
    }//NEXT    
    if((numButton0 == 9 || numButton2 == 3) && rectMenuY > 10 && millis() - pau > 100) {
      tft.fillRect(0, 0, 17, 92, BackColor); //TO BACKGROUND COLOR 3 ROWS
      rectMenuY -= 22; tft.fillRect(rectMenuX, rectMenuY + 3, 17, 22, color565(255,255,255));
      pau = millis();
    }//PREV 
    //IF ENTER KEY PRESSED
    if(numButton2 == 1 && rectMenuY == 0)  {
      CONNECT_WIFI(0); 
      if(numButton2 == 8) {return;} 
      WIFI_CONFIG(); 
      PressedButton(0); 
      break;
    }//GO TO CONNECT_WIFI()
    if(numButton2 == 1 && rectMenuY == 22) {
      DISCONNECT_WIFI(); 
      if(numButton2 == 8) {return;} 
      WIFI_CONFIG(); 
      PressedButton(0); 
      break;
    }
    if(numButton2 == 1 && rectMenuY == 44) {
      numButton0 = -1; 
      numButton2 = -1; 
      delay(100); 
      NOTE(2); 
      MyText = ""; 
      if(numButton2 == 8) {return;} 
      WIFI_CONFIG(); 
      PressedButton(0); 
      break;
    }//TO EDIT SSID//HERE AFTER NOTE()
    if(numButton2 == 1 && rectMenuY == 66) {
      numButton0 = -1; 
      numButton2 = -1; 
      delay(100); 
      NOTE(3); 
      MyText = ""; 
      if(numButton2 == 8) {return;} 
      WIFI_CONFIG(); 
      PressedButton(0); 
      break;
    }//TO EDIT PASSWORD//HERE AFTER NOTE()                                                
  }//END WHILE 
  InMenu = 0;
}


void CONNECT_WIFI(int start_device) {
  numButton0 = -1; 
  numButton2 = -1; 
  delay(100);
  tft.fillScreen(BackColor); 
  String stringSSID = EEPROM.readString(400); //EEPROM.readString(400).substring(1, EEPROM.readString(400).length()-1);
  String stringPASS = EEPROM.readString(450);          
  startX = 2; 
  startY = 2;  
  MyText = "Connecting to "; 
  event(); 
  startX = 2; 
  startY = 24; 
  MyText = stringSSID; 
  event();  

  const char* ssid = stringSSID.c_str();//"";
  const char* password = stringPASS.c_str();//""; 
        
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  int dots = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    if(dots < 7) {
      MyText =  "."; 
      event();
    } 
    dots++; 
    if(dots > 4) {break;}
  }
  if(WiFi.status() == WL_CONNECTED) {                                       
    startX = 2; 
    startY = 46; 
    MyText = "CONNECTED"; 
    event();
    startX = 2; 
    startY = 68;
    MyText = "[WiFi] IP address: "; 
    event();
    startX = 2; 
    startY = 90;
    MyText = WiFi.localIP().toString(); 
    event();
    tft.drawBitmap(214, 0, NET_PIC, 25, 21, WHITE);
  } else {
    MyText = "network not available "; 
    event(); 
    WiFi.disconnect();
  }

  PressedButton(0);
  if(start_device == 0) {
    while(numButton0 != 14 && numButton2 != 1) {
      PressedButton(0); 
      if(numButton2 == 8) {return;}
    }
  } //CHECK FOR ESC KEY 
  scaleX = 1; 
  scaleY = 1; 
  numButton0 = -1; 
  numButton2 = -1;//BEFORE EXIT
}


void DISCONNECT_WIFI() {
  numButton0 = -1; 
  numButton2 = -1; 
  delay(100);
  tft.fillScreen(BackColor);
  WiFi.disconnect(); 
  if(WiFi.status() != WL_CONNECTED) {
    MyText = "disconnect OK"; 
    scaleX = 1; 
    scaleY = 1; 
    startX = 32; 
    startY = 12; 
    event();
  } else {
    MyText = "please, try again"; 
    scaleX = 1; 
    scaleY = 1; 
    startX = 2; 
    startY = 105; 
    event();
  }
  PressedButton(0);
  while(numButton0 != 14 && numButton2 != 1) {
    PressedButton(0); 
    if(numButton2 == 8) {
      numButton0 = -1; 
      numButton2 = -1; 
      delay(100); 
      return;
    }
  } //CHECK FOR ESC KEY
  numButton0 = -1; 
  numButton2 = -1;
}