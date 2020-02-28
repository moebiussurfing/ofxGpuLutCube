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

	bool setupLUT(std::string s);
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
	int lutIndex_PRE = 0;
	int numLuts;

	//shader
	ofShader lutFilter;
	ofPlanePrimitive plane;

	//gl
	GLuint texture3D;
	int LUTsize = 32;
	int LUT3dSize;
	struct RGB { float r, g, b; };

	//-

	//API

	int geNumtLuts()
	{
		return numLuts;
	}
	void setSelectedLut(int i)
	{
		lutIndex_PRE = lutIndex;
		lutIndex = i;
		if (lutIndex <= 0)
			lutIndex = 0;
		else if (lutIndex > numLuts - 1)
			lutIndex = numLuts - 1;

		bool bLoaded;
		bLoaded = setupLUT(lutPaths[lutIndex]);
		if (bLoaded)
			ofLogNotice(__FUNCTION__) << "LUT file loaded";
		else
		{
			ofLogError(__FUNCTION__) << "LUT file " + ofToString(lutIndex) + " not loaded";
			ofLogError(__FUNCTION__) << "back to previous lut " + ofToString(lutIndex_PRE) + " that worked";
			lutIndex = lutIndex_PRE;
			//must check if size is refused 
			setupLUT(lutPaths[lutIndex]);
		}
	}
};
