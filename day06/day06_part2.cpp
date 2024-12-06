#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>

template <typename S>
std::ostream &operator<<(std::ostream &os, const std::vector<S> &vector)
{
	for (auto i : vector)
		os << i;
	return os;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &pair)
{
	os << "(" << pair.first << ", " << pair.second << ")";
	return os;
}

// returns x, y
std::pair<int, int> findGuard(std::vector<std::vector<char>> layout)
{
	for (std::vector<std::vector<char>>::iterator yt = layout.begin(); yt != layout.end(); yt++)
	{
		std::vector<char>::iterator xt = std::find((*yt).begin(), (*yt).end(), '^');

		if (xt != (*yt).end())
		{
			return (std::make_pair(
				std::distance((*yt).begin(), xt),
				std::distance(layout.begin(), yt)));
		}
	}
	return (std::make_pair(-1, -1));
}

template <typename T, typename U>
std::pair<T, U> operator+(const std::pair<T, U> &l, const std::pair<T, U> &r)
{
	return (std::make_pair(l.first + r.first, l.second + r.second));
}

void	drawOriginalPath(std::vector<std::vector<char>> &layout, std::pair<int, int> start_pos)
{
	int curr_direction = 0;
	int rows = layout.size();
	int cols = layout[0].size();

	// x, y
	std::pair<int, int> curr_coords = start_pos;
	std::vector<char> guard = {'^', '>', 'v', '<'};
	// x, y: up, right, down, left
	std::vector<std::pair<int, int>> direction = {
		{0, -1}, {1, 0}, {0, 1}, {-1, 0}
	};

	while (true)
	{
		// check front
		std::pair<int, int> front = curr_coords + direction[curr_direction];

		// out of bounds, stop
		if (front.first < 0 || front.first >= cols || front.second < 0 || front.second >= rows)
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
	}
}

bool	simulateGuardMovement(std::vector<std::vector<char>> &layout, std::pair<int, int> start_pos)
{
	int curr_direction = 0;
	int rows = layout.size();
	int cols = layout[0].size();

	// x, y
	std::pair<int, int> curr_coords = start_pos;
	std::vector<char> guard = {'^', '>', 'v', '<'};
	// x, y: up, right, down, left
	std::vector<std::pair<int, int>> direction = {
		{0, -1}, {1, 0}, {0, 1}, {-1, 0}
	};
	std::set<std::pair<std::pair<int, int>, std::pair<int, int>>>	prev_turns;

	while (true)
	{
		// check front
		std::pair<int, int> front = curr_coords + direction[curr_direction];

		// out of bounds, stop
		if (front.first < 0 || front.first >= cols || front.second < 0 || front.second >= rows)
		{
			layout[curr_coords.second][curr_coords.first] = 'X';
			break;
		}
		
		// turn
		if (layout[front.second][front.first] == '#')
		{
			// check if we have been here before
			// do not put this outside the if check, it makes the complexity become O(n^2), which increases runtime by ~5s
			if (prev_turns.find(std::make_pair(curr_coords, direction[curr_direction])) != prev_turns.end())
				return (true);

			prev_turns.insert(std::make_pair(curr_coords, direction[curr_direction]));
			curr_direction = (curr_direction + 1) % 4;
			continue;
		}
		
		// move
		layout[curr_coords.second][curr_coords.first] = 'X';
		layout[front.second][front.first] = guard[curr_direction];
		curr_coords = front;
	}
	return (false);
}

bool isLoopingWithObstacleAt(std::vector<std::vector<char>> layout, std::pair<int, int> start_pos, std::pair<int, int> obstacale_pos)
{
	layout[obstacale_pos.second][obstacale_pos.first] = '#';

	return (simulateGuardMovement(layout, start_pos));
}

int bruteForcePotentialObstacles(std::vector<std::vector<char>> layout, std::pair<int, int> start_pos)
{
	int loops = 0;
	int rows = layout.size();
	int cols = layout[0].size();

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			if (layout[y][x] != 'X')
				continue;

			if (isLoopingWithObstacleAt(layout, start_pos, std::make_pair(x, y)))
				loops++;
		}
	}
	return (loops);
}

int main(int argc, char **argv)
{

	if (argc != 2)
		return (-1);

	std::string filename(argv[1]);
	std::fstream data(filename, std::fstream::in);
	std::string line;

	if (!data.is_open())
		return (-1);

	std::vector<std::vector<char>> layout;

	while (std::getline(data, line))
		layout.push_back(std::vector<char>(line.begin(), line.end()));

	std::pair<int, int> guard_coords = findGuard(layout);

	drawOriginalPath(layout, guard_coords);

	std::cout << bruteForcePotentialObstacles(layout, guard_coords) << std::endl;

	return (0);
}