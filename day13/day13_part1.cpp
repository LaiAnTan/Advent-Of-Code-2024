#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

struct PositiveLinearEq {
	int x_coef;
	int y_coef;
	int res;
};

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	os << "[";
	for (auto i = vector.begin(); i != vector.end() - 1; i++)
		os << *i << ", ";
	os << *(vector.end() - 1) << "]";
	return os;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &pair)
{
	os << "(" << pair.first << ", " << pair.second << ")";
	return os;
}

std::ostream &operator << (std::ostream &os, const PositiveLinearEq &eq)
{
	os << eq.x_coef << "x + " << eq.y_coef<< "y = " << eq.res;
	return (os);
}

float determinant(int a, int b, int c, int d)
{
	return (a * d - c * b);
}

std::pair<int, int> solveSimultaneousEqs(std::pair<PositiveLinearEq, PositiveLinearEq> eqs)
{
	PositiveLinearEq eq1 = eqs.first;
	PositiveLinearEq eq2 = eqs.second;

	int det = determinant(eq1.x_coef, eq1.y_coef, eq2.x_coef, eq2.y_coef);

	if (det == 0)
		throw std::runtime_error("No or infinite solutions.");

	// Cramer's rule
	int det_x = (eq1.res * eq2.y_coef) - (eq2.res * eq1.y_coef);
	int det_y = (eq1.x_coef * eq2.res) - (eq2.x_coef * eq1.res);

	if (det_x % det != 0 || det_y % det != 0)
		throw std::runtime_error("The solutions are not integers.");

	int x = det_x / det;
	int y = det_y / det;

	return std::make_pair(x, y);
}

int main(int argc, char **argv)
{

	if (argc != 2)
			return (-1);
		
	std::string		filename(argv[1]);
	std::fstream	data(filename, std::fstream::in);
	std::string		line;

	if (!data.is_open())
		return (-1);

	std::vector<std::pair<PositiveLinearEq, PositiveLinearEq>>	claw_machines;

	while (std::getline(data, line))
	{
		PositiveLinearEq e1;
		PositiveLinearEq e2;

		e1.x_coef = std::stoi(line.substr(line.find("X+") + 2, line.find(",") - line.find("X+")));
		e2.x_coef = std::stoi(line.substr(line.find("Y+") + 2));

		std::getline(data, line);

		e1.y_coef = std::stoi(line.substr(line.find("X+") + 2, line.find(",") - line.find("X+")));
		e2.y_coef = std::stoi(line.substr(line.find("Y+") + 2));

		std::getline(data, line);

		e1.res = std::stoi(line.substr(line.find("X=") + 2, line.find(",") - line.find("X=")));
		e2.res = std::stoi(line.substr(line.find("Y=") + 2));
	
		claw_machines.push_back(std::make_pair(e1, e2));

		std::getline(data, line);
	}

	int tokens = 0;

	for (std::pair<PositiveLinearEq, PositiveLinearEq> simul : claw_machines)
	{
		std::pair<int, int>	solution;

		std::cout << simul << std::endl;

		try
		{
			solution = solveSimultaneousEqs(simul);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			continue;
		}

		std::cout << solution << std::endl;

		if (solution.first < 0 || solution.first > 100 || solution.second < 0 || solution.second > 100)
		{
			std::cout << "Solution out of bounds" << std::endl;
			continue;
		}

		tokens += solution.first * 3 + solution.second;
	}

	std::cout << tokens << std::endl;

	return (0);
}