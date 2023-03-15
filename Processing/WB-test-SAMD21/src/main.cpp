/***********************************************************
 * 
 * Code for Weather-Ballon launch
 * Author: Michael Williams
 * Organization: BYU Spacecraft Club
 * 
 * First version: 2/16/2023
 * 
 * for use with Seeeed Studio XIAO SAMD21 processor chip
 * 
************************************************************/

// MacroDirectives and library includes
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SD.h>



// Global constants

#define SEALEVELPRESSURE_HPA   1013.25           // Pressure at sea level to be used in getting altitude.
#define CAPTURE_ALTITUDE       1290.5           // Altitude to start capture, units in meters



// Pin Naming

#define Pressure_Sensor_Pin   2
#define SD_Card_Pin           1
#define Camera_Pin            3

// // HardWare pins on SAMD21
// #define SDA   4
// #define SCL   5
// #define SCK   8
// #define MISO  9
// #define MOSI  10

// All function declarations
void perception();
bool reachedAltitude();
void planning();
void action();
void log();
float read_altitude();
float read_pressure();
float read_humidity();
float read_pressure();
void write_image();
void read_image();
void write_logfile(String message);
void take_image();
void send_transmission();


// other

Adafruit_BME280 pressure_sensor(Pressure_Sensor_Pin); // Object for pressure sensor
File logfile;


void setup() {
  // add all setup commands here

  // Serial Setup
  Serial.begin(9600);
  SD.begin(SD_Card_Pin);


  // Pressure sensor setup
  pressure_sensor.begin();  


  pinMode(1, OUTPUT);
  digitalWrite(0, HIGH);

  // while(true) {
  //   digitalWrite(1, HIGH);
  //   delay(50);
  //   digitalWrite(1, LOW);
  //   delay(50);
  // }

  


  // pinMode(2, OUTPUT);
  // digitalWrite(2, HIGH);
  


  // pinMode(1, OUTPUT);
  // digitalWrite(1, HIGH);
  



  // pinMode(3, OUTPUT);
  // digitalWrite(3, HIGH);

}

void loop() {
  // Principle of Perception, Planning, Action
  //      Also included a log function for logging events, 
  //      could be done interspersed through the program instead

  perception();
  planning();
  action();
  log();


  // test only section:


}



/*
  * SECTION 1
  * Perception
  * 
  * Sensors:
  * atmospheric sensor
  * 
*/
void perception() {
  if(reachedAltitude()) {
    write_logfile("Altitude reached");
  } else {
    write_logfile("Altitude not reached");
  }
}


bool reachedAltitude() {
  if (read_altitude() > CAPTURE_ALTITUDE) {
    return true;
  } else {
    return false;
  }
}



/*
  * SECTION 2 
  * Planning
  * 
  * State Machines:
  * 
*/
void planning() {
  write_logfile("Current Altitude = ");
}


// Add state machine to determine when to take a picture and activate camera

// Add state machine to manage the sending of data to SD card

// Add state machine to manage the sending of data from SD card to radio and activate radio




/*
* SECTION 3
* Action
* 
* Functions:
* 
* 
*/
void action() {
  float alt = read_altitude();
  write_logfile(String(alt));
}

// add fucntion to run camera capture

// add function to write to SD card

// add function to read from SD card

// add function to activate and send data over radio




/*
 * SECTION 4 
 * logging
 * 
 * determine what to log, and how to write log file so we can read post mission
 * 
*/
void log() {
  delay(2000);

}











/*
 * 
 * SECTION 5
 * Functions for other use
 * 
 * Subsections:
 * BME pressure sensor reading
 * SD card control 
 * Camera operations
 * Radio operations
 * 
*/


// BME pressure sensor reading
float read_altitude() { // units meters
  return pressure_sensor.readAltitude(SEALEVELPRESSURE_HPA);
}
float read_pressure() { // units hpa
 return pressure_sensor.readPressure() / 100.0F;
}
float read_humidity() { // units %
  return pressure_sensor.readHumidity();
}
float read_temperature() { // units °C
  return pressure_sensor.readTemperature();
}

// SD card control Functions
void write_image() {

}
void read_image() {

}
void write_logfile(String message) {
  logfile = SD.open("logfile.txt", FILE_WRITE);

   if (logfile) {
    logfile.println(message);
    logfile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening logfile.txt");
  }
  Serial.println(message);
}

// Camera control functions
void take_image() {
  
}

// Radio control Functions
void send_transmission() {

}
