  
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // includes the LiquidCrystal Library
LiquidCrystal_I2C lcd(0x27, 16, 2); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 8; 
const int haptic = 7;
 
long duration;
float distanceCm, distanceInch;

void setup() {
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 

  lcd.begin(20, 4); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(haptic, OUTPUT); 
  Serial.begin(9600);
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  distanceInch = duration * 0.0133 / 2;
  Serial.print("Distance (cm):");
  Serial.println(distanceCm); 


  if (distanceCm > 64) {
    digitalWrite(haptic, HIGH);

    tone(buzzer, 1000);
    delay(50);
    noTone(buzzer);
  }
  else{
    digitalWrite(haptic, LOW);
  }
  
  
  //lcd.clear();
  lcd.setCursor(0, 0); // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("Distance: "); // Prints string "Distance" on the LCD
  lcd.print(distanceCm); // Prints the distance value from the sensor
  //lcd.print(" cm");
   
  delay(250);
}
