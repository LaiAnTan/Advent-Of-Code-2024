#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

typedef	std::pair<int, int>	Coordinate;
typedef std::set<Coordinate> Region;

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	os << "[";
	for (auto i = vector.begin(); i != vector.end() - 1; i++)
		os << *i << " ";
	os << *(vector.end() - 1) << "]";
	return os;
}

template <typename S>
std::ostream &operator << (std::ostream &os, const std::set<S> &set)
{
	os << "{";
	for (auto i = set.begin(); i != set.end(); ++i) {
		if (i != set.begin()) {
			os << " ";
		}
		os << *i;
	}
	os << "}";
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

void	getRegionAux(Coordinate curr, Region &region, char identifier, std::vector<std::vector<char>> garden)
{
	// recursively get region
	if (garden[curr.second][curr.first] != identifier)
		return;

	region.insert(curr);

	int rows = garden.size();
	int cols = garden[0].size();
	std::vector<std::pair<int, int>>	directions = {
		{0, -1}, {0, 1}, {-1, 0}, {1, 0}
	};

	for (std::pair<int, int> d : directions)
	{
		Coordinate n = {curr.first + d.first, curr.second + d.second};

		if (n.first < 0 || n.second < 0 || n.first >= cols || n.second >= rows ||
			region.find(n) != region.end())
			continue;

		getRegionAux(n, region, identifier, garden);
	}
}

Region	getRegion(Coordinate start_pos, std::vector<std::vector<char>> garden)
{
	Region new_region;
	char identifier = garden[start_pos.second][start_pos.first];
	getRegionAux(start_pos, new_region, identifier, garden);
	return (new_region);
}

bool	isPartOfAnyExistingRegions(Coordinate pos, std::vector<Region> regions)
{
	for (std::vector<Region>::iterator jt = regions.begin(); jt != regions.end(); jt++)
	{
		if ((*jt).find(pos) != (*jt).end())
			return (true);
	}
	return (false);
}

std::map<char, std::vector<Region>>	seperateIntoRegions(
	std::vector<std::vector<char>> garden
)
{
	std::map<char, std::vector<Region>>	region_map;

	int rows = garden.size();
	int cols = garden[0].size();

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			std::map<char, std::vector<Region>>::iterator	it = region_map.find(garden[y][x]);

			// key already exists
			if (it != region_map.end())
			{
				std::vector<Region>	regions = (*it).second;

				// check if region already exists
				if (isPartOfAnyExistingRegions(std::make_pair(x, y), regions))
					continue;

				(*it).second.push_back(getRegion(std::make_pair(x, y), garden));
			}
			// key does not exist
			else
			{
				std::vector<Region>	regions;
				Region new_region = getRegion(std::make_pair(x, y), garden);
				regions.push_back(new_region);
				region_map[garden[y][x]] = regions;
			}
		}
	}
	return (region_map);
}

int countGroups(std::vector<Coordinate>	line, std::pair<int, int> direction)
{
	int groups = 1;
	std::vector<int>	nums;

	// up / down, count horizontally
	if (direction == std::make_pair(0, -1) || direction == std::make_pair(0, 1))
	{
		for (std::vector<Coordinate>::iterator it = line.begin(); it != line.end(); it++)
			nums.push_back((*it).first);
	}
	// left / right, count vertically
	else if (direction == std::make_pair(-1, 0) || direction == std::make_pair(1, 0))
	{
		for (std::vector<Coordinate>::iterator it = line.begin(); it != line.end(); it++)
			nums.push_back((*it).second);
		
	}
	std::sort(nums.begin(), nums.end());

	for (std::vector<int>::iterator	xt = nums.begin() + 1; xt != nums.end(); xt++)
	{
		if (*xt != *(xt - 1) + 1) // if the current number is not beside the previous number, it is in a seperate group
			groups++;
	}
	// std::cout << "Counting group for " <<  nums << ": " << groups << std::endl;
	return (groups);
}

int isOutOfBounds(Coordinate c, int rows, int cols)
{
	return (c.first < 0 || c.second < 0 || c.first >= cols || c.second >= rows);
}

int getRegionSides(Region region, std::vector<std::vector<char>> garden)
{
	// up, down, left, right
	std::vector<std::pair<int, int>>	directions = {
		{0, -1}, {0, 1}, {-1, 0}, {1, 0}
	};

	// up, down, left, right
	std::map<std::pair<int, int>, std::function<bool(Coordinate, Coordinate)>> comparison_map = {
		{{0, -1}, [](Coordinate c1, Coordinate c2) { return (c1.second < c2.second); }}, // smaller means above
		{{0, 1}, [](Coordinate c1, Coordinate c2) { return (c1.second > c2.second); }},
		{{-1, 0}, [](Coordinate c1, Coordinate c2) { return (c1.first < c2.first); }},
		{{1, 0}, [](Coordinate c1, Coordinate c2) { return (c1.first < c2.first); }}
	};

	int sides = 0;

	std::vector<Coordinate>	r(region.begin(), region.end());

	for (std::pair<int, int> d : directions)
	{

		auto min_max = std::minmax_element(r.begin(), r.end(), comparison_map[d]);

		// top => top to bottom
		// bottom => bottom to top
		// left => left to right
		// right => right to left

		// up, down, left, right
		std::map<std::pair<int, int>, std::pair<int, int>>	range_map = {
			{{0, -1}, std::make_pair(min_max.first->second, min_max.second->second)},
			{{0, 1}, std::make_pair(min_max.second->second, min_max.first->second)},
			{{-1, 0}, std::make_pair(min_max.first->first, min_max.second->first)},
			{{1, 0}, std::make_pair(min_max.first->first, min_max.second->first)}
		};

		std::pair<int, int> range = range_map[d];

		for (int i = range.first; i <= range.second; i++)
		{
			std::vector<Coordinate>	row;

			// only get exposed elements
			std::copy_if(r.begin(), r.end(), std::back_inserter(row), [i, d, directions, &garden](Coordinate c){

				Coordinate n = std::make_pair(c.first + d.first, c.second + d.second);

				bool is_on_same_axis = (d == directions[0] || d == directions[1]) ? (c.second == i) : (c.first == i);

				return (is_on_same_axis && (isOutOfBounds(n, garden.size(), garden[0].size()) ||
						garden[n.second][n.first] != garden[c.second][c.first]));
			});

			// count groups of tiles and add to sides
			if (!row.empty())
				sides += countGroups(row, d);
		}
	}

	return (sides);
}

int getRegionArea(Region region)
{
	return (region.size());
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

	std::vector<std::vector<char> >	garden;

	while (std::getline(data, line))
		garden.push_back(std::vector<char>(line.begin(), line.end()));

	std::cout << "Input Layout: " << std::endl;
	for (auto row : garden)
		std::cout << row << std::endl;

	std::map<char, std::vector<Region>>	region_map = seperateIntoRegions(garden);

	std::cout << region_map << std::endl;

	int	total_cost = 0;

	for (std::map<char, std::vector<Region>>::iterator	mt = region_map.begin(); mt != region_map.end(); mt++)
	{
		std::vector<Region>	regions = (*mt).second;

		for (Region r : regions)
		{

			int sides = getRegionSides(r, garden);
			std::cout << (*mt).first << " region has " << getRegionSides(r, garden) << " sides" << std::endl;
			int area = getRegionArea(r);

			total_cost += sides * area;
		}
	}

	std::cout << total_cost << std::endl;
	
	return (0);
}