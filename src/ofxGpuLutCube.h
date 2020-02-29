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

private:
	std::string path_Global = "ofxGpuLutCube";
	std::string path_Shader = path_Global + "/" + "Shaders/lut_filter";
public:
	std::string path_LUT_files = path_Global + "/" + "LUT";
	//data structure will be:
	//bin/data/ofxGpuLutCube/LUT/-> .cube files
	//bin/data/ofxGpuLutCube/Shaders/-> shader files

	//----

	//API

public:
	void setup();
	void draw();///full screen at position 0,0 by default

	////TODO:
	////custom position and size..
	//void draw(float x, float y);
	//void setSize(float w, float h);

	void begin();
	void end();

	//browse
	void next();
	void previous();
	int geNumtLuts();
	void setSelectedLut(int i);
	void setVflip(bool b)
	{
		bFlip = b;
	}
	
	//----

public:
	//params
	ofParameterGroup params;
	ofParameter<std::string> LUTname;
	ofParameter<int> lutIndex;
	ofParameter<float> control1;
	ofParameter<float> control2;//not used yet
	ofParameter<void> bPrevious;
	ofParameter<void> bNext;

	//internal gui
	//ofxPanel gui;
	//void drawGui();

	void exit();
	void windowResized(int w, int h);

private:
	//callbacks
	void Changed_params(ofAbstractParameter &e);
	//buttons
	ofEventListener listener_bPrev;
	ofEventListener listener_bNext;

	bool DISABLE_Callbacks;
	int lutIndex_PRE = -1;

	bool loadLUT(std::string s);

	ofFbo fbo;

	//files
	void setupFiles();
	std::string LUTpath;
	vector<std::string> lutPaths;
	vector<std::string> lutNames;

public:
	int numLuts;

private:
	//shader
	ofShader lutFilter;
	ofPlanePrimitive plane;

	//gl
	GLuint texture3D;
	int LUTsize = 32;
	int LUT3dSize;
	struct RGB { float r, g, b; };

	//-

	//TODO:
	//WORKAROUND
	//v flipping issues
	bool bFlip = true;
	//ofTexture texFlipped;
};
