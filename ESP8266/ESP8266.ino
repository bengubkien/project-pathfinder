#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

WiFiClient espClient;
PubSubClient client(espClient);

const char *ssid = "Fibertel WiFi979 2.4GHz";
const char *password = "0041391611";
const char *broker = "mqtt.beebotte.com";
const char *topic = "project_pathfinder/motor_control";
const char *user = "token_jIKumPGtxHdtKuMa";
const int port = 1883;

#define SLAVE_ADDR 1

void setup() {
  Wire.begin();            // Initialize I2C communications as master.
  Serial.begin(115200);    // Set-up of the serial monitor.
  setupWiFi();             // Set-up Wi-Fi.
  setupMQTT();             // Set-up communications protocol with the app (MQTT).
}

void loop() {
 client.loop();
}

void setupWiFi()
{
  delay(100);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
   delay(1000);
   Serial.println("Connecting to WiFi...");
  }
  Serial.println("ESP8266 connected to WiFi.");

}

void setupMQTT()
{
  client.setServer(broker,port);
  client.setCallback(callback);
  while (!client.connected()) {
     String client_ID = "ESP8266";
     Serial.printf("%s connected through MQTT.\n", client_ID.c_str());
     if (client.connect(client_ID.c_str(), user, NULL)) {
         Serial.println("Beebotte connected.");
     } else {
         Serial.print("Error in the MQTT connection with state ");
         Serial.print(client.state());
         delay(2000);
     }
  }
  client.publish(topic, "ESP8266 connected to topic.");
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message obtained in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");

  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(payload, length);
  Wire.endTransmission();

  Serial.println("Command sent to slave.");
}

