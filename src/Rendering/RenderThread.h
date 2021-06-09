#include "ofMain.h"
#include "Renderer.h"

class RenderThread : public ofThread {
private:
	Renderer *renderer;
	glm::vec2 dim;
	glm::vec2 subSection;
	int samples;
	float *percent;
public:
	void setup(Renderer *r, glm::vec2 sub, glm::vec2 d, int s, float &p);
	void threadedFunction();
};
