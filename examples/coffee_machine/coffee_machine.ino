#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <viot.h>


const char* ssid = "Mani's sick WiFi";
const char* password = "ketamine";
const char* apiKey = "66307eb0aa2570c4c9ac6d7048eee495d518c76cdcf35f47b70829262aad9c2f";

const char* sensor_name = "COFFEE_MACHINE";
const char* ota_password = "141569";

WiFiClient espClient;
viot viotClient(espClient);

void small_coffee(DynamicJsonDocument doc){ 
  Serial.println("Making small coffee...");
  digitalWrite(5, HIGH);
  delay(200);
  digitalWrite(5, LOW);
}

void large_coffee(DynamicJsonDocument doc){
  Serial.println("Making large coffee...");
  digitalWrite(4, HIGH);
  delay(200);
  digitalWrite(4, LOW);
}

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
	delay(500);
	Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup_ota() {
  ArduinoOTA.setHostname(sensor_name);
  ArduinoOTA.setPassword(ota_password);

  ArduinoOTA.onStart([]() {
	Serial.println("Starting");
  });
  ArduinoOTA.onEnd([]() {
	Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
	Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
	Serial.printf("Error[%u]: ", error);
	if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
	else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
	else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
	else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
	else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void reconnect() {
  while (!viotClient.connected()) {
	Serial.print("Attempting MQTT connection...");

	if (viotClient.connect()) {
	  Serial.println("Connected");
	} else {
	  Serial.print("failed, rc=");
	//   Serial.print(viotClient.state());
	  Serial.println(" try again in 5 seconds");
	  // Wait 5 seconds before retrying
	  delay(5000);
	}
  }
}

void setup() {
	Serial.begin(115200);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);

	Serial.println("Initializing coffee machine...");

	setup_wifi();
	setup_ota();

	viotClient.setApiKey(apiKey);
	viotClient.setClient(espClient);
	viotClient.setState("{}");

	if(!viotClient.connected())
		reconnect();

	viotClient.on("small", small_coffee);
	viotClient.on("large", large_coffee);

}

void loop() {
  
  if (WiFi.status() != WL_CONNECTED) {
	delay(1);
	Serial.print("WIFI Disconnected. Attempting reconnection.");
	setup_wifi();
	return;
  }

	
  ArduinoOTA.handle();

  viotClient.loop();
  delay(20);

}