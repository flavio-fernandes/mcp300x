ifneq ($V,1)
Q ?= @
endif

DEBUG	= -g -O0
#DEBUG	= -O2

.SUFFIXES: .cpp .o
.PHONY:	clean

CC	= g++
INCLUDE	= -I/usr/local/include -I./src
CFLAGS	= $(DEBUG) $(INCLUDE) -Winline -pipe -Wall -pedantic

LDFLAGS	= 
LIBS    = -lwiringPi

SRC	= main.cpp \
	  $(addprefix src/, mcp300x.cpp)

OBJ	= $(SRC:.cpp=.o)

# Note: to compile verbose, use V=1. Eg:   V=1 make all

all:	sudo_mcp300x

mcp300x:	$(OBJ)
		$Q echo [Link]
		$Q $(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

sudo_mcp300x:	mcp300x
	$Q sudo chown root:root mcp300x
	$Q sudo chmod u+s mcp300x

.cpp.o:
	$Q echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) mcp300x *~ core tags cscope.*

