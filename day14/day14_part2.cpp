#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <unistd.h>
#include <termios.h>

typedef std::pair<std::pair<int, int>, std::pair<int, int>>	Robot;

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

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i = vector.begin(); i != vector.end() - 1; i++)
		(*i != 0) ? os << *i : os << " ";
	(*(vector.end() - 1) != 0) ? os << *(vector.end() - 1) : os << "";
	return os;
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

	}

	int start = 0;
	int end = 10000; // answer is 6752
	int x_bound = 101;
	int y_bound = 103;

	for (int i = start; i < end; i++)
	{
		std::vector<std::vector<int>>	board(y_bound, std::vector<int>(x_bound, 0));

		for (Robot r: robots)
		{
			std::pair<int, int>	new_pos = std::make_pair(
				(((r.first.first + (r.second.first * i)) % x_bound + x_bound) % x_bound),
				(((r.first.second + (r.second.second * i)) % y_bound + y_bound) % y_bound)
			);

			board[new_pos.second][new_pos.first] += 1;
		}

		std::cout << "\033[2J\033[1;1H";
		std::flush(std::cout);
		std::cout << " ---- Second " << i << " ---- (Press enter for next frame)" << std::endl;
		for (std::vector<int> row : board)
			std::cout << row << std::endl;

		char c = getchar();
		while (c != '\n')
			c = getchar();
	}

	return (0);
}