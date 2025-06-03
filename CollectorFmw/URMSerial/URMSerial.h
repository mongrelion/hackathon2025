
#ifndef URMSERIAL_H
#define URMSERIAL_H

#include <Arduino.h>
#include <HardwareSerial.h>

class URMSerial {
public:
    URMSerial();
    void begin(uint8_t rx, uint8_t tx, long baudRate);
    long requestDistance();  // returns distance in mm

private:
    HardwareSerial* serialPort;
};

#endif
