#pragma once

#include "ofMain.h"


///	TODO:
///
///	+	must check sizes of lut files and adapt to this
///			now is looking for LUT_3D_SIZE 32 files
///			we have sometimes LUT_3D_SIZE 16
///			search until LUT_3D_SIZE 32 line and get the 32 size
///			this is are typical headers:
///
///				TITLE "16x16x16 Adobe/IRIDAS cube 3D LUT exported with GrossGrade v0.1.027.1 (beta) 64 bit"
///				DOMAIN_MIN 0 0 0
///				DOMAIN_MAX 1 1 1
///				LUT_3D_SIZE 16
///				0.015699999 0.105899997 0.392199993
///			
///				TITLE "Earlybird"
///				dir.getName(dirLoadIndex)
///				0.1216 0.0196 0.0196


class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void gotMessage(ofMessage msg);
	void dragEvent(ofDragInfo dragInfo);
	
	void loadLUT(string path);
	void applyLUT(ofPixelsRef pix);
	
	bool doLUT;

	int dirLoadIndex;
	ofDirectory dir;
	glm::vec3 lutPos;
	glm::vec3 thumbPos;
	
	bool LUTloaded;
	int indexImage = 0;
	void refreshImage();

#define MAX_LUT_SIZE 32
	int LutSizeCurr = MAX_LUT_SIZE;

	glm::vec3 lut[MAX_LUT_SIZE][MAX_LUT_SIZE][MAX_LUT_SIZE];
	
	ofImage lutImg;
	
	ofImage sourceImg;
	
	void doExportThumb();
	
};
