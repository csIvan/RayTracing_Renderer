#include "RenderThread.h"

void RenderThread::setup(Renderer *r, glm::vec2 sub, glm::vec2 d, int s, float &p) {
	renderer = r;
	dim = d;
	subSection = sub;
	samples = s;
	percent = &p;
}

void RenderThread::threadedFunction() {
	glm::vec2 subDim = dim / 4.0f;
	glm::vec2 start = glm::vec2(subSection.x * subDim.x, subSection.y * subDim.y);

	renderer->mtRender(start, subDim, samples, *percent);
}
