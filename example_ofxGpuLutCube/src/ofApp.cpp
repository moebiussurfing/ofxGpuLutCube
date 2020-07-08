#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	LUT.setup();

	gui.setup("CONTROL");
	gui.add(LUT.params);
	gui.setPosition(10, 300);

	index = 0;
	image.load("picture.jpg");
	//image.load("david.jpg");

	myFont.load(OF_TTF_MONO, 60);
	
}

//--------------------------------------------------------------
void ofApp::update()
{
	LUT.begin();

	//draw scene
	ofRectangle r(0, 0, image.getWidth(), image.getHeight());
	r.scaleTo(ofGetWindowRect());
	image.draw(r.x, r.y, r.width, r.height);

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

	//ofEnableArbTex();//NOTE: i am not sure why, sometimes is required when combining with other fbo's
	//debug
	//cout << "ofGetUsingArbTex(): " << ofGetUsingArbTex() << endl;

	//gui
	gui.draw();

	//info
	string str;
	str += "PRESS UP/DOWN TO BROWSE '.cube' FILES\n";
	str += "FROM FOLDER 'data/" + LUT.path_LUT_files + "/'\n";
	str += "[" + ofToString(LUT.lutIndex) + "/" + ofToString(LUT.numLuts - 1) + "] " + ofToString(LUT.LUTname);
	ofDrawBitmapStringHighlight(str, 10, 20);

	//debug filename
	if (true) {
		ofPushStyle();
		float w = (myFont.getStringBoundingBox(LUT.LUTname, 0, 0)).width;
		float x = ofGetWidth() * 0.5f - w * 0.5f;
		float y = ofGetHeight() - 75;
		ofSetColor(255, 225);
		myFont.drawString(LUT.LUTname, x, y);
		ofSetColor(0, 255);
		myFont.drawString(LUT.LUTname, x + 1, y - 1);
		ofPopStyle();
	}
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
	else if (key == OF_KEY_RIGHT)
	{
		index++;
		if (index == NUM_PICTS) index = 0;
		if (index == 0) image.load("picture.jpg");
		if (index == 1) image.load("david.jpg");
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	LUT.windowResized(w, h);
}

