#pragma once

#include "ofMain.h"
#include "UI.h"
#include "../Scene/Box.h"

class ofApp : public ofBaseApp {

public:
	// OpenFrameworks scene variables
	ofEasyCam  mainCam;
	ofCamera  *theCam;    // set to current camera either mainCam or sideCam
	ofCamera sideCam;
	ofCamera previewCam;
	ofLight lightScene;
	ofMaterial material;

	UI ui;
	Scene scene;

	void setup();
	void update();
	void draw();

	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
	void dragEvent(ofDragInfo dragInfo);

};
