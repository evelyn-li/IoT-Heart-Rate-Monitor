#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <WiFi.h>
#include <HttpClient.h>

const int RESET_PIN = 26;
const int MFIO_PIN = 27;

SparkFun_Bio_Sensor_Hub bioHub(RESET_PIN, MFIO_PIN); // sensor
bioData body; // sensor data

char ssid[50] = "UCInet Mobile Access"; // name of wifi network
char pass[50] = "";
const char kHostname[] = "35.164.17.81"; // name of server to connect to


void setup()
{
  Serial.begin(9600);
  Wire.begin();

  // sensor setup
  int status = bioHub.begin();
  if (status != 0)
    Serial.println("Could not communicate with the sensor!");

  status = bioHub.configBpm(MODE_ONE);
  if (status != 0)
    Serial.println("Failed to configure sensor.");

  // connect to a WiFi network
	delay(1000);
	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, pass);
	while (WiFi.status() != WL_CONNECTED) 
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.println("MAC address: ");
	Serial.println(WiFi.macAddress());
}


void loop()
{
  int err = 0;
	WiFiClient c;
	HttpClient http(c);

	// read sensor data
	body = bioHub.readBpm();
	Serial.print("Heartrate: ");
	Serial.println(body.heartRate); 
	Serial.print("Confidence: ");
	Serial.println(body.confidence); 
	Serial.print("Oxygen: ");
	Serial.println(body.oxygen); 
	Serial.print("Status: ");
	Serial.println(body.status); 

	// send data to the cloud server via an HTTP request
	String path = "/data?heartRate=" + String(body.heartRate) + "&confidence=" + String(body.confidence);
	err = http.get(kHostname, 5000, path.c_str(), NULL);

	// error checking
	if (err == 0) 
	{
		Serial.println("startedRequest ok");

		err = http.responseStatusCode();
		if (err >= 0) 
		{
			Serial.print("Got status code: ");
			Serial.println(err);
		} 
		else 
		{
			Serial.print("Getting response failed: ");
			Serial.println(err);
		}
	} 
	else 
	{
		Serial.print("Connect failed: ");
		Serial.println(err);
	}
	http.stop();

  delay(1000); 
}

