#include "WebSocketsServer.h"
#include <WiFi.h>

#define TOWS_SIZE 256
void hexdump(const void *mem, uint32_t len, uint8_t cols = 16);


void _proxyWebSocketEvent(WebSocketsServerCore *server, uint8_t num, WStype_t type, uint8_t * payload, size_t length);

class ProxyWebSocketsServer : public WebSocketsServer {

    friend void _proxyWebSocketEvent(WebSocketsServerCore *server, uint8_t num, WStype_t type, uint8_t * payload, size_t length);

  public:

    ProxyWebSocketsServer(uint16_t port,
                          const String &destHost,
                          uint16_t destPort,
                          const String & origin = "",
                          const String & protocol = "arduino") :
        _destHost(destHost),
        _destPort(destPort),
        WebSocketsServer(port, origin, protocol) {
        onEvent(_proxyWebSocketEvent);
        begin();
    }
    // virtual ~ProxyWebSocketsServer(void);

    void loop(void) {
        _loop();
        WebSocketsServer::loop();
    }

  private:
    void _loop(void);
    String _destHost;
    uint16_t _destPort;
    WiFiClient *_dest[WEBSOCKETS_SERVER_CLIENT_MAX] = {nullptr};
};

