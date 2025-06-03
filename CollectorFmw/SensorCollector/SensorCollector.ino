#include <HardwareSerial.h>

HardwareSerial URM(1);

uint8_t EnTempCmd[4] = { 0x11, 0x00, 0x00, 0x11 };
uint8_t DistCmd[4] = { 0x22, 0x00, 0x00, 0x22 };

void setup() {
  Serial.begin(115200);
  URM.begin(9600, SERIAL_8N1, 45, 46);
  delay(100);
  Serial.println("URM37 lokal JSON-utskrift...");
}

void loop() {
  float temp = NAN;
  float dist = NAN;

  // TEMP
  URM.flush();
  URM.write(EnTempCmd, sizeof(EnTempCmd));

  // Vänta in 4 bytes, max 100 ms
  uint8_t tResp[4];
  int tReceived = 0;
  unsigned long tStart = millis();
  while (tReceived < 4 && (millis() - tStart) < 100) {
    if (URM.available()) {
      tResp[tReceived++] = URM.read();
    }
  }

  if (tReceived == 4 && tResp[0] == 0x11) {
    uint16_t rawTemp = (tResp[1] << 8) | tResp[2];
    temp = rawTemp / 10.0;
  } else {
    Serial.print("Felaktigt svar (");
    Serial.print(tReceived);
    Serial.println(" bytes)");
  }

  delay(100);

  // DIST
  URM.flush();
  URM.write(DistCmd, sizeof(DistCmd));

  // Vänta in 4 bytes, max 100 ms
  uint8_t dResp[4];
  int dReceived = 0;
  unsigned long dStart = millis();
  while (dReceived < 4 && (millis() - dStart) < 100) {
    if (URM.available()) {
      dResp[dReceived++] = URM.read();
    }
  }
  if (dReceived == 4 && dResp[0] == 0x22) {
    uint16_t rawDist = (dResp[1] << 8) | dResp[2];
    dist = rawDist;
  } else {
    Serial.print("Felaktigt svar (");
    Serial.print(dReceived);
    Serial.println(" bytes)");
  }

  char payload[100];
  snprintf(payload, sizeof(payload), "{\"temperature\":%.1f,\"distance\":%.1f}", temp, dist);
  Serial.println(payload);

  delay(5000);
}
