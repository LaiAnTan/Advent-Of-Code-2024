#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <numeric>
#include <set>

typedef std::map<char, std::vector<std::pair<int, int>>> antennaMap;

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


template <typename T, typename U>
std::ostream &operator << (std::ostream &os, const std::map<T, U> &map)
{
	for (auto i : map)
		os << i.first << ": " << i.second << "\n";
	return os;
}

std::map<char, std::vector<std::pair<int, int>>>	buildAntennaMap(std::vector<std::vector<char>> layout)
{
	antennaMap	map;
	int rows = layout.size();
	int cols = layout[0].size();


	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			if (layout[y][x] == '.')
				continue;
			
			antennaMap::iterator it = map.find(layout[y][x]);

			if (it != map.end())
				(*it).second.push_back(std::make_pair(x, y));
			else
				map[layout[y][x]] = std::vector<std::pair<int, int>>{std::make_pair(x, y)};
		}
	}
	return (map);
}

template <typename T>
std::vector<std::pair<T, T>>	getAllCombinations(std::vector<T> v)
{
	int size = v.size();
	std::vector<std::pair<T, T>>	pairs;

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			pairs.push_back(std::make_pair(v[i], v[j]));
		}
	}

	return (pairs);
}

std::set<std::pair<int, int>>	getAntinodesInBounds(
	int	y_bound,
	int	x_bound,
	std::pair<int, int> p1,
	std::pair<int, int> p2
)
{
	int	hd = std::abs(p2.first - p1.first);
	int	vd = std::abs(p2.second - p1.second);

	std::set<std::pair<int, int>>	antinodes;

	antinodes.insert(p1);
	antinodes.insert(p2);

	// p1 left p2 right
	if (p1.first >= p2.first)
		std::swap(p1, p2);

	// make p1 go to the left until out of bounds
	while (true)
	{
		p1.first -= hd;
		p1.second = p1.second < p2.second ? p1.second - vd : p1.second + vd;

		if (p1.first < 0 || p1.second < 0 || p1.second >= y_bound)
			break;
		
		antinodes.insert(p1);
	}

	// make p2 go to the right until until out of bounds
	while (true)
	{
		p2.first += hd;
		p2.second = p1.second < p2.second ? p2.second + vd : p2.second - vd;

		if (p2.first >= x_bound || p2.second < 0 || p2.second >= y_bound)
			break;
		
		antinodes.insert(p2);
	}

	std::cout << "Antinodes found: " << antinodes << std::endl;

	return (antinodes);
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

	std::cout << "Input Layout: " << std::endl;

	for (auto row : layout)
		std::cout << row << std::endl;
	
	std::cout << std::endl;

	antennaMap	antenna_map = buildAntennaMap(layout);

	std::cout << "Antenna Map: " << std::endl;
	std::cout << antenna_map << std::endl;

	std::set<std::pair<int, int>>	unique_antinodes;

	for (std::pair<char, std::vector<std::pair<int, int>>> x : antenna_map)
	{
		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>	combinations = getAllCombinations(x.second);

		for (std::pair<std::pair<int, int>, std::pair<int, int>> pair : combinations)
		{
			std::cout << "Checking: " << pair << std::endl;

			std::set<std::pair<int, int>>	antinodes = getAntinodesInBounds(layout.size(), layout[0].size(), pair.first, pair.second);

			unique_antinodes.insert(antinodes.begin(), antinodes.end());
		}
	}

	for (auto antinode : unique_antinodes)
	{
		layout[antinode.second][antinode.first] = '#';
	}

	std::cout << "Antinodes Layout: " << std::endl;

	for (auto row : layout)
		std::cout << row << std::endl;
	
	std::cout << std::endl;

	// std::cout << unique_antinodes << std::endl;

	std::cout << unique_antinodes.size() << std::endl;

	return (0);
}