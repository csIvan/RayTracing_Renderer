#pragma once

#include "ofMain.h"

// Custom UI Tab class
class Tab : public ofRectangle {
private:
	string label;
	bool isActive;
	ofTrueTypeFont font;

public:
	Tab(float x, float y, float width, float height, string label, bool active)
		: ofRectangle(x, y, width, height), label(label), isActive(active) {
		font.load("Fonts/Poppins/Poppins-Medium.ttf", 10);
	};

	~Tab() {};

	// Draw rounded rectangle image
	void draw() {
		ofPath path;
		path.setFillColor(isActive ? ofColor(130) : ofColor(50));
		path.rectRounded(x, y, width, height, 10);
		path.draw();
		ofSetColor(isActive ? 255 : 120);
		font.drawString(label, x + 17, y + height / 2 + 5);
	}

	bool isMousePressed(int mouseX, int mouseY) {
		return this->inside(mouseX, mouseY);
	}

	void setActive(bool active) {
		isActive = active;
	}
};