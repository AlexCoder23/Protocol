#include <WiFi.h>
#include <WebSocketsClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>

const char* ssid = "best_wifi";
const char* password = "132alhack";

const char* websocket_server = "alexcoder23.ru";
const int websocket_port = 443;
const char* websocket_path = "/wnet/ws";

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_DISCONNECTED) {
    Serial.printf("[WSc] Disconnected!\n");
  } else if (type == WStype_CONNECTED) {
    Serial.printf("[WSc] Connected to url: %s\n", payload);

    webSocket.sendTXT("add@testbot");
  } else if (type == WStype_TEXT) {
    String message = (char*)payload;
    Serial.print("[WSc] Получено: ");
    Serial.println(message);

    if (message == "info@You added as bot") {
      Serial.println("Бот успешно зарегистрирован!");
    } else if (message.startsWith("error@")) {
      Serial.print("Ошибка: ");
      Serial.println(message.substring(6));
    } else if (message.startsWith("mess@")) {
      int firstAt = message.indexOf('@');
      int secondAt = message.indexOf('@', firstAt + 1);
      
      String client_sid = message.substring(firstAt + 1, secondAt);
      String client_msg = message.substring(secondAt + 1);
      
      Serial.print("Сообщение от клиента ");
      Serial.print(client_sid);
      Serial.print(": ");
      Serial.println(client_msg);

      String reply = "Hello from ESP32 bot! You said: " + client_msg;
      webSocket.sendTXT("scsend@" + client_sid + "@" + reply);
    }
  } else if (type == WStype_ERROR) {
      Serial.println("[WSc] Ошибка соединения!");
  }
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi подключен");
  Serial.print("IP адрес: ");
  Serial.println(WiFi.localIP());

  webSocket.setExtraHeaders("Cookie: auth=\"$auth$\"");
  
  webSocket.setReconnectInterval(5000);

  webSocket.onEvent(webSocketEvent);

  webSocket.beginSSL(websocket_server, websocket_port, websocket_path);
}

void loop() {
  webSocket.loop();
}
