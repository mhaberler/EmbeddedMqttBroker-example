
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WebSocketsServer.h>
#include <ProxyWebSocketsServer.h>
#include "IPAddress.h"
#include "Client.h"
#include "Stream.h"
#include <unordered_map>
using namespace std;



// unordered_map<uint8_t, WiFiClient *> proxy_clients;



// WiFiClient proxyConns[WEBSOCKETS_SERVER_CLIENT_MAX];

// void hexdump(const void *mem, uint32_t len, uint8_t cols = 16);


// void toWs(void) {
//     for (auto& pc: proxy_clients) {
//         WiFiClient *cp = pc.second;
//         while (cp->available()) {
//             uint8_t buffer[TOWS_SIZE];
//             int res = cp->read(buffer, sizeof(buffer));


//         }
//     }
//     // cout << " " << x.first << ":" << x.second << endl;
// }


// void webSocketEvent(WSclient_t *client, uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

//     // client->
//     WiFiClient *cp = NULL;

//     auto p = proxy_clients.find(num);
//     if ( p != proxy_clients.end() ) {
//         cp = p->second;
//     }

//     switch(type) {
//         case WStype_DISCONNECTED:
//             Serial.printf("[%u] Disconnected!\n", num);
//             if (cp && cp->connected()) {
//                 cp->stop();
//             }
//             break;
//         case WStype_CONNECTED: {
//                 if (cp && cp->connected()) {
//                     cp->stop(); 
//                 }
//                 IPAddress ip = webSocket.remoteIP(num);
//                 Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

//                 WiFiClient *cp = new WiFiClient;
//                 if (!cp->connect(PROXY_DEST, PROXY_PORT)) {
//                     delete cp;
//                     log_e("proxy connect failed");
//                     break;
//                 }
//                 proxy_clients[num] = cp;
//                 log_i("proxy connect success %u", num);
//             }
//             break;
//         case WStype_TEXT:
//             Serial.printf("[%u] get Text: %s\n", num, payload);
//             if (cp && cp->connected()) {
//                 cp->write(payload, length);
//             }
//             break;
//         case WStype_BIN:
//             Serial.printf("[%u] get binary length: %u\n", num, length);
//             hexdump(payload, length);
//             if (cp && cp->connected()) {
//                 cp->write(payload, length);
//             }
//             // send message to client
//             // webSocket.sendBIN(num, payload, length);
//             break;
//         case WStype_ERROR:
//         case WStype_FRAGMENT_TEXT_START:
//         case WStype_FRAGMENT_BIN_START:
//         case WStype_FRAGMENT:
//         case WStype_FRAGMENT_FIN:
//             Serial.printf("wsmessage %u\n", type);

//             break;
//     }

// }
