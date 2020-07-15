#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	GpuLutCube.setup();

	//test image
	index = 0;
	image.load("picture.jpg");
}

//--------------------------------------------------------------
void ofApp::update()
{
	GpuLutCube.begin();
	{
		//draw scene
		ofRectangle r(0, 0, image.getWidth(), image.getHeight());
		r.scaleTo(ofGetWindowRect());
		image.draw(r.x, r.y, r.width, r.height);
	}
	GpuLutCube.end();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	//filtered image
	GpuLutCube.draw();

	//-

	//original thumb image
	float ww = ofGetWidth() *0.20f;
	float ratio = image.getHeight() / image.getWidth();
	image.draw(ofGetWidth() - ww, 0, ww, ww*ratio);

	//-

	//gui
	GpuLutCube.drawGui();

	//help
	GpuLutCube.drawHelp();
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	//browse luts
	if (key == OF_KEY_DOWN)
	{
		GpuLutCube.loadNext();
	}
	else if (key == OF_KEY_UP)
	{
		GpuLutCube.loadPrevious();
	}
	else if (key == OF_KEY_BACKSPACE)
	{
		GpuLutCube.loadRandomize();
	}

	//browse test image
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
	GpuLutCube.windowResized(w, h);
}

