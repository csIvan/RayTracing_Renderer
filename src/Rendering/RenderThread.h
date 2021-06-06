#include "ofMain.h"

class RenderThread : public ofThread {
private:
	ofImage *image;
	glm::vec2 dim;
	glm::vec2 subSection;
	int samples;
public:
	void setup(ofImage *img, glm::vec2 sub, glm::vec2 d, int s);
	void threadedFunction();
};
