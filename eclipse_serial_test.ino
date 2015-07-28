#include <ADC.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

//declare ADC 
ADC *adc = new ADC(); // adc object

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

//analog pins
#define readPin  A6
#define readPin1  A7 
#define readPin2  A2
#define readPin3  A3
#define backLightPin 5
#define ledPin 4

// global variables
byte c = 0; // incoming data from java app

boolean sensor1 = false;
boolean sensor2 = false;
boolean sensor3 = false;
boolean sensor4 = false;

float xPosition = 6; // position on the x axis
float prevX = 6; // previous position on x axis
int prevY1 = 0, prevY2 = 0, prevY3 = 0, prevY4 = 0; // previous values of position on y axis
boolean tftShow = true; // start tft
int reading1 = 0, reading2 = 0, reading3 = 0, reading4 = 0; // analog readings for graph
int prevReading1 = 0, prevReading2 = 0, prevReading3 = 0, prevReading4 = 0; // previous analog readings
boolean graphBeginFlag = true; // enable graph
boolean graphEnd = false;
int s1History[106] = {0}; // array for storing incoming readings
int s2History[106] = {0}; // array for storing incoming readings
int s3History[106] = {0}; // array for storing incoming readings
int s4History[106] = {0}; // array for storing incoming readings

long time = 0, time2 = 0; // contain time in millis
int dataSensor2[200] = {0}; // values of sensor 2
int dataSensor3[200] = {0}; // values of sensor 3
int dataSensor4[200] = {0}; // values of sensor 4
int count = 0; // keeps count of analog readings taken

void setup() {
  Serial.begin(115200); //set baud rate
  // set state of pins
  pinMode(ledPin, OUTPUT);
  pinMode(backLightPin, OUTPUT);
  pinMode(readPin, INPUT);
  pinMode(readPin1, INPUT);
  pinMode(readPin2, INPUT);
  pinMode(readPin3, INPUT);
  
  // setup ADC
  adc->setAveraging(32); // set number of averages
  adc->setResolution(12); // set bits of resolution
  
  adc->setAveraging(32, ADC_1); // set number of averages
  adc->setResolution(12, ADC_1); // set bits of resolution
  
  adc->setConversionSpeed(ADC_VERY_HIGH_SPEED); // change the conversion speed
  adc->setConversionSpeed(ADC_VERY_HIGH_SPEED, ADC_1); // change the conversion speed
  
  adc->setSamplingSpeed(ADC_VERY_HIGH_SPEED); // change the sampling speed
  adc->setSamplingSpeed(ADC_VERY_HIGH_SPEED, ADC_1); // change the sampling speed  
  
  clearArrays(); 
  
  // start TFT
  tft.begin();
  digitalWrite(backLightPin,LOW);
  
  delay(1000);
}

void loop(){
  // handle serial data, if any
  memset(dataSensor2, 0, sizeof(dataSensor2));
  memset(dataSensor3, 0, sizeof(dataSensor3));
  memset(dataSensor4, 0, sizeof(dataSensor4));
  if (Serial.available()){
    c = Serial.read();
    if(c == 3)sensor1 = true;
    else if(c == 4)sensor2 = true;
    else if(c == 5)sensor3 = true;
    else if(c == 6)sensor4 = true;
  }
  
  if(c == 0){
    if (tftShow == false){
      clearArrays();
      tftShow = true;
    }
    sensor1 = false;
    sensor2 = false;
    sensor3 = false;
    sensor4 = false;
  }
  else if(c == 1){ // when incoming serial data is 1
    tftShow = true;
    digitalWrite(ledPin, HIGH); // turns ON the sampling indicator the inbuilt LED
    digitalWrite(backLightPin,LOW); // turns off screen backlight
    tft.fillRect(0, 0, tft.width(), tft.height(), ILI9341_BLACK); // draw a black rectangle
    Serial.print("*"); // signifies start of new sampling data
   
    time2 = millis(); // get start time
    for(int i = 0; i < 2000; i++){
      if (Serial.available()){ // check for any incoming serial data
        c = Serial.read(); // get incoming byte
        if(c == 0)break; // incoming command has changed so stop sampling  
      }
      
      
      if (sensor1)Serial.println((uint16_t)adc->analogRead(readPin)); //send data for sensor 1 to java app
      if ((millis() - time) >= 100){ // sample once every 100 milliseconds
        if (sensor2)dataSensor2[i/10] = (uint16_t)adc->analogRead(readPin1);
        if (sensor3)dataSensor3[i/10] = (uint16_t)adc->analogRead(readPin2);
        if (sensor4)dataSensor4[i/10] = (uint16_t)adc->analogRead(readPin3);
        time = millis(); // get time
      }
      count++; // keep track of number of analog readings
      if(i == 1999){
        delay(10);
        Serial.println("#");// signifies that sampling has ended
        Serial.println("Sampling has ended");
        if (sensor1){Serial.print("Sensor 1>> ");Serial.print(count);Serial.print(" samples in "); Serial.print(millis()-time2); Serial.println("ms");}
        if (sensor2){Serial.print("Sensor 2>> ");Serial.print(count/10);Serial.print(" samples in "); Serial.print(millis()-time2); Serial.println("ms");}
        if (sensor3){Serial.print("Sensor 3>> ");Serial.print(count/10);Serial.print(" samples in "); Serial.print(millis()-time2); Serial.println("ms");}
        if (sensor4){Serial.print("Sensor 4>> ");Serial.print(count/10);Serial.print(" samples in "); Serial.print(millis()-time2); Serial.println("ms");}
        Serial.println("\n>> Acquiring data. This will take a while...");
        Serial.print("$");
        count = 0;
      }
      delay(10);
    }
    
    digitalWrite(ledPin, LOW); // dim the attached LED
    if(c == 0){
      // do nothing
    }
    else{
      // sending values of sensor 2, 3 and 4 to java app
      delay(1000);
      Serial.print("$");
      delay(10);
      if (sensor2){
        for(int j = 0; j < 200; j++){
          Serial.println(dataSensor2[j]);
          delay(10);
        }
        delay(1000);
      }
      Serial.print("$");
      delay(10);
      if (sensor3){
        for(int j = 0; j < 200; j++){
          Serial.println(dataSensor3[j]);
          delay(10);
        }
        delay(1000);
      }
      Serial.print("$");
      delay(10);
      if (sensor4){
        for(int j = 0; j < 200; j++){
          Serial.println(dataSensor4[j]);
          delay(10);
        }
      }
      Serial.print("$");
    }
    c = 0;
    count = 0; // clear recorded number of samples
  }
  else if(c == 2){
    if (tftShow){
      clearTFT(); // clear TFT 
      tftSetup(); // setup TFT
      setupInterface(); // build TFT interface
      tftShow = false; // stop the tft from seting up again
      graphBeginFlag = true; 
      
    }    
    graph();
    manageTft();
    graphBeginFlag = false;
  }  
}
