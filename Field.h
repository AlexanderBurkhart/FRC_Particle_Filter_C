#pragma once
#include <vector>
class Field
{
public:
	void init(int world_size);

	std::pair<int, int> dz_tl;
	std::pair<int, int> dz_tr;
	std::pair<int, int> dz_bl;
	std::pair<int, int> dz_br;

	std::vector<std::pair<int, int>> landmarks;
	std::vector<std::pair<int, int>> deadzones;
	int world_size;
};