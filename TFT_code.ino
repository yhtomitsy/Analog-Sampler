void setupInterface() {
  tft.drawLine(0, 40, 0, tft.height(), ILI9341_WHITE); // draw lines for the graph
  tft.drawLine(5, 40, 5, tft.height(), ILI9341_WHITE); // draw lines for the graph
  
  for (int y = 40; y <= tft.height(); y+=20){
    tft.drawLine(0, y, 5, y, ILI9341_WHITE); // draw lines for the graph
  }
  tft.drawLine(0, 239, 5, 239, ILI9341_WHITE); // draw lines for the graph
}

void testRects() {
  tft.fillRect(0, 40, tft.width(), tft.height(), ILI9341_BLACK); // draw rectangle on which sensor values will be displayed
}

void showSensorReadings(int r1, int r2, int r3, int r4 ){
  tft.setTextSize(2); //set font size

  if(abs(reading1 - prevReading1) > 12){
    tft.fillRect(0, 0, 80, 39, 0xC618); // draw rectangle on which sensor values will be displayed
    tft.setCursor(0, 10); // st position of cursor
    tft.setTextColor(ILI9341_RED); // set font color and
    if (sensor1){
      tft.print(r1 * 3.3 / 4096);
      Serial.print("S1>> "); Serial.println(reading1);
    }
    else tft.print("-.--");
    tft.print("V");
    
  }
  if(abs(reading2 - prevReading2) > 12){
    tft.fillRect(80, 0, 80, 39, 0xC618); // draw rectangle on which sensor values will be displayed
    tft.setCursor(80, 10); // st position of cursor
    tft.setTextColor(0xF81F); // set font color and
    if (sensor2){
      tft.print(r2 * 3.3 / 4096);
      Serial.print("S2>> "); Serial.println(reading2);
    }
    else tft.print("-.--");
    tft.print("V");
  }
  if(abs(reading3 - prevReading3) > 12){
    tft.fillRect(160, 0, 80, 39, 0xC618); // draw rectangle on which sensor values will be displayed
    tft.setCursor(160, 10); // st position of cursor
    tft.setTextColor(ILI9341_BLUE); // set font color and
    if (sensor3){
      tft.print(r3* 3.3 / 4096);
      Serial.print("S3>> "); Serial.println(reading3);
    }
    else tft.print("-.--");
    tft.print("V");
  }
  if(abs(reading4 - prevReading4) > 12){
    tft.fillRect(240, 0, 80, 39, 0xC618); // draw rectangle on which sensor values will be displayed
    tft.setCursor(240, 10); // st position of cursor
    tft.setTextColor(0x03E0); // set font color and
    if (sensor4){
      tft.print(r4 * 3.3 / 4096);
      Serial.print("S4>> "); Serial.println(reading4);
    }
    else tft.print("-.--");
    tft.print("V");
  }
}

void graph(){
  getReadings(); // get analog readings
  showSensorReadings(reading1, reading2, reading3, reading4);
  
  // map readings
  int y1 = map(reading1, 0, 4096, 240, 40);
  int y2 = map(reading2, 0, 4096, 240, 40);
  int y3 = map(reading3, 0, 4096, 240, 40);
  int y4 = map(reading4, 0, 4096, 240, 40);
  
  appendData(y1, y2, y3, y4); // add readings to history arrays
  
  if (graphEnd){
    tft.drawLine(6, 40, 6, tft.height(), ILI9341_BLACK); // clear fist pixel of every line
    graphEnd = false; // prevent this from happening until the graph ends
  }
  if (!graphBeginFlag){
    if (sensor1){
      tft.drawLine(prevX, prevY1, xPosition, y1, ILI9341_RED); // draw lines for the graph
      tft.drawLine(prevX+3, s1History[105], xPosition+3, s1History[104], ILI9341_BLACK); // draw lines for the graph
    }
    
    if (sensor2){
      tft.drawLine(prevX, prevY2, xPosition, y2, 0xF81F); // draw lines for the graph
      tft.drawLine(prevX+3, s2History[105], xPosition+3, s2History[104], ILI9341_BLACK); // draw lines for the graph
    }
    
    if (sensor3){
      tft.drawLine(prevX, prevY3, xPosition, y3, ILI9341_BLUE); // draw lines for the graph
      tft.drawLine(prevX+3, s3History[105], xPosition+3, s3History[104], ILI9341_BLACK); // draw lines for the graph
    }
    
    if (sensor4){
      tft.drawLine(prevX, prevY4, xPosition, y4, ILI9341_GREEN); // draw lines for the graph
      tft.drawLine(prevX+3, s4History[105], xPosition+3, s4History[104], ILI9341_BLACK); // draw lines for the graph
    }
  }
  if (xPosition == 6){ // clear first line segment at beginning of graph
    if (sensor1)tft.drawLine(xPosition, s1History[105], xPosition+3, s1History[104], ILI9341_BLACK); // draw lines for the graph
    if (sensor2)tft.drawLine(xPosition, s2History[105], xPosition+3, s2History[104], ILI9341_BLACK); // draw lines for the graph
    if (sensor3)tft.drawLine(xPosition, s3History[105], xPosition+3, s3History[104], ILI9341_BLACK); // draw lines for the graph
    if (sensor4)tft.drawLine(xPosition, s4History[105], xPosition+3, s4History[104], ILI9341_BLACK); // draw lines for the graph
  }
  
  prevY1 = y1;
  prevY2 = y2;
  prevY3 = y3;
  prevY4 = y4;
  prevX = xPosition;
  
  
  
  prevReading1 = reading1;
  prevReading2 = reading2;
  prevReading3 = reading3;
  prevReading4 = reading4;
}

void tftSetup(){
  //setup the TFT
  digitalWrite(5,HIGH);
  delay(100);
  tft.fillScreen(ILI9341_BLACK); // set screen background to black
  tft.setRotation(1); // set in landscape mode
  testRects(); // draw a rectangle
  delay(1000);
}

void manageTft(){
  if(xPosition >= 318){
    xPosition = 6; // set xpostion back to initial position
    prevX = 6; // set previous value of x
    graphEnd = true; // allow first pixels of graph to be cleared at beginning of new graph
  }
  else
    xPosition = xPosition + 3;
    delay(90);
}
