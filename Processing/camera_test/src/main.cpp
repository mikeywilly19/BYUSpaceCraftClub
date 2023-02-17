#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>

#define SDA 13
#define SCL 12
#define CS 6 // control pin


ArduCAM myCAM(OV2640, CS);

void setup() {
  Serial.begin(921600);
  Wire.begin();

  Serial.println(F("ACK CMD ArduCAM Start! END"));
  
  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW);
  
  myCAM.set_format(JPEG);
  // myCAM.set_size(MEDIUM);
}

void loop() {
  Serial.println("Taking Picture");
  myCAM.clear_fifo_flag();
  myCAM.start_capture();
  
  while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
  uint8_t buf;
  uint32_t len = 0;
  
  myCAM.CS_LOW();
  myCAM.set_fifo_burst();
  buf = myCAM.read_fifo();
  len = myCAM.read_fifo_length();
  myCAM.CS_HIGH();
  
  if (len >= MAX_FIFO_SIZE) {
    Serial.println("Over size.");
    return;
  }
  
  // char name[15];
  // sprintf(name, "IMAGE%05d.jpg", num++);
  // FILE imgFile = SPIFFS.open(name, "w");
  // imgFile.write(buf, len);
  // imgFile.close();
  
  Serial.print("Picture taken and saved with name: ");
  Serial.println(buf);
  delay(1000);
}