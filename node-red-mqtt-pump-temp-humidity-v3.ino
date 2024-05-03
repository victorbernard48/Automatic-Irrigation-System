/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "credentials.h"
#include "time.h"

//const char* ntpServer = "pool.ntp.org";
// Asia server
//const char* ntpServer = "asia.pool.ntp.org";
const char* ntpServer = "in.pool.ntp.org";
//const long  gmtOffset_sec = 0;
//For INDIA time GMT +5:30 
const long gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

// Replace the next variables with your SSID/Password combination
// These settings have been moved to credentials.h file 
// const char* ssid = "REPLACE_WITH_YOUR_SSID";
// const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.1.22";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//uncomment the following lines if you're using SPI
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

//Adafruit_BME280 bme; // I2C
#define DHTPIN 27     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
//Temp hum
float temperature = 0;
float humidity = 0;

// Pump Pin and Pump on time
const int pumpPin = 25;
//#define pumpOnTime 10000
long pumpOnTime = 90000;
const int ledPin = 33;

// Pump timing and setting
// Define the morning irrigation time
int   morningHour = 7;
int   morningMin = 30;
int   morningSec = 0;
// Define the evening irrigation time
int   eveningHour = 7;
int   eveningMin = 30;
int   eveningSec = 0;

void setup() {
  Serial.begin(115200);
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  //status = bme.begin();  
  Serial.println(F("DHTxx test!"));

  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(pumpPin, OUTPUT);
  pinMode(ledPin,OUTPUT);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function
// Print Local Time
void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  //Serial.println(&timeinfo);
  //Serial.println(&timeinfo, "%H");
}

// Function
// Start pump for specified time
void startPumpForSpecifiedTime(int pumpOntime){
  digitalWrite(pumpPin, HIGH);
  Serial.println("Pump started");
  delay(pumpOntime);
  digitalWrite(pumpPin, LOW);
  Serial.println("Pump shutdown");
  //delay(pumpOntime);
}
// Function
// Callback to collect messages every 5 mins
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }

  // Pump timing control
  // Morning Time
    if (String(topic) == "esp32/pump/morninghour") {
      Serial.println("Setting morning pump hour output to ");
      //set the morning hour to what is set in dashboard
      morningHour = messageTemp.toInt();
      Serial.print(morningHour);
    }
    if (String(topic) == "esp32/pump/morningmin") {
      Serial.println("Setting morning pump min output to ");
      //set the morning hour to what is set in dashboard
      morningMin = messageTemp.toInt();
      Serial.println(morningMin);
      Serial.print("inside callback");
    }
  // Evening Time
    if (String(topic) == "esp32/pump/eveninghour") {
      Serial.println("Setting evevnning pump hour output to ");
      //set the morning hour to what is set in dashboard
      eveningHour = messageTemp.toInt();
      Serial.print(eveningHour);
    }
    if (String(topic) == "esp32/pump/eveningmin") {
      Serial.println("Setting evening pump min output to ");
      //set the morning hour to what is set in dashboard
      eveningMin = messageTemp.toInt();
      Serial.print(eveningMin);
    }
  // Pump ON Time
    if(String(topic) == "esp32/pump/pumpontime"){
      Serial.println("Setting pump ON time to");
      long temp_time = messageTemp.toInt();
      pumpOnTime = temp_time * 60000;
      Serial.print(pumpOnTime);
    }

    // Manual Pump Control
      // Changes the output state according to the message
  if (String(topic) == "esp32/pump/manualcontrol") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
    digitalWrite(pumpPin, HIGH);
    Serial.println("Pump started manually.");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(pumpPin, LOW);
      Serial.println("Pump stopped manually.");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
      client.subscribe("esp32/pump/morninghour");
      client.subscribe("esp32/pump/morningmin");
      client.subscribe("esp32/pump/eveninghour");
      client.subscribe("esp32/pump/eveningmin");
      client.subscribe("esp32/pump/pumpontime");
      client.subscribe("esp32/pump/manualcontrol");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Function
// Check if it is time to turn on pump and trigger pump.
void checkTimeAndStartPump(){
  delay(5000);
  printLocalTime();
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time.");
  }
  int currentHour = timeinfo.tm_hour;
  int currentMins = timeinfo.tm_min;
  int currentSecs = timeinfo.tm_sec;
  Serial.print(eveningMin);
  Serial.println("inside check");
  // if((currentHour==morningHour)&&(currentMins==morningMin)){
  //   if(reservoirHasWater()){
  //     Serial.println("Trigger Time: &currentHour : &currentMins");
  //     startPumpForSpecifiedTime(pumpOnTime);
  //   }
  // }
  // Morning Trigger
  if((currentHour==morningHour)&&(currentMins==morningMin)&&(currentSecs>=0)){
      Serial.println((String)"Trigger Time: " + currentHour + ":" + currentMins + ":" + currentSecs);
      startPumpForSpecifiedTime(pumpOnTime);
  }
  // Evening Trigger  
  if((currentHour==eveningHour)&&(currentMins==eveningMin)&&(currentSecs>=0)){
      Serial.println((String)"Trigger Time: " + currentHour + ":" + currentMins + ":" + currentSecs);
      startPumpForSpecifiedTime(pumpOnTime);
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // Temperature in Celsius
    temperature = dht.readTemperature();   
    // Uncomment the next line to set temperature in Fahrenheit 
    // (and comment the previous temperature line)
    //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit
    
    // Convert the value to a char array
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("esp32/temperature", tempString);

    humidity = dht.readHumidity();
    
    // Convert the value to a char array
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    client.publish("esp32/humidity", humString);  
  }
  // Check time and trigger pump for specified time
  checkTimeAndStartPump();

}