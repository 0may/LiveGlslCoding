#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings displaySettings;

	// setup window
	displaySettings.width = 1280;
	displaySettings.height = 720;
	//displaySettings.setPosition(ofVec2f(1960, 20));
	displaySettings.setPosition(ofVec2f(400, 20));
	displaySettings.setGLVersion(4, 4);
	//displaySettings.windowMode = OF_FULLSCREEN;
	displaySettings.windowMode = OF_WINDOW;
	displaySettings.multiMonitorFullScreen = false;
	auto displayWindow = ofCreateWindow(displaySettings);

	auto displayApp = make_shared<ofApp>();

	ofRunApp(displayWindow, displayApp);
	ofRunMainLoop();
}
