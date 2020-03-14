#include "viot.h"

#include "PubSubClient.h"


#include "Arduino.h"
#include <ArduinoJson.h>

PubSubClient mqtt;

viot::viot(){
	Serial.println("viot initialized with no options..");
	setServer("mqtt.viot.co.uk", 1883);
}

viot::viot(Client& client){
	delay(500);
	Serial.println("viot initialized with client");
	setClient(client);
	setServer("mqtt.viot.co.uk", 1883);
}

viot& viot::setServer(const char * domain, uint16_t port) {
	Serial.println();
	Serial.print("setServer(");
	Serial.print(domain);
	Serial.print(", ");
	Serial.print(port);
	Serial.println(")");
	this->domain = domain;
	this->port = port;

	mqtt.setServer(domain, port);
	return *this;
}

viot& viot::setState(const char * state){
	// TODO: need to update this to call state updated.
	this->_state = state;
	return *this;

}

viot& viot::setTemplate(const char * viotTemplate){
	this->_template = viotTemplate;
	return *this;
}

viot& viot::setClient(Client& client){
	Serial.print("Set client called");

	mqtt.setClient(client);

	return *this;
}


viot& viot::setApiKey(const char* apiKey){
	Serial.print("\nSet api key to");
	Serial.println(apiKey);

	this->apiKey = apiKey;
}

void viot::on(const char* event, std::function<void (DynamicJsonDocument json)> func){
	Serial.println("viot on called");
	_events[event] = func;
}

boolean viot::connect(const char* apiKey){
	setApiKey(apiKey);
	return connect();
}

String viot::getTopic(const char* command){
	String topic = "device/";
	topic += this->apiKey;
	topic += "/";
	topic += command;

	const char* _topic = topic.c_str(); 
	Serial.println("THE FINAL TOPIC IS:");
	Serial.println(_topic); 
	Serial.println("that's it.");
	return topic;

	// return "this-tiopci";
}

void viot::cb(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");

  Serial.print(topic);
  Serial.print("] ");

	char json[length];
	for (int i = 0; i < length; i++) {
		json[i] = (char) payload[i];
		Serial.print((char)payload[i]);
	}

	DynamicJsonDocument doc(1024);
	deserializeJson(doc, json);

	const char* command = doc["command"];

	String emitTopic = getTopic("emit");
	if(strcmp(command, "status") == 0){
		const char* connectedPayload = "{\"command\": \"status\", \"message\": \"online\"}";
		mqtt.publish(emitTopic.c_str(), connectedPayload);
	}
	else if(strcmp(command, "template") == 0){
		const char* response = "{\"command\": \"template\", \"message\": {\"state\":{}}}";
		// char buffer[strlen(this->_template) + strlen(_state) + 5];

		// sprintf(buffer, templateJson, this->_state);
		Serial.println("TEMPLATE PUBLISH....");


		mqtt.publish(emitTopic.c_str(), response);
	} else {

		Serial.println("fail");
		Serial.println(command);

		auto e = _events.find(command);
		if(e != _events.end()) {
			// SOCKETIOCLIENT_DEBUG("[SIoC] trigger event %s\n", event);
			e->second(doc);
		} else {
			Serial.println("event not found..");
			Serial.println(command);
			// SOCKETIOCLIENT_DEBUG("[SIoC] event %s not found. %d events available\n", event, _events.size());
		}

	}

	

}

boolean viot::connect(){
	String clientId = "ESP8266Client-";
	clientId += String(random(0xffff), HEX);

	Serial.print("our api key is");
	Serial.println(this->apiKey);

	Serial.println("Attempting mqtt connection with mqtt lib");
	bool connected = mqtt.connect(clientId.c_str(), this->apiKey, this->apiKey);
	if(!connected) return false;

	mqtt.setCallback(std::bind(&viot::cb, this,  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	Serial.println("connected to connect..");

	String receiveTopic = getTopic("receive");
	String emitTopic = getTopic("emit");
	
	mqtt.subscribe(receiveTopic.c_str());

	const char* connectedPayload = "{\"command\": \"connect\"}";
	mqtt.publish(emitTopic.c_str(), connectedPayload);

	return true;
}


boolean viot::connected(){
	return mqtt.connected();
}

boolean viot::loop(){
	return mqtt.loop();
}
