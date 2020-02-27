# ofxGpuLutCube

openFrameworks addon to apply LUT (Color Lookup Table) filter shader from standard '.cube' files. (sizes 16, 32, 64)

Inspired from https://github.com/yasuhirohoshino/ofxGpuLut that does the same but with special '.png' files.

Cool app to batch convert and create LUT files:
https://grossgrade.com/


## Screenshots

![Alt text](/screenshot1.JPG?raw=true "screenshot1")
![Alt text](/screenshot2.JPG?raw=true "screenshot2")
![Alt text](/screenshot3.JPG?raw=true "screenshot3")
![Alt text](/screenshot4.JPG?raw=true "screenshot4")


## Usage

```c++
ofApp.h:

#include "ofxGpuLutCube.h"
ofxGpuLutCube LUT;

//--

ofApp.cpp:

//setup()
LUT.setup();

//update()
LUT.begin();
//draw scene
LUT.end();

//draw()
LUT.draw();
```


## Features

- easy integration into your ofApp
- LUT files folder browsing
- parameter to set filter mix/dry
- included example


## Tested system

- OF 0.11
- Visual Studio 2017


## About

Addon modifications by moebiussurfing
https://github.com/moebiussurfing

Based on and using the code of HUBRIS:
https://github.com/youandhubris/GPU-LUT-OpenFrameworks
https://forum.openframeworks.cc/t/gpu-lut-photoshop/18070

BY HUBRIS [http://cargocollective.com/hubris]
[http://github.com/youandhubris]

THANKS to @johanjohan
https://github.com/johanjohan 
to point me to this source here https://github.com/yasuhirohoshino/ofxGpuLut/issues/3
pow2 snippet from @johanjohan

THANKS to
https://github.com/yasuhirohoshino 
for the original https://github.com/yasuhirohoshino/ofxGpuLut


## TODO:

- set draw position and size
- better solution to v flip thing..
- no use internal fbo or getter setter by reference..