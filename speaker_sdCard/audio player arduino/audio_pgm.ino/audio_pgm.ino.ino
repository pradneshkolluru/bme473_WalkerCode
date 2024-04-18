//TMRpcm library is needed

#include "SD.h"
#define SD_ChipSelectPin 10
// #include "TMRpcm.h"
#include "SPI.h"

// TMRpcm tmrpcm;

void setup(){
// tmrpcm.speakerPin=9;
  Serial.begin(9600);
  delay(1000);

  Serial.println("SD fail");

  if(!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }
  else{
    Serial.println("SD found");
  }
  // tmrpcm.setVolume(6);
  // tmrpcm.play("test.wav");

}

void loop() {
  // put your main code here, to run repeatedly:

  // Serial.println("test");
  // delay(1000);

}
