#pragma once

#include "ofMain.h"

///	TODO:
/// +	files hanlde snippets:
/// 		https://github.com/roymacdonald/faceCropAndNormalize/blob/master/src/ofApp.cpp
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

#define USE_FILE_BROWSER	//use ImGui: browse materials and tweak some settings
#ifdef USE_FILE_BROWSER
#include "ofxImGui.h"
#endif

class thumbsBrowser
{
public:

	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);

	bool loadLUTaddon(std::string path);//addon 
	void loadLUT(std::string path);//OF example
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
	int LUT3dSize = MAX_LUT_SIZE;

	glm::vec3 lut[MAX_LUT_SIZE][MAX_LUT_SIZE][MAX_LUT_SIZE];

	ofImage lutImg;

	ofImage sourceImg;

	void doExportThumb();

	struct RGB { float r, g, b; };

	std::string LUTname;


	//----

#ifdef USE_FILE_BROWSER
		//int getCurrentIndex();

	ofParameter<std::string> nameMat;
	ofParameter<int> sizeThumb;
	ofParameter<int> indexBrowser;
	//bool bAutoResize;//TODO:

//private:
	string pathGlobal = "ofxLitSphere";
	//ofParameter<string> pathFull;
	string matName;

	ofParameter<bool> bPathDirCustom;
	ofParameter<string> pathDirCustom;

	//public:
	//	string getName()
	//	{
	//		pathFull = pathGlobal + "/" + "MatCapZBrush/Lib/\\";
	//		auto _names = ofSplitString(matName, pathFull);
	//		ofLogNotice() << __FUNCTION__ << "_names[1]: " << _names[1];
	//		return _names[1];
	//	}
	//private:
	//	string getPath()
	//	{
	//		pathFull = pathGlobal + "/" + "MatCapZBrush/Lib/";
	//		return pathFull;
	//	}

	//	int indexMat;
	//	int sizeDirMats = -1;
	//	string pathDirMats = "-1";
	string pathDirPreviews = "-1";

	//	//browser
	//	string mapCapName;
	//	string inputPath;

		//-

public:
	void processOpenFileSelection(ofFileDialogResult openFileResult);

	bool bShowGui;
	bool isVisibleGui() {
		return bShowGui;
	}
	void setVisibleGui(bool b) {
		bShowGui = b;
	}
	void setToggleVisibleGui() {
		bShowGui = !bShowGui;
	}

	//-

private:
	bool bShowBrowser = true;

private:
	void setupGui();
	void refreshFiles();
	//void updateGui();
	void drawGui();

	ofxImGui::Gui gui_ImGui;
	void draw_ImGui(int x, int y, int w, int h);

private:
	ofDirectory dirThumbs;
	vector<ofTexture> textureSource;
	vector<GLuint> textureSourceID;

	string pathFull;//real lut file

	string getName()//from matName
	{
		pathFull = pathGlobal + "/" + "thumbs/";
		auto _names = ofSplitString(matName, pathFull);
		ofLogNotice() << __FUNCTION__ << "_names[1]: " << _names[1];
		return _names[1];
	}

	//--

	//ImGui theme
	void ModernDarkTheme()
	{
		auto& style = ImGui::GetStyle();
		style.ChildRounding = 0;
		style.GrabRounding = 0;
		style.FrameRounding = 2;
		style.PopupRounding = 0;
		style.ScrollbarRounding = 0;
		style.TabRounding = 2;
		style.WindowRounding = 0;
		style.FramePadding = { 4, 4 };
		style.WindowTitleAlign = { 0.0, 0.5 };
		style.ColorButtonPosition = ImGuiDir_Left;
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = { 1.0f, 1.0f, 1.0f, 1.00f };				//
		colors[ImGuiCol_TextDisabled] = { 0.25f, 0.25f, 0.25f, 1.00f };		//
		colors[ImGuiCol_WindowBg] = { 0.09f, 0.09f, 0.09f, 0.94f };			//
		colors[ImGuiCol_ChildBg] = { 0.11f, 0.11f, 0.11f, 1.00f };			//
		colors[ImGuiCol_PopupBg] = { 0.11f, 0.11f, 0.11f, 0.94f };			//
		colors[ImGuiCol_Border] = { 0.07f, 0.08f, 0.08f, 1.00f };
		colors[ImGuiCol_BorderShadow] = { 0.00f, 0.00f, 0.00f, 0.00f };
		colors[ImGuiCol_FrameBg] = { 0.35f, 0.35f, 0.35f, 0.54f };			//
		colors[ImGuiCol_FrameBgHovered] = { 0.31f, 0.29f, 0.27f, 1.00f };
		colors[ImGuiCol_FrameBgActive] = { 0.40f, 0.36f, 0.33f, 0.67f };
		colors[ImGuiCol_TitleBg] = { 0.1f, 0.1f, 0.1f, 1.00f };
		colors[ImGuiCol_TitleBgActive] = { 0.3f, 0.3f, 0.3f, 1.00f };		//
		colors[ImGuiCol_TitleBgCollapsed] = { 0.0f, 0.0f, 0.0f, 0.61f };
		colors[ImGuiCol_MenuBarBg] = { 0.18f, 0.18f, 0.18f, 0.94f };		//
		colors[ImGuiCol_ScrollbarBg] = { 0.00f, 0.00f, 0.00f, 0.16f };
		colors[ImGuiCol_ScrollbarGrab] = { 0.24f, 0.22f, 0.21f, 1.00f };
		colors[ImGuiCol_ScrollbarGrabHovered] = { 0.31f, 0.29f, 0.27f, 1.00f };
		colors[ImGuiCol_ScrollbarGrabActive] = { 0.40f, 0.36f, 0.33f, 1.00f };
		colors[ImGuiCol_CheckMark] = { 0.84f, 0.84f, 0.84f, 1.0f };			//
		colors[ImGuiCol_SliderGrab] = { 0.8f, 0.8f, 0.8f, 1.0f };			//		
		colors[ImGuiCol_SliderGrabActive] = { 0.55f, 0.55f, 0.55f, 1.00f }; //
		colors[ImGuiCol_Button] = { 0.55f, 0.55f, 0.55f, 0.40f };			//
		colors[ImGuiCol_ButtonHovered] = { 0.15f, 0.15f, 0.15f, 0.62f };	//	
		colors[ImGuiCol_ButtonActive] = { 0.60f, 0.60f, 0.60f, 1.00f };		//
		colors[ImGuiCol_Header] = { 0.84f, 0.36f, 0.05f, 0.0f };			//
		colors[ImGuiCol_HeaderHovered] = { 0.25f, 0.25f, 0.25f, 0.80f };	//
		colors[ImGuiCol_HeaderActive] = { 0.42f, 0.42f, 0.42f, 1.00f };
		colors[ImGuiCol_Separator] = { 0.35f, 0.35f, 0.35f, 0.50f };		//
		colors[ImGuiCol_SeparatorHovered] = { 0.31f, 0.29f, 0.27f, 0.78f };
		colors[ImGuiCol_SeparatorActive] = { 0.40f, 0.36f, 0.33f, 1.00f };
		colors[ImGuiCol_ResizeGrip] = { 1.0f, 1.0f, 1.0f, 0.25f };			//
		colors[ImGuiCol_ResizeGripHovered] = { 1.00f, 1.0f, 1.0f, 0.4f };	//
		colors[ImGuiCol_ResizeGripActive] = { 1.00f, 1.00f, 1.0f, 0.95f };	//
		colors[ImGuiCol_Tab] = { 0.18f, 0.18f, 0.18f, 1.0f };				//
		colors[ImGuiCol_TabHovered] = { 0.58f, 0.58f, 0.58f, 0.80f };		//
		colors[ImGuiCol_TabActive] = { 0.6f, 0.60f, 0.60f, 1.00f };
		colors[ImGuiCol_TabUnfocused] = { 0.07f, 0.10f, 0.15f, 0.97f };
		colors[ImGuiCol_TabUnfocusedActive] = { 0.14f, 0.26f, 0.42f, 1.00f };
		colors[ImGuiCol_PlotLines] = { 0.66f, 0.60f, 0.52f, 1.00f };
		colors[ImGuiCol_PlotLinesHovered] = { 0.98f, 0.29f, 0.20f, 1.00f };
		colors[ImGuiCol_PlotHistogram] = { 0.60f, 0.59f, 0.10f, 1.00f };
		colors[ImGuiCol_PlotHistogramHovered] = { 0.72f, 0.73f, 0.15f, 1.00f };
		colors[ImGuiCol_TextSelectedBg] = { 0.27f, 0.52f, 0.53f, 0.35f };
		colors[ImGuiCol_DragDropTarget] = { 0.60f, 0.59f, 0.10f, 0.90f };
		colors[ImGuiCol_NavHighlight] = { 0.51f, 0.65f, 0.60f, 1.00f };
		colors[ImGuiCol_NavWindowingHighlight] = { 1.00f, 1.00f, 1.00f, 0.70f };
		colors[ImGuiCol_NavWindowingDimBg] = { 0.80f, 0.80f, 0.80f, 0.20f };
		colors[ImGuiCol_ModalWindowDimBg] = { 0.11f, 0.13f, 0.13f, 0.35f };
	}
#endif
};

