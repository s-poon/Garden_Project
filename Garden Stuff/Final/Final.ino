


// Libraries to Include
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Definitions
#define DHTTYPE DHT11
const int drySoil = 350;
const int wetSoil = 300;
const int waterHour1 = 8;
const int waterHour2 = 17;
const int waterMinute = 30;
const int waterSecond = 11;
const int checkHour1 = 8;
const int checkHour2 = 17;
const int checkMinute = 10;
const int checkSecond = 11;
char filename[] = "datalog.csv";

//Setting Pins
const int chipSelect = 53;
const int moistureSensors[] = {A0, A1, A2};
const int tempSensor = 11;
const int sunlightSensor = A3;
const int relay = 12;

//Initializing DHT
DHT_Unified dht(tempSensor, DHTTYPE);


// Delete all code relating to serial before implementing
void setup(){
  Serial.begin(9600);

  //Initializing Pins
  for(int i = 0; i < 3; i ++){
    pinMode(moistureSensors[i], INPUT);
  }
  pinMode(sunlightSensor, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(tempSensor, INPUT);

  if(! SD.begin(chipSelect)){
    Serial.println("Card failed, or not present");
    while(1);
  }

  //Begin Functions
  dht.begin();

  //Adding labels to the columns
  if(!SD.exists(filename)){
    String header = "month,day,hr,min,sec,ms1,ms2,ms3,sl,temp,hum";
    File dataFile = SD.open(filename, FILE_WRITE);
    if(dataFile){
      dataFile.println(header);
      dataFile.close();
    }
  }

  

}

void loop(){
  // Data Variables
  static float moistureData[] = {0, 0, 0};
  static float moistureDataPercentage[] = {0, 0, 0};
  float temperature;
  float humidity;
  int lightData;
  tmElements_t tm;
  String dataString = "";

  //Logic Variables
  static bool watering = 0;
  static int waterDuration = 0;
  static int solenoidState = 0;
  static int waterGarden = 0;
  static int waterTime = 6;

  //Check the time
  RTC.read(tm);

  //Output the time given by month,day,hour,minutes,seconds
  dataString += String(tm.Month);
  dataString += "/";
  dataString += String(tm.Day);
  dataString += "/";
  dataString += String(tm.Hour);
  dataString += ":";
  dataString += String(tm.Minute);
  dataString += ":";
  dataString += String(tm.Second);
  dataString += ",";
  
  //Read the analog sensors and add to the string
  for(int i = 0; i < 3; i ++){
    moistureData[i] = analogRead(moistureSensors[i]);
    moistureDataPercentage[i] = moistureData[i]/10.24;
    dataString += String(moistureDataPercentage[i]);
    dataString += ",";
  }
  lightData = analogRead(sunlightSensor);
  lightData = lightData/10.24;
  dataString += String(lightData);
  dataString += ",";

  //Read the temp and humidity
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  dataString += String(event.temperature);
  dataString += ",";
  dht.humidity().getEvent(&event);
  dataString += String(event.relative_humidity);

  //logic to check if the garden needs to be watered
  if((tm.Minute == checkMinute) && (tm.Second < checkSecond)){
    for(int i = 0; i < 3; i ++){
      if(moistureData[i] >= drySoil){
        waterGarden = 1;
      }
    }
  }

  //watering
  if((waterGarden == 1) && ((tm.Hour == waterHour1) || (tm.Hour == waterHour2)) && (tm.Minute == waterMinute) && (tm.Second < waterSecond)){
    digitalWrite(relay, HIGH);
    watering = 1;
  }

  if(watering == 1){
    waterDuration ++;
  }
  //Stop watering
  if(waterDuration == waterTime){
    digitalWrite(relay, LOW);
    waterDuration = 0;
    watering = 0;
    waterGarden = 0;
  }

  

  //Write to SD Card
  File dataFile = SD.open(filename, FILE_WRITE);

  if(dataFile){
    dataFile.println(dataString);
    dataFile.close();
    Serial.print(waterGarden);
    Serial.println(dataString);
  }
  else{
    Serial.println("error opening datalog.csv");
  }
  delay(30000);
}
