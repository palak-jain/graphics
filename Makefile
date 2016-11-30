#
# THIS IS THE DEFAULT MAKEFILE FOR CS148
# Author: njoubert
# GLSL includes by Siddhartha Chaudhuri

#Code   -------------------------------

### YOU CAN CHANGE THESE VARIABLES AS YOU ADD CODE:
TARGET := viewer
HEADERS := $(wildcard ./src/core/*.hpp) $(wildcard ./src/*.hpp)
CSOURCES := $(wildcard ./src/core/*.c) $(wildcard ./src/*.c)
CXXSOURCES := $(wildcard ./src/core/*.cpp) $(wildcard ./src/*.cpp)

#Libraries -------------------------------

#Check for OSX. Works on 10.5 (and 10.4 but untested)
#This line still kicks out an annoying error on Solaris.
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	#Assume Mac
	INCLUDE := -I/usr/local/include -I/opt/local/include
	LIBRARY := -L/usr/local/lib -L/opt/local/lib -lm
	FRAMEWORK := -framework GLUT -framework OpenGL -framework Carbon
	MACROS := -DOSX
	PLATFORM := Mac
else
	#Assume X11
	INCLUDE := -I/usr/local/include -I/opt/local/include
	LIBRARY := -L/usr/local/lib -L/opt/local/lib -lglut -lGLU -lGL -lm
	FRAMEWORK :=
	MACROS :=
	PLATFORM := *Nix
endif

#Basic Stuff -----------------------------

CC := cc
CXX := c++
CFLAGS := -g2 -O2 -Wall $(INCLUDE) $(MACROS) -Wno-deprecated-declarations
CXXFLAGS := $(CFLAGS)
LDFLAGS := $(FRAMEWORK) $(LIBRARY)

#-----------------------------------------

%.o : %.c
	@echo "Compiling .c to .o for $(PLATFORM) Platform:  $<"
	@$(CC) -c -Wall $(CFLAGS) $< -o $@

%.o : %.cpp
	@echo "Compiling .cpp to .o for $(PLATFORM) Platform:  $<"
	@$(CXX) -c -Wall $(CXXFLAGS) $< -o $@

OBJECTS = $(CSOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)

$(TARGET): $(OBJECTS) $(HEADERS)
	@echo "Linking .o files into:  $(TARGET)"
	@$(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

default: $(TARGET)

all: default

clean:
	@echo "Removing compiled files:  $<"
	$(RM) -f $(OBJECTS) *~ $(TARGET)
