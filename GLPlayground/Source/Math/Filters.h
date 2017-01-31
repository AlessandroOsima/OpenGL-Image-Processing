#pragma once
#include <glm/glm.hpp>
class Filters
{
public:
	static glm::mat3 GenerateSmoothingMatrix();
	static glm::mat3 GenerateSharpenMatrix(float SharpenFactor);
	static glm::mat3 GenerateTestGaussMatrix();
	static glm::mat3 GenerateHighBoost(float BoostFactor);
	static glm::mat3 GenerateLaplacian();

	//moar
};

