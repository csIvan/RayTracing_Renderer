#pragma once

// Graphics & Math
#define ZERO_VECTOR glm::vec3(0,0,0)
#define RAY_BIAS 0.001f
#define EPSILON 0.05

// LSysteem
#define HEIGHT_MULTIPLIER 0.7
#define RADIUS_MULTIPLIER 0.8

// Rendering
#define MAX_RAY_STEPS 100
#define DIST_THRESHOLD .0001
#define MAX_THRESHOLD 15

#define GRID_LINES 100

// Scene
#define NEAR_CLIP 0.1
#define DEFAULT_RADIUS 0.5
#define DEFAULT_COLOR ofColor::gray
#define SELECTED_COLOR ofColor::yellow