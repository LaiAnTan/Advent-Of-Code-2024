#include <iostream>
#include <fstream>
#include <vector>
#include <map>

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i : vector)
		os << i << " ";
	return os;
}

template <typename T, typename U>
std::ostream &operator << (std::ostream &os, const std::map<T, U> &map)
{
	for (auto i : map)
		os << i.first << ": " << i.second << "\n";
	return os;
}

std::vector<int>	parseDiskMap(std::string disk_map)
{
	int count = 0;
	int id = 0;

	std::vector<int>	v;

	for (char c : disk_map)
	{
		int limit = std::stoi(std::string(1, c));
		if (count % 2 == 0)
		{
			for (int i = 0; i < limit; i++)
				v.push_back(id);
			id++;
		}
		else
		{
			for (int i = 0; i < limit; i++)
				v.push_back(-1);
		}
		count++;
	}
	return (v);
}

void	shuffle(std::vector<int> &disk)
{
	// use two pointer

	std::vector<int>::iterator left = disk.begin();
	std::vector<int>::iterator right = std::prev(disk.end());

	while (left < right)
	{
		while (*left != -1)
			left++;
		
		while (*right == -1)
			right--;

		// std::cout << "Left: " << *left << std::endl;
		// std::cout << "Right: " << *right << std::endl;
		
		if (*left == -1 && *right != -1)
			std::iter_swap(left, right);

		// std::cout << disk << std::endl;

		left++;
		right--;
	}
}

long calcCheckSum(std::vector<int> ordered_disk)
{
	int i = 0;
	long check_sum = 0;
	while (ordered_disk[i] != -1)
	{
		check_sum += ordered_disk[i] * i;
		i++;
	}
	return (check_sum);
}

int main(int argc, char **argv)
{

	if (argc != 2)
			return (-1);
		
	std::string		filename(argv[1]);
	std::fstream	data(filename, std::fstream::in);
	std::string		disk_map;

	if (!data.is_open())
		return (-1);

	std::getline(data, disk_map);

	std::vector<int>	v = parseDiskMap(disk_map);

	std::cout << v << std::endl;

	shuffle(v);

	std::cout << v << std::endl;

	std::cout << calcCheckSum(v) << std::endl;

	return (0);
}