#pragma once

#include "ofMain.h"

#include "ofxGpuLutCube.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:

	ofxGpuLutCube LUT;

	ofxPanel gui;

	ofImage image;

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void windowResized(int w, int h);
};
