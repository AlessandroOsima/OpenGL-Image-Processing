#pragma once
#include <glm/glm.hpp>
//Utilities functions that generate matrices for the box filter shader
class Filters
{
public:
	//Smoothing box filter
	static glm::mat3 GenerateSmoothingMatrix();
	//a 3x3 matrix formed of -1, the center can be 9 or 8 based on KeepBackground
	static glm::mat3 GenerateSharpenMatrix(bool KeepBackground);
	//This is the sharpen matrix used by OpenCV samples
	static glm::mat3 GenerateSharpenFactor5();
	static glm::mat3 GenerateTestGaussMatrix();
	static glm::mat3 GenerateHighBoost(float BoostFactor);
	static glm::mat3 GenerateLaplacian();
};

