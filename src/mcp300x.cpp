#include <wiringPi.h>

#include "mcp300x.h"

Mcp300x::Mcp300x(int pinClock, int pinDigitalOut, int pinDigitalIn, int pinChipSelect) :
  pinClock(pinClock), pinDigitalOut(pinDigitalOut), pinDigitalIn(pinDigitalIn), pinChipSelect(pinChipSelect) {
  pinMode(pinClock, OUTPUT);
  pinMode(pinDigitalOut, INPUT);
  pinMode(pinChipSelect, OUTPUT);
  pinMode(pinDigitalIn, OUTPUT);
}

Mcp300x::~Mcp300x() {
  // nitpick: restore pins as input
  pinMode(pinClock, INPUT);
  // pinMode(pinDigitalOut, INPUT);
  pinMode(pinChipSelect, INPUT);
  pinMode(pinDigitalIn, INPUT);
}

int Mcp300x::readAnalog(int pinChannel) const {
  Int8U cmdOut;
  int cmdOutBits;
  int valueOut = 0;
  
  if (pinChannel < 0 || pinChannel > lastChannelPin()) {
    return -1;
  }

  // obtain command out bits + pinChannel, based on chip
  getCmdOutInfo(pinChannel, cmdOut, cmdOutBits);

  // initiate communication with device
  // toggle cs and start clock low  
  digitalWrite(pinChipSelect, HIGH);
  digitalWrite(pinClock, LOW);
  delay(1);  // let things settle in...
  digitalWrite(pinChipSelect, LOW);
  
  for (int i = 0; i < cmdOutBits; ++i) {
    digitalWrite(pinDigitalIn, (cmdOut & 0x80) ? HIGH : LOW);
    cmdOut <<= 1; // shift out bit just used
    tickClock();
  }

  tickClock(); // read (skip) one empty bit  

  // read 10 ADC bits
  for (int i = 0; i < 10; ++i) {
    tickClock();
    valueOut <<= 1; // make room for next bit
    if (digitalRead(pinDigitalOut) == HIGH) {
      valueOut |= 1;
    }
  }

  tickClock(); // read (skip) null bit
  
  return valueOut;
}

void Mcp300x::tickClock() const {
    digitalWrite(pinClock, HIGH);
    digitalWrite(pinClock, LOW);
}
