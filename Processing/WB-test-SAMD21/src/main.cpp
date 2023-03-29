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
#include <ArduCAM.h>
// #include "memorysaver.h"


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
void read_image(String name);
void write_logfile(String message);
void take_image();
void send_transmission();
void init_camera();
void write_new_captures();


// sensor initializations

Adafruit_BME280 pressure_sensor(Pressure_Sensor_Pin); // Object for pressure sensor
ArduCAM myCAM( OV2640, Camera_Pin ); // Camera
File logfile;

/*********************************************************
 * 
 * Global Flags for use in state machines and Actions
 * 
 * 
**********************************************************/
bool START = false;
int imageIndex = 0;


// fsmMain signals 
// State Machine States
#define IDLE      0
#define HUB       1
#define IMAGE     2
#define WRITE     3
#define READ      4
#define TRANSMIT  5


// Input Signals to control state Machine states
int AltitudeReached   = 0;
int CaptureDone       = 0;
int WriteDone         = 0;
int Transmit          = 0;
int ReadDone          = 0;
int TransmissionSent  = 0;
int CaptureImage      = 0;

// Output Signals to control robot actions
int ACTION            = 0;
#define WAIT_IMAGE      0
#define TAKE_IMAGE      1 
#define WRITE_IMAGE     2 
#define READ_IMAGE      3
#define TRANSMIT_IMAGE  4 


void setup() {
  // add all setup commands here

  // Serial Setup
  Serial.begin(9600);
  Wire.begin();
  SPI.begin();


  // SD card setup
  pinMode(SD_Card_Pin, OUTPUT); // setup the camera chip select
  digitalWrite(SD_Card_Pin, HIGH); // disable camera

  if(!SD.begin(SD_Card_Pin)){
    Serial.println("Failed to Initalize SD Card");
  }


  // Camera Setup
  pinMode(Camera_Pin, OUTPUT); // setup the camera chip select
  digitalWrite(Camera_Pin, HIGH); // disable camera

  Wire.begin();
  SPI.begin();

  init_camera();

  // Pressure sensor setup
  pinMode(Pressure_Sensor_Pin, OUTPUT); // setup the BME chip select
  digitalWrite(Camera_Pin, HIGH);  // disable pressure sensor

  pressure_sensor.begin();

}

void loop() {
  // Principle of Perception, Planning, Action
  //      Also included a log function for logging events, 
  //      could be done interspersed through the program instead

  perception();
  if(START) {
    planning();
    action();
  }
  // log();
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
  if( reachedAltitude) {
    AltitudeReached = 1;
    START = true;
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
  fsmMain();
}

void fsmMain() {
  static int mainState = IDLE;

  switch(mainState) {
    case IDLE:
      ACTION = WAIT_IMAGE;
      
      // State Transition logic
      if(AltitudeReached) {
        mainState = HUB;
      }
      break;
    
    case HUB:
      ACTION = WAIT_IMAGE;

      // State Transition Logic
      if (!AltitudeReached) {
        mainState = IDLE;
      } else if(CaptureImage) {
        mainState = IMAGE;
      } else if (Transmit) {
        mainState = READ;
      }
      break;

    case IMAGE:
      ACTION = TAKE_IMAGE;
      isCameraDone();

      // State Transition Logic
      if(CaptureDone) {
        mainState = WRITE;
      }
      break;

    case WRITE:
      ACTION = WRITE_IMAGE;

      // State Transition Logic
      if(WriteDone) {
        mainState = HUB;
      }
      break;

    case READ:
      ACTION = READ_IMAGE;

      // State Transition Logic
      if(ReadDone) {
        mainState = TRANSMIT;
      }
      break;

    case TRANSMIT:
      ACTION = TRANSMIT_IMAGE;

      // State Transition Logic
      if(TransmissionSent) {
        mainState = HUB;
      }
      break;
  }
}


void isCameraDone(){
  if(myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)) 
    CaptureDone = 1;
  else 
    CaptureDone = 0;
}


/*
* SECTION 3
* Action
* 
* Functions:
* 
*/
void action() {
  switch(ACTION) {
    case WAIT_IMAGE:
      // For now, do nothing
    break;
    case TAKE_IMAGE:

      take_image();

      break;
    case WRITE_IMAGE:

      write_new_captures();

      break;
    case READ_IMAGE:

      // FIXME:: add functions to read the image needed

      break;
    case TRANSMIT_IMAGE:

      // FIXME:: add functions to transmit the image over radio module

      break;
  }
  
}



/*
 * SECTION 4 
 * logging
 * 
 * determine what to log, and how to write log file so we can read post mission
 * 
*/
void log() {

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

  digitalWrite(Pressure_Sensor_Pin, LOW);
  float result = pressure_sensor.readAltitude(SEALEVELPRESSURE_HPA);
  digitalWrite(Pressure_Sensor_Pin, HIGH);
  
  return result;
}
float read_pressure() { // units hpa

  digitalWrite(Pressure_Sensor_Pin, LOW);
  float result = pressure_sensor.readPressure() / 100.0F;
  digitalWrite(Pressure_Sensor_Pin, HIGH);

 return result;
}
float read_humidity() { // units %

  digitalWrite(Pressure_Sensor_Pin, LOW);
  float result = pressure_sensor.readHumidity();
  digitalWrite(Pressure_Sensor_Pin, HIGH);

  return result;
}
float read_temperature() { // units °C

  digitalWrite(Pressure_Sensor_Pin, LOW);
  float result = pressure_sensor.readTemperature();
  digitalWrite(Pressure_Sensor_Pin, HIGH);

  return result;
}

// SD card control Functions
void write_new_captures(){
  // get the name of the new image
  String name = getImageName();


  // if(!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)) return; // camera not done yet
  File outfile = SD.open(name, O_WRITE | O_CREAT); // the arduino sd library is limited to file names of 8 characters wide by 3 wide in extention
  if(!outfile){
    write_logfile("Failed to crate image file");
    return;
  }
  write_logfile("Image Capture Ready");
  uint32_t length = myCAM.read_fifo_length(); //read image length
  myCAM.set_fifo_burst(); //Set fifo burst mode for easy reads
  uint8_t data;
  while(length > 0){
    myCAM.CS_LOW(); // active the camera
    myCAM.set_fifo_burst(); // has to be reset as far a I know every time
    data = SPI.transfer(0x00);
    myCAM.CS_HIGH(); // deactive the camera for sd
    outfile.write(data);
    length--;
  }
  write_logfile("Finished Image Write");
  myCAM.clear_fifo_flag();
  outfile.close();
  if(!SD.exists(name)){
    write_logfile("Created Image file and wrote to it but it donst exist after writing");
  }

}

void read_image(String name) {

}

void write_logfile(String message) {
  logfile = SD.open("logfile.txt", FILE_WRITE);

   if (logfile) {
    logfile.print(getTime() + " ");
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
  //clear flags
  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  //start flags
  write_logfile("Starting Camera Capture");
  myCAM.start_capture();

}


// Write the data from Camera onto SD card 


//initalize camera
void init_camera(){
  //reset camera
  myCAM.write_reg(0x07, 0x80);
  delay(100);
  myCAM.write_reg(0x07, 0x00);
  //check test register
  while(1){ // just writes to a test port and if it can read it back then it is good
    //Check if the ArduCAM SPI bus is OK
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    uint8_t temp = myCAM.read_reg(ARDUCHIP_TEST1);
    if (temp != 0x55){
      write_logfile("Failed to connect to Camera via SPI");
      delay(10000);
      continue; // retry untill good connection
    }else{
      write_logfile("Succesfully connected to Camera modual via SPI");
      break;   // good connection continue with the code
    }
  }
  //check the camera type
  uint8_t vid, pid;
  while(1){
    //Check if the camera module type is OV2640
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
      write_logfile("Failed to find the Correct Camera Module");
      delay(1000);
      continue;
    }else{
      break;
    }
  }
  //setup camera settings
  myCAM.set_format(JPEG);
  myCAM.InitCAM();
  myCAM.OV2640_set_JPEG_size(OV2640_1600x1200);
}


// Radio control Functions
void send_transmission() {

}

String getTime(){
int ts = millis() / 1000;
int tm = ts / 60;
int th = tm / 60;
tm = tm % 60;
ts = ts % 60;
String timeStamp = th + ":" + tm; 
timeStamp +=":" + ts;
return timeStamp;
}

String getImageName(){
  static int nextWrite = 0;

  String imageName = nextWrite + ".jpg";
  nextWrite++;

  // Write the image name and timestamp to a logfile

  File imageLog = SD.open("ImageLog.txt", O_APPEND);
  if(!imageLog) {
    write_logfile("Failed to write to the image log");
  }
  String message = "Name - " + imageName + "    Time - " + getTime() + "    Altitude - " + read_altitude();
  imageLog.println(message);
  write_logfile(message);

  return imageName;
}
