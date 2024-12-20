#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

typedef std::pair<int, int> Coordinate, Direction;

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i = vector.begin(); i != vector.end(); i++)
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

Coordinate findTilePos(char tile, std::vector<std::vector<char>> board)
{
	int rows = board.size();
	int cols = board[0].size();

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			if (board[y][x] == tile)
				return (std::make_pair(x, y));
		}
	}
	return (std::make_pair(-1, -1));
}

void buildInitialPathAux(
	Coordinate prev,
	Coordinate curr,
	Coordinate end,
	std::vector<Coordinate> &path,
	std::vector<std::vector<char>> &racetrack
)
{
	if (curr == end)
	{
		path.push_back(end);
		return;
	}

	std::vector<Direction> ds = {
		{0, -1}, {0, 1}, {1, 0}, {-1, 0}
	};
	
	for (Direction d: ds)
	{
		Coordinate n = std::make_pair(curr.first + d.first, curr.second + d.second);
		if (n == prev || racetrack[n.second][n.first] == '#')
			continue;
		path.push_back(curr);
		return (buildInitialPathAux(curr, n, end, path, racetrack));
	}
}

std::vector<Coordinate>	buildInitialPath(std::vector<std::vector<char>>	racetrack)
{
	Coordinate start_pos = findTilePos('S', racetrack);
	Coordinate end_pos = findTilePos('E', racetrack);

	std::vector<Coordinate>	path;

	buildInitialPathAux({-1, -1}, start_pos, end_pos, path, racetrack);
	return (path);
}

std::map<int, int> findCheats(std::vector<std::vector<char>> racetrack)
{
	std::map<int, int>	time_saved_cheat_counts; // number of cheats that save key amount of picoseconds.
	std::vector<Coordinate> path = buildInitialPath(racetrack);

	// try all possible cheats for every point on the original path.
	// for each point, try to get to points after it (that are distance max 20) by cheating.
	for (size_t i = 0; i < path.size(); ++i)
	{
		Coordinate point = path[i];

		// filter points that are < 20 distance away from current point
		// (endpoint, cheat distance)
		for (std::vector<Coordinate>::iterator pi = path.begin() + i; pi != path.end(); pi++)
		{
			int manhattan = std::abs(point.first - (*pi).first) + std::abs(point.second - (*pi).second);
			if (manhattan >= 0 && manhattan <= 20)
			{
				int picoseconds_saved = std::distance(path.begin() + i, pi) - manhattan;
				time_saved_cheat_counts[picoseconds_saved] += 1;
			}
		}
	}
	return (time_saved_cheat_counts);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (-1);
		
	std::string		filename(argv[1]);
	std::fstream	data(filename, std::fstream::in);
	std::string		line;

	std::vector<std::vector<char>>	racetrack;

	while (std::getline(data, line))
		racetrack.push_back(std::vector<char>(line.begin(), line.end()));

	for (auto row : racetrack)
		std::cout << row << std::endl;

	std::map<int, int> time_saved_cheat_counts = findCheats(racetrack);

	std::cout << time_saved_cheat_counts << std::endl;

	int total_cheats_over_100 = 0;

	for (std::pair<int, int> kv : time_saved_cheat_counts)
	{
		if (kv.first < 100)
			continue;
		total_cheats_over_100 += kv.second;
	}

	std::cout << total_cheats_over_100 << std::endl;

	return (0);
}