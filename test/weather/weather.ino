/*
 * Code for getting weather data from OpenWheatherMap API
 *
 * Made by: NIHAL T P
 * GitHub: https://github.com/nihaltp
 * LinkedIn: https://www.linkedin.com/in/nihal-tp/
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials  // TODO: Change credentials
warning "Change the values of ssid and password before running the code."
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// OpenWeatherMap API  // TODO: Change values
warning "Change the values of apiKey, city, and countryCode before running the code."
const char* apiKey = "YOUR_OPENWEATHERMAP_API_KEY";
const char* city = "YOUR_CITY";
const char* countryCode = "IN";

// API URL
String weatherURL = "http://api.openweathermap.org/data/2.5/weather?q=" + String(city) + "," + String(countryCode) + "&appid=" + String(apiKey) + "&units=metric";

#define timeInterval 60000
long int timeNow = 0;

// MARK: setup
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");

  fetchWeatherData();
  timeNow = millis();
}

void loop() {
  // Fetch data if there is any Serial input
  if (Serial.available() > 0) {
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
        float temperature = doc["main"]["temp"];                         // Get temperature
        int humidity = doc["main"]["humidity"];                          // Get humidity
        String weather = doc["weather"][0]["description"].as<String>();  // Get weather condition

        Serial.println("Temperature: " + String(temperature) + "°C");
        Serial.println("Humidity: " + String(humidity) + "%");
        Serial.println("Weather: " + weather);
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
