# ofxGpuLutCube

openFrameworks addon to apply LUT (Color Lookup Table) filter shader from the standard '.cube' files. (sizes 16, 32, 64)

All process is done into GPU, so it's faster than other alternatives that use the CPU like the bundled \openFrameworks\examples\graphics\lutFilterExample'

Inspired from https://github.com/yasuhirohoshino/ofxGpuLut that does the almost same GPU process but with 'special .png' files.

Cool Windows-only free app to batch convert and create LUT files:
https://grossgrade.com/


## Screenshots

![Alt text](/screenshot0.JPG?raw=true "screenshot0")
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

//update() or draw()
LUT.begin();
    //draw your scene
LUT.end();

//draw()
LUT.draw();
```


## Features

- easy integration into your ofApp with an ofParameterGroup
- LUT files folder browsing using GUI or keys
- float parameter to set filter mix/dry
- int parameter to select LUT index 
- string parameter to show LUT name
- included example with /data/ files (shader + LUT.cube files)


## Tested system

- OF 0.11
- Visual Studio 2017


## About

Addon modifications by MoebiusSurfing
https://github.com/moebiussurfing

Based on and using the code of HUBRIS:

https://github.com/youandhubris/GPU-LUT-OpenFrameworks

https://forum.openframeworks.cc/t/gpu-lut-photoshop/18070

BY HUBRIS [http://cargocollective.com/hubris]
[http://github.com/youandhubris]

THANKS to @johanjohan:
https://github.com/johanjohan 
to point me to this source here: 
https://github.com/yasuhirohoshino/ofxGpuLut/issues/3

pow2 snippet from @johanjohan.

THANKS to
https://github.com/yasuhirohoshino 
for the original https://github.com/yasuhirohoshino/ofxGpuLut


## TODO:

- set draw position and size (x, y, w, h)..
- a better solution to v flip that sometimes happens..
- not use internal FBO or add getter/setter to use by reference..
- better handling of arb tex modes..
- a faster loader of files, threaded or cache some files..
- settings to store/load LUT by name..
- .cube files browser system and thumbs maybe with ofxImGui..

PLEASE FEEL FREE TO ADD MODIFICATIONS OR FEATURES AND TO SEND ME PULL REQUESTS