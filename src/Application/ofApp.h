#pragma once

#include "ofMain.h"
#include "Definitions.h"
#include "../UI/UI.h"
#include "../Scene/Scene.h"

class ofApp : public ofBaseApp{
private:
	// OF scene variables
	ofEasyCam interactiveCam;
	ofCamera *mainCam;
	ofCamera sideCam, previewCam;
	ofLight light;
	ofMaterial material;

	// Custom classes variables
	UI *ui;
	Scene *scene;

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	void dragEvent(ofDragInfo dragInfo);
		
};
