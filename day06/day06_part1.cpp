#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i : vector)
		os << i;
	return os;
}

// returns x, y
std::pair<int, int>	findGuard(std::vector<std::vector<char>> layout)
{
	for (std::vector<std::vector<char>>::iterator yt = layout.begin(); yt != layout.end(); yt++)
	{
		std::vector<char>::iterator xt = std::find((*yt).begin(), (*yt).end(), '^');

		if (xt != (*yt).end())
		{
			return (std::make_pair(
				std::distance((*yt).begin(), xt),
				std::distance(layout.begin(), yt)
			));
		}
	}
	return (std::make_pair(-1, -1));
}

template <typename T,typename U>
std::pair<T,U> operator+(const std::pair<T,U> &l,const std::pair<T,U> &r)
{
	return (std::make_pair(l.first + r.first, l.second + r.second));
}

void	simulateGuardMovement(std::vector<std::vector<char>> &layout, std::pair<int, int> start_pos)
{
	// x, y: up, right, down, left
	std::vector<std::pair<int, int>> direction = {
		{0, -1}, {1, 0}, {0, 1}, {-1, 0}
	};

	std::vector<char> guard = {'^', '>', 'v', '<'};
	
	int curr_direction = 0;

	// x, y
	std::pair<int, int> curr_coords = start_pos;

	int rows = layout.size();
	int cols = layout[0].size();

	while (true)
	{
		// std::cout << "\nnext turn" << std::endl;

		// check front
		std::pair<int, int> front = curr_coords + direction[curr_direction];

		// out of bounds, stop
		if (front.first < 0 || front.first >= cols ||
			front.second < 0 || front.second >= rows)
		{
			layout[curr_coords.second][curr_coords.first] = 'X';
			break;
		}
		
		// turn
		if (layout[front.second][front.first] == '#')
		{
			curr_direction = (curr_direction + 1) % 4;
			continue;
		}
		
		// move
		layout[curr_coords.second][curr_coords.first] = 'X';
		layout[front.second][front.first] = guard[curr_direction];
		curr_coords = front;

		for (auto i = layout.begin(); i != layout.end(); i++)
			std::cout << *i << std::endl;
	}
}

int	countDistinctPositions(std::vector<std::vector<char>> layout)
{
	int distinct_pos_count = 0;
	
	for (std::vector<char> row : layout)
	{
		for (char c : row)
		{
			if (c == 'X')
				distinct_pos_count++;
		}
	}
	return (distinct_pos_count);
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

	std::vector<std::vector<char> >	layout;

	while (std::getline(data, line))
		layout.push_back(std::vector<char>(line.begin(), line.end()));

	std::pair<int, int> guard_coords = findGuard(layout);

	std::cout << guard_coords.first << "," << guard_coords.second << std::endl;

	simulateGuardMovement(layout, guard_coords);

	int distinct_pos_count = countDistinctPositions(layout);

	std::cout << distinct_pos_count << std::endl;

	return (0);
}