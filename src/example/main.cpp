
/**
 * @file simpleMqttBroker.ino
 * @author Alex Cajas (alexcajas505@gmail.com)
 * @brief
 * Simple example of using this Mqtt Broker
 * @version 1.0.0
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ProxyWebSocketsServer.h>

#include "EmbeddedMqttBroker.h"
using namespace mqttBrokerName;
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

#define PROXY_DEST "127.0.0.1"
#define TOWS_SIZE 256
ProxyWebSocketsServer *webSocket; 

/******************* mqtt broker ********************/
uint16_t mqttPort = 8884;
MqttBroker broker(mqttPort);
WiFiClient espClient;

PubSubClient clientMqtt(espClient);

void callback(char *topic, byte *payload, unsigned int length);
// void webSocketEvent(WSclient_t *client,uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void wifi_event_cb(WiFiEvent_t event, WiFiEventInfo_t info);
void wifiScan(void);

void reconnect() {
    // Loop until we're reconnected
    while (!clientMqtt.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (clientMqtt.connect("arduinoClient")) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            clientMqtt.publish("outTopic","hello world");
            // ... and resubscribe
            clientMqtt.subscribe("blah");
        } else {
            Serial.print("failed, rc=");
            Serial.print(clientMqtt.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void hexdump(const void *mem, uint32_t len, uint8_t cols) {
    const uint8_t* src = (const uint8_t*) mem;
    Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
    for(uint32_t i = 0; i < len; i++) {
        if(i % cols == 0) {
            Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
        }
        Serial.printf("%02X ", *src);
        src++;
    }
    Serial.printf("\n");
}


void setup() {

    /**
     * @brief To see outputs of broker activity
     * (message to publish, new client's id etc...),
     * set your core debug level higher to NONE (I recommend INFO level).
     * More info: @link https://github.com/alexCajas/EmbeddedMqttBroker @endlink
     */
    delay(3000);
    Serial.begin(115200);
    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.println("simple mqtt broker");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.useStaticBuffers(true);
    WiFi.mode(WIFI_STA);
    // WiFi.disconnect();
    delay(100);
    // wifiScan();
    // WiFi.enableProv(true);
    WiFi.onEvent(wifi_event_cb);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(1000);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");

    // Start the mqtt broker
    broker.setMaxNumClients(9); // set according to your system.
    broker.startBroker();
    Serial.println("broker started");

    // Print the IP address
    Serial.print("Use this ip: ");
    Serial.println(WiFi.localIP());
    Serial.print("and this port: ");
    Serial.println(mqttPort);
    Serial.println("To connect to mqtt broker");

    // conect mqtt client to broker
    IPAddress ip = WiFi.localIP();
    clientMqtt.setServer(ip, mqttPort);
    clientMqtt.setBufferSize(255);

    // clientMqtt.setCallback(callback);
    webSocket = new ProxyWebSocketsServer(8883, PROXY_DEST, mqttPort);
}
uint32_t last;
#define INTERVAL 1000

void loop() {
    if (!clientMqtt.connected()) {
        reconnect();
    }
    clientMqtt.loop();
    if (webSocket)
        webSocket->loop();

    if (millis() - last > INTERVAL) {
        last = millis();
        clientMqtt.publish("blah", "fasel");
    }
}


// void callback(char *topic, byte *payload, unsigned int length) {
//     Serial.printf("message  [%s] %u ", topic, length);
//     for (int i = 0; i < length; i++) {
//         Serial.print((char)payload[i]);
//     }
//     Serial.println();
// }