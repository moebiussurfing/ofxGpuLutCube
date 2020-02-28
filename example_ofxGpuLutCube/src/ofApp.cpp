#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	LUT.setup();

	gui.setup("CONTROL");
	gui.add(LUT.params);
	gui.setPosition(10, 300);


	image.load("picture.jpg");

}

//--------------------------------------------------------------
void ofApp::update()
{
	LUT.begin();

	//draw scene
	image.draw(0, 0, ofGetWidth(), ofGetHeight());

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

	ofEnableArbTex();//i am not sure why, sometimes is required when combining with other fbo's

	//gui
	gui.draw();

	//info
	string str;
	str += "PRESS UP/DOWN TO BROWSE '.cube' FILES\n";
	str += "FROM FOLDER 'data/" + LUT.path_LUT_files + "/'\n";
	str += "[" + ofToString(LUT.lutIndex) + "/" + ofToString(LUT.numLuts - 1) + "] " + ofToString(LUT.LUTname);
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
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	LUT.windowResized(w, h);
	
}

