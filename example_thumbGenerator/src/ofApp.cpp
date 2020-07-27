#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::refreshImage() {
	sourceImg.clear();
	if (indexImage == 0) sourceImg.loadImage("image0.jpg");
	else if (indexImage == 1) sourceImg.loadImage("image1.png");
	else if (indexImage == 2) sourceImg.loadImage("image2.jpg");
	else if (indexImage == 3) sourceImg.loadImage("image3.jpg");
	else if (indexImage == 4) sourceImg.loadImage("image4.jpg");
	else if (indexImage == 5) sourceImg.loadImage("image5.jpg");
}
//--------------------------------------------------------------
void ofApp::setup() {
	indexImage = 2;
	refreshImage();

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

	thumbPos = { lutImg.getWidth()*0.5f - 80, -lutImg.getHeight()*0.5f - 60, 0 };
	lutPos = { -lutImg.getWidth()*0.5f, -lutImg.getHeight()*0.5f, 0 };

#ifdef USE_FILE_BROWSER
	setupGui();
#endif
}

//--------------------------------------------------------------
void ofApp::update() {

	if (doLUT && sourceImg.isAllocated()) {
		applyLUT(sourceImg.getPixels());
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor::gray, ofColor::black);
	//ofTranslate(ofGetWidth()*0.5f, ofGetHeight()*0.5f, 0);

	int x, y;
	x = 400;
	y = 10;
	if (sourceImg.isAllocated()) sourceImg.draw(x, y);

	if (sourceImg.isAllocated()) lutPos.x = x + sourceImg.getWidth() + 10;
	lutPos.y = y;

	if (doLUT && lutImg.isAllocated()) {
		lutImg.draw(lutPos.x, lutPos.y);
	}


	if (doLUT) {

		if (lutImg.isAllocated()) lutImg.draw(lutPos.x, lutPos.y);

		ofDrawRectangle(thumbPos.x - 3, thumbPos.y - 3, 166, 126);

		//ofDrawBitmapStringHighlight(dir.getName(dirLoadIndex), lutPos.x, -lutPos.y+50);
		string str;
		str += "\nIMAGE       : " + ofToString(indexImage);
		str += "\nLUT FILE    : " + dir.getName(dirLoadIndex) + " " + ofToString(dirLoadIndex) + "/" + ofToString(dir.size());
		str += "\nLUT_3D_SIZE : " + ofToString(LUT3dSize);
		str += "\n";
		str += "\nKEYS        : UP/DOWN ARROWS TO BROWSE LUT FILES";
		str += "\nKEYS        : RIGHT TO CHANGE SOURCE IMAGE";
		ofDrawBitmapStringHighlight(str, 20, 500);

	}
	//else {
	//		vidGrabber.draw(0,0);
	//}

#ifdef USE_FILE_BROWSER
	drawGui();
#endif
}

//--------------------------------------------------------------
bool ofApp::loadLUTaddon(std::string path)
{
	LUTloaded = false;//OF example
	ofFile file(path);

	ifstream LUTfile(path.c_str());
	vector<RGB> LUT;

	//-

	//code from 
	//https://github.com/johanjohan to point me to this source here https://github.com/yasuhirohoshino/ofxGpuLut/issues/3

	const string key_LUT_3D_SIZE = "LUT_3D_SIZE";

	//must check if size is refused 
	bool bErrorBadSize = false;

	//A. multiple pow2 lut sizes: 16, 32, 64
	LUT3dSize = 32;//most common
	while (!LUTfile.eof())
	{
		string row;
		getline(LUTfile, row);

		if (row.empty()) continue;

		RGB line;
		if (sscanf(row.c_str(), "%f %f %f", &line.r, &line.g, &line.b) == 3) {
			LUT.push_back(line);
			//lut[x][y][z]
		}
		else if (ofIsStringInString(row, key_LUT_3D_SIZE)) {
			vector<string> subs = ofSplitString(ofTrim(row), " ");
			if (subs.size() >= 2) {
				ofLogVerbose(__FUNCTION__) << "found key_LUT_3D_SIZE: " << subs[1];
				LUT3dSize = ofToInt(subs[1]);
			}
		}
	}//while

	if (LUT.size() != (pow(LUT3dSize, 3.0)))
	{
		ofLogError(__FUNCTION__) << "LUT size is incorrect.";
		bErrorBadSize = true;

		LUTname = "INCORRECT SIZE " + path;

		//return;//skip load this lut
		//std::exit(1);
	}
	else if (ofNextPow2(LUT3dSize) != LUT3dSize)
	{
		ofLogError(__FUNCTION__) << "LUT needs to be pow2.";
		bErrorBadSize = true;

		LUTname = "POW2 ERROR " + path;

		//return;//skip load this lut
		//std::exit(1);
	}

	ofLogVerbose(__FUNCTION__) << "LUT.size(): " << LUT.size() << " --> " << int(ceil(pow(LUT.size(), 1.0 / 3.0)));
	ofLogVerbose(__FUNCTION__) << "LUT3dSize: " << LUT3dSize;

	//-

	//B. fixed lut size 32
	//while (!LUTfile.eof()) {
	//	string LUTline;
	//	getline(LUTfile, LUTline);
	//	if (LUTline.empty()) continue;
	//	RGB line;
	//	if (sscanf(LUTline.c_str(), "%f %f %f", &line.r, &line.g, &line.b) == 3) LUT.push_back(line);
	//}
	//if (LUT.size() != (pow(LUTsize, 3.0)))
	//{
	//	ofLogError() << "LUT size is incorrect.";
	//	return;
	//}

	//--

	if (bErrorBadSize == false)//if lut size/format is valid
	{
		////disable rectangle textures
		//const bool bArbTex = ofGetUsingArbTex();
		//ofDisableArbTex();

		////reference from http://content.gpwiki.org/index.php/OpenGL:Tutorials:3D_Textures
		////create a 3D texture
		//glEnable(GL_TEXTURE_3D);
		//glGenTextures(1, &texture3D);
		//glBindTexture(GL_TEXTURE_3D, texture3D);
		//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		//glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, LUT3dSize, LUT3dSize, LUT3dSize, 0, GL_RGB,
		//	GL_FLOAT, &LUT[0]);
		//glBindTexture(GL_TEXTURE_3D, 0);
		//glDisable(GL_TEXTURE_3D);

		//if (bArbTex) { ofEnableArbTex(); }

		//-

		//int l = 0;
		//for (int z = 0; z < LUT3dSize; z++) {//TODO: must check sizes../ num lines?
		//	for (int y = 0; y < LUT3dSize; y++) {
		//		for (int x = 0; x < LUT3dSize; x++) {
		//			//glm::vec3 cur;
		//			//file >> cur.x >> cur.y >> cur.z;
		//			//lut[x][y][z] = cur;

		//			lut[x][y][z] = LUT[l].b;

		//				l++;
		//		}
		//	}
		//}
	}
	else {
		ofLogError(__FUNCTION__) << "LUT size is incorrect!";
	}
	LUTloaded = true;

	//-

	//must check if size or cuve file is refused 
	if (bErrorBadSize) return false;
	else return true;
}

//--------------------------------------------------------------
void ofApp::loadLUT(string path) {//OF example
	LUTloaded = false;

	ofFile file(path);
	string line;
	for (int i = 0; i < 5; i++) {//TODO: ? must check text content, not only num of lines... maybe not, allways are 4 lines
		getline(file, line);
		ofLogNotice(__FUNCTION__) << "Skipped line: " << line;
		ofLogNotice(__FUNCTION__) << "line " + ofToString(i) + ": " << line;//LUT_3D_SIZE 16

		if (i == 3) {
			ofLogNotice(__FUNCTION__) << "> line " + ofToString(i) + ": " << line;//LUT_3D_SIZE 16
			auto _lutsize = ofSplitString(line, "LUT_3D_SIZE ");
			ofLogNotice(__FUNCTION__) << "LUT_3D_SIZE: " << (_lutsize[1]) << endl;

			LUT3dSize = ofToInt(_lutsize[1]);
		}
	}

	//for (int z = 0; z < 32; z++) {//TODO: must check sizes../ num lines?
	//	for (int y = 0; y < 32; y++) {
	//		for (int x = 0; x < 32; x++) {
	//			glm::vec3 cur;
	//			file >> cur.x >> cur.y >> cur.z;
	//			lut[x][y][z] = cur;
	//		}
	//	}
	//}

	for (int z = 0; z < LUT3dSize; z++) {//TODO: must check sizes../ num lines?
		for (int y = 0; y < LUT3dSize; y++) {
			for (int x = 0; x < LUT3dSize; x++) {
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
		//TODO: update all to size 16. now it's 32 only.
		for (size_t y = 0; y < pix.getHeight(); y++) {//iterate all image pixels
			for (size_t x = 0; x < pix.getWidth(); x++) {

				ofColor color = pix.getColor(x, y);//pixel source color 

				int lutPos[3];//pixel lut
				for (int m = 0; m < 3; m++) {
					//lutPos[m] = color[m] / 8;
					//if (lutPos[m] == 31) {
					//	lutPos[m] = 30;//? smash pixel ?
					//}

					if (LUT3dSize == 32) {
						lutPos[m] = color[m] / 8;

						if (lutPos[m] == LUT3dSize - 1) {
							lutPos[m] = LUT3dSize - 2;//? smash pixel ?
						}
					}
					//TODO: ?
					else if (LUT3dSize == 16) {
						lutPos[m] = color[m] / 16;

						if (lutPos[m] == LUT3dSize - 1) {
							lutPos[m] = LUT3dSize - 2;//? smash pixel ?
						}
					}
				}

				glm::vec3 start = lut[lutPos[0]][lutPos[1]][lutPos[2]];
				glm::vec3 end = lut[lutPos[0] + 1][lutPos[1] + 1][lutPos[2] + 1];

				for (int k = 0; k < 3; k++) {
					float amount;

					if (LUT3dSize == 32) {
						amount = (color[k] % 8) / 8.0f;
					}
					else if (LUT3dSize == 16) {//
						amount = (color[k] % 8) / 16.0f;
					}

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

	case OF_KEY_RIGHT:
		if (indexImage <= 4) indexImage++;
		else indexImage = 0;
		refreshImage();
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
	//apply
	string str = "thumbs/" + dir.getName(dirLoadIndex) + "_" + ofToString(dirLoadIndex) + "_thumb.jpg";
	applyLUT(sourceImg.getPixels());

	//save
	ofLogNotice(__FUNCTION__) << "saving: " << str;
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


#ifdef USE_FILE_BROWSER

//--------------------------------------------------------------
void ofApp::drawGui() {
	if (bShowGui) {
		if (bShowBrowser) {
			gui_ImGui.begin();
			{
				draw_ImGui(300, 10, 300, 800);//window position and size
			}
			gui_ImGui.end();
		}
	}
}

//--------------------------------------------------------------
void ofApp::refreshFiles() {
	dirThumbs.reset();
	dirThumbs.listDir(pathDirCustom);
	dirThumbs.allowExt("png");
	dirThumbs.allowExt("PNG");
	dirThumbs.allowExt("jpg");
	dirThumbs.allowExt("JPG");

	indexBrowser.setMax(dirThumbs.size() - 1);

	textureSource.clear();
	textureSource.resize(dirThumbs.size());
	textureSourceID.clear();
	textureSourceID.resize(dirThumbs.size());

	for (int i = 0; i < dirThumbs.size(); i++) {
		textureSourceID[i] = gui_ImGui.loadTexture(textureSource[i], dirThumbs.getPath(i));
	}
}

//--------------------------------------------------------------
void ofApp::setupGui() {
	indexBrowser.set("LUT THUMB Index", -1, 0, 1);
	nameMat.set("LUT name", "");
	sizeThumb.set("THUMB SIZE", 100, 40, 300);

	bPathDirCustom.set("MODE CUSTOM", false);
	pathDirCustom.set("PATH", "-1");

	bShowGui = true;
	//bAutoResize = true;

	//--

	//gui.addFont("fonts\\Verdana.ttf");
	gui_ImGui.setup();

	//theme
	ModernDarkTheme();

	//ImGui::GetIO().MouseDrawCursor = false;

	//-

	//scan, load and populate thumbs
	//pathDirPreviews = "thumbs/";
	pathGlobal = "";
	pathDirPreviews = pathGlobal + "/" + "thumbs/";
	pathDirCustom = pathDirPreviews;

	refreshFiles();
}

////--------------------------------------------------------------
//void ofApp::updateGui() {
//}

//--------------------------------------------------------------
void ofApp::draw_ImGui(int x, int y, int w, int h) {

	auto mainSettings = ofxImGui::Settings();
	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
	//ImGui::Text("ofApp");

	//--

	//->required to allow helpers..but do not stores ini settings..
	if (ofxImGui::BeginWindow("Thumbs Browser Settings", mainSettings, false))
	//ImGui::Begin("Thumbs Browser Settings");//raw
	{
		ofxImGui::AddParameter(sizeThumb);
		string str0 = ofToString(indexBrowser);
		string str1 = str0 + "/" + ofToString(dirThumbs.size() - 1);
		ImGui::Text(str1.c_str());
		ImGui::Text(nameMat.get().c_str());
		//ofxImGui::AddParameter(indexBrowser);
		ofxImGui::AddParameter(nameMat);

		ofxImGui::AddParameter(bPathDirCustom);
		ofxImGui::AddParameter(pathDirCustom);
		
		
		if (ImGui::Button("Select Folder")) {
			//Open the Open File Dialog
			ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a jpg or png", true, "thumbs");

			//Check if the user opened a file
			if (openFileResult.bSuccess) {

				ofLogNotice(__FUNCTION__) << ("User selected a file");

				//We have a file, check it and process it
				processOpenFileSelection(openFileResult);

			}
			else {
				ofLogNotice(__FUNCTION__) << ("User hit cancel");
			}
		}
	}
	//ImGui::End();//raw
	ofxImGui::EndWindow(mainSettings);

	//--

	////Displacement
	//if (ofxImGui::BeginWindow("Displacement", mainSettings, false))//->required to allow helpers..but do not stores ini settings..
	//{
	//	ofxImGui::AddGroup(params_Displacement, mainSettings);
	//}
	//ofxImGui::EndWindow(mainSettings);

	//--

	//if (ofxImGui::BeginWindow("MatCap", mainSettings, false))//->required to allow helpers..but do not stores settings..
	ImGui::Begin("ofxGpuLutCube");
	{
		ImVec2 button_sz((float)sizeThumb.get(), (float)sizeThumb.get());

		ImGuiStyle& style = ImGui::GetStyle();
		int buttons_count = dirThumbs.size();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		for (int n = 0; n < buttons_count; n++)
		{
			ImGui::PushID(n);
			string name = ofToString(n);

			//customize colors
			if (n == indexBrowser)//when selected
			{
				const ImVec4 color1 = ImVec4(0.1, 0.1, 0.1, 0.8);//changes button color to black
				ImGui::PushStyleColor(ImGuiCol_Button, color1);
			}
			else { //not selected
				const ImVec4 color2 = style.Colors[ImGuiCol_Button];//do not changes the color
				ImGui::PushStyleColor(ImGuiCol_Button, color2);
			}

			//-

			//image button
			if (ImGui::ImageButton(GetImTextureID(textureSourceID[n]), button_sz))
			{
				ofLogNotice(__FUNCTION__) << "[ " + ofToString(n) + " ] THUMB : " + dirThumbs.getName(n);

				indexBrowser = n;

				nameMat = dirThumbs.getName(indexBrowser);

				//string str = "[" + ofToString(indexBrowser) + "] " + getName();
				//nameMat = getName();

				dirLoadIndex = indexBrowser;
				dirLoadIndex = (int)ofClamp(dirLoadIndex, 0, dir.size()-1);
				loadLUT(dir.getPath(dirLoadIndex));

				//apply
				applyLUT(sourceImg.getPixels());
			}

			//-

			//customize colors
			ImGui::PopStyleColor();

			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
			if (n + 1 < buttons_count && next_button_x2 < window_visible_x2) ImGui::SameLine();
			ImGui::PopID();
		}
	}
	ImGui::End();
	//ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {

	ofLogNotice(__FUNCTION__) << ("getName(): " + openFileResult.getName());
	ofLogNotice(__FUNCTION__) << ("getPath(): " + openFileResult.getPath());

	bPathDirCustom = true;
	pathDirCustom = openFileResult.getPath();

	refreshFiles();
}

#endif