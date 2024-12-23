#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <climits>

template <typename S>
std::ostream &operator<<(std::ostream &os, const std::vector<S> &vector)
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
std::ostream &operator<<(std::ostream &os, const std::map<T, U> &map)
{
	for (auto i : map)
		os << i.first << ": " << i.second << "\n";
	return os;
}

typedef std::pair<int, int> Coordinate, Direction;

Coordinate findTilePos(char tile, std::vector<std::string> board)
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

bool isInBounds(Coordinate pos, std::vector<std::string> keypad)
{
	int rows = keypad.size();
	int cols = keypad[0].size();

	return (pos.first >= 0 && pos.second >= 0 && pos.first < cols && pos.second < rows);
}

struct CompareDistance
{
	bool operator()(const std::pair<int, Coordinate> &a,
					const std::pair<int, Coordinate> &b)
	{
		return (a.first > b.first); // Min-heap based on the first element of the pair
	}
};

std::vector<std::string> allPossibleShortestSequences(char from, char to, std::vector<std::string> &keypad)
{
	Coordinate start_pos = findTilePos(from, keypad);
	Coordinate end_pos = findTilePos(to, keypad);
	std::map<Direction, char> dm = {
		{{0, -1}, '^'},
		{{0, 1}, 'v'},
		{{-1, 0}, '<'},
		{{1, 0}, '>'}};
	std::vector<Direction> ds = {
		{0, -1}, {0, 1}, {1, 0}, {-1, 0}};

	std::map<Coordinate, int> dist;
	std::map<Coordinate, std::vector<std::string>> paths;
	std::priority_queue<std::pair<int, Coordinate>, std::vector<std::pair<int, Coordinate>>, CompareDistance> pq;

	for (int y = 0; y < (int)keypad.size(); y++)
	{
		for (int x = 0; x < (int)keypad[0].size(); x++)
		{
			if (keypad[y][x] != ' ')
			{
				Coordinate pos = std::make_pair(x, y);
				dist[pos] = INT_MAX;
				paths[pos] = {};
			}
		}
	}

	dist[start_pos] = 0;
	paths[start_pos] = {""};

	pq.push({0, start_pos});

	while (!pq.empty())
	{
		auto [curr_dist, curr_pos] = pq.top();
		pq.pop();

		// cost is curr tile + turn + 1
		// find neighbours - neighbours are: next tile + same tile but diff direction
		std::vector<std::pair<int, Coordinate>> neighbours;

		// 1. adjacent tile neighbours
		for (Direction d : ds)
		{
			Coordinate next = std::make_pair(curr_pos.first + d.first, curr_pos.second + d.second);
			if (isInBounds(next, keypad) && keypad[next.second][next.first] != ' ')
				neighbours.push_back({1, next});
		}

		for (std::pair<int, Coordinate> n : neighbours)
		{
			int dist_thru_curr = curr_dist + n.first;
			Coordinate n_pos = n.second;
			// Direction n_dir = n.second.second;

			// if found shorter path, change all paths to n.second to the shortest path(s)
			if (dist_thru_curr < dist[n.second])
			{
				dist[n.second] = dist_thru_curr;

				paths[n.second] = {};
				for (std::string path : paths[curr_pos])
					paths[n.second].push_back(path + dm[{n_pos.first - curr_pos.first, n_pos.second - curr_pos.second}]);

				pq.push({dist[n_pos], n_pos});
			}
			// if found path of same length, append it to the list of path(s)
			else if (dist_thru_curr == dist[n.second])
			{
				for (std::string path : paths[curr_pos])
					paths[n.second].push_back(path + dm[{n_pos.first - curr_pos.first, n_pos.second - curr_pos.second}]);
			}
		}
	}

	std::vector<std::string> paths_to_end;

	if (paths.find(end_pos) != paths.end())
	{
		for (const auto &path : paths[end_pos])
			paths_to_end.push_back(path);
	}

	if (paths_to_end.empty())
		return (std::vector<std::string>());

	std::sort(paths_to_end.begin(), paths_to_end.end());

	int shortest = paths_to_end[0].size();

	std::vector<std::string> shortest_seqs;

	for (auto it = paths_to_end.begin(); it != paths_to_end.end() && (int)(*it).size() == shortest; it++)
		shortest_seqs.push_back((*it));

	return (shortest_seqs);
}

std::map<std::pair<char, char>, std::vector<std::string>> generateSequenceMap(std::vector<std::string> &keypad)
{
	std::map<std::pair<char, char>, std::vector<std::string>> sm;

	std::string all_chars = "";

	for (std::string row : keypad)
		all_chars += row;

	for (char from : all_chars)
	{
		for (char to : all_chars)
		{
			if (from == ' ' || to == ' ')
				continue;

			sm[{from, to}] = allPossibleShortestSequences(from, to, keypad);
			if (sm[{from, to}].empty())
				sm[{from, to}] = {""};
		}
	}

	return (sm);
}

// keypad is vector of string, illegal positions marked by space char ' '
// keypad must contain 'A' as starting point

std::vector<std::string> allPossibleDirectionalSequences(
	std::string code,
	std::map<std::pair<char, char>, std::vector<std::string>> &seq_map
)
{
	std::vector<std::string> ss = {""};
	char curr = 'A';

	for (char c : code)
	{
		// we need to generate all possible move sequences.
		std::vector<std::string> next;
		for (std::string s : ss)
		{
			for (std::string n : seq_map[{curr, c}])
				next.push_back(s + n + "A");
		}
		ss = next;
		curr = c;
	}
	return (ss);
}

// above this is subpar legacy code

std::string generateMoveSequence(Coordinate from, Coordinate to, std::vector<std::string> keypad)
{
	std::string s = "";
	std::vector<Direction> ds = {
		{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

	std::map<Direction, char> dm = {
		{{0, -1}, '^'},
		{{0, 1}, 'v'},
		{{-1, 0}, '<'},
		{{1, 0}, '>'}};

	// find distance from curr_pos to c
	int h_dist = to.first - from.first;
	int v_dist = to.second - from.second;

	Direction v_dir = (v_dist == 0) ? std::make_pair(0, 0) : ((v_dist > 0) ? ds[1] : ds[0]);
	Direction h_dir = (h_dist == 0) ? std::make_pair(0, 0) : ((h_dist > 0) ? ds[3] : ds[2]);

	Coordinate empty_pos = findTilePos(' ', keypad);

	// if same row as space, move vertically all the way first, then move horizontally
	if (empty_pos.second == from.second)
	{
		s.append(std::string(std::abs(v_dist), dm[v_dir]));
		s.append(std::string(std::abs(h_dist), dm[h_dir]));
	}
	// if same col as space, move horizontally all the way, then move horizontally
	else if (empty_pos.first == from.first)
	{
		s.append(std::string(std::abs(h_dist), dm[h_dir]));
		s.append(std::string(std::abs(v_dist), dm[v_dir]));
	}
	else
	{
		if (dm[h_dir]  == '<')
		{
			s.append(std::string(std::abs(h_dist), dm[h_dir]));
			s.append(std::string(std::abs(v_dist), dm[v_dir]));
		}
		else if (dm[v_dir] == 'v')
		{
			s.append(std::string(std::abs(v_dist), dm[v_dir]));
			s.append(std::string(std::abs(h_dist), dm[h_dir]));
		}
		else
		{
			// does not matter, do whatever
			s.append(std::string(std::abs(h_dist), dm[h_dir]));
			s.append(std::string(std::abs(v_dist), dm[v_dir]));
		}
	}

	s.append("A");
	return (s);
}

std::string buildDirectionalSequence(std::string code, std::vector<std::string> keypad)
{
	std::string sequence = "";
	Coordinate curr = findTilePos('A', keypad);

	for (char c : code)
	{
		// find c on board
		Coordinate next = findTilePos(c, keypad);
		std::string s = generateMoveSequence(curr, next, keypad);
		sequence += s;
		curr = next;
	}
	return (sequence);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (-1);

	std::string filename(argv[1]);
	std::fstream data(filename, std::fstream::in);
	std::string line;

	std::vector<std::string> codes;

	while (std::getline(data, line))
		codes.push_back(line);

	std::vector<std::string> door_keypad = {
		"789",
		"456",
		"123",
		" 0A"
	};
	std::vector<std::string> robot_keypad = {
		" ^A",
		"<v>"
	};

	std::map<std::pair<char, char>, std::vector<std::string>> door_map = generateSequenceMap(door_keypad);
	std::map<std::pair<char, char>, std::vector<std::string>> robot_map = generateSequenceMap(robot_keypad);

	int complexity = 0;
	for (std::string code : codes)
	{
		std::cout << "Code: " << code << std::endl;

		std::string s = buildDirectionalSequence(code, door_keypad);
		for (int i = 0; i < 2; i++)
			s = buildDirectionalSequence(s, robot_keypad);

		std::cout << "Min length: " << s.length() << std::endl;
		std::cout << "Num: " << std::stoi(code) << std::endl;
		complexity += s.length() * std::stoi(code);
	}

	std::cout << complexity << std::endl;

	return (0);
}