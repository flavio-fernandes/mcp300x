# mcp300x
c++ codebase to control analog to digital converter in Raspberry Pi

This small c++ codebase uses the [wiringPi][wpi] library to control a [mpc300x][mpc] analog to digital converter (ADC). You can use this as a standalone program, or embed it into your [raspberry pi][rpi] project.

### Install steps

1) Install the **build essential package**
1) Install [wiringPi][wpi]
1) Clone and build this project

##### 1) Install the **build essential package**
    sudo apt-get install build-essential

##### 2) Install [wiringPi][wpi]
    cd someDir
    git clone git://git.drogon.net/wiringPi.git
    cd wiringPi && ./build
    
##### 3) Clone and build this project
    cd someDir
    git clone https://github.com/flavio-fernandes/mcp300x.git
    cd mcp300x && make

### Usage

To use the standalone program, call **./mcp300x** or copy it to a location used by your **$PATH**. The _--help_ should give you all you need to know:

    $ ./mcp300x --help
    Usage: mcp300x [OPTION...] 
    mcp300x, a simple program to read analog values from Mcp3002 or Mcp3008

      -2, --mpc3002              Using mpc3002 (default)
      -8, --mpc3008              Using mpc3008
      -c, --clock=gpioPin        Clock GPIO pin
      -d, --delay=milliseconds   Delay per sample (0 => none)
      -i, --digitaln=gpioPin, --mosi=gpioPin
                                 Digital In GPIO pin (write command)
      -o, --digitalout=gpioPin, --miso=gpioPin
                                 Digital Out GPIO pin (read values)
      -r, --repeats[=count]      Number of sample reads (0 => infinite)
      -s, --chipSelect=gpioPin   Chip Select GPIO pin

The default gpio pins are hard coded in _[main.cpp][maincpp]_. Feel free to modify them to suit your needs.

In order to use the Mcp300x code, use the files located in the **src** directory. They should be self contained and easy enough to port over to your project. If not, please submit a pull request and help me make this better. Enjoy!

[wpi]: http://wiringpi.com/download-and-install/ "Wiring Pi"
[rpi]: https://www.raspberrypi.org/ "Raspberry Pi"
[mpc]: http://educypedia.karadimov.info/library/21841A.pdf "Analog-to-Digital Converter"
[maincpp]: https://github.com/flavio-fernandes/mcp300x/blob/master/main.cpp "main.cpp"

