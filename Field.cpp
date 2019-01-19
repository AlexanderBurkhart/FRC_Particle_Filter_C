#include "Field.h"

std::pair<int, int> CreatePair(int x, int y)
{
	return std::pair<int, int>{x, y};
}

void Field::init(int world_size = 100)
{
	this->world_size = world_size;
	
	dz_tl = CreatePair(40,60);
	dz_tr = CreatePair(60,60);
	dz_bl = CreatePair(40,40);
	dz_br = CreatePair(60,40);

	deadzones = std::vector<std::pair<int, int>>{ dz_tl,dz_tr,dz_bl,dz_br };

	landmarks = std::vector<std::pair<int, int>>{ CreatePair(0,0),
		CreatePair(50,39),
		CreatePair(60,39),
		CreatePair(65,0),
		CreatePair(0,40),
		CreatePair(99,99),
		CreatePair(0,99),
		CreatePair(99,0),
		CreatePair(40,61),
		CreatePair(50,61),
		CreatePair(50,99) };
}
