#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <IRsend.h>
#include <viot.h>


const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.


const char* ssid = "6 Cemetery Avenue - 2.4";
const char* password = "20551599";
const char* apiKey = "3cf9476e0cdf9c510a237491e7d00f8edce673ecede357b807d25fa2faa125a5";

WiFiClient espClient;
viot viotClient(espClient);

void tv_power(DynamicJsonDocument doc){ irsend.sendNEC(0x2FD48B7, 32); }
void menu(DynamicJsonDocument doc){ irsend.sendNEC(0x27D2CD3, 32); }
void up(DynamicJsonDocument doc){ irsend.sendNEC(0x2FD9867, 32); }
void left(DynamicJsonDocument doc){ irsend.sendNEC(0x2FD42BD, 32); }
void right(DynamicJsonDocument doc){ irsend.sendNEC(0x2FD02FD, 32); }
void down(DynamicJsonDocument doc){ irsend.sendNEC(0x2FDB847, 32); }
void ok(DynamicJsonDocument doc){ irsend.sendNEC(0x2FD847B, 32); }
void back(DynamicJsonDocument doc){ irsend.sendNEC(0x2FD26D9, 32); }
void _exit(DynamicJsonDocument doc){ irsend.sendNEC(0x2FDC23D, 32); }

void volume_up(DynamicJsonDocument doc){ irsend.sendNEC(0x2FD58A7, 32); }
void volume_down(DynamicJsonDocument doc){ irsend.sendNEC(0x2FD7887, 32); }

void channel_up(DynamicJsonDocument doc){ irsend.sendNEC(0x2FDD827, 32); }
void channel_down(DynamicJsonDocument doc){ irsend.sendNEC(0x2FDF807, 32); }
void mute(DynamicJsonDocument doc){ irsend.sendNEC(0x2FD08F7, 32); }
void info(DynamicJsonDocument doc){ irsend.sendNEC(0x2FD6897, 32); }
void netflix(DynamicJsonDocument doc){ irsend.sendNEC(0x27D54AB, 32); }
void input(DynamicJsonDocument doc){ irsend.sendNEC(0x2FD28D7, 32); }
void fp(DynamicJsonDocument doc){ irsend.sendNEC(0x27DE11E, 32); }

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

void setup(){
    Serial.begin(9600);
    irsend.begin();
    setup_wifi();
//    viotClient.setServer("192.168.0.\108", 1883);
    viotClient.setApiKey(apiKey);
    viotClient.setClient(espClient);
    // viotClient.setTemplate(viotTemplate);
    viotClient.setState("{}");

    if(!viotClient.connected())
        reconnect();

      

    // Serial.println(strlen(viotTemplate));


    viotClient.on("tv-power", tv_power);
    viotClient.on("menu", menu);
    viotClient.on("up", up);
    viotClient.on("left", left);
    viotClient.on("right", right);
    viotClient.on("down", down);
    viotClient.on("ok", ok);
    viotClient.on("back", back);
    viotClient.on("exit", _exit);
    viotClient.on("volume-up", volume_up);
    viotClient.on("volume-down", volume_down);
    viotClient.on("channel-up", channel_up);
    viotClient.on("channel-down", channel_down);
    viotClient.on("mute", mute);
    viotClient.on("info", info);
    viotClient.on("netflix", netflix);
    viotClient.on("input", input);
    viotClient.on("freeview-play", fp);

    

    Serial.println("Connected to wifi, setting the VIOT server url");

}

void loop(){
    viotClient.loop();
    delay(20);
}
