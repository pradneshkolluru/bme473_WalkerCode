#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

const int InfraredSensorPin = 4;//Connect the signal pin to the digital pin 4
//const int LedDisp = 13;
const int buzzer = 12;
const int trigPin = 9;
const int echoPin = 10;

long duration;
float distanceCm, distanceInch;

const int ledPin1 = 2;
const int ledPin2 = 3;
const int ledPin3 = 5;


int size = 3;

float* distanceMeasurements = new float[size];
int index = 0;
float* baseLineAvg = new float[size];
int baseIndex = 0;



// SEN0019 TIMER
//const int LedDisp = 13;


// Define the time threshold (in milliseconds) for user absence
const unsigned long userAbsenceThreshold = 2000;  // Adjust the duration as needed

// Initialize variables
bool userInFrame = false;
unsigned long userAbsenceStartTime = 0;


void setup()
{
  Serial.begin(57600);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
    
  Serial.println("Start!");
  pinMode(InfraredSensorPin,INPUT);
  //pinMode(LedDisp,OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  for (int i = 0; i < size; i++) {
    distanceMeasurements[i] = 0;
    baseLineAvg[i] = 0;
  }


  //digitalWrite(LedDisp,LOW);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }
  
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  Serial.println("");
  delay(100);
  
}

void loop()
{ 

  Serial.println("test");
  digitalWrite(ledPin1, LOW);  // Turn on LED 1
  digitalWrite(ledPin2, LOW);   // Turn off LED 2
  digitalWrite(ledPin3, LOW);   // Turn off LED 2
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  //if(digitalRead(InfraredSensorPin) == LOW)  digitalWrite(LedDisp,HIGH);
  //else  digitalWrite(LedDisp,LOW);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  distanceCm = duration * 0.034 / 2;
 

  

  if (distanceCm < 100 and distanceCm > 20){

    distanceMeasurements[index] = distanceCm;
    index = (index + 1) % 5;

  }

  distanceInch = duration * 0.0133 / 2;
  Serial.print("Distance (cm):");
  
   


  float avgDistance = average(distanceMeasurements);

  Serial.println(avgDistance);

  baseLineAvg[baseIndex] = avgDistance;
  baseIndex = (baseIndex + 1) % size;

  


  //50

  Serial.println(g.acceleration.z);

  Serial.println("");

  if (allAboveThreshold(38) && g.acceleration.z > abs(0.01)) {

  // if (distanceCm > 38 && distanceCm < 100 && g.acceleration.z > abs(0.01)) {

  // if (allAboveThreshold(38)) {
    tone(buzzer, 1000);
    delay(50);
    noTone(buzzer);
    delay(250);
    digitalWrite(ledPin1, HIGH);  // Turn on LED 1
    digitalWrite(ledPin2, LOW);   // Turn off LED 2
    digitalWrite(ledPin3, LOW);   // Turn off LED 2

  }
  
  
  bool infraredSensorState = digitalRead(InfraredSensorPin);

  // Print the sensor state (for debugging)
  Serial.println(!infraredSensorState, BIN);

  if (infraredSensorState == LOW) {
    // User is in frame
    userInFrame = true;
    userAbsenceStartTime = 0;  // Reset the absence timer
    noTone(buzzer);
  } else {
    // User is not in frame
    if (userInFrame) {
      // Check if the absence timer is started
      if (userAbsenceStartTime == 0) {
        userAbsenceStartTime = millis();  // Start the absence timer
      } else {
        // Check if the absence duration has exceeded the threshold
        if (millis() - userAbsenceStartTime >= userAbsenceThreshold) {
          // Notify user absence
          tone(buzzer, 1000);
          delay(50);
          noTone(buzzer);
          Serial.println("User is out of position");
          digitalWrite(ledPin1, LOW);  // Turn on LED 1
          digitalWrite(ledPin2, HIGH);   // Turn off LED 2
          digitalWrite(ledPin3, LOW);   // Turn off LED 3

          userInFrame = false;
        }
      }
    } else {
      // User is still not in frame
      tone(buzzer, 1000);
      delay(50);
      noTone(buzzer);
      Serial.println("User is out of position");
      digitalWrite(ledPin1, LOW);  // Turn on LED 1
      digitalWrite(ledPin2, HIGH);   // Turn off LED 2
      digitalWrite(ledPin3, LOW);   // Turn off LED 3
    }
  }


  

  //Serial.print("Rotation X: ");
  //Serial.print(g.gyro.x*180/3.14);
  //Serial.print(", Y: ");
  //Serial.print(g.gyro.y*180/3.14);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z*180/3.14);
  Serial.println(" deg/s");

  if (g.gyro.z*180/3.14 > 90 || g.gyro.z*180/3.14 < -90) {
    tone(buzzer, 1000);
    delay(50);
    noTone(buzzer);
    digitalWrite(ledPin1, LOW);  // Turn on LED 1
    digitalWrite(ledPin2, LOW);   // Turn off LED 2
    digitalWrite(ledPin3, HIGH);   // Turn off LED 3
  }

  delay(50);
}

float average (float * array)  // assuming array is int.
{
  long sum = 0L ;  // sum will be larger than an item, long for safety.
  for (int i = 0 ; i < size ; i++)
    sum += array [i] ;
  return  ((float) sum) / size ;  // average will be fractional, so float may be appropriate.
}

bool allAboveThreshold(float threshold) {

  bool accel = false;

  for (int i = 0; i < size; i++) {
    if (distanceMeasurements[i] <= threshold) {
      return false; // As soon as one measurement is not above the threshold, return false
    }
  }
  return true; // All measurements are above the threshold
}
