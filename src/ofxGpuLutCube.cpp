#include "ofxGpuLutCube.h"

//--------------------------------------------------------------
void ofxGpuLutCube::setup()
{
	//-

	//load the shader
	lutFilter.load(path_Shader);

	//read lut files folder
	setupFiles();

	//-

	//params
	lutIndex.set("LUT", 0, 0, numLuts - 1);
	LUTname.set("", "");//no name to reduce gui panel space
	LUTname.setSerializable(false);//not interested to save name, just the index
	control1.set("MIX", 1, 0, 1);
	bPrevious.set("PREVIOUS");
	bNext.set("NEXT");

	params.setName("ofxGpuLutCube");
	params.add(lutIndex);
	params.add(LUTname);
	params.add(bNext);
	params.add(bPrevious);

	params.add(control1);//mix

	//TODO:
	//another control exposes in the shader but not used now.
	//control2.set("control2", 1, 0, 1);
	//params.add(control2);

	//params.add(bFlip);//not implemented
	
	//-

	//gui
	setupGui();

	//-

	//draw help font
	fontSize = 15;
	font.load(OF_TTF_MONO, fontSize);

	//-

	//calbacks

	//DISABLE_Callbacks = true;

	//group
	ofAddListener(params.parameterChangedE(), this, &ofxGpuLutCube::Changed_params);

	//buttons
	listener_bPrev = bPrevious.newListener([this](const void * sender) {
		ofParameter<void> * p = (ofParameter<void> *)sender;
		string name = p->getName();
		ofLogNotice(__FUNCTION__) << "Changed_button_param: 'PREVIOUS'";
		if (name == "PREVIOUS")
		{
			loadPrevious();
		}
		});
	listener_bNext = bNext.newListener([this](const void * sender) {
		ofParameter<void> * p = (ofParameter<void> *)sender;
		string name = p->getName();
		ofLogNotice(__FUNCTION__) << "Changed_button_param: 'NEXT'";
		if (name == "NEXT")
		{
			loadNext();
		}
		});

	DISABLE_Callbacks = false;

	//--

	//will load first LUT file on folder (auto bc changed callback)
	lutIndex = 0;

	//--

	//source

	//fbo settings

	bool bArbPRE = ofGetUsingArbTex();
	ofDisableArbTex();

	ofFbo::Settings settings;
	settings.internalformat = GL_RGBA;
	settings.width = ofGetWidth();
	settings.height = ofGetHeight();
	fbo.allocate(settings);

	if (bArbPRE) ofEnableArbTex();
	else ofDisableArbTex();

	//-

	//TODO:
	//v flipping issues
	//fbo.getTextureReference().getTextureData().bFlipTexture = bFlip;
	//set a plane to texture
	if (!bFlip)
	{
		plane.set(ofGetWidth(), ofGetHeight(), 2, 2);//size, col, rows..
		plane.setPosition(0.5f*ofGetWidth(), 0.5f*ofGetHeight(), 0);
	}
	else
	{
		plane.set(ofGetWidth(), -ofGetHeight(), 2, 2);
		plane.setPosition(0.5f*ofGetWidth(), 0.5f*ofGetHeight(), 0);
	}

	//-

	//ofEnableArbTex();//NOTE: i am not sure why, sometimes is required when combining with other fbo's
}

//gui
//--------------------------------------------------------------
void ofxGpuLutCube::setupGui()
{
	gui.setDefaultWidth(300);
	gui.setup("CONTROL");
	gui.add(params);
	gui.setPosition(10, 300);
}
//--------------------------------------------------------------
void ofxGpuLutCube::drawGui()
{
	gui.draw();
}

//--------------------------------------------------------------
void ofxGpuLutCube::setupFiles()
{
	ofDirectory dir;
	dir.allowExt("cube");
	dir.allowExt("CUBE");
	dir.listDir(path_LUT_files);
	dir.sort();
	numLuts = dir.size();
	ofLogNotice(__FUNCTION__) << "LUTs path   : " << path_LUT_files;
	ofLogNotice(__FUNCTION__) << "LUTs amount : " << numLuts;

	lutNames.resize(numLuts);
	lutPaths.resize(numLuts);

	for (int i = 0; i < numLuts; i++)
	{
		std::string lutPath = dir.getPath(i);
		lutPaths[i] = lutPath;

		ofStringReplace(lutPath, path_LUT_files, "");
		ofStringReplace(lutPath, ".cube", "");
		ofStringReplace(lutPath, ".CUBE", "");
		ofStringReplace(lutPath, "\\", "");//delete '\' char from name
		lutNames[i] = lutPath;

		//ofLogNotice(__FUNCTION__) << "[" << i << "] " << lutPaths[i];
		//ofLogNotice(__FUNCTION__) << "     " << lutNames[i];
		ofLogNotice(__FUNCTION__) << "[" << i << "] " << lutNames[i];
	}

	lutIndex = 0;
}

//--------------------------------------------------------------
bool ofxGpuLutCube::loadLUT(std::string s)
{
	//ofLogNotice(__FUNCTION__) << "Index : " << lutIndex;
	//ofLogNotice(__FUNCTION__) << "Path  : " << lutPaths[lutIndex];
	//ofLogNotice(__FUNCTION__) << "Name  : " << lutNames[lutIndex];
	ofLogNotice(__FUNCTION__) << lutIndex << "/" << numLuts << " : " << lutPaths[lutIndex];

	LUTname = lutNames[lutIndex];

	//-

	LUTpath = ofToDataPath(s);
	ifstream LUTfile(LUTpath.c_str());

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

		LUTname = "INCORRECT SIZE " + lutNames[lutIndex];

		//return;//skip load this lut
		//std::exit(1);
	}
	else if (ofNextPow2(LUT3dSize) != LUT3dSize)
	{
		ofLogError(__FUNCTION__) << "LUT needs to be pow2.";
		bErrorBadSize = true;

		LUTname = "POW2 ERROR " + lutNames[lutIndex];

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
		//disable rectangle textures
		const bool bArbTex = ofGetUsingArbTex();
		ofDisableArbTex();

		//reference from http://content.gpwiki.org/index.php/OpenGL:Tutorials:3D_Textures
		//create a 3D texture
		glEnable(GL_TEXTURE_3D);
		glGenTextures(1, &texture3D);
		glBindTexture(GL_TEXTURE_3D, texture3D);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, LUT3dSize, LUT3dSize, LUT3dSize, 0, GL_RGB, GL_FLOAT, &LUT[0]);
		glBindTexture(GL_TEXTURE_3D, 0);
		glDisable(GL_TEXTURE_3D);

		if (bArbTex) ofEnableArbTex();
		else ofDisableArbTex();
	}

	//-

	//must check if size or cuve file is refused 
	if (bErrorBadSize)
		return false;
	else
		return true;
}

//--------------------------------------------------------------
void ofxGpuLutCube::begin()
{
	fbo.begin();
	ofClear(0, 0);
}

//--------------------------------------------------------------
void ofxGpuLutCube::end()
{
	fbo.end();
}

//--------------------------------------------------------------
void ofxGpuLutCube::drawHelp()
{
	float x, y;
	x = 20;
	y = 100;

	//browsing info
	string str;
	str += "PRESS UP/DOWN TO BROWSE '.cube' FILES\n";
	str += "FROM FOLDER 'data/" + path_LUT_files + "/'\n";
	str += "[" + ofToString(lutIndex) + "/" + ofToString(numLuts - 1) + "] " + ofToString(LUTname);
	drawTextBoxed(str, x, y);

	//filename
	//bottom cenetered
	float w = getWidthBBtextBoxed(LUTname);
	x = ofGetWidth() * 0.5f - w * 0.5f;
	y = ofGetHeight() - 75;
	drawTextBoxed(LUTname, x, y);
}

//--------------------------------------------------------------
void ofxGpuLutCube::draw()
{
	//run lut shader
	lutFilter.begin();

	//source

	//TODO:
	//v flipping issues
	//if (bFlip)
	//fbo.getTextureReference().getTextureData().bFlipTexture = true;
	//if (bFlip)
	//{
	//	texFlipped = fbo.getTexture();
	//	float pos = ofGetWidth() / 2.;
	//	texFlipped.draw(ofGetWidth(), 0, -pos, ofGetHeight());
	//}

	lutFilter.setUniformTexture("tex", fbo.getTextureReference(), 0);
	lutFilter.setUniformTexture("lutTexure", GL_TEXTURE_3D, texture3D, 1);
	lutFilter.setUniform1f("lutSize", LUT3dSize);
	lutFilter.setUniform2f("mouse", control1.get(), control2.get());//control: can rename 'mouse' (into shader filestoo) to something like controlxy..

	ofPushStyle();
	{
		ofSetColor(ofColor::white);
		plane.draw();
	}
	ofPopStyle();

	lutFilter.end();
}

////TODO:
////custom position and size..
////--------------------------------------------------------------
//void ofxGpuLutCube::draw(float x, float y)
//{
//
//}
////--------------------------------------------------------------
//void ofxGpuLutCube::setSize(float w, float h)
//{
//
//}

//--------------------------------------------------------------
void ofxGpuLutCube::windowResized(int w, int h)
{
	ofLogNotice(__FUNCTION__) << w << "," << h;

	bool bArbPRE = ofGetUsingArbTex();
	ofDisableArbTex();

	fbo.allocate(w, h);

	if (bArbPRE) ofEnableArbTex();
	else ofDisableArbTex();

	//-

	//TODO:
	//v flipping issues
	//fbo.getTextureReference().getTextureData().bFlipTexture = bFlip;

	//set a plane to texture
	if (!bFlip)
	{
		plane.set(w, h, 2, 2);//size, col, rows..
		plane.setPosition(0.5f*w, 0.5f*h, 0);
	}
	else
	{
		plane.set(w, -h, 2, 2);
		plane.setPosition(0.5f*w, 0.5f*h, 0);
	}
}

//--------------------------------------------------------------
void ofxGpuLutCube::loadNext()
{
	ofLogNotice(__FUNCTION__);

	DISABLE_Callbacks = true;
	int i = lutIndex;
	i++;
	if (i >= numLuts) i = 0;
	DISABLE_Callbacks = false;

	lutIndex = i;
}

//--------------------------------------------------------------
void ofxGpuLutCube::loadPrevious()
{
	ofLogNotice(__FUNCTION__);

	DISABLE_Callbacks = true;
	int i = lutIndex;
	i--;
	if (i <= 0) i = numLuts - 1;
	DISABLE_Callbacks = false;

	lutIndex = i;
}

//--------------------------------------------------------------
void ofxGpuLutCube::loadRandomize()
{
	ofLogNotice(__FUNCTION__);

	loadLut(ofRandom(numLuts));
}

//--------------------------------------------------------------
int ofxGpuLutCube::geNumtLuts()
{
	return numLuts;
}

//--------------------------------------------------------------
void ofxGpuLutCube::exit()
{
	ofRemoveListener(params.parameterChangedE(), this, &ofxGpuLutCube::Changed_params);
}

//--------------------------------------------------------------
void ofxGpuLutCube::loadLut(int lut)
{
	ofLogNotice(__FUNCTION__) << lut;
	setSelectedLut(lut);
}

//--------------------------------------------------------------
void ofxGpuLutCube::setSelectedLut(int lut)
{
	ofLogNotice(__FUNCTION__) << lut;
	DISABLE_Callbacks = true;
	if (lut < 0) lut = 0;
	else if (lut > numLuts - 1) lut = numLuts - 1;
	DISABLE_Callbacks = false;

	lutIndex = lut;
}

void ofxGpuLutCube::Changed_params(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		string name = e.getName();

		//exclude debugs
		if (name != "" && 
			name != "exclude")
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		if (name == "LUT" && lutIndex_PRE != lutIndex)
		{
			//ignore sliders on/release to avoid double loading the same lut

			DISABLE_Callbacks = true;
			if (lutIndex < 0) lutIndex = 0;
			else if (lutIndex > numLuts - 1) lutIndex = numLuts - 1;
			DISABLE_Callbacks = false;

			bool b = loadLUT(lutPaths[lutIndex]);

			//TODO:
			//bool bLoaded;
			//bLoaded = loadLUT(lutPaths[lutIndex]);
			//if (bLoaded)
			//	ofLogNotice(__FUNCTION__) << "LUT file loaded";
			//else
			//{
			//	ofLogError(__FUNCTION__) << "LUT file " + ofToString(lutIndex) + " not loaded";
			//	ofLogError(__FUNCTION__) << "back to previous lut " + ofToString(lutIndex_PRE) + " that worked";
			//	lutIndex = lutIndex_PRE;
			//	//must check if size is refused 
			//	loadLUT(lutPaths[lutIndex]);
			//}

			lutIndex_PRE = lutIndex;
		}
	}
}