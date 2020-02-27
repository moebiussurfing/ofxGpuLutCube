#pragma once

#include "ofMain.h"

#include "ofxGpuLutCube.h"
#include "ofxGui.h"

//#define USE_ofxSceneTEST
#ifdef USE_ofxSceneTEST
#include "ofxSceneTEST.h"
#endif

class ofApp : public ofBaseApp {

public:

	ofxGpuLutCube LUT;

	ofxPanel gui;

#ifdef USE_ofxSceneTEST
	ofxSceneTEST scene;
#else
	ofImage image;
#endif

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void windowResized(int w, int h);
};
