/*
 * Code for getting weather data from OpenWheatherMap API
 * and uploading it to Blynk cloud (V0 - V2)
 *
 * Made by: NIHAL T P
 * GitHub: https://github.com/nihaltp
 * LinkedIn: https://www.linkedin.com/in/nihal-tp/
 */

#define BLYNK_PRINT Serial

#warning "Change blynk credentials before running the code"
// Blynk Authentication code  // TODO: Change Auth
#define BLYNK_TEMPLATE_ID "TMPLXXXXXXXXX"
#define BLYNK_TEMPLATE_NAME "TEMPLATE"
#define BLYNK_AUTH_TOKEN "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials  // TODO: Change credentials
#warning "Change the values of ssid and password before running the code."
#define ssid "YOUR_WIFI_SSID"
#define password "YOUR_WIFI_PASSWORD"

// OpenWeatherMap API  // TODO: Change values
#warning "Change the values of apiKey, city, and countryCode before running the code."
#define apiKey "YOUR_OPENWEATHERMAP_API_KEY"
#define city "YOUR_CITY"
#define countryCode "YOUR_COUNTRY_CODE"

// API URL
String weatherURL = "http://api.openweathermap.org/data/2.5/weather?q=" + String(city) + "," + String(countryCode) + "&appid=" + String(apiKey) + "&units=metric";

#define timeInterval 60000
long int timeNow = 0;
float temperature;
int humidity;
String weather;

void fetchWeatherData(void);
void sendData();

// MARK: setup
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");

  fetchWeatherData();
  timeNow = millis();
}

// MARK: loop
void loop() {
  // Fetch data if there is any Serial input
  if (Serial.available() > 0) {
    String input = Serial.readString();  // Reads and clears the input from serial
    
    if (millis() - timeNow > timeInterval) {
      fetchWeatherData();
      timeNow = millis();
    }
    else {
      // Calculates the time left in seconds
      int timeLeft = (timeInterval - (millis() - timeNow)) / 1000;
      Serial.println("Please wait for " + String(timeLeft) + "seconds before fetching data again!");
    }
  }
  
  Blynk.run();
}

// MARK: fetchWeatherData
void fetchWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, weatherURL);  // Initialize HTTP request
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Weather Data Received:");
      Serial.println(payload);

      // Parse JSON response
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        temperature = doc["main"]["temp"];                        // Get temperature
        humidity = doc["main"]["humidity"];                       // Get humidity
        weather = doc["weather"][0]["description"].as<String>();  // Get weather condition
        
        Serial.println("Temperature: " + String(temperature) + "°C");
        Serial.println("Humidity: " + String(humidity) + "%");
        Serial.println("Weather: " + weather);
        
        sendData();
      } else {
        Serial.println("JSON Parsing Error!");
      }
    } else {
      Serial.println("Failed to fetch data! HTTP Error: " + String(httpCode));
    }
    http.end();
  } else {
    Serial.println("Wi-Fi not connected!");
    WiFi.reconnect();
  }
}

// MARK: sendData
// Function to send data to Blynk
void sendData() {
  Blynk.virtualWrite(V0, weather);      // Sending String to Virtual Pin V0
  Blynk.virtualWrite(V1, humidity);     // Sending int    to Virtual Pin V1
  Blynk.virtualWrite(V2, temperature);  // Sending float  to Virtual Pin V2
}
