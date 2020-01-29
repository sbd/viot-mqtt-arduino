#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <IRsend.h>
#include <viot.h>


const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.


const char* ssid = "Mani's sick WiFi";
const char* password = "ketamine";
const char* apiKey = "6da37620264bdaf6c306ab6a684ce2ef00a044ef5a7b8ce576466c6014352f74";

WiFiClient espClient;
viot viotClient(espClient);

void tv_power(DynamicJsonDocument doc){ irsend.sendRC5(0xC, 12); }
void menu(DynamicJsonDocument doc){ irsend.sendRC5(0x30, 12); }
void up(DynamicJsonDocument doc){ irsend.sendRC5(0x14, 12); }
void left(DynamicJsonDocument doc){ irsend.sendRC5(0x15, 12); }
void right(DynamicJsonDocument doc){ irsend.sendRC5(0x16, 12); }
void down(DynamicJsonDocument doc){ irsend.sendRC5(0x13, 12); }
void ok(DynamicJsonDocument doc){ irsend.sendRC5(0x35, 12); }
void qmenu(DynamicJsonDocument doc){ irsend.sendRC5(0x2B, 12); }
void back(DynamicJsonDocument doc){ irsend.sendRC5(0xA, 12); }
void _exit(DynamicJsonDocument doc){ irsend.sendRC5(0x25, 12); }
void volume_up(DynamicJsonDocument doc){ irsend.sendRC5(0x10, 12); }
void volume_down(DynamicJsonDocument doc){ irsend.sendRC5(0x11, 12); }
void channel_up(DynamicJsonDocument doc){ irsend.sendRC5(0x20, 12); }
void channel_down(DynamicJsonDocument doc){ irsend.sendRC5(0x21, 12); }
void mute(DynamicJsonDocument doc){ irsend.sendRC5(0xD, 12); }
void info(DynamicJsonDocument doc){ irsend.sendRC5(0x12, 12); }
void netflix(DynamicJsonDocument doc){ irsend.sendRC5(0x27, 12); }
void input(DynamicJsonDocument doc){ irsend.sendRC5(0x38, 12); }
void youtube(DynamicJsonDocument doc){ irsend.sendRC5(0x3B, 12); }

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
    viotClient.setServer("192.168.0.108", 1883);
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
    viotClient.on("q-menu", qmenu);
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
    viotClient.on("youtube", youtube);

    

    Serial.println("Connected to wifi, setting the VIOT server url");

}

void loop(){
    viotClient.loop();
    delay(20);
}