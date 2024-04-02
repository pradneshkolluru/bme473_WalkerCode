// Complete Instructions to Get and Change ESP MAC Address: https://RandomNerdTutorials.com/get-change-esp32-esp8266-mac-address-arduino/

#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
#endif

unsigned long lastTime = 0;

unsigned long timerDelay = 10000;




String serverName = "http://vcm-35196.vm.duke.edu:5000/post_lift";

const char* ssid = "DukeOpen";
// const char* password =  "yourNetworkPass";

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial) ;

  Serial.println("");
  Serial.println("");
  Serial.println("_______________________________");
  

  WiFi.begin(ssid);

  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  Serial.println("");
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);

}

// the loop function runs over and over again forever
void loop() {
  //Send an HTTP POST request every 10 minutes




  if ((millis() - lastTime) > timerDelay) {

    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      // Specify content-type header
      http.addHeader("Content-Type", "application/json");

      int count = 3;
      int avgDuration = 4;
      int avgHeight = 5;

      // Construct JSON payload
      String payload = "{\"count\": " + String(count) + ", \"avgDuration\": " + String(avgDuration) + ", \"avgHeight\": " + String(avgHeight) + "}";

      int httpResponseCode = http.POST(payload);

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);


      String responseMessage = http.getString();
      Serial.print("HTTP Response message: ");
      Serial.println(responseMessage);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
