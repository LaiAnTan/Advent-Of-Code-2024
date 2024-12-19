#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <climits>
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

struct CompareDistance
{
	bool operator()(const std::pair<int, Coordinate> &a, const std::pair<int, Coordinate> &b)
	{
		return (a.first > b.first); // Min-heap based on the first element of the pair
	}
};

int dijkstra(Coordinate start, Coordinate end, std::vector<std::vector<char>> board)
{
	// up, down, left, right
	std::vector<Direction> ds = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
	std::map<Coordinate, int> dist;
	std::set<Coordinate>	visited;
	std::priority_queue<std::pair<int, Coordinate>, std::vector<std::pair<int, Coordinate>>, CompareDistance> pq;

	for (int y = 0; y < (int) board.size(); y++)
	{
		for (int x = 0; x < (int) board[0].size(); x++)
		{
			if (board[y][x] != '#')
				dist[std::make_pair(x, y)] = INT_MAX;
		}
	}

	dist[start] = 0;
	pq.push({0, start});

	while (!pq.empty())
	{
		auto [curr_dist, curr_pos] = pq.top();
		pq.pop();

		if (visited.find(curr_pos) != visited.end())
			continue;
		
		visited.insert(curr_pos);

		std::vector<std::pair<int, Coordinate>> neighbours;

		for (Direction d : ds)
		{
			Coordinate next = std::make_pair(curr_pos.first + d.first, curr_pos.second + d.second);
			if (next.first < 0 || next.second < 0 || next.first >= (int) board[0].size() || next.second >= (int) board.size() || board[next.second][next.first] != '#')
				neighbours.push_back({1, next});
		}

		for (std::pair<int, Coordinate> n : neighbours)
		{
			int n_weight = n.first;
			Coordinate n_pos = n.second;
			if (visited.find(n_pos) != visited.end())
				continue;

			int new_dist = curr_dist + n_weight;
			if (new_dist < dist[n_pos])
			{
				dist[n_pos] = new_dist;
				pq.push({new_dist, n_pos});
			}
		}
	}

	return (dist[end]);
}

void	simulateObstaclePlacement(std::vector<std::pair<int, int>> obstacles, int count, std::vector<std::vector<char>> &board)
{
	for (int i = 0; i < count; i++)
		board[obstacles[i].second][obstacles[i].first] = '#';
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (-1);
		
	std::string		filename(argv[1]);
	std::fstream	data(filename, std::fstream::in);
	std::string		line;

	std::vector<std::pair<int, int>>	obstacles;

	while (std::getline(data, line))
	{
		obstacles.push_back(std::make_pair(
			std::stoi(line.substr(0, line.find(','))),
			std::stoi(line.substr(line.find(',') + 1))
		));
	}

	std::cout << obstacles << std::endl;

	int rows = 71;
	int cols = 71;

	std::vector<std::vector<char>>	board;

	for (int y = 0; y < cols; y++)
		board.push_back(std::vector<char>(rows, '.'));

	simulateObstaclePlacement(obstacles, 1024, board);

	for (auto row : board)
		std::cout << row << std::endl;

	Coordinate start = {0, 0};
	Coordinate end = {70, 70};

	std::cout << dijkstra(start, end, board) << std::endl;
}