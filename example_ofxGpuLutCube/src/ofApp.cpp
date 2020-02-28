#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	LUT.setup();

	gui.setup("CONTROL ofxGpuLutCube");
	gui.add(LUT.params);
	gui.setPosition(10, 300);

	//scene
#ifndef USE_ofxSceneTEST
	image.load("picture.jpg");
#endif
}

//--------------------------------------------------------------
void ofApp::update()
{
	LUT.begin();

	//-

	//draw scene

#ifdef USE_ofxSceneTEST
	scene.drawAll();
#else
	image.draw(0, 0, ofGetWidth(), ofGetHeight());
#endif

	//-

	LUT.end();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	//filtered image
	LUT.draw();

	//original thumb image
	float ww = ofGetWidth() *0.20f;
	float ratio = image.getHeight() / image.getWidth();
	image.draw(ofGetWidth() - ww, 0, ww, ww*ratio);

	//-

	//gui
	gui.draw();

	//info
	string str;
	str += "PRESS UP/DOWN TO BROWSE '.cube' FILES\n";
	str += "FROM FOLDER 'data/" + LUT.path_LUT_files + "/'\n";
	str += "[" + ofToString(LUT.lutIndex) + "/" + ofToString(LUT.numLuts) + "] " + ofToString(LUT.LUTname);
	ofDrawBitmapStringHighlight(str, 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	//browse luts
	if (key == OF_KEY_DOWN)
	{
		LUT.next();
	}
	else if (key == OF_KEY_UP)
	{
		LUT.previous();
	}

	//--

	//scene
#ifdef USE_ofxSceneTEST
	if (key == 's')
	{
		bool b = !scene.getGuiVisible();
		scene.setGuiVisible(b);
	}
#endif
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	LUT.windowResized(w, h);
}

