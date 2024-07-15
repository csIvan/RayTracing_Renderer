#include "Renderer.h"
#include "../../Scene/Scene.h"


ofImage Renderer::render(int samples, const RenderCam &cam, const Scene &scene) {
	isRendering = true;
	std::atomic<float> percent(0.0f);
	renderSection(0, 0, imageWidth, imageHeight, samples, cam, scene, percent);
	isRendering = false;
	return image;
}

ofImage Renderer::renderMT(int threadCount, int samples, const RenderCam &cam, const Scene &scene) {
	isRendering = true;
	numberOfThreads = threadCount;
	std::atomic<float> percent(0.0f);
	int rowsPerThread = imageHeight / numberOfThreads;
	vector<thread> threads;

	for (int i = 0; i < numberOfThreads; i++) {
		int startY = i * rowsPerThread;
		int endY = (i == numberOfThreads - 1) ? imageHeight : startY + rowsPerThread;
		int startX = 0;
		int endX = imageWidth;
		threads.emplace_back(&RayTracer::renderSection, this, startX, startY, endX, endY, samples, ref(cam), ref(scene), ref(percent));
	}

	for (int i = 0; i < numberOfThreads; i++) {
		threads[i].join();
	}
	isRendering = false;
	return image;
}

void Renderer::renderSection(int startX, int startY, int endX, int endY, int samples, const RenderCam &cam, const Scene &scene, std::atomic<float> &percent) {
	// Set up Random Number Generator for jitter supersampling
	random_device randomSeed;
	mt19937 randomGenerator(randomSeed());
	uniform_real_distribution<> distribution(0.0, 1.0f);

	for (int row = startY; row < endY; row++) {
		for (int column = startX; column < endX; column++) {
			glm::vec3 finalColor = ZERO_VECTOR;
			int numSamples = static_cast<int>(sqrt(samples));
			float sampleSize = 1.0f / numSamples;

			for (int i = 0; i < numSamples; i++) {
				for (int j = 0; j < numSamples; j++) {
					float jitterX = (numSamples == 1) ? 0.5f : distribution(randomGenerator);
					float jitterY = (numSamples == 1) ? 0.5f : distribution(randomGenerator);

					Ray ray = cam.getRay((column + (j + jitterX) * sampleSize) / imageWidth,
						1 - (row + (i + jitterY) * sampleSize) / imageHeight);

					ofColor color;
					if (castRay(ray, color, scene)) {
						finalColor += glm::vec3(color.r, color.g, color.b);
					}
					else {
						finalColor += ZERO_VECTOR;	// dark background
					}
				}
			}
			finalColor /= samples;

			{	// mutex lock scope
				lock_guard<mutex> lock(imageMutex);
				image.setColor(column, row, ofColor(finalColor.x, finalColor.y, finalColor.z));
			}
		}
		percent.fetch_add((100.0f) / imageHeight, memory_order_relaxed);
		std::printf("\rRendering... %d%%", static_cast<int>(percent));
	}
}
