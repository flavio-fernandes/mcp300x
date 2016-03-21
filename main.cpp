#include "mcp300x.h"

#include <stdlib.h>
#include <argp.h>
#include <iostream>
#include <wiringPi.h>

using namespace std;

const char* argp_program_version = "Mcp300x analog reader 0.1";
const char* argp_program_bug_address = "< https://github.com/flavio-fernandes/mcp300x >";
static const char doc[] =
  "mcp300x, a simple program to read analog values from Mcp3002 or Mcp3008";
static const char args_doc[] = ""; // no extra args used

static struct argp_option options[] = {
  {"clock",      'c', "gpioPin",      0,                   "Clock GPIO pin" },
  {"digitalout", 'o', "gpioPin",      0,                   "Digital Out GPIO pin (read values)" },
  {"miso",       0,   0,              OPTION_ALIAS },
  {"digitaln",   'i', "gpioPin",      0,                   "Digital In GPIO pin (write command)" },
  {"mosi",       0,   0,              OPTION_ALIAS },
  {"chipSelect", 's', "gpioPin",      0,                   "Chip Select GPIO pin" },
  {"delay",      'd', "milliseconds", 0,                   "Delay per sample (0 => none)" },
  {"repeats",    'r', "count",        OPTION_ARG_OPTIONAL, "Number of sample reads (0 => infinite)" },
  {"mpc3002",    '2', 0,              0,                   "Using mpc3002 (default)" }, 
  {"mpc3008",    '8', 0,              0,                   "Using mpc3008" }, 
  {0}
};

typedef struct Arguments_t {
  int pinClock;
  int pinDigitalOut;  // miso (read)
  int pinDigitalIn;   // mosi (write)
  int pinChipSelect;
  int sampleIntervalInMilliseconds;
  int numberOfSamples;  // 0 (or less) => infinite
  bool isMcp3002;

  bool mcpParamProvided;
} Arguments;

void setDefaultArguments(Arguments& arguments) {
  arguments.pinClock = 23;
  arguments.pinDigitalOut = 24;
  arguments.pinDigitalIn = 25;
  arguments.pinChipSelect = 21;
  arguments.sampleIntervalInMilliseconds = 1000; // 1 sec
  arguments.numberOfSamples = 3;
  arguments.isMcp3002 = true;
  arguments.mcpParamProvided = false;
}

static error_t parse_opt(int key, char* arg, struct argp_state* state) {
  Arguments* arguments = (Arguments*) state->input;

  switch (key) {
  case 'c':
    arguments->pinClock = atoi(arg);
    break;
  case 'o':
    arguments->pinDigitalOut = atoi(arg);
    break;
  case 'i':
    arguments->pinDigitalIn = atoi(arg);
    break;
  case 's':
    arguments->pinChipSelect = atoi(arg);
    break;
  case 'd':
    arguments->sampleIntervalInMilliseconds = atoi(arg);
    break;
  case 'r':
    arguments->numberOfSamples = arg ? atoi(arg) : 0;
    break;
  case '2':
    if (arguments->mcpParamProvided && !arguments->isMcp3002) {
      cout << "Error: mpc3002 and mpc3008 cannot be used together" << endl;;
      argp_usage(state);
    }
    arguments->isMcp3002 = true;
    arguments->mcpParamProvided = true;
    break;
  case '8':
    if (arguments->mcpParamProvided && arguments->isMcp3002) {
      cout << "Error: mpc3002 and mpc3008 cannot be used together" << endl;;
      argp_usage(state);
    }
    arguments->isMcp3002 = false;
    arguments->mcpParamProvided = true;
    break;
  case ARGP_KEY_INIT:
    setDefaultArguments(*arguments); 
    break;
  case ARGP_KEY_ARG:
    argp_usage(state); // no extra arguments expected.
    break;
  case ARGP_KEY_NO_ARGS:
    break;
  case ARGP_KEY_END:
    break;
  default:
    return ARGP_ERR_UNKNOWN;
    break;
  }
  
  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char* argv[]) {
  Arguments arguments;
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  
  wiringPiSetupGpio();

  const Mcp300x* mcp300x;
  if (arguments.isMcp3002) {
    mcp300x = new Mcp3002(arguments.pinClock, arguments.pinDigitalOut, arguments.pinDigitalIn,
			  arguments.pinChipSelect);
  } else {
    mcp300x = new Mcp3008(arguments.pinClock, arguments.pinDigitalOut, arguments.pinDigitalIn,
			  arguments.pinChipSelect);
  }

  cout << "Starting read analog of " << mcp300x->lastChannelPin() + 1 << " pins. ";
  if (arguments.numberOfSamples != 1) {
    if (arguments.numberOfSamples > 1) cout << arguments.numberOfSamples; else cout << "Infinite";
    cout << " samples,";
    if (arguments.sampleIntervalInMilliseconds > 0) {
      cout << " every " << arguments.sampleIntervalInMilliseconds << " milliseconds.";
    } else {
      cout << " as fast as possible.";
    }
  }
  cout << endl;

  unsigned int sample = 1;
  while (true) {
    for (int i=0; i <= mcp300x->lastChannelPin(); ++i) {
      cout << "sample " << sample << ", analog pin " << i << ": " << mcp300x->readAnalog(i) << endl;
    }
    if (++sample > (unsigned int) arguments.numberOfSamples && arguments.numberOfSamples > 0) break;
    if (arguments.sampleIntervalInMilliseconds > 0) delay(arguments.sampleIntervalInMilliseconds);
  }
  
  delete mcp300x;
  return 0;
}
      
