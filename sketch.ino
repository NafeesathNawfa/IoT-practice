#include <DHT.h>

#define DHTPIN 15     // GPIO pin connected to DHT22 data pin
#define DHTTYPE DHT22 // Sensor model type

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing DHT22 Sensor...");
  
  dht.begin();
}

void loop() {
  // Read humidity and temperature (Celsius)
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();

  // Check if readings failed (NaN = Not a Number)
  if (isnan(humidity) || isnan(tempC)) {
    Serial.println("Error: Failed to read from DHT22 sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity, 1);
    Serial.print("% \t Temperature: ");
    Serial.print(tempC, 1);
    Serial.println("°C");
  }

  // Wait 1 second (1000ms) before reading again
  delay(1000);
}