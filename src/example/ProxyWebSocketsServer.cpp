#include <ProxyWebSocketsServer.h>

void ProxyWebSocketsServer::_loop(void) {
    for (int i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
        WiFiClient *d = _dest[i];

        while (d && d->available()) {
            uint8_t buffer[TOWS_SIZE];
            int res = d->read(buffer, sizeof(buffer));
            if ((res > 0) && (_clients[i].status == WSC_CONNECTED)) {
                this->sendBIN(i, buffer, res);
            }
        }
    }
}


void _proxyWebSocketEvent(WebSocketsServerCore *server, uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    ProxyWebSocketsServer *pws = (ProxyWebSocketsServer*)server;

    WSclient_t *wsc = &pws->_clients[num];
    WiFiClient *wc = pws->_dest[num];

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected\n", num);
            if (wc && wc->connected()) {
                wc->stop();
                delete wc;
                pws->_dest[num] = nullptr;
            }
            break;
        case WStype_CONNECTED: {
                if (wc && wc->connected()) {
                    wc->stop();
                    delete wc;
                    pws->_dest[num] = nullptr;
                }
                IPAddress ip = wsc->tcp->remoteIP() ;
                uint16_t port = wsc->tcp->remotePort() ;
                Serial.printf("[%u] Connected from %d.%d.%d.%d port %u url: %s\n", num, ip[0], ip[1], ip[2], ip[3], port, payload);

                WiFiClient *cp = new WiFiClient;

                if (!cp->connect(pws->_destHost.c_str(), pws->_destPort)) {
                    delete cp;
                    pws->_dest[num] = nullptr;
                    log_e("proxy connect failed");
                    break;
                }
                pws->_dest[num] = cp;
                log_i("proxy connect success %u", num);
            }
            break;
        case WStype_TEXT:
            // Serial.printf("[%u] get Text: %s\n", num, payload);
            if (wc && wc->connected()) {
                wc->write(payload, length);
            }
            break;
        case WStype_BIN:
            // Serial.printf("[%u] get binary length: %u\n", num, length);
            // hexdump(payload, length);
            if (wc && wc->connected()) {
                wc->write(payload, length);
            }
            break;
        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            Serial.printf("wsmessage %u\n", type);
            break;
    }
}