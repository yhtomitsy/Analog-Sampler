void getReadings(){ // get analog readings
  reading1 = (uint16_t)adc->analogRead(readPin);
  reading2 = (uint16_t)adc->analogRead(readPin1);
  reading3 = (uint16_t)adc->analogRead(readPin2);
  reading4 = (uint16_t)adc->analogRead(readPin3);
  
  //if (sensor1){Serial.print("S1>> "); Serial.println(reading1);delay(10);}
  
  //if (sensor2){Serial.print("S2>> "); Serial.println(reading2);delay(10);}
  
  //if (sensor3){Serial.print("S3>> "); Serial.println(reading3);delay(10);}
  
  //if (sensor4){Serial.print("S4>> "); Serial.println(reading4);delay(10);}
  
}

void appendData(int s1, int s2, int s3, int s4){
  for (int n = 105; n > 0; n--){          // shift history array to the right, oldest value [300] is lost
    if (sensor1)s1History[n] = s1History[(n - 1)];    // shift data right, oldest value lost, [1] becomes [0] the current value
    if (sensor2)s2History[n] = s2History[(n - 1)]; 
    if (sensor3)s3History[n] = s3History[(n - 1)]; 
    if (sensor4)s4History[n] = s4History[(n - 1)]; 
  }
  if (sensor1)s1History[0] = s1;
  if (sensor2)s2History[0] = s2;
  if (sensor3)s3History[0] = s3;
  if (sensor4)s4History[0] = s4;
}

void clearArrays(){
  // clear arrays
  for (int i =0; i < 106; i++){
    s1History[i] = 40;
    s2History[i] = 40;
    s3History[i] = 40;
    s4History[i] = 40;
  }
}

void clearTFT(){
  reading1 = 0;
  reading2 = 0;
  reading3 = 0;
  reading4 = 0;
  prevReading1 = 0;
  prevReading2 = 0;
  prevReading3 = 0;
  prevReading4 = 0;
  xPosition = 6;
  prevX = 6;
  prevY1 = 0; 
  prevY2 = 0; 
  prevY3 = 0; 
  prevY4 = 0;
}

