#pragma once
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
#include <vector>
#include "Field.h"

class Robot
{
public:
	std::random_device rd{};
	Field* field;


	float x;
	float y;
	float orientation;
	float forward_noise;
	float turn_noise;
	float sense_noise;

	void init(float x, float y, bool randomPos);
	Robot* move(float fwd, float heading);
	void set(float new_x, float new_y, float new_orientation);
	std::pair<float,float> getPos();
	bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	bool isColliding(float x1, float y1, float x2, float y2, float rx1, float ry1, float rx2, float ry2);
	bool canSense(std::pair<int, int> landmark);
	std::vector<float> sense();
	float Gaussian(float mu, float sigma, float x);
	float measurement_prob(std::vector<float>);
	void set_noise(float new_f_noise, float new_t_noise, float new_s_noise);
	float eval(std::pair<int, int> r, std::vector<std::pair<int, int>> p);
	float gauss(float un, float dos);
	std::string PosString();
};