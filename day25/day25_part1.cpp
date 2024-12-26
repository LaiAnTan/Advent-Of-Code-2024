#include <iostream>
#include <fstream>
#include <vector>
#include <set>

typedef std::vector<int> Lock, Key;

template <typename S>
std::ostream &operator<<(std::ostream &os, const std::vector<S> &vector)
{
	for (auto i = vector.begin(); i != vector.end(); i++)
		os << *i << " ";
	return os;
}

bool isAll(char c, std::string s)
{
	for (char x : s)
	{
		if (x != c)
			return (false);
	}
	return (true);
}

int countUniquePairs(std::vector<Lock> locks, std::vector<Key> keys)
{
	std::set<std::pair<Lock, Key>> pairs;

	for (Lock l : locks)
	{
		for (Key k : keys)
		{
			bool overlap = false;
			for (int i = 0; i < (int) l.size(); i++)
			{
				if (l[i] + k[i] > 5)
				{
					overlap = true;
					break;
				}
			}
			if (!overlap)
				pairs.insert({l, k});
		}
	}

	return (pairs.size());
}


int main(int argc, char **argv)
{
	if (argc != 2)
		return (-1);

	std::string filename(argv[1]);
	std::fstream data(filename, std::fstream::in);
	std::string line;

	std::vector<Lock> locks;
	std::vector<Key> keys;

	while (!data.eof())
	{
		std::vector<std::string> curr;

		while (std::getline(data, line) && !line.empty())
			curr.push_back(line);

		std::vector<int> schematic(curr[0].size(), 0);

		for (std::string row : curr)
		{
			for (int i = 0; i < (int) row.size(); i++)
			{
				if (row[i] == '#')
					schematic[i]++;
			}
		}

		for (int &c : schematic)
			c--;

		if (isAll('#', curr.front()))
			locks.push_back(schematic);
		else if (isAll('#', curr.back()))
			keys.push_back(schematic);
	}

	std::cout << "Locks: " << std::endl;
	for (Lock l : locks)
		std::cout << l << std::endl;
	std::cout << "Keys: " << std::endl;
	for (Key k : keys)
		std::cout << k << std::endl;
	
	std::cout << countUniquePairs(locks, keys) << std::endl;

	return (0);
}