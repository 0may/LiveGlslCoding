# LiveGlslCoding

A tool for live graphics coding using GLSL. openFrameworks is used to display GLSL shader
code from a vertex and fragment shader file. The files are tested for modifications and validity
during runtime and reloaded if necessary.

An arbitrary code editor may be used to edit the GLSL code. For Lange Nacht der Wissenschaften 2017
@ AdBK Nürnberg I used Notepad++ with its Topmost plugin for a transparent editor 
window that stays on top.

openFrameworks version: 0.9.8

## Notes

Edit the "bool checkProgramLinkStatus(GLuint program)" in ofShader.h
to be public. This is required for checking the validity of shader files before 
loading.

On Lange Nacht der Wissenschaften an Arduino based input console was connected to 
the setup providing two switches and three potentiometers for visitors to change 
parameters in the code. Set the m_arduinoConnected variable in ofApp::setup() to false
if no arduino is connected.