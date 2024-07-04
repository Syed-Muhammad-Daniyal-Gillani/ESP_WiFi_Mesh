This is an ESP Wifi Mesh Library code having the following features:
1. Connection capability of upto 5 nodes, 1 Master and 4 slaves.
2. User defined Network configuration capability.
3. MQTT Connection capability.

The project is currently incomplete and not tested as I do not have the relevant hardware to test this.
Apart from this I am facing some build errors when trying to build the library as this was created in VSCode using platformIO extension and I am still relatively new to all this Platform, CMake files and ESP32 libraries and stuff so I may need some more time to actually learn and make this library workable on ESP32 hardware.
Anyways these are the directories where all the relevant files are placed:

├── .pio/
├── .vscode/
├── include/
│   └── README
├── lib/
│   └── wifi_mesh/
│       ├── wifi_mesh.cpp //Mesh library cpp file
│       ├── wifi_mesh.h   //Mesh library header file
│       └── CMakeLists.txt
├── src/
│   ├── CMakeLists.txt
│   └── main.cpp          //Main file
├── test/
│   └── README
├── .gitignore
├── CMakeLists.txt
├── platformio.ini
└── sdkconfig.esp32doit-devkit-v1

To all the viewers and evaluators out there, I hope you like my project..
