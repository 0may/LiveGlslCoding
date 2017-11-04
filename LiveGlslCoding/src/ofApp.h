#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	bool initArduino();

	void updateShader();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
		
protected:

	ofShader m_shader;
	ofShader m_testshader;

	ofArduino m_arduino;
	bool m_arduinoConnected;

	bool m_showValues;
	float m_eps;

	float m_switch1;
	float m_switch2;
	float m_param1;
	float m_param2;
	float m_param3;

	ofVec4f m_param1_s;
	ofVec4f m_param2_s;
	ofVec4f m_param3_s;

	HANDLE m_hFragShaderFile;
	FILETIME m_fragWriteTime;
	
	HANDLE m_hVertShaderFile;
	FILETIME m_vertWriteTime;

	float m_lastShaderUpdateCallTime;

	string m_fragShaderPath;
	string m_fragShaderRunPath;
	string m_vertShaderPath;
	string m_vertShaderRunPath;

};
