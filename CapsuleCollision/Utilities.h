#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>

const int ki_SCREENWIDTH = 1600;
const int ki_SCREENHEIGHT = 900;
const float kf_NORMALX = static_cast<float>(ki_SCREENHEIGHT) / static_cast<float>(ki_SCREENWIDTH);
const glm::vec3 lightPos = glm::vec3(0.0f, 10000.0f, 0.0f);
static std::random_device rd;
static std::mt19937 gen(rd());

// (C++11) Returns a double between the lower and upper bound specified.
static double Random(double _lowerBound, double _upperBound)
{	
	std::uniform_real_distribution<> dis(_lowerBound, _upperBound);
	return dis(gen);
}