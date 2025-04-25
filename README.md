# Safetown Senior Robot Arduino Workspace
### Cedarville Senior Design 2024-25
Author: Caleb Willson

This repository contains the arduino workspace for development of the SafeTown Senior Robot.

## Documentation
Full Doxygen documentation can be found in [docs.html](./docs.html)

## Features
* Basic line following
* Basic stop line detection and intersection navigation
* Event management system for thread safe inter-compnent communication
* Lazily refreshed display
* Fully customizable, dynamically constructed menu
* File explorer for navigating the virtual file system
* ESP32 packet based communication protocol
* Photo requesting and saving

Note that the following features from the Junior robot are currently not supported by the Senior robot

* Route management
* Data logging
* Tile prediction and advanced intersection navigation
* Obstacle detection
* PID Controller based line following

## Compilation Instructions

### Cloning from Github
The repository and its submodules can be cloned with the following command.

```bash
git clone --recurse-submodules https://github.com/CrWillson/ESPPhotoProcessor.git
```

This project uses the same Pi Pico board in the Arduino IDE as the Junior robot. Refer to the Junior robot set up instructions for initializing the Arduino IDE for programming the Pi Pico.

Once the Arduino IDE has been set up for programming the Junior robot, the following steps must be followed:

1. Increase the file system size to larger than the minimum size. It does not need to be as large as possible but it should be big. 
2. Enable run time type information (RTTI). The Senior robot uses RTTI for distinguishing between different types of lines in the menu during run time. This can be done through the same drop down menu where you set the file system size.

