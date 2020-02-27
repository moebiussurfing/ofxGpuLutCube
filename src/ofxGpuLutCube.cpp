#include "ofxGpuLutCube.h"
//--------------------------------------------------------------
void ofxGpuLutCube::setupFiles()
{
	ofDirectory dir;
	dir.allowExt("cube");
	dir.allowExt("CUBE");
	dir.listDir(path_LUT_files);
	dir.sort();
	numLuts = dir.size();
	ofLogNotice(__FUNCTION__) << "num LUTs:" << numLuts;

	lutNames.resize(numLuts);
	lutPaths.resize(numLuts);

	for (int i = 0; i < numLuts; i++)
	{
		std::string lutPath = dir.getPath(i);
		lutPaths[i] = lutPath;

		ofStringReplace(lutPath, "LUT", "");
		ofStringReplace(lutPath, ".cube", "");
		ofStringReplace(lutPath, ".CUBE", "");
		ofStringReplace(lutPath, "\\", "");//deltete '\' char from name
		lutNames[i] = lutPath;

		ofLogNotice(__FUNCTION__) << "lutPaths[" << i << "]: " << lutPaths[i];
		ofLogNotice(__FUNCTION__) << "lutNames[" << i << "]: " << lutNames[i];
	}

	lutIndex = 0;
}

//--------------------------------------------------------------
void ofxGpuLutCube::setupLUT(std::string s)
{
	ofLogNotice(__FUNCTION__) << "lutIndex:" << lutIndex;
	ofLogNotice(__FUNCTION__) << "lutPaths[lutIndex]:" << lutPaths[lutIndex];
	ofLogNotice(__FUNCTION__) << "lutNames[lutIndex]:" << lutNames[lutIndex];
	LUTname = lutNames[lutIndex];

	//-

	LUTpath = ofToDataPath(s);
	ifstream LUTfile(LUTpath.c_str());

	vector<RGB> LUT;

	//-

	//code from 
	//https://github.com/johanjohan to point me to this source here https://github.com/yasuhirohoshino/ofxGpuLut/issues/3

	const string key_LUT_3D_SIZE = "LUT_3D_SIZE";

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
				ofLogNotice(__FUNCTION__) << "found key_LUT_3D_SIZE: " << subs[1];
				LUT3dSize = ofToInt(subs[1]);
			}
		}
	}//while

	if (LUT.size() != (pow(LUT3dSize, 3.0)))
	{
		ofLogError(__FUNCTION__) << "LUT size is incorrect.";
		return;//skip load this lut
		//std::exit(1);
	}
	else if (ofNextPow2(LUT3dSize) != LUT3dSize)
	{
		ofLogError(__FUNCTION__) << "LUT needs to be pow2.";
		return;//skip load this lut
		//std::exit(1);
	}

	ofLogNotice(__FUNCTION__) << "LUT.size(): " << LUT.size() << " --> " << int(ceil(pow(LUT.size(), 1.0 / 3.0)));
	ofLogNotice(__FUNCTION__) << "LUT3dSize: " << LUT3dSize;

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

	//disable rectangle textures
	ofDisableArbTex();

	//create a 3D texture
	//reference from http://content.gpwiki.org/index.php/OpenGL:Tutorials:3D_Textures
	glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &texture3D);
	glBindTexture(GL_TEXTURE_3D, texture3D);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, LUT3dSize, LUT3dSize, LUT3dSize, 0, GL_RGB,
		GL_FLOAT, &LUT[0]);
	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);

	//-

	//ofEnableArbTex();
}

//--------------------------------------------------------------
void ofxGpuLutCube::setup()
{
	//params
	params.setName("ofxGpuLutCube");
	control1.set("MIX", 1, 0, 1);
	control2.set("control2", 1, 0, 1);//not used in shader
	params.add(control1);
	//params.add(control2);//not used in shader. could addit to other function..

	//gui
	//gui.setup("LUT");
	//gui.add(params);

	//-

	//load the shader
	lutFilter.load("Shaders/ShaderLUT/lut_filter");

	//read lut files folder
	setupFiles();

	//load first LUT file on folder
	lutIndex = 0;
	LUTname = lutNames[lutIndex];
	setupLUT(lutPaths[lutIndex]);

	//--

	//source
	fbo.allocate(ofGetWidth(), ofGetHeight());

	//TODO:
	//v flipping issues
	//fbo.getTextureReference().getTextureData().bFlipTexture = bFlip;

	//set a plane to texture
	if (!bFlip)
	{
		plane.set(ofGetWidth(), ofGetHeight(), 2, 2);//size, col, rows..
		plane.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	}
	else
	{
		plane.set(ofGetWidth(), -ofGetHeight(), 2, 2);
		plane.setPosition(0.5f*ofGetWidth(), 0.5f*ofGetHeight(), 0);
	}
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
void ofxGpuLutCube::draw()
{
	//ofEnableArbTex();//not required

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

	plane.draw();

	lutFilter.end();
}

//--------------------------------------------------------------
void ofxGpuLutCube::windowResized(int w, int h) {
	fbo.allocate(ofGetWidth(), ofGetHeight());
	//fbo.getTextureReference().getTextureData().bFlipTexture = bFlip;

	//TODO:
	//v flipping issues
	//fbo.getTextureReference().getTextureData().bFlipTexture = bFlip;

	//set a plane to texture
	if (!bFlip)
	{
		plane.set(ofGetWidth(), ofGetHeight(), 2, 2);//size, col, rows..
		plane.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	}
	else
	{
		plane.set(ofGetWidth(), -ofGetHeight(), 2, 2);
		plane.setPosition(0.5f*ofGetWidth(), 0.5f*ofGetHeight(), 0);
	}
}

//--------------------------------------------------------------
void ofxGpuLutCube::next()
{
	lutIndex++;
	if (lutIndex >= numLuts)
		lutIndex = 0;
	setupLUT(lutPaths[lutIndex]);
}

//--------------------------------------------------------------
void ofxGpuLutCube::previous()
{
	lutIndex--;
	if (lutIndex <= 0)
		lutIndex = numLuts - 1;
	setupLUT(lutPaths[lutIndex]);
}

//for internal gui
////--------------------------------------------------------------
//void ofxGpuLutCube::drawGui()
//{
//	gui.draw();
//}
////--------------------------------------------------------------
//void ofxGpuLutCube::keyPressed(int key)
//{
//	//browse luts
//	if (key == OF_KEY_DOWN)
//	{
//
//	}
//	else if (key == OF_KEY_UP)
//	{
//
//	}
//}
