void UART_CHAT() {
  tft.fillScreen(color565(255, 255, 255));
  tft.fillRect(0, 165, 240, 73, BLUE);                  //tft.fillRoundRect(-5,165,250,73,15,TFT_BLUE); //DRAW ROUNDRECT BOTTOM
  tft.fillRect(0, 2, 240, 73, color565(210, 190, 255)); //tft.fillRoundRect(-5,2,250,73,15,color565(210,190,255)); //DRAW ROUNDRECT TOP
  tft.fillRect(0, 77, 240, 73, color565(255, 180, 210)); //tft.fillRoundRect(-5,77,250,73,15,color565(255,180,210)); //DRAW ROUNDRECT MIDDLE
  MyText = "9600"; 
  startX = 158; 
  startY = 210; 
  charColo = color565(200, 200, 200); 
  event(); 
  charColo = WHITE;
  //Serial.begin(115200);
  while(numButton0 != 14) { 
    PressedButton(0); 
    if(numButton2 == 8) {return;}
    noteStr = "";
    NOTE(4); 
    if(noteStr != "" && numButton2 == 1) {
      tft.setTextWrap(false);
      tft.setCursor(80, 20);
      tft.setTextColor(WHITE);
      tft.setTextSize(1);
      tft.println(noteStr);

      Serial.begin(9600); 
      MyText = noteStr; 
      Serial.println(MyText);
      //tft.fillRect(0,0,240,75,TFT_WHITE); 
      tft.fillRoundRect(-5, 2, 250, 73, 15, color565(200, 200, 255)); //CLEAN TOP WINDOW
      startX = 2; 
      startY = 2; 
      scaleX = 1; 
      scaleY = 1; 
      charColo = BLACK; 
      event(); 
      charColo = WHITE; 
      tft.fillRoundRect(-5, 165, 250, 73, 15, BLUE); //CLEAN BOTTOM WINDOW 
      noteStr = ""; 
      MyText = "";
    }
  }
  Serial.end();
}