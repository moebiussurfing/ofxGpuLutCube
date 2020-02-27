//addon modifications by moebiussurfing
//https://github.com/moebiussurfing
//
//based in and using the code of HUBRIS:
//https://github.com/youandhubris/GPU-LUT-OpenFrameworks
//BY HUBRIS [http://cargocollective.com/hubris]
//[http://github.com/youandhubris]
//
//THANKS to
//https://github.com/yasuhirohoshino for the original https://github.com/yasuhirohoshino/ofxGpuLut
//
//THANKS to 
//https://github.com/johanjohan to point me to this source here https://github.com/yasuhirohoshino/ofxGpuLut/issues/3

#pragma once

#include "ofMain.h"

//for internal gui
//#include "ofxGui.h"

class ofxGpuLutCube {

public:

	std::string path_LUT_files = "LUT";

	//ofxPanel gui;
	ofParameterGroup params;
	ofParameter<float> control1;
	ofParameter<float> control2;

	void setup();
	void draw();
	void windowResized(int w, int h);
	
	//for internal gui
	//void update();
	//void drawGui();
	//void keyPressed(int key);

	//browse
	void next();
	void previous();

	void setupLUT(std::string s);
	void setupFiles();

	void begin();
	void end();
	
	ofFbo fbo;

	//TODO:
	//WORKAROUND
	//v flipping issues
	bool bFlip = true;
	//ofTexture texFlipped;

	//files
	std::string LUTpath;
	std::string LUTname;
	vector<std::string> lutPaths;
	vector<std::string> lutNames;
	int lutIndex;
	int numLuts;
	
	//shader
	ofShader lutFilter;
	ofPlanePrimitive plane;

	//gl
	GLuint texture3D;
	int LUTsize = 32;
	int LUT3dSize;
	struct RGB { float r, g, b; };
};
