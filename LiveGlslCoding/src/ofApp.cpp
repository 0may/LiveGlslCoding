#include "ofApp.h"
#include <Windows.h>
#include <WinBase.h>
#include <fileapi.h>
#include <Shlwapi.h>

//--------------------------------------------------------------
void ofApp::setup(){



	m_showValues = true;
	m_eps = 0.003;

	m_switch1 = 0.0;
	m_switch2 = 0.0;
	m_param1 = 0.0;
	m_param2 = 0.0;
	m_param3 = 0.0;

	m_arduinoConnected = true;

	if (m_arduinoConnected)
		initArduino();

	m_fragShaderPath = ofToDataPath("shader.frag", true);
	m_fragShaderRunPath = ofToDataPath("shader.frag.run", true);
	m_vertShaderPath = ofToDataPath("shader.vert", true);
	m_vertShaderRunPath = ofToDataPath("shader.vert.run", true);

	m_fragWriteTime.dwLowDateTime = 0;
	m_fragWriteTime.dwHighDateTime = 0;
	m_vertWriteTime.dwLowDateTime = 0;
	m_vertWriteTime.dwHighDateTime = 0;

	m_lastShaderUpdateCallTime = -10.0;

	cout << "fragment shader path: " << m_fragShaderPath.c_str() << endl;
	cout << "fragment shader runtime path: " << m_fragShaderRunPath.c_str() << endl;
	cout << "vertex shader path: " << m_vertShaderPath.c_str() << endl;
	cout << "vertex shader runtime path: " << m_vertShaderRunPath.c_str() << endl;

	cout << "fragment shader file exists: " << (PathFileExists(m_fragShaderPath.c_str()) ? "TRUE" : "FALSE") << endl;
	cout << "vertex shader file exists: " << (PathFileExists(m_vertShaderPath.c_str()) ? "TRUE" : "FALSE") << endl;

	m_hFragShaderFile = CreateFileA(m_fragShaderPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	m_hVertShaderFile = CreateFileA(m_vertShaderPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	cout << "fragment shader file opened: " << (m_hFragShaderFile == INVALID_HANDLE_VALUE ? "FAILED" : "DONE") << endl;
	cout << "vertex shader file opened: " << (m_hVertShaderFile == INVALID_HANDLE_VALUE ? "FAILED" : "DONE") << endl;


	if (!CopyFileA(m_fragShaderPath.c_str(), m_fragShaderRunPath.c_str(), FALSE)) {
		cout << "copy runtime fragment shader: FAILED -- error: " << GetLastError() << endl;
	}
	if (!CopyFileA(m_vertShaderPath.c_str(), m_vertShaderRunPath.c_str(), FALSE)) {
		cout << "copy runtime vertex shader: FAILED -- error: " << GetLastError() << endl;
	}

	updateShader();
}

//--------------------------------------------------------------
void ofApp::update(){
	float v;
	float t = ofGetElapsedTimef();

	if (m_arduinoConnected) {
		m_arduino.update();

		m_switch1 = m_arduino.getDigital(2);
		m_switch2 = m_arduino.getDigital(3);

		v = m_arduino.getAnalog(0) / 1023.0f;
		if (v < m_eps || v > 1.0 - m_eps || abs(m_param1 - v) >= m_eps)
			m_param1 = v;

		if (m_switch1)
			m_param1_s.x = v;
		else
			m_param1_s.y = v;

		if (m_switch2)
			m_param1_s.z = v;
		else
			m_param1_s.w = v;


		v = m_arduino.getAnalog(1) / 1023.0f;
		if (v < m_eps || v > 1.0 - m_eps || abs(m_param2 - v) >= m_eps)
			m_param2 = v;

		if (m_switch1)
			m_param2_s.x = v;
		else
			m_param2_s.y = v;

		if (m_switch2)
			m_param2_s.z = v;
		else
			m_param2_s.w = v;


		v = m_arduino.getAnalog(2) / 1023.0f;
		if (v < m_eps || v > 1.0 - m_eps || abs(m_param3 - v) >= m_eps)
			m_param3 = v;

		if (m_switch1)
			m_param3_s.x = v;
		else
			m_param3_s.y = v;

		if (m_switch2)
			m_param3_s.z = v;
		else
			m_param3_s.w = v;


	}

	if (t - m_lastShaderUpdateCallTime > 0.5) {
		updateShader();
		m_lastShaderUpdateCallTime = t;
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0);

	m_shader.begin();

	m_shader.setUniform1f("TIME", ofGetElapsedTimef());
	m_shader.setUniform2f("RESOLUTION", ofVec2f(ofGetWidth(), ofGetHeight()));
	m_shader.setUniform1f("SWITCH_1", m_switch1);
	m_shader.setUniform1f("SWITCH_2", m_switch2);
	m_shader.setUniform1f("PARAM_1", m_param1);
	m_shader.setUniform1f("PARAM_2", m_param2);
	m_shader.setUniform1f("PARAM_3", m_param3);
	m_shader.setUniform4f("PARAM_1s", m_param1_s);
	m_shader.setUniform4f("PARAM_2s", m_param2_s);
	m_shader.setUniform4f("PARAM_2s", m_param3_s);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	m_shader.end();


	if (m_showValues) {
		ofDrawBitmapStringHighlight("SWITCH 1: " + ofToString(m_switch1), 10, 20, ofColor(0, 150), ofColor(255, 255));
		ofDrawBitmapStringHighlight("SWITCH 2: " + ofToString(m_switch2), 10, 42, ofColor(0, 150), ofColor(255, 255));
		ofDrawBitmapStringHighlight("PARAM  1: " + ofToString(m_param1), 10, 72, ofColor(0, 150), ofColor(255, 255));
		ofDrawBitmapStringHighlight("PARAM  2: " + ofToString(m_param2), 10, 94, ofColor(0, 150), ofColor(255, 255));
		ofDrawBitmapStringHighlight("PARAM  3: " + ofToString(m_param3), 10, 116, ofColor(0, 150), ofColor(255, 255));
	}
}

bool ofApp::initArduino() {

	if (m_arduino.connect("COM4", 57600)) {

		m_arduino.sendDigitalPinMode(2, ARD_INPUT);
		m_arduino.sendDigitalPinMode(3, ARD_INPUT);

		m_arduino.sendAnalogPinReporting(0, ARD_ON);
		m_arduino.sendAnalogPinReporting(1, ARD_ON);
		m_arduino.sendAnalogPinReporting(2, ARD_ON);

		return true;
	}
	else
		return false;
}



void ofApp::updateShader() {

	FILETIME writeTime;

	bool reloadShader = false;


	if (m_hFragShaderFile == INVALID_HANDLE_VALUE) {
		m_hFragShaderFile = CreateFileA(m_fragShaderPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	if (m_hFragShaderFile != INVALID_HANDLE_VALUE) {

		GetFileTime(m_hFragShaderFile, NULL, NULL, &writeTime);

		if (writeTime.dwHighDateTime != m_fragWriteTime.dwHighDateTime || writeTime.dwLowDateTime != m_fragWriteTime.dwLowDateTime) {

			if (m_testshader.load("shader.vert.run", "shader.frag") && m_testshader.checkProgramLinkStatus(m_testshader.getProgram())) {

				if (CopyFileA(m_fragShaderPath.c_str(), m_fragShaderRunPath.c_str(), FALSE)) {
					cout << "frag shader reload" << endl;
					m_fragWriteTime = writeTime;
					reloadShader = true;
				}
			}
			else {
				m_fragWriteTime = writeTime;
			}
		}
	}


	if (m_hVertShaderFile == INVALID_HANDLE_VALUE) {
		m_hVertShaderFile = CreateFileA(m_vertShaderPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	if (m_hVertShaderFile != INVALID_HANDLE_VALUE) {

		GetFileTime(m_hVertShaderFile, NULL, NULL, &writeTime);

		if (writeTime.dwHighDateTime != m_vertWriteTime.dwHighDateTime || writeTime.dwLowDateTime != m_vertWriteTime.dwLowDateTime) {

			if (m_testshader.load("shader.vert", "shader.frag.run") && m_testshader.checkProgramLinkStatus(m_testshader.getProgram())) {

				if (CopyFileA(m_vertShaderPath.c_str(), m_vertShaderRunPath.c_str(), FALSE)) {
					cout << "vert shader reload" << endl;
					m_vertWriteTime = writeTime;
					reloadShader = true;
				}
			}
			else {
				m_vertWriteTime = writeTime;
			}
		}
	}

	if (reloadShader) {
		m_shader.load("shader.vert.run", "shader.frag.run");
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 258)
		m_showValues = !m_showValues;
	else if (key == 257)
		ofToggleFullscreen();

	if (!m_arduinoConnected) {
		if (key == 49)
			m_switch1 = abs(m_switch1 - 1.0);
		else if (key == 50)
			m_switch2 = abs(m_switch2 - 1.0);
		else if (key == 51) {
			m_param1 += 0.05;
			if (m_param1 > 1.0)
				m_param1 = 0.0;

			if (m_switch1)
				m_param1_s.x = m_param1;
			else
				m_param1_s.y = m_param1;

			if (m_switch2)
				m_param1_s.z = m_param1;
			else
				m_param1_s.w = m_param1;
		}
		else if (key == 52) {
			m_param2 += 0.05;
			if (m_param2 > 1.0)
				m_param2 = 0.0;

			if (m_switch1)
				m_param2_s.x = m_param2;
			else
				m_param2_s.y = m_param2;

			if (m_switch2)
				m_param2_s.z = m_param2;
			else
				m_param2_s.w = m_param2;
		}
		else if (key == 53) {
			m_param2 += 0.05;
			if (m_param2 > 1.0)
				m_param2 = 0.0;

			if (m_switch1)
				m_param3_s.x = m_param3;
			else
				m_param3_s.y = m_param3;

			if (m_switch2)
				m_param3_s.z = m_param3;
			else
				m_param3_s.w = m_param3;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
