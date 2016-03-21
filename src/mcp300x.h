#ifndef __MCP300X_H

#define __MCP300X_H

// Some really good Refs:
//
//      http://ww1.microchip.com/downloads/en/DeviceDoc/21294C.pdf
//      https://learn.adafruit.com/reading-a-analog-in-and-controlling-audio-volume-with-the-raspberry-pi
//      https://dmt195.wordpress.com/2012/09/26/mcp3002-example-code-for-raspberry-pi-adc-through-spi/
//      https://ms-iot.github.io/content/en-US/win10/samples/arduino-wiring/Potentiometer.htm
//

#ifndef Int8U
typedef unsigned char Int8U;
#endif // ifndef Int8U

class Mcp300x {
public:
  Mcp300x(int pinClock, int pinDigitalOut, int pinDigitalIn, int pinChipSelect);
  virtual ~Mcp300x();

  // Read analog value on provided channel pin. Note that this should be
  // a number from 0 to 'n', depending on the MCP300x model being used.
  // Returns -1 if pinChannel is invalid, or a 10bit value (0 to 1023).
  int readAnalog(int pinChannel) const;

  virtual int lastChannelPin() const = 0;

protected:
  virtual void getCmdOutInfo(const int pinChannel, Int8U& cmdOut, int& cmdOutBitsCount) const = 0;

private:
  // Note: If you need to change these pins, simply instantiate new Mcp300x
  //       (... or make these non-const public members :))
  const int pinClock;
  const int pinDigitalOut;  // aka miso. read from this pin
  const int pinDigitalIn;   // aka mosi. write to this pin
  const int pinChipSelect;

  void tickClock() const;
  
  // not implemented
  Mcp300x();  // = delete;
  Mcp300x(const Mcp300x& other);  // = delete;
  Mcp300x& operator=(const Mcp300x& other);  // = delete;
}; // Mcp300x


class Mcp3002 : public Mcp300x {
public:
  Mcp3002(int pinClock, int pinDigitalOut, int pinDigitalIn, int pinChipSelect) :
    Mcp300x(pinClock, pinDigitalOut, pinDigitalIn, pinChipSelect) { }

  virtual int lastChannelPin() const { return 1; }
  virtual void getCmdOutInfo(const int pinChannel, Int8U& cmdOut, int& cmdOutBitsCount) const {
    cmdOut = 0x6 | pinChannel;  // sgl and odd bits, plus the channel to be used
    cmdOutBitsCount = 3;
    cmdOut <<= (8 - cmdOutBitsCount);
  }
}; // Mcp3002


class Mcp3008 : public Mcp300x {
public:
  Mcp3008(int pinClock, int pinDigitalOut, int pinDigitalIn, int pinChipSelect) :
    Mcp300x(pinClock, pinDigitalOut, pinDigitalIn, pinChipSelect) { }

  virtual int lastChannelPin() const { return 7; }
  virtual void getCmdOutInfo(const int pinChannel, Int8U& cmdOut, int& cmdOutBitsCount) const {
    cmdOut = 0x18 | pinChannel;  // sgl and odd bits, plus the channel to be used
    cmdOutBitsCount = 5;
    cmdOut <<= (8 - cmdOutBitsCount);
  }
}; // Mcp3008

#endif // ifndef __MCP300X_H
