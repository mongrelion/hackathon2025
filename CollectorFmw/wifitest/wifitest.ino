#include <WiFi.h>
#include <PubSubClient.h>

// WiFi & MQTT-inställningar
const char* ssid = "EliteWifi";               // Öppet nätverk
const char* mqtt_server = "192.168.1.10";     // Byt till din MQTT-broker IP

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.print("Ansluter till WiFi");
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi anslutet");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Ansluter till MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("ansluten.");
    } else {
      Serial.print("felkod=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // Skicka test-payload
  const char* payload = "{\"status\":\"OK\"}";
  client.publish("sensor/test", payload);
  Serial.println(payload);

  delay(5000);
}
