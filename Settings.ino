void SETTINGS() {
  tft.fillScreen(BackColor); 
  scaleX = 1; 
  scaleY = 1; 
  charColo = WHITE; 
  startX = 19; startY = 2;   MyText = "Яркость дисплея";  event();
  startX = 19; startY = 24;  MyText = "Яркость РУ/EN";    event();
  startX = 19; startY = 46;  MyText = "Цвет оформления";  event();
  startX = 19; startY = 68;  MyText = "Wi-Fi при вкл-ии"; event();
  startX = 19; startY = 90;  MyText = "Точность данных";  event();
  startX = 19; startY = 112; MyText = "TimeOffset";       event();
  startX = 19; startY = 134; MyText = "Режим сна";        charColo = inactElem; event(); charColo = WHITE;
   
  rectMenuX = 0; 
  rectMenuY = 0; //CURSOR
  tft.fillRect(rectMenuX, rectMenuY + 2, 17, 22, color565(255,255,255));//FIRST 
  double pau = millis();
  PressedButton(0);                   
  while(numButton0 != 14) { 
    PressedButton(0); 
    if(numButton2 == 8) {return;}  
    if((numButton0 == 10 || numButton2 == 4) && rectMenuY < 130 && millis() - pau > 100) {
      tft.fillRect(0, 0, 17, 158, BackColor); //TO BACKGROUND COLOR 7 ROWS
      rectMenuY += 22; 
      tft.fillRect(rectMenuX, rectMenuY + 2, 17, 22, color565(255, 255, 255));
      pau = millis();
    }//NEXT    
    if((numButton0 == 9 || numButton2 == 3) && rectMenuY > 10 &&  millis() - pau > 100) {
      tft.fillRect(0, 0, 17, 158, BackColor); //TO BACKGROUND COLOR 7 ROWS
      rectMenuY -= 22; 
      tft.fillRect(rectMenuX, rectMenuY + 2, 17, 22, color565(255, 255, 255));
      pau = millis();
    }//PREV 

    //IF ENTER KEY PRESSED
    if(numButton2 == 1 && rectMenuY == 0)   {BRIGHTNESS();        if(numButton2 == 8) {return;} SETTINGS(); PressedButton(0); break;}
    if(numButton2 == 1 && rectMenuY == 22)  {BRIGHTNESS_LAYOUT(); if(numButton2 == 8) {return;} SETTINGS(); PressedButton(0); break;}
    if(numButton2 == 1 && rectMenuY == 44)  {CHANGE_THEME();      if(numButton2 == 8) {return;} SETTINGS(); PressedButton(0); break;}
    if(numButton2 == 1 && rectMenuY == 66)  {WIFI_ON_START();     if(numButton2 == 8) {return;} SETTINGS(); PressedButton(0); break;}
    if(numButton2 == 1 && rectMenuY == 88)  {/*FINEDATA();*/          if(numButton2 == 8) {return;} SETTINGS(); PressedButton(0); break;}  
    if(numButton2 == 1 && rectMenuY == 110) {/*TIMEOFFSETLOC();*/     if(numButton2 == 8) {return;} SETTINGS(); PressedButton(0); break;} 
    if(numButton2 == 1 && rectMenuY == 132) {}              
  }//END WHILE 
  InMenu = 0;  
}


void CHANGE_THEME() {
  int colorR[] = {  0,190,190,230,  0,  0,180,  0};//{60, 180,0,  0,  0,  50, 90, 0  };
  int colorG[] = {180,  0,  0,  0,  0,  0,  0,  0};//{0,  0,  0,  0,  0,  0,  0,  80 };
  int colorB[] = {  0,190,130,  0,  0,250,250, 70};//{70, 250,0,  70, 200,20, 0,  0  };

  int flag = 100;
  int wi = 0;
  int he = 0;
  int inte = 0;
  MyText = "Текст";
  while(inte < 8) {
    tft.fillRect(wi,he,120,60,color565(colorR[inte],colorG[inte],colorB[inte])); //DRAW BACKGROUNDS 
    startX = wi + 27; 
    startY = he + 17; 
    event(); 
    wi += 120; 
    if(wi > 239) {
      wi = 0; 
      he += 60;
    }
    inte ++;
  }
  wi = 0; 
  he = 0; 
  inte = 0;    
  while(inte < 8) {
    tft.drawRect(wi,he,120,60,BLACK); //DRAW GRID
    startX = wi + 27; 
    startY = he + 17; 
    event(); 
    wi += 120; 
    if(wi > 239) {
      wi = 0; 
      he += 60;
    }
    inte ++;
  }        

  tft.drawRect(0, 0, 120, 60, WHITE);
  wi = 0; 
  he = 0; 
  inte = 0;
  double pau = millis();
  while(numButton0 != 14) { 
    PressedButton(0); 
    if(numButton2 == 8) {return;}
    if(inte < 7 && (numButton0 == 10 || numButton2 == 4) && millis() - pau > 100) {
      inte++; //NEXT RECT
      tft.drawRect(wi, he, 120, 60, BLACK);
      wi += 120; 
      if(wi > 239) {wi = 0; he += 60;}
      tft.drawRect(wi, he, 120, 60, WHITE);
      pau = millis();
    }
    if(inte > 0 && (numButton0 == 9 || numButton2 == 3) && millis() - pau > 100) {
      inte--; //PREVIOUS RECT
      tft.drawRect(wi, he, 120, 60, BLACK);
      wi -= 120; 
      if(wi < 0) {
        wi = 120; 
        he -= 60;
      }
      tft.drawRect(wi, he, 120, 60, WHITE);
      pau = millis();
    }
    if(numButton2 == 1) {//IF ENTER
      EEPROM.writeInt(500, flag); EEPROM.commit();//FLAG FOR NOT DEFAULT
      EEPROM.writeInt(505, colorR[inte]); EEPROM.commit();
      EEPROM.writeInt(506, colorG[inte]); EEPROM.commit();
      EEPROM.writeInt(507, colorB[inte]); EEPROM.commit();
      //BackColor = color565(colorR[inte],colorG[inte],colorB[inte]);

      if(EEPROM.readInt(500) == 100) {
        BackColor = color565(EEPROM.readInt(505),EEPROM.readInt(506),EEPROM.readInt(507));
      } //SET NEW THEME
      else {
        BackColor = color565(60,0,70);
      } //IF FAIL
      break;
    }
  }
}


void WIFI_ON_START() {
  numButton0 = -1; 
  numButton2 = -1; 
  delay(200);
  tft.fillScreen(BackColor); 
  startX = 19; 
  startY = 2;  
  MyText = "Автома-е подкл."; 
  event();
  startX = 19; 
  startY = 24; 
  MyText = "Без автоподкл."; 
  event();
  if(EEPROM.readInt(525) == 1) {
    startX = 223; 
    startY = 2;  
    symbolDraw(167);
  }
  if(EEPROM.readInt(525) == 0) {
    startX = 223; 
    startY = 24; 
    symbolDraw(167);
  }

  rectMenuX = 0; 
  rectMenuY = 0;//CURSOR
  tft.fillRect(rectMenuX, rectMenuY + 1, 17, 22, color565(255, 255, 255));//FIRST 
  double pau = millis();
  PressedButton(0);
  while(numButton0 != 14) { 
    PressedButton(0); 
    if(numButton2 == 8) {return;}
    if((numButton0 == 10 || numButton2 == 4) && rectMenuY < 20 && millis() - pau > 100) {
      tft.fillRect(0, 0, 17, 52, BackColor); //TO BACKGROUND COLOR 2 ROWS
      rectMenuY += 22; 
      tft.fillRect(rectMenuX, rectMenuY + 1, 17, 22, color565(255, 255, 255));
      pau = millis();
    }//NEXT    
    if((numButton0 == 9 || numButton2 == 3) && rectMenuY > 10 && millis() - pau > 100) {
      tft.fillRect(0, 0, 17, 52, BackColor); //TO BACKGROUND COLOR 2 ROWS
      rectMenuY -= 22; 
      tft.fillRect(rectMenuX, rectMenuY + 1, 17, 22, color565(255, 255, 255));
      pau = millis();
    }//PREV 
    if(numButton2 == 1) {//IF ENTER KEY PRESSED
      if(rectMenuY == 0)  {
        EEPROM.writeInt(525, 1); 
        EEPROM.commit();
        tft.fillRect(223, 0, 17, 52, BackColor);
        startX = 223; 
        startY = 2;  
        symbolDraw(167);
      }
      if(rectMenuY == 22) {
        EEPROM.writeInt(525, 0); 
        EEPROM.commit();
        tft.fillRect(223, 0, 17, 52, BackColor);
        startX = 223; 
        startY = 24; 
        symbolDraw(167);
      }                 
    }
  }//END WHILE   
}


void BRIGHTNESS() {
  numButton0 = -1; 
  numButton2 = -1; 
  delay(100);
  tft.fillRect(0, 0, 240, 60, BackColor);
  tft.fillRect(0, 60, 240, 60, color565(255, 255, 255));
  tft.fillRect(0, 120, 240, 60, color565(0, 0, 0));
  tft.fillRect(0, 180, 240, 60, color565(0, 255, 0));
  int BR0 = BRIG;
  //tft.setFreeFont(FM18);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.setTextWrap(true);
  tft.setCursor(94, 38); 
  tft.setTextColor(WHITE); 
  tft.print(BR0);
  PressedButton(0);//NECESSARY
  while(numButton0 != 14 && numButton2 != 1) { 
    PressedButton(0); 
    if(numButton2 == 8) {return;} 
    if((numButton2 == 4 || numButton0 == 9) && BRIG < 249) {
      BRIG += 5; 
      analogWrite(7, BRIG); //BL++ MAX 250
      tft.setCursor(94, 38); 
      tft.setTextColor(BackColor);  
      tft.print(BR0);
      BR0 = BRIG; 
      tft.setCursor(94, 38); 
      tft.setTextColor(WHITE); 
      tft.print(BR0); 
    }
    if((numButton2 == 3 || numButton0 == 10) && BRIG > 14)  {
      BRIG -= 5; 
      analogWrite(7, BRIG); //BL-- MIN 10
      tft.setCursor(94, 38); 
      tft.setTextColor(BackColor);  
      tft.print(BR0);
      BR0 = BRIG; 
      tft.setCursor(94, 38); 
      tft.setTextColor(WHITE); 
      tft.print(BR0);
    }                           
  } 
  EEPROM.writeInt(490, BRIG); 
  EEPROM.commit(); 
  //numButton0 = -1; numButton2 = -1; 
}


void BRIGHTNESS_LAYOUT() {
  numButton0 = -1; 
  numButton2 = -1; 
  delay(100);
  tft.fillScreen(BLACK);
  startX = 0;   
  startY = 69;  
  symbolDraw(167); //LEFT  ARROW
  startX = 226; 
  startY = 69;  
  symbolDraw(168); //RIGHT ARROW
  int BR0 = BRIG_LAYOUT;
  //tft.setFreeFont(FM18);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.setTextWrap(true);
  tft.setCursor(94, 86); 
  tft.setTextColor(WHITE); 
  tft.print(BR0);
  PressedButton(0);//NECESSARY
  while(numButton0 != 14 && numButton2 != 1) { 
    PressedButton(0); 
    if(numButton2 == 8) {return;} 
    if((numButton2 == 4 || numButton0 == 9) && BRIG_LAYOUT < 250) {
      BRIG_LAYOUT += 5; 
      analogWrite(3, BRIG_LAYOUT); //MAX 250
      analogWrite(10, BRIG_LAYOUT); //++
      tft.setCursor(94, 86); 
      tft.setTextColor(BLACK); 
      tft.print(BR0);
      BR0 = BRIG_LAYOUT; 
      tft.setCursor(94, 86); 
      tft.setTextColor(WHITE); 
      tft.print(BR0); 
    }
    if((numButton2 == 3 || numButton0 == 10) && BRIG_LAYOUT > 14)  {
      BRIG_LAYOUT -= 5; 
      analogWrite(3, BRIG_LAYOUT); //MIN 10
      analogWrite(10, BRIG_LAYOUT); //--
      tft.setCursor(94, 86); 
      tft.setTextColor(BLACK); 
      tft.print(BR0);
      BR0 = BRIG_LAYOUT; 
      tft.setCursor(94, 86); 
      tft.setTextColor(WHITE); 
      tft.print(BR0);
    }                           
  } 
  EEPROM.writeInt(494, BRIG_LAYOUT); 
  EEPROM.commit(); 
  analogWrite(3,  BRIG_LAYOUT); //RU
  analogWrite(10, BRIG_LAYOUT); //ENG 
}