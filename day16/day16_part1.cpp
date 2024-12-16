#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <algorithm>
#include <climits>
#include <unistd.h>
#include <termios.h>

typedef	std::pair<int, int>	Coordinate;
typedef std::pair<int, int>	Direction;

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i = vector.begin(); i != vector.end(); i++)
		os << *i;
	return os;
}

template <typename S>
std::ostream &operator << (std::ostream &os, const std::set<S> &set)
{
	for (auto i = set.begin(); i != set.end(); i++)
		os << *i << " ";
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

void printMaze(const std::vector<std::vector<char>>& maze) {
    // Move the cursor up by the number of rows in the maze
    std::cout << "\033[2J\033[1;1H";

    for (const std::vector<char>& row : maze) {
        for (char cell : row) {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    std::flush(std::cout);
    usleep(10000);
}

struct CompareDistance {
    bool operator()(const std::pair<int, std::pair<Coordinate, Direction>>& a,
                    const std::pair<int, std::pair<Coordinate, Direction>>& b)
	{
        return (a.first > b.first); // Min-heap based on the first element of the pair
    }
};

int lowestCostPath(Coordinate start, Direction start_dir, Coordinate end, std::vector<std::vector<char>> maze)
{
	// up, down, left, right
	std::vector<Direction> ds = {
		{0, -1}, {0, 1}, {-1, 0}, {1, 0}
	};

	std::map<std::pair<Coordinate, Direction>, int>	dist;
	std::priority_queue<
		std::pair<int, std::pair<Coordinate, Direction>>,
		std::vector<std::pair<int, std::pair<Coordinate, Direction>>>,
		CompareDistance
	>	pq;
	std::set<std::pair<Coordinate, Direction>>		visited;

	for (int y = 0; y < (int) maze.size(); y++)
	{
		for (int x = 0; x < (int) maze[0].size(); x++)
		{
			if (maze[y][x] != '#')
			{
				Coordinate pos = std::make_pair(x, y);
				for (Direction d : ds)
					dist[{pos, d}] = INT_MAX;
			}
		}
	}

	dist[{start, start_dir}] = 0;
	pq.push({0, {start, start_dir}});

	while (!pq.empty())
	{
		auto [curr_dist, curr_pair] = pq.top();
		auto [curr_pos, curr_dir] = curr_pair;
		pq.pop();

		if (visited.find(curr_pair) != visited.end())
			continue;
		
		visited.insert(curr_pair);

		// cost is curr tile + turn + 1
		// find neighbours - neighbours are: next tile + same tile but diff direction
		std::vector<std::pair<int, std::pair<Coordinate, Direction>>>	neighbours;

		// 1. adjacent tile neighbours
		Coordinate next = std::make_pair(curr_pos.first + curr_dir.first, curr_pos.second + curr_dir.second);

		if (maze[next.second][next.first] != '#')
			neighbours.push_back({curr_dist + 1, {next, curr_dir}});
		
		// 2. rotational neighbours
		for (Direction d : ds)
		{
			if (curr_dir != d)
				neighbours.push_back({curr_dist + turnCost(curr_dir, d), {curr_pos, d}});
		}

		for (std::pair<int, std::pair<Coordinate, Direction>> n : neighbours)
		{
			if (visited.find(n.second) == visited.end())
			{
				int new_dist = n.first;
				if (new_dist < dist[n.second])
				{
					dist[n.second] = new_dist;
					pq.push({new_dist, n.second});
				}
			}
		}
	}

	// find shortest distance to E from any direction
	std::vector<int>	dist_to_end = {
		dist[{end, ds[0]}], dist[{end, ds[1]}], dist[{end, ds[2]}], dist[{end, ds[3]}]
	};

	std::cout << dist_to_end << std::endl;

	return (*std::min_element(dist_to_end.begin(), dist_to_end.end()));
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

	std::cout << "Start pos: " << start_pos << std::endl;
	std::cout << "End pos: " << end_pos << std::endl;

	std::cout << lowestCostPath(start_pos, start_dir, end_pos, maze) << std::endl;

	return (0);
}