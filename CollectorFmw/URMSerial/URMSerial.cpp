
#include "URMSerial.h"

URMSerial::URMSerial() {}

void URMSerial::begin(uint8_t rx, uint8_t tx, long baudRate) {
    serialPort = new HardwareSerial(1);  // UART1
    serialPort->begin(baudRate, SERIAL_8N1, rx, tx);
}

long URMSerial::requestDistance() {
    uint8_t cmd[] = {0x55, 0x00, 0x02, 0x01, 0x01};  // Distance measurement command
    serialPort->write(cmd, sizeof(cmd));

    unsigned long start = millis();
    while (serialPort->available() < 4) {
        if (millis() - start > 100) return -1; // timeout
    }

    uint8_t header = serialPort->read();
    if (header != 0x55) return -1;

    uint8_t index = serialPort->read();    // Not used
    uint8_t distH = serialPort->read();
    uint8_t distL = serialPort->read();

    return ((long)distH << 8) | distL;  // mm
}
