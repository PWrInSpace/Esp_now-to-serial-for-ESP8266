#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t targetAdress[] = {0x94, 0x3C, 0xC6, 0x24, 0xE3, 0x54};
bool ledState;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {

  if (!sendStatus) Serial.println("Sent");
}

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {

  Serial.println((char*) incomingData);
}

void setup() {
  
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  Serial.setTimeout(10);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_now_init();

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(targetAdress, ESP_NOW_ROLE_SLAVE, 0, NULL, 0);

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
  if (Serial.available()) {

    ledState = !ledState;

    String rxData = "";

      while(Serial.available()) {

          rxData += Serial.readString();
          delay(2);
      }


    digitalWrite(2, ledState);

    esp_now_send(targetAdress, (uint8_t*) rxData.c_str(), rxData.length());
  }
}