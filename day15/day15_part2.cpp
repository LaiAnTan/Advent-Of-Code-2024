#include <iostream>
#include <fstream>
#include <vector>
#include <map>

typedef	std::pair<int, int>	Coordinate;

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

Coordinate findRobotPos(std::vector<std::vector<char>> warehouse)
{
	int rows = warehouse.size();
	int cols = warehouse[0].size();

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			if (warehouse[y][x] == '@')
				return (std::make_pair(x, y));
		}
	}
	return (std::make_pair(-1, -1));
}

void moveHorizontalBoxesAux(Coordinate curr, std::pair<int, int> direction, std::vector<std::vector<char>> &warehouse)
{
	Coordinate next = std::make_pair(curr.first + direction.first, curr.second + direction.second);

	if (warehouse[next.second][next.first] == '[' || warehouse[next.second][next.first] == ']')
		moveHorizontalBoxesAux(next, direction, warehouse);

	// base case, no box
	if (warehouse[next.second][next.first] == '#')
		return;
	
	if (warehouse[next.second][next.first] == '.')
	{
		// move a (box or robot) to here
		warehouse[next.second][next.first] = warehouse[curr.second][curr.first];
		warehouse[curr.second][curr.first] = '.';
		return;
	}
}

void moveHorizontalBoxes(
	std::vector<std::vector<char>> &warehouse,
	std::pair<int, int> direction,
	Coordinate &robot_pos
)
{
	// we treat the robot as a box to move as well, as it will move with them
	moveHorizontalBoxesAux(robot_pos, direction, warehouse);

	Coordinate new_robot_pos = std::make_pair(robot_pos.first + direction.first, robot_pos.second + direction.second);

	if (warehouse[new_robot_pos.second][new_robot_pos.first] == '@')
		robot_pos = new_robot_pos;
}

bool isPossibleToVerticallyMoveBoxes(
	Coordinate curr,
	std::pair<int, int> direction,
	std::vector<std::vector<char>> &warehouse
)
{
	Coordinate adjacent = std::make_pair(curr.first + direction.first, curr.second + direction.second);

	if (warehouse[adjacent.second][adjacent.first] == '#')
		return false;

	if (warehouse[adjacent.second][adjacent.first] == '[' || warehouse[adjacent.second][adjacent.first] == ']')
	{
		Coordinate adjacent_pair = std::make_pair(
			(warehouse[adjacent.second][adjacent.first] == '[') ? adjacent.first + 1 : adjacent.first - 1,
			adjacent.second
		);

		return (isPossibleToVerticallyMoveBoxes(adjacent, direction, warehouse) &&
			isPossibleToVerticallyMoveBoxes(adjacent_pair, direction, warehouse));
	}
	
	return (true);
}

void moveVerticalBoxesAux(
	Coordinate curr,
	std::pair<int, int> direction,
	std::vector<std::vector<char>> &warehouse
)
{
	Coordinate adjacent = std::make_pair(curr.first + direction.first, curr.second + direction.second);

	if (warehouse[adjacent.second][adjacent.first] == '[' || warehouse[adjacent.second][adjacent.first] == ']')
	{
		Coordinate adjacent_pair = std::make_pair(
			(warehouse[adjacent.second][adjacent.first] == '[') ? adjacent.first + 1 : adjacent.first - 1,
			adjacent.second
		);
		moveVerticalBoxesAux(adjacent, direction, warehouse);
		moveVerticalBoxesAux(adjacent_pair, direction, warehouse);
	}

	if (warehouse[adjacent.second][adjacent.first] == '#')
		return;
	if (warehouse[adjacent.second][adjacent.first] == '.')
	{
		// only need to shift itself, the other side will be handled recursively
		warehouse[adjacent.second][adjacent.first] = warehouse[curr.second][curr.first];
		warehouse[curr.second][curr.first] = '.';
		return;
	}
}

void moveVerticalBoxes(
	std::vector<std::vector<char>> &warehouse,
	std::pair<int, int> direction,
	Coordinate &robot_pos
)
{
	// we treat the robot as a box to move as well, as it will move with them
	if (isPossibleToVerticallyMoveBoxes(robot_pos, direction, warehouse))
		moveVerticalBoxesAux(robot_pos, direction, warehouse);

	Coordinate new_robot_pos = std::make_pair(robot_pos.first + direction.first, robot_pos.second + direction.second);

	if (warehouse[new_robot_pos.second][new_robot_pos.first] == '@')
		robot_pos = new_robot_pos;
}

void simulateRobotMovement(std::vector<std::vector<char>> &warehouse, std::vector<char> instructions)
{
	// up, down, left, right
	std::map<char, std::pair<int, int>>	direction_map = {
		{'^', {0, -1}},
		{'v', {0, 1}},
		{'<', {-1, 0}},
		{'>', {1, 0}}
	};

	Coordinate	robot_pos = findRobotPos(warehouse);

	for (char in : instructions)
	{
		// std::cout << "Direction: " << in << std::endl;

		std::pair<int, int> dir = direction_map[in];

		Coordinate n = std::make_pair(robot_pos.first + dir.first, robot_pos.second + dir.second);

		if (warehouse[n.second][n.first] == '.')
		{
			warehouse[n.second][n.first] = '@';
			warehouse[robot_pos.second][robot_pos.first] = '.';
			robot_pos = n;
		}
		else if (warehouse[n.second][n.first] == '[' || warehouse[n.second][n.first] == ']')
		{
			if (in == '^' || in == 'v')
				moveVerticalBoxes(warehouse, dir, robot_pos);
			else if (in == '<' || in == '>')
				moveHorizontalBoxes(warehouse, dir, robot_pos);
		}

		// if (warehouse[n.second][n.first] == '#') - do nothing
		// for (std::vector<char> row : warehouse)
		// 	std::cout << row << std::endl;
	}
}

int sumAllBoxPos(std::vector<std::vector<char>> warehouse)
{
	int rows = warehouse.size();
	int cols = warehouse[0].size();
	int res = 0;
	
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			if (warehouse[y][x] == '[')
				res += (100 * y) + x;
		}
	}
	return (res);
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

	std::vector<std::vector<char>>	warehouse;
	std::vector<char>	instructions;

	while (std::getline(data, line) && !line.empty())
	{
		std::vector<char> tiles(line.begin(), line.end());
		std::vector<char> supersized;

		for (char tile : tiles)
		{
			if (tile == 'O')
			{
				supersized.push_back('[');
				supersized.push_back(']');
			}
			else if (tile == '@')
			{
				supersized.push_back('@');
				supersized.push_back('.');
			}
			else
			{
				supersized.push_back(tile);
				supersized.push_back(tile);
			}
		}
		warehouse.push_back(supersized);
	}

	while (std::getline(data, line))
	{
		std::vector<char>	i(line.begin(), line.end());
		instructions.insert(instructions.end(), i.begin(), i.end());
	}

	for (std::vector<char> row : warehouse)
		std::cout << row << std::endl;

	std::cout << instructions << std::endl;

	simulateRobotMovement(warehouse, instructions);

	std::cout << sumAllBoxPos(warehouse) << std::endl;

	return (0);
}