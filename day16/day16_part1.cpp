#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

typedef	std::pair<int, int>	Coordinate;
typedef std::pair<int, int>	Direction;

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i = vector.begin(); i != vector.end(); i++)
		os << *i;
	return os;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &pair)
{
	os << "(" << pair.first << ", " << pair.second << ")";
	return os;
}

Coordinate findTilePos(char tile, std::vector<std::vector<char>> maze)
{
	int rows = maze.size();
	int cols = maze[0].size();

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			if (maze[y][x] == tile)
				return (std::make_pair(x, y));
		}
	}
	return (std::make_pair(-1, -1));
}


// helper to return the lowest cost turning 'from' -> 'to'
int turnCost(Direction from, Direction to)
{
	// up, left, down, right
	std::vector<Direction> ds = {
		{0, -1}, {1, 0}, {0, 1}, {-1, 0}
	};

	if (from == to)
		return (0);

	int from_idx = std::find(ds.begin(), ds.end(), from) - ds.begin();
	int to_idx = std::find(ds.begin(), ds.end(), to) - ds.begin();

	return (std::max(std::abs(from_idx - to_idx), std::abs(to_idx - from_idx)) * 1000);
}

void lowestCostPathAux(
	Coordinate curr_pos,
	Direction curr_dir,
	int curr_cost,
	Coordinate end,
	std::set<Coordinate> visited,
	std::vector<int> &costs,
	std::vector<std::vector<char>> maze
)
{
	// base case: we reach the end
	if (curr_pos == end)
	{
		costs.push_back(curr_cost);
		return;
	}

	// up, down, left, right
	std::vector<Direction> ds = {
		{0, -1}, {0, 1}, {-1, 0}, {1, 0}
	};

	// try all possible paths, turn if necessary
	for (Direction d : ds)
	{
		Coordinate next = std::make_pair(curr_pos.first + d.first, curr_pos.second + d.second);

		if (maze[next.second][next.first] == '#' || visited.find(next) != visited.end())
			continue;

		// lowest cost turning
		int cost_to_turn = turnCost(curr_dir, d);

		visited.insert(next);
		lowestCostPathAux(next, d, curr_cost + 1 + cost_to_turn, end, visited, costs, maze);
		visited.erase(next);
	}
}

int lowestCostPath(Coordinate start, Direction start_dir, Coordinate end, std::vector<std::vector<char>> maze)
{
	std::vector<int> costs;
	lowestCostPathAux(start, start_dir, 0, end, {}, costs, maze);
	return (*std::min_element(costs.begin(), costs.end()));
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

	std::vector<std::vector<char>>	maze;

	while (std::getline(data, line))
		maze.push_back(std::vector<char>(line.begin(), line.end()));

	for (std::vector<char> row : maze)
		std::cout << row << std::endl;

	Coordinate start_pos = findTilePos('S', maze);
	Coordinate end_pos = findTilePos('E', maze);
	Direction start_dir = {1, 0};

	std::cout << lowestCostPath(start_pos, start_dir, end_pos, maze) << std::endl;

	return (0);
}