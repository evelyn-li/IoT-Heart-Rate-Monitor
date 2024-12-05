#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Bio_Sensor_Hub_Library.h>

const int RESET_PIN = 26;
const int MFIO_PIN = 27;

SparkFun_Bio_Sensor_Hub bioHub(RESET_PIN, MFIO_PIN); // sensor
bioData body; // sensor data


void setup()
{
  Serial.begin(9600);
  Wire.begin();

  int status = bioHub.begin(); // initialize sensor
  if (status != 0)
    Serial.println("Could not communicate with the sensor!");

  status = bioHub.configBpm(MODE_ONE); // configure BPM settings
  if (status != 0)
    Serial.println("Failed to configure sensor.");

  Serial.println("Loading up the buffer with data....");
  delay(4000); // account for data lagging
}


void loop()
{
  body = bioHub.readBpm(); // read sensor data
  Serial.print("Heartrate: ");
  Serial.println(body.heartRate); 
  Serial.print("Confidence: ");
  Serial.println(body.confidence); 
  Serial.print("Oxygen: ");
  Serial.println(body.oxygen); 
  Serial.print("Status: ");
  Serial.println(body.status); 

  delay(1000); 
}

