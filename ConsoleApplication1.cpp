// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Robot.h"
#include <random>

int main()
{
	Robot* robot = new Robot();
	robot->init(0, 20, false);
	std::cout << robot->PosString();
	
	robot->set_noise(1.0, 0.1, 1.0);

	int T = 100;
	int N = 1000;
	std::vector<Robot*> P;
	for (int i = 0; i != N; i++)
	{
		Robot* x = new Robot();
		x->init(0,0,true);
		x->set_noise(0.05, 0.05, 5.0);
		P.push_back(x);
	}
	for (int i = 0; i != T; i++)
	{
		robot = robot->move(2, 0);
		auto dists = robot->sense();

		std::vector<Robot*> P2;
		for (int i = 0; i != N; i++)
		{
			P2.push_back(P[i]->move(2, 0));
		}
		P = P2;

		std::vector<float> W;
		for (int i = 0; i != N; i++)
		{
			W.push_back(P[i]->measurement_prob(dists));
		}

		std::vector<Robot*> P3;

		int index = static_cast<int>(rand() % N);
		float beta = 0.0f;
		auto mw = -1.0f;

		for (auto t : W)
		{
			if (t > mw)
				mw = t;
		}

		for (int i = 0; i != N; i++)
		{
			beta += rand() % 2 * mw;
			while (beta > W[index])
			{
				beta -= W[index];
				index = (index + 1) % N;
			} 
			P3.push_back(P[index]);
		}
		P = P3;

		int num_p = P.size();
		Robot* best_p = new Robot();
		float highest_weight = -1.0f;

		for (int i = 0; i != num_p; i++)
		{
			auto p_weight = P[i]->measurement_prob(dists);
			if (p_weight > highest_weight)
			{
				highest_weight = p_weight;
				best_p = P[i];
			}
		}
		std::cout << "Actual Pos [ X: " << robot->x << " | Y: " << robot->y << " ] Particle Pos [ X: " << best_p->getPos().first << " | Y: " << best_p->getPos().second << "]\n";

		


	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
