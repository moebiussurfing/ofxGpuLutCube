#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	sourceImg.loadImage("image1.png");
	//sourceImg.loadImage("image2.jpg");

	int size = 300;
	sourceImg.resize(size, size);

	dir.allowExt("cube");
	dir.listDir("LUTs/");
	dir.sort();
	if (dir.size() > 0) {
		dirLoadIndex = 0;
		loadLUT(dir.getPath(dirLoadIndex));
		doLUT = true;
	}
	else {
		doLUT = false;
	}

	lutImg.allocate(size, size, OF_IMAGE_COLOR);
	//lutImg.allocate(640, 480, OF_IMAGE_COLOR);


	thumbPos = { lutImg.getWidth()*0.5f - 80, -lutImg.getHeight()*0.5f - 60, 0 };
	lutPos = { -lutImg.getWidth()*0.5f, -lutImg.getHeight()*0.5f, 0 };

	ofBackground(0);
	ofSetColor(255);
}

//--------------------------------------------------------------
void ofApp::update() {

	if (doLUT) {
		applyLUT(sourceImg.getPixels());
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor::gray, ofColor::black);

	//ofTranslate(ofGetWidth()*0.5f, ofGetHeight()*0.5f, 0);

	sourceImg.draw(0, 0);

	lutPos.x = sourceImg.getWidth() + 10;
	lutPos.y = 0;

	if (doLUT) {
		lutImg.draw(lutPos.x, lutPos.y);
	}


	if (doLUT) {

		lutImg.draw(lutPos.x, lutPos.y);

		ofDrawRectangle(thumbPos.x - 3, thumbPos.y - 3, 166, 126);

		//ofDrawBitmapStringHighlight(dir.getName(dirLoadIndex), lutPos.x, -lutPos.y+50);
		string str = dir.getName(dirLoadIndex) + " " + ofToString(dirLoadIndex) + "/" + ofToString(dir.size());

		ofDrawBitmapStringHighlight(str, 20, 500);

		ofDrawBitmapStringHighlight("Use the up and down arrows of your keyboard \nto go through the different filters", 20, 540);

	}
	//else {
	//		vidGrabber.draw(0,0);
	//}
}

//--------------------------------------------------------------
void ofApp::loadLUT(string path) {
	LUTloaded = false;

	ofFile file(path);
	string line;
	for (int i = 0; i < 5; i++) {//TODO: ? must check text content, not only num of lines... maybe not, allways are 4 lines
		getline(file, line);
		ofLog() << "Skipped line: " << line;
	}
	for (int z = 0; z < 32; z++) {//TODO: must check sizes../ num lines?
		for (int y = 0; y < 32; y++) {
			for (int x = 0; x < 32; x++) {
				glm::vec3 cur;
				file >> cur.x >> cur.y >> cur.z;
				lut[x][y][z] = cur;
			}
		}
	}

	LUTloaded = true;
}
//--------------------------------------------------------------
void ofApp::applyLUT(ofPixelsRef pix) {
	if (LUTloaded) {

		for (size_t y = 0; y < pix.getHeight(); y++) {
			for (size_t x = 0; x < pix.getWidth(); x++) {

				ofColor color = pix.getColor(x, y);

				int lutPos[3];
				for (int m = 0; m < 3; m++) {
					lutPos[m] = color[m] / 8;
					if (lutPos[m] == 31) {
						lutPos[m] = 30;
					}
				}

				glm::vec3 start = lut[lutPos[0]][lutPos[1]][lutPos[2]];
				glm::vec3 end = lut[lutPos[0] + 1][lutPos[1] + 1][lutPos[2] + 1];

				for (int k = 0; k < 3; k++) {
					float amount = (color[k] % 8) / 8.0f;
					color[k] = (start[k] + amount * (end[k] - start[k])) * 255;
				}

				lutImg.setColor(x, y, color);

			}
		}

		lutImg.update();
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
	case ' ':
		doLUT ^= true;
		break;

	case OF_KEY_UP:
		dirLoadIndex++;
		if (dirLoadIndex >= (int)dir.size()) {
			dirLoadIndex = 0;
		}
		loadLUT(dir.getPath(dirLoadIndex));
		doExportThumb();
		break;

	case OF_KEY_DOWN:
		dirLoadIndex--;
		if (dirLoadIndex < 0) {
			dirLoadIndex = dir.size() - 1;
		}
		loadLUT(dir.getPath(dirLoadIndex));
		doExportThumb();
		break;

	default:
		break;
	}
}

void ofApp::doExportThumb() {
	string str = "thumbs/" + dir.getName(dirLoadIndex) +"_" + ofToString(dirLoadIndex) + "_thumb.jpg";
	applyLUT(sourceImg.getPixels());
	cout << "saving: " << str << endl;
	lutImg.saveImage(str);
	//lutImg.save(str);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
