#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>

// WiFi & MQTT
const char* ssid = "EliteWifi";
const char* mqtt_server = "163.172.149.51";  // Din broker-IP

const char* mqtt_user = "guest";
const char* mqtt_pass = "guest";

WiFiClient espClient;
PubSubClient client(espClient);

// URM37 UART
HardwareSerial URM(1);
uint8_t EnTempCmd[4] = { 0x11, 0x00, 0x00, 0x11 };
uint8_t DistCmd[4] = { 0x22, 0x00, 0x00, 0x22 };

void setup_wifi() {
  WiFi.begin(ssid);
  Serial.print("Ansluter till WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi anslutet");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Ansluter till MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
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
  URM.begin(9600, SERIAL_8N1, 45, 46);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // --- Temperatur
  URM.flush();
  URM.write(EnTempCmd, sizeof(EnTempCmd));
  delay(80);
  uint8_t tResp[4];
  int tLen = 0;
  while (URM.available() && tLen < 4) tResp[tLen++] = URM.read();
  float temp = NAN;
  if (tLen == 4 && tResp[0] == 0x11) {
    temp = ((tResp[1] << 8) | tResp[2]) / 10.0;
  }

  // --- Avstånd
  URM.flush();
  URM.write(DistCmd, sizeof(DistCmd));
  delay(80);
  uint8_t dResp[4];
  int dLen = 0;
  while (URM.available() && dLen < 4) dResp[dLen++] = URM.read();
  float dist = NAN;
  if (dLen == 4 && dResp[0] == 0x22) {
    dist = ((dResp[1] << 8) | dResp[2]);
  }

  // --- Skapa JSON-payload
  char payload[100];
  snprintf(payload, sizeof(payload), "{\"temperature\":%.1f,\"distance\":%.1f}", temp, dist);
  Serial.println(payload);

  // --- Publicera om ansluten
  if (client.connected()) {
    client.publish("sensor/urm37", payload);
  }

  delay(5000);
}
