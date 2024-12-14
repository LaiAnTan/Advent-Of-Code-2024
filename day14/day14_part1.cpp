#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>


typedef std::pair<std::pair<int, int>, std::pair<int, int>>	Robot;

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	os << "[";
	for (auto i = vector.begin(); i != vector.end() - 1; i++)
		os << *i << " ";
	os << *(vector.end() - 1) << "]";
	return os;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &pair)
{
	os << "(" << pair.first << ", " << pair.second << ")";
	return os;
}

template <typename T, typename U>
std::ostream &operator << (std::ostream &os, const std::map<T, U> &map)
{
	for (auto i : map)
		os << i.first << ": " << i.second << "\n";
	return os;
}

int findQuadrant(std::pair<int, int> pos, int x_bound, int y_bound)
{
	std::pair<int, int> mid = std::make_pair(std::ceil(x_bound / 2), std::ceil(y_bound / 2));

	if (pos.first < mid.first)
	{
		if (pos.second < mid.second)
			return (1);
		else if (pos.second > mid.second)
			return (3);
	}
	else if (pos.first > mid.first)
	{
		if (pos.second < mid.second)
			return (2);
		else if (pos.second > mid.second)
			return (4);
	}
	return (0);
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

	std::vector<Robot>	robots;

	while (std::getline(data, line))
	{
		std::pair<int, int>	position;
		std::pair<int, int>	velocity;
		
		std::string	pos_str = line.substr(line.find("p=") + 2, line.find(" ") - (line.find("p=") + 2));
		std::string	velo_str = line.substr(line.find("v=") + 2);

		position = std::make_pair(std::stoi(pos_str.substr(0, pos_str.find(","))), std::stoi(pos_str.substr(pos_str.find(",") + 1)));
		velocity = std::make_pair(std::stoi(velo_str.substr(0, velo_str.find(","))), std::stoi(velo_str.substr(velo_str.find(",") + 1)));

		robots.push_back(std::make_pair(position, velocity));

		std::cout << "p = " << position << ", v = " << velocity << std::endl;
	}

	int seconds = 100;

	// test.txt use 11, 7
	int x_bound = 101;
	int y_bound = 103;

	std::vector<std::vector<int>>	board(y_bound, std::vector<int>(x_bound, 0));

	std::map<int, int>	robots_count = {
		{1, 0},
		{2, 0},
		{3, 0},
		{4, 0}
	};

	for (Robot r: robots)
	{
		std::pair<int, int>	new_pos = std::make_pair(
			(((r.first.first + (r.second.first * seconds)) % x_bound + x_bound) % x_bound),
			(((r.first.second + (r.second.second * seconds)) % y_bound + y_bound) % y_bound)
		);

		board[new_pos.second][new_pos.first] += 1;

		int quadrant = findQuadrant(new_pos, x_bound, y_bound);

		if (quadrant != 0)
			robots_count[quadrant] += 1;
	}

	for (std::vector<int> row : board)
		std::cout << row << std::endl;

	std::cout << robots_count << std::endl;

	int safety_factor = 1;
	for (std::pair<int, int> quadrant : robots_count)
		safety_factor *= quadrant.second;

	std::cout << safety_factor << std::endl;

	return (0);
}