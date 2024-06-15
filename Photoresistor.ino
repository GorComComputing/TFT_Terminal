void PHOTORESISTOR_PROG() {
  numButton0 = -1; 
  numButton2 = -1; 
  delay(200);
  tft.fillScreen(BackColor);
  tft.drawBitmap(0, 0, PHOTORESISTOR_BIG_PIC, 101, 83, WHITE);
  //tft.drawFastHLine(0,86, 240, color565(255,255,255)); //CHART AREA TOP
  //tft.drawFastHLine(0,188,240, color565(255,255,255)); //CHART AREA BOTTOM

//  tft.setFreeFont(FM12); //OPTIONAL FONT //THE FONT FILE MUST BE IN THE FOLDER
  //tft.setTextWrap(true);
  tft.setTextSize(1);
  tft.setCursor(0, 212);   
  tft.setTextColor(WHITE); 
  tft.print("GND");
  tft.setCursor(198, 212); 
  tft.setTextColor(WHITE); 
  tft.print("3V3");
  tft.drawFastHLine(42, 206, 17, color565(255, 255, 255)); //GND-
  tft.drawRect(59, 198, 32, 17, color565(255, 255, 255)); //RESISTOR
  tft.drawFastHLine(91, 206, 55, color565(255, 255, 255)); //--
  tft.drawBitmap(146, 193, PHOTORESISTOR_PIC, 32, 26, WHITE);
  tft.drawFastHLine(178, 206, 20, color565(255, 255, 255)); //-3V3 
  tft.drawCircle(120, 206, 2, color565(255, 255, 255));    // DOT
  tft.drawFastVLine(120, 206, 13, color565(255, 255, 255)); // |

  //startX = 204; startY = 15; scaleX = 2; scaleY = 2; MyText = "%"; event();
  int Raw = 0, relRaw = 0, RawPrev = 0, relRawPrev = 0, Raw0 = 0, istab = 0; 
  double timDraw = millis(); 
  int memo[241]; 
  int r = 0; 
  int vShift = 177; 
  int next = 0;//
  for (int q = 0; q < 241; q++) {
    memo[q]   = -1;
  }
  //charColo = TFT_WHITE; scaleX = 2; scaleY = 2;
  tft.setTextSize(1); 
//  tft.setFreeFont(FM24);
  tft.setCursor(206, 52); 
  tft.setTextColor(WHITE); 
  tft.print("%"); //%
  PressedButton(0);
  while(numButton0 != 14 && numButton2 != 1) { 
    PressedButton(0); 
    if(numButton2 == 8) {return;} 
    istab += 1;
    Raw0 = Raw0 + analogRead(2); 
    if(istab > 4) { 
      next = 1;//> 7
      Raw = Raw0/(istab-0);
      relRaw = (Raw - 0)/40;
      if(relRaw > 100) {relRaw = 100;} 
      if(relRaw < 0) {relRaw = 0;} // %
      if(relRaw != relRawPrev) {
        //scaleX = 2; scaleY = 2; startX = 110; startY = 17;  MyText = relRawPrev; charColo = BackColor; event(); 
        //tft.setCursor(125, 52);  tft.setTextColor(BackColor); tft.print(relRawPrev); 
        tft.setTextColor(BackColor, BackColor); 
    //    tft.drawNumber(relRawPrev, 125, 22, 6);
        //scaleX = 2; scaleY = 2; startX = 110; startY = 17;  MyText = relRaw;     charColo = TFT_WHITE;     event(); relRawPrev = relRaw;} 
        //tft.setCursor(125, 52);  
        tft.setTextColor(WHITE, BackColor); 
    //    tft.drawNumber(relRaw, 125, 22, 6); 
        relRawPrev = relRaw;
      }    //tft.print(relRaw); relRawPrev = relRaw;} //%  
      if(Raw != RawPrev) {
        //scaleX = 2; scaleY = 1; startX = 70;  startY = 215; MyText = RawPrev;    charColo = BackColor; event();  
        //tft.setCursor(66, 234); tft.setTextColor(BackColor);  tft.print(RawPrev);
        tft.setTextColor(BackColor, BackColor); 
    //    tft.drawNumber(RawPrev, 95, 220, 4);     
        //scaleX = 2; scaleY = 1; startX = 70;  startY = 215; MyText = Raw;        charColo = TFT_WHITE;     event(); RawPrev = Raw;} 
        //tft.setCursor(66, 234);  
        tft.setTextColor(WHITE, BackColor); 
    //    tft.drawNumber(Raw, 95, 220, 4); 
        RawPrev = Raw;
      }//      tft.print(Raw);    RawPrev = Raw;}       //RAW 
      istab = 0; Raw0 = 0;       
    }

    if((millis() - timDraw) > 10 && next > 0) {
      if(r < -19) {r = 0;}
      for (int z=r; z < 240; z+= 20) {
        tft.fillRect(z, 85, 2, 2, BackColor); 
        tft.fillRect(z, 188, 2, 2, BackColor); 
      } 
      r -= 1; 
      for (int z = r; z < 240; z += 20) {
        tft.fillRect(z, 85, 2, 2, WHITE); 
        tft.fillRect(z, 188, 2, 2, WHITE);
      } //DOTS                                      
                                                
      for (int x = 0; x < 240; x++) {          
        tft.drawFastVLine(x, vShift - memo[x], 10, BackColor);
        //tft.fillRect(x,vShift - memo[x], 1,10, BackColor); //BackColor
        if(x != 239) {
          memo[x] = memo[x+1];
          if(memo[x] != -1) {
            tft.drawFastVLine(x, vShift - memo[x], 10, color565(255, 255, 80));
          }
        }
        //tft.fillRect(x,vShift - memo[x], 1,10, 
        if(x == 239) {
          memo[x] = analogRead(2)/44; 
          tft.drawFastVLine(x, vShift - memo[x], 10, color565(255, 255, 80));
        }
        //tft.fillRect(x,vShift - memo[x], 1,10,  
      } 
      timDraw = millis();
    }   
  } // END WHILE
}