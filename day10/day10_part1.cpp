#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i : vector)
		os << i << " ";
	return os;
}

template <typename S>
std::ostream &operator << (std::ostream &os, const std::set<S> &set)
{
	for (auto i : set)
		os << i << " ";
	return os;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &pair)
{
	os << "(" << pair.first << ", " << pair.second << ")";
	return os;
}

void	followTrailheadAux(
	std::vector<std::vector<int>> topological_map,
	std::pair<int, int>	curr_coords,
	std::set<std::pair<int, int>> &summits
)
{
	if (topological_map[curr_coords.second][curr_coords.first] == 9)
	{
		summits.insert(curr_coords);
		return;
	}

	int rows = topological_map.size();
	int cols = topological_map[0].size();

	std::vector<std::pair<int, int>>	directions = {
		{0, -1}, {0, 1}, {-1, 0}, {1, 0}
	};

	for (std::pair<int, int> d : directions)
	{
		std::pair<int, int> n = {curr_coords.first + d.first, curr_coords.second + d.second};

		if (n.first < 0 || n.second < 0 || n.first >= cols || n.second >= rows)
			continue;

		if (topological_map[n.second][n.first] == topological_map[curr_coords.second][curr_coords.first] + 1)
			followTrailheadAux(topological_map, n, summits);
	}
}

std::set<std::pair<int, int>>	followTrailhead(
	std::vector<std::vector<int>> topological_map,
	std::pair<int, int>	start_coords
)
{
	std::set<std::pair<int, int>>	unique_summits;

	followTrailheadAux(topological_map, start_coords, unique_summits);

	return	(unique_summits);
}

std::vector<std::pair<int, int>>	getAllTrailheads(std::vector<std::vector<int>> topological_map)
{
	int rows = topological_map.size();
	int cols = topological_map[0].size();
	std::vector<std::pair<int, int>>	trailheads;

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			if (topological_map[y][x] == 0)
				trailheads.push_back(std::make_pair(x, y));
		}
	}
	return (trailheads);
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

	std::vector<std::vector<int> >	topological_map;

	while (std::getline(data, line))
	{
		std::vector<int>	v;

		for (char digit : line)
			v.push_back(digit - '0');
		
		topological_map.push_back(v);
	}

	std::cout << "Input Layout: " << std::endl;

	for (auto row : topological_map)
		std::cout << row << std::endl;

	std::vector<std::pair<int, int>>	trailheads = getAllTrailheads(topological_map);

	std::cout << "Trailheads: " << trailheads << std::endl;

	int	total = 0;

	for (std::pair<int, int> th : trailheads)
	{
		std::set<std::pair<int, int>>	unique_summits = followTrailhead(topological_map, th);

		total += unique_summits.size();
	}

	std::cout << total << std::endl;

	return (0);
}