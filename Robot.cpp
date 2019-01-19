#include "Robot.h"
# define M_PI           3.14159265358979323846  /* pi */
void Robot::init(float x, float y, bool randomPos)
{
	field = new Field();
	field->init(100);
	field->deadzones = field->deadzones;
	field->world_size = field->world_size;

	this->x = x;
	this->y = y;

	if (randomPos)
	{
		this->x = rand() % field->world_size;
		this->y = rand() % field->world_size;
	}

	orientation = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (M_PI*2)));
	field->landmarks = field->landmarks; 
	forward_noise = 0.0f;
	turn_noise = 0.0f;
	sense_noise = 0.0f;

}
Robot* Robot::move(float fwd, float heading)
{
	orientation += heading + gauss(0, this->turn_noise);
	orientation = std::fmod(orientation, M_PI*2);
	auto x_fwd = sin(this->orientation)*fwd + gauss(0.0, this->forward_noise);
	auto y_fwd = cos(this->orientation)*fwd + gauss(0.0, this->forward_noise);
	x += x_fwd;
	y += y_fwd;
	x = std::fmod(x, field->world_size);
	y = std::fmod(y, field->world_size);

	Robot* robot = new Robot();
	robot->set(this->x, this->y, this->orientation);
	robot->set_noise(this->forward_noise, this->turn_noise, this->sense_noise);
	robot->field = field;
	return robot;
}
void Robot::set(float new_x, float new_y, float new_orientation)
{
	this->x = new_x;
	this->y = new_y;
	this->orientation = new_orientation;
}
std::pair<float,float> Robot::getPos()
{
	return std::pair<float,float>{this->x,this->y};
}
bool Robot::lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{

	auto uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	auto uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

	return (uA >= 0 and uA <= 1 and uB >= 0 and uB <= 1);
}
bool Robot::isColliding(float x1, float y1, float x2, float y2, float rx1, float ry1, float rx2, float ry2)
{
	auto left = lineLine(x1, y1, x2, y2, rx1, ry2, rx1, ry1);
	auto right = lineLine(x1, y1, x2, y2, rx2, ry2, rx2, ry1);
	auto top = lineLine(x1, y1, x2, y2, rx2, ry2, rx1, ry2);
	auto bottom = lineLine(x1, y1, x2, y2, rx2, ry1, rx1, ry1);

	return left || right || top || bottom;
}
bool Robot::canSense(std::pair<int, int> landmark)
{
	auto bl = field->deadzones[2];
	auto tr = field->deadzones[1];
	return !(isColliding(landmark.first, landmark.second, x, y, tr.first, tr.second, bl.first, bl.second));
}
//penis

std::vector<float> Robot::sense()
{
	std::vector<float> dists;
	for (auto landmark : field->landmarks)
	{
		if (canSense(landmark))
		{
			auto dist = sqrt(powf((x - landmark.first), 2) + pow((y - landmark.second), 2));
			dist += gauss(0.0, this->sense_noise);
			dists.push_back(dist);
		}
		else {
			dists.push_back(-1);
		}
	}
	return dists;
}
float Robot::Gaussian(float mu, float sigma, float x)
{
	return exp(-(powf((mu - x), 2) / powf(sigma, 2) / 2.0) / sqrt(2.0 * M_PI * powf(sigma, 2)));
}
float Robot::measurement_prob(std::vector<float> measurement)
{
	auto prob = 1.0;
	for (int i = 0; i != field->landmarks.size(); i++)
	{
		if (measurement[i] == -1)
			continue;
		auto landmark = field->landmarks[i];
		auto dist = sqrt(powf((x - landmark.first), 2) + pow((y - landmark.second), 2));
		auto g =  Gaussian(dist, this->sense_noise, measurement[i]);
		prob *= g;
	}
	return prob;
}
void Robot::set_noise(float new_f_noise, float new_t_noise, float new_s_noise)
{
	forward_noise = new_f_noise;
	turn_noise = new_t_noise;
	sense_noise = new_s_noise;
}

float Robot::eval(std::pair<int, int> r, std::vector<std::pair<int, int>>p)
{
	auto sum = 0.0f;
	for (int i = 0; i != p.size(); i++)
	{
		auto dx = (p[i].first - r.first + (field->world_size / 2)) % field->world_size - (field->world_size / 2.0);
		auto dy = (p[i].second - r.second + (field->world_size / 2)) % field->world_size - (field->world_size / 2.0);
		auto err = sqrt(dx*dx + dy * dy);
		sum += err;
	}

	return sum / p.size();
}

float Robot::gauss(float un, float dos)
{
	if (un == 0.0f && dos == 0.0f)
		return 0.0f;
	std::mt19937 gen{ rd() };

	std::normal_distribution<> d{ un,dos };
	return d(gen);
}

std::string Robot::PosString()
{
	return "X: " + std::to_string(this->x) + " | Y: " + std::to_string(this->y) + "\n";
}