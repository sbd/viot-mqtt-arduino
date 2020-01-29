
#include <Arduino.h>
#include <map>
#include "Client.h"
#include <ArduinoJson.h>

#define MQTT_MAX_PACKET_SIZE 1024

#ifndef viot_h
#define viot_h

class viot{
    private:
        const char* apiKey;
        const char* domain;
        const char* _template;
        const char* _state;
        uint16_t port;

        void callback(char* topic, byte* payload, unsigned int length);
        void cb(char* topic, byte* payload, unsigned int length);
        String getTopic(const char* topic);
        std::map<String, std::function<void (DynamicJsonDocument json)>> _events;

    public:
        viot();
        viot(Client& client);

        viot& setClient(Client& client);
        viot& setServer(const char * domain, uint16_t port);
        viot& setApiKey(const char * apiKey);
        viot& setTemplate(const char* viotTemplate);
        viot& setState(const char* state);

        

        void on(const char* event, std::function<void (DynamicJsonDocument)>);
    
        boolean connected();
        boolean connect();
        boolean connect(const char* apiKey);
        boolean loop();
};

#endif