# ofxGpuLutCube

openFrameworks addon to apply LUT (Color Lookup Table) filters with a GPU shader using standard **.cube** files. (sizes 16, 32, 64)

**All process is done live and fast into the GPU**,  
so it's faster than other alternatives that use the CPU.  
(like the bundled `openFrameworks\examples\graphics\lutFilterExample`)

**Inspired from:**  
https://github.com/yasuhirohoshino/ofxGpuLut  
that does the almost same GPU process but with special `.png` files.

A (maybe useful) cool Windows-only free app to batch convert and create LUT files:  
https://grossgrade.com/  



## Screenshots

![Alt text](/readme_images/screenshot0.JPG?raw=true "screenshot0")
![Alt text](/readme_images/screenshot1.JPG?raw=true "screenshot1")
![Alt text](/readme_images/screenshot2.JPG?raw=true "screenshot2")
![Alt text](/readme_images/screenshot3.JPG?raw=true "screenshot3")
![Alt text](/readme_images/screenshot4.JPG?raw=true "screenshot4")


## Usage

### ofApp.h
```c++
#include "ofxGpuLutCube.h"

ofxGpuLutCube GpuLutCube;
```
  
### ofApp.cpp
```c++
//setup()
GpuLutCube.setup();

//update() or draw()
GpuLutCube.begin();
{
    //draw and feed your scene here
}
GpuLutCube.end();

//draw()
GpuLutCube.draw();

//-

//optional:

//gui
GpuLutCube.drawGui();

//help
GpuLutCube.drawHelp();

```


## Features

- Easy integration into your `ofApp` with an `ofParameterGroup` and an `ofxGui` panel
- LUT files folder browsing using GUI or keys.
- Float parameter to set filter mix/dry.
- Int parameter to select `LUT index`. 
- String parameter to show `LUT name`.
- Included example with `/data/` files (`LUT.cube` files + shader files).


## Tested system

- OF 0.11
- Visual Studio 2017


## About

Addon modifications by **MoebiusSurfing**  
https://github.com/moebiussurfing
  

Based on and using the code of **HUBRIS**:
https://github.com/youandhubris/GPU-LUT-OpenFrameworks  
https://forum.openframeworks.cc/t/gpu-lut-photoshop/18070  

BY HUBRIS  
[http://cargocollective.com/hubris]  
[http://github.com/youandhubris]  

THANKS to **@johanjohan**:  
https://github.com/johanjohan  
to point me to this source here:  
https://github.com/yasuhirohoshino/ofxGpuLut/issues/3  

**pow2** snippet from **@johanjohan**.  

**THANKS to:**  
https://github.com/yasuhirohoshino  
for the original https://github.com/yasuhirohoshino/ofxGpuLut


## TODO:

- Set custom draw position and size (x, y, w, h).
- A better solution to realtime vertical flip that sometimes happens.
- To not use internal FBO or add getter/setter to use by reference.
- A faster loader of files, threaded or cache some files.
- Settings to store/load LUT by name.
- `.cube` files browser system and thumbs maybe with `ofxImGui`.
  
  
**PLEASE FEEL FREE TO ADD MODIFICATIONS/ISSUES/FEATURES AND TO SEND ME PULL REQUESTS**  

## License
MIT License.