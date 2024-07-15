#pragma once

#include <random>
#include <mutex>
#include <atomic>
#include <thread>
#include "ofMain.h"
#include "../RenderCam.h"
#include "../Shading/Shader.h"
#include "../../Scene/SceneObject.h"

class Scene; // forward declaration

// Base Renderer class
class Renderer {
protected:
	Shader shader;
	ofImage image;
	mutex imageMutex;
	int imageWidth, imageHeight;
	int numberOfThreads;
	bool isRendering;

public:
	virtual ~Renderer() {};


	// Take in the current 3D scene and render a 2D image (single thread)
	ofImage render(int samples, const RenderCam &cam, const Scene &scene);


	// Multithreaded function for rendering; splits the work into sections
	ofImage renderMT(int threadCount, int samples, const RenderCam &cam, const Scene &scene);


	// Renders subsection of image; specified by given dimensions
	void renderSection(int startX, int startY, int endX, int endY, int samples, const RenderCam &cam, const Scene &scene, std::atomic<float> &percent);


	// Check if given ray intersects with objects in the scene; if it does, update the color parameter
	virtual bool castRay(const Ray &ray, ofColor &color, const Scene &scene, int depth = 0) { return false; }


	// Returned rendered image
	ofImage getImage() { return image; }


	// Set the resolution of the image
	void setImageResolution(int width, int height) {
		imageWidth = width;
		imageHeight = height;
		image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);
	}

	// Determine if the Renderer is currently rendering
	bool IsRendering() {
		return isRendering;
	}
};