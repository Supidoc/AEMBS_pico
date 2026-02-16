# Template project for AEMBS Pico Board
This project uses CMake with Ninja and the RP2040 SDK to blink the on-board LED.

## Build
Project has `Debug`, `Release` and `Test` targets, using CMake Presets. 

Configure:
```
cmake --list-presets
cmake --preset Debug
cmake --preset Release
```

Build:
```
cmake --build --list-presets
cmake --build --preset app-debug
cmake --build --preset app-release
```
