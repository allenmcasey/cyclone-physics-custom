# New makefile to build the Cyclone physics engine for Linux.

# Determine architecture (Linux or Mac OS X).
PLATFORM = $(shell uname)

ifeq ($(PLATFORM), Linux)
    LDFLAGS = -lGL -lGLU -lglut
else
    $(error This OS is not Ubuntu Linux. Aborting)
endif

# Demo files path.
DEMOPATH = ./src/demos/

# Build directory path
BUILDPATH = ./build/

# Demo core files.
DEMOCOREFILES = $(DEMOPATH)main.cpp $(DEMOPATH)app.cpp $(DEMOPATH)timing.cpp

# Demo files.
# DEMOLIST = ballistic bigballistic blob bridge explosion fireworks flightsim fracture platform ragdoll sailboat
DEMOLIST = lighter ballistic

# Cyclone core files.
# CYCLONEFILES = ./src/body.cpp ./src/collide_coarse.cpp ./src/collide_fine.cpp ./src/contacts.cpp ./src/fgen.cpp ./src/joints.cpp ./src/plinks.cpp ./src/pworld.cpp ./src/random.cpp ./src/world.cpp
CYCLONEFILES = ./src/core.cpp ./src/particle.cpp ./src/pfgen.cpp ./src/pcontacts.cpp ./src/plinks.cpp
.PHONY: clean

# Build the project
all: build $(DEMOLIST)

# Create build directory
build:
	mkdir ./build

# Compile demo files
$(DEMOLIST):
	g++ -O2 -Iinclude $(DEMOCOREFILES) $(CYCLONEFILES) $(DEMOPATH)$@/$@.cpp -o $(BUILDPATH)$@ $(LDFLAGS) 

# Remove build directory
clean:
	rm -r ./build
