# mcp300x
c++ codebase to control analog to digital converter in Raspberry Pi

This small c++ codebase uses the [wiringPi][wpi] library to control a [mcp300x][mcp] analog to digital converter (ADC). You can use this as a standalone program, or embed it into your [raspberry pi][rpi] project.

### Install steps

##### 1) Install the **build essential package**
    sudo apt-get install build-essential

##### 2) Install [wiringPi][wpi]
    cd someDir
    git clone git://git.drogon.net/wiringPi
    cd wiringPi && ./build
    
##### 3) Clone and build this project
    cd someDir
    git clone https://github.com/flavio-fernandes/mcp300x
    cd mcp300x && make

### Usage

To use the standalone program, call **./mcp300x** or copy it to a location used by your **$PATH**. The _--help_ should give you all you need to know:

    $ ./mcp300x --help
    Usage: mcp300x [OPTION...] 
    mcp300x, a simple program to read analog values from Mcp3002 or Mcp3008

      -2, --mcp3002              Using mcp3002 (default)
      -8, --mcp3008              Using mcp3008
      -c, --clock=gpioPin        Clock GPIO pin
      -d, --delay=milliseconds   Delay per sample (0 => none)
      -i, --digitaln=gpioPin, --mosi=gpioPin
                                 Digital In GPIO pin (write command)
      -o, --digitalout=gpioPin, --miso=gpioPin
                                 Digital Out GPIO pin (read values)
      -r, --repeats[=count]      Number of sample reads (0 => infinite)
      -s, --chipSelect=gpioPin   Chip Select GPIO pin

The default gpio pins are hard coded in _[main.cpp][maincpp]_. Feel free to modify them to suit your needs.

In order to use the Mcp300x code, use the files located in the **[src][srcdir]** directory. They should be self contained and easy enough to port over to your project. If not, please submit a pull request and help me make this better.

Last, but certainly not least, let me list a few additional links that were essential in the making of this repo:
  - [Microchip doc](http://ww1.microchip.com/downloads/en/DeviceDoc/21294C.pdf)
  - [Adafruit Learn](https://learn.adafruit.com/reading-a-analog-in-and-controlling-audio-volume-with-the-raspberry-pi)
  - [Ladyada Youtube](https://youtu.be/6OCuAdEn9ZI?t=17m45s)
  - [dmt195 today](https://dmt195.wordpress.com/2012/09/26/mcp3002-example-code-for-raspberry-pi-adc-through-spi/)
  - [Arduino Wiring](https://ms-iot.github.io/content/en-US/win10/samples/arduino-wiring/Potentiometer.htm)

Enjoy!

[wpi]: http://wiringpi.com/download-and-install/ "Wiring Pi"
[rpi]: https://www.raspberrypi.org/ "Raspberry Pi"
[mcp]: http://educypedia.karadimov.info/library/21841A.pdf "Analog-to-Digital Converter"
[maincpp]: https://github.com/flavio-fernandes/mcp300x/blob/master/main.cpp "main.cpp"
[srcdir]: https://github.com/flavio-fernandes/mcp300x/tree/master/src "src directory"
