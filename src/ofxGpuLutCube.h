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


///
///	TODO:
///
///	+	add loading presets by name not by index (to allow add more cube files)
///	+	add preview clickable thumbs of all cube files
///	


#pragma once

#include "ofMain.h"

#include "ofxGui.h"//for internal gui

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

public:

	void setup();

	void draw();//draw processed image with full screen size and with position 0, 0 
	//TODO: 
	////resizable drawing
	////custom position and size..
	//void draw(float x, float y);
	//void setSize(float w, float h);

	void exit();
	void windowResized(int w, int h);
	
	//--

	//internal gui
	ofxPanel gui;
	void setupGui();
	void drawGui();
	
	//help
	void drawHelp();

	//----

	//API

	//feed
	void begin();
	void end();

	//browse
	void loadNext();
	void loadPrevious();
	void loadRandomize();

	int geNumtLuts();
	void setSelectedLut(int i);
	void loadLut(int i);
	void setVflip(bool b)//must be called before setup(). not implemented yet to work in realtime when drawing..
	{
		bFlip = b;
	}
	
	//----

public:

	ofParameterGroup params;
	ofParameter<std::string> LUTname;
	ofParameter<int> lutIndex;
	ofParameter<float> control1;
	ofParameter<float> control2;//not used yet
	ofParameter<void> bPrevious;
	ofParameter<void> bNext;

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
	ofParameter<bool> bFlip{ "vFlip", true };
	//ofTexture texFlipped;

	//----

	//helpers

	ofTrueTypeFont font;
	float fontSize;

	//--------------------------------------------------------------
	void drawTextBoxed(string text, int x, int y, int alpha = 255)
	{
		ofPushStyle();

		int pad = 20;

		if (!font.isLoaded()) {
			ofDrawBitmapStringHighlight(text, x, y);
		}
		else {
			//bbox
			ofSetColor(0, alpha);
			ofFill();
			ofRectangle _r(font.getStringBoundingBox(text, x, y));
			_r.setWidth(_r.getWidth() + pad);
			_r.setHeight(_r.getHeight() + pad);
			_r.setX(_r.getPosition().x - pad / 2.);
			_r.setY(_r.getPosition().y - pad / 2.);
			ofDrawRectangle(_r);

			//text
			ofSetColor(255, 255);
			ofNoFill();
			font.drawString(text, x, y);
		}

		ofPopStyle();
	}
	//--------------------------------------------------------------
	float getWidthBBtextBoxed(string text) {
		return (font.getStringBoundingBox(text,0,0)).getWidth();
	}
};
