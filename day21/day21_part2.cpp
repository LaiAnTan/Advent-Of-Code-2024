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

std::map<std::pair<char, char>, long> generateCountsMap(std::string code)
{
	std::map<std::pair<char, char>, long> map;

	for (std::string::iterator it = code.begin(); it != code.end() - 1; it++)
		map[{*it, *(it + 1)}] += 1;

	return (map);
}

std::map<std::pair<char, char>, std::string> generateSequenceMap(std::vector<std::string> &keypad)
{
	std::map<std::pair<char, char>, std::string> sm;

	std::string all_chars = "";

	for (std::string row : keypad)
		all_chars += row;

	for (char from : all_chars)
	{
		for (char to : all_chars)
		{
			if (from == ' ' || to == ' ')
				continue;
			sm[{from, to}] = generateMoveSequence(findTilePos(from, keypad), findTilePos(to, keypad), keypad);
		}
	}
	return (sm);
}

long getLength(std::map<std::pair<char, char>, long> counts)
{
	int len = 0;
	for (auto c : counts)
		len += c.second;
	return (len);
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

	// save the number of counts to and from
	std::map<std::pair<char, char>, std::string> robot_seq_map = generateSequenceMap(robot_keypad);
	std::map<std::pair<char, char>, std::string> door_seq_map = generateSequenceMap(door_keypad);

	int complexity = 0;
	for (std::string code : codes)
	{
		std::cout << "Code: " << code << std::endl;
		
		std::string s = buildDirectionalSequence(code, door_keypad);
		std::map<std::pair<char, char>, long>	counts = generateCountsMap(s);

		for (int i = 0; i < 3; i++)
		{
			std::cout << "iteration " << i + 1 << std::endl;
			// in each iteration, instead of building the string we use the counts and the sequence map to update itself
			std::map<std::pair<char, char>, long>	next;

			// for each entry, we find the string that it translates to
			// then, update the corresponding counts
			for (auto c : counts)
			{
				std::string moves = robot_seq_map[c.first];
				std::cout << c.first << " => " << moves << std::endl;
				if (moves == "A")
				{
					next[{'A', 'A'}] += c.second;
					std::cout << std::make_pair('A', 'A') << ": " << next[{'A', 'A'}] << std::endl;
				}

				for (std::string::iterator it = moves.begin(); it != moves.end() - 1; it++)
				{
					next[{*it, *(it + 1)}] += c.second;
					std::cout << std::make_pair(*it, *(it + 1)) << ": " << next[{*it, *(it + 1)}] << std::endl;
				}
			}
			counts = next;
		}

		complexity += getLength(counts) * std::stoi(code);
	}
	std::cout << complexity << std::endl;

	return (0);
}

/*
new plan:

- at every iteration, find the shortest method => too slow

*/