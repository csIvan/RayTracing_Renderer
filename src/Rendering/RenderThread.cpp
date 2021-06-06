#include "RenderThread.h"

void RenderThread::setup(ofImage *img, glm::vec2 sub, glm::vec2 d, int s) {
	image = img;
	dim = d;
	subSection = sub;
	samples = s;
}

void RenderThread::threadedFunction() {

}
