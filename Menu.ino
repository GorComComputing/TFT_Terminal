void startMenu() {       
  tft.fillScreen(BackColor);  
  int numMenu = 1; 
  startY = 0; 
  InMenu = 0; 
  rectMenuX = 0; 
  rectMenuY = 0; 
  scaleX = 1; 
  scaleY = 1; 
  //charColo = WHITE;
  charColo = color565(255, 255, 85);
  while(numMenu <= 11) {
    drawMenu(numMenu); 
    numMenu ++; 
    startY += 21;
  }
  rectMenuY = 0; 
  startY = rectMenuY; 
  selectMenuP = 1;
  if(WiFi.status() == WL_CONNECTED) {
    tft.drawBitmap(214, 0,  NET_PIC, 25, 21, color565(255, 255, 85));
  }
  tft.fillRect(rectMenuX, rectMenuY + 1, 17, 22, color565(0, 170, 170));  //CURSOR //FIRST [MENU]   
}


void drawMenu(int numMenu) {
  if(numMenu == 1)  {startX = 17; MyText = "Wi-Fi"; event();}
  if(numMenu == 2)  {startX = 17; MyText = "COFFEE "; event(); scaleX = 2; symbolDraw(171); scaleX = 1;}
  if(numMenu == 3)  {startX = 17; MyText = "PICO "; event(); scaleX = 2; symbolDraw(171); scaleX = 1;}
  if(numMenu == 4)  {startX = 17; MyText = "UART"; event();}
  if(numMenu == 5)  {startX = 17; MyText = "Заметка"; event();}
  //if(numMenu == 4)  {startX = 17; MyText = "ЧАСЫ "; event(); scaleX = 2; symbolDraw(166); scaleX = 1; MyText =" HUMIDITY"; event();}
  if(numMenu == 6)  {startX = 17; MyText = "Фоторезистор"; event(); tft.drawBitmap(195, 88, PHOTORESISTOR_PIC, 32, 26, charColo);} //Игры /// games //charColo = inactElem; charColo = TFT_WHITE; tft.drawBitmap(195, 205,  PHOTORESISTOR_PIC, 32, 26, TFT_WHITE);
  //if(numMenu == 6)  {startX = 17; MyText = "BLUETOOTH app-s"; event();}
  //if(numMenu == 6)  {startX = 17; MyText = "RGB тест цвета"; event();}
  if(numMenu == 7)  {startX = 17; MyText = "Погода"; event();}
  if(numMenu == 8)  {startX = 17; MyText = "Вольтметр [4 ch]"; event();}
  if(numMenu == 9) {startX = 17; MyText = "Настройки"; event();}
  //if(numMenu == 11) {startX = 17; MyText = "МОЙ УМНЫЙ ДОМ "; charColo = inactElem; event(); scaleX = 2; symbolDraw(171); scaleX = 1; charColo = WHITE;}
}


void moveCursor() {
  if((numButton0 == 10 || numButton2 == 4) && millis() - timePast > 100 && selectMenuP < 11) {//NEXT MENU AND DELAY
    timePast = millis() - 1;
    tft.fillRect(rectMenuX, rectMenuY + 1, 17, 22, BackColor);
    rectMenuY += 21; 
    selectMenuP++;
    if(selectMenuP == 8) {
      tft.fillRect(rectMenuX, rectMenuY + 1, 5, 22, color565(255,0,0));//RGB MENU_P4  
      tft.fillRect(rectMenuX + 5, rectMenuY + 1, 6, 22, color565(0,255,0));
      tft.fillRect(rectMenuX + 11, rectMenuY + 1, 6, 22, color565(0,0,255));
    } else {
      tft.fillRect(rectMenuX, rectMenuY + 1, 17, 22, color565(0, 170, 170));
    }
  }
  if((numButton0 == 9 || numButton2 == 3) && millis() - timePast > 100 && selectMenuP > 1) {//PREVIOUS MENU AND DELAY 
    timePast = millis() - 1;
    tft.fillRect(rectMenuX, rectMenuY + 1, 17, 22, BackColor);
    rectMenuY -= 21; 
    selectMenuP--;     
    if(selectMenuP == 8) {
      tft.fillRect(rectMenuX, rectMenuY + 1, 5, 22, color565(255,0,0));//RGB MENU_P4  
      tft.fillRect(rectMenuX + 5,rectMenuY + 1, 6, 22, color565(0,255,0));
      tft.fillRect(rectMenuX + 11, rectMenuY + 1, 6, 22, color565(0,0,255));
    } else {
      tft.fillRect(rectMenuX, rectMenuY + 1, 17, 22, color565(0, 170, 170));
    }
  }
}


void chooseMenu() {
  if(selectMenuP == 1)  {InMenu = 1; WIFI_CONFIG();}
  if(selectMenuP == 2)  {InMenu = 1; COFFEE();}
  if(selectMenuP == 3)  {InMenu = 1; PICO();}
  if(selectMenuP == 4)  {InMenu = 1; UART_CHAT(); /*CHATS();*/}
  if(selectMenuP == 5)  {InMenu = 1; NOTE(1);}
  //if(selectMenuP == 3)  {InMenu = 1; /*INFORMER();*/}
  if(selectMenuP == 6)  {InMenu = 1; PHOTORESISTOR_PROG();} //GAMES //InMenu = 1; RAFT();
  //if(selectMenuP == 5)  {InMenu = 1; /*BLUETOOTHAPPS();*/}
  //if(selectMenuP == 7)  {InMenu = 1; /*MENURGB();*/}
  if(selectMenuP == 7)  {InMenu = 1; WEATHER();}
  if(selectMenuP == 8)  {InMenu = 1; VOLTMETER();}
  if(selectMenuP == 9) {InMenu = 1; SETTINGS();}
  //if(selectMenuP == 11) {InMenu = 1; /*SMART_HOME();*/} 
  //if(selectMenuP == 12) {} 
}