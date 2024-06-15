void VOLTMETER(){
  tft.fillScreen(BLUE);
  //tft.setFreeFont(FM24);
  tft.setCursor(5, 35);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  //tft.setTextWrap(true);
  MyText = "< 3V3 т. к. после       R=200k";
  startX = 2; 
  startY = 150; 
  charColo = color565(0, 0, 70); 
  event(); 
  charColo = WHITE; //DRAW TEXT
  MyText = "Press any button  to check. Esc=exit";
  startX = 2; 
  startY = 196; 
  event(); //DRAW TEXT
  float Q0, Q1, Q2, Q3;
  int U0, U1, U2, U3;
  resolution = 4096.00/32752.00;//VOLT
  float maxU;
  PressedButton(0);
  while(numButton0 != 14) {
    PressedButton(0); 
    if(numButton2 == 8) {return;} //(0)TO CHECK ONLY 2 CIRCUIS FOR ESC
    
    U0 = ads_read0(); result0 = (U0*resolution)/1000.00;
    U1 = ads_read1(); result1 = (U1*resolution)/1000.00;
    U2 = ads_read2(); result2 = (U2*resolution)/1000.00;
    U3 = ads_read3(); result3 = (U3*resolution)/1000.00; 
    maxU = max(max(result0, result1), max(result2, result3)); 
    if(maxU - result0 < 0.1 && maxU - result1 < 0.1 && maxU - result2 < 0.1 && maxU - result3 < 0.1) {
      tft.fillRect(150, 0, 30, 148, BLUE);
    }   
                 
    if(abs(Q0 - result0) > 0.02) {
      tft.setCursor(5, 35); 
      tft.setTextColor(BLUE);  
      tft.print(Q0);
      Q0 = result0; 
      tft.setCursor(5, 35); 
      tft.setTextColor(WHITE); 
      tft.print(Q0);  
      if(abs(result0 - maxU) > 0.1) {
        startX = 150; 
        startY = 7;   
        symbolDraw(167);
      }
    }//DRAW <=
                               
    if(abs(Q1 - result1) > 0.02) {
      tft.setCursor(5, 70); 
      tft.setTextColor(BLUE);  
      tft.print(Q1);
      Q1 = result1; 
      tft.setCursor(5, 70); 
      tft.setTextColor(WHITE); 
      tft.print(Q1);  
      if(abs(result1 - maxU) > 0.1) {
        startX = 150; 
        startY = 42;  
        symbolDraw(167);
      }
    }//DRAW <=
    
    if(abs(Q2 - result2) > 0.02) {
      tft.setCursor(5, 105); 
      tft.setTextColor(BLUE);  
      tft.print(Q2);
      Q2 = result2; 
      tft.setCursor(5, 105); 
      tft.setTextColor(WHITE); 
      tft.print(Q2); 
      if(abs(result2 - maxU) > 0.1) {
        startX = 150; 
        startY = 77;  
        symbolDraw(167);
      }
    }//DRAW <=

    if(abs(Q3 - result3) > 0.02) {
      tft.setCursor(5, 140); 
      tft.setTextColor(BLUE);  
      tft.print(Q3);
      Q3 = result3; 
      tft.setCursor(5, 140); 
      tft.setTextColor(WHITE); 
      tft.print(Q3); 
      if(abs(result3 - maxU) > 0.1) {
        startX = 150; 
        startY = 112; 
        symbolDraw(167);
      }
    }//DRAW <=                             
  } InMenu = 0;
}