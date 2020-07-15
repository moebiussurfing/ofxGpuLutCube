#pragma once

#include "ofMain.h"

#include "ofxGpuLutCube.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void windowResized(int w, int h);

	ofxGpuLutCube GpuLutCube;

	//test image
	ofImage image;
	int index = 0;
#define NUM_PICTS 2
};
