
/**
 * @file simpleMqttBroker.ino
 * @author Alex Cajas (alexcajas505@gmail.com)
 * @brief
 * Simple example of using this Mqtt Broker
 * @version 1.0.0
 */

#include <WiFi.h>
#include <PubSubClient.h>

#include "EmbeddedMqttBroker.h"
using namespace mqttBrokerName;
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;


/******************* mqtt broker ********************/
uint16_t mqttPort = 1883;
MqttBroker broker(mqttPort);
WiFiClient espClient;

PubSubClient clientMqtt(espClient);

void callback(char *topic, byte *payload, unsigned int length);

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

    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(500);
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

    clientMqtt.setCallback(callback);
}

void loop() {
    if (!clientMqtt.connected()) {
        reconnect();
    }
    clientMqtt.loop();
}


void callback(char *topic, byte *payload, unsigned int length) {
    Serial.printf("message  [%s] %u ", topic, length);
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}