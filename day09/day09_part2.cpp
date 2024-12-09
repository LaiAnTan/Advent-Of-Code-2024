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


class DiskObject
{
	public:
		DiskObject(int id, int size) : id(id), size(size) {};

		int id;
		int size;
};

std::ostream &operator << (std::ostream &os, const DiskObject &obj)
{
	if (obj.id == -1)
		os << std::string(obj.size, '.');
	else
		os << std::string(obj.size, 'X');
	
	return (os);
}

std::vector<DiskObject>	parseDiskMap(std::string disk_map)
{
	int count = 0;
	int id = 0;

	std::vector<DiskObject>	v;

	for (char c : disk_map)
	{
		int size = std::stoi(std::string(1, c));

		if (size == 0)
		{
			count++;
			continue;
		}

		if (count % 2 == 0)
			v.push_back(DiskObject(id++, size));
		else
			v.push_back(DiskObject(-1, size));
		count++;
	}
	return (v);
}

void	shuffle(std::vector<DiskObject> &disk)
{
	// use two pointer
	std::vector<DiskObject>::iterator left = disk.begin();
	std::vector<DiskObject>::iterator right = disk.end();

	while (right != disk.begin())
	{
		--right;
		left = disk.begin();

		if ((*right).id == -1)
			continue;

		while (left < right && ((*left).id != -1 || (*left).size - (*right).size < 0))
			left++;
		
		if (left == right)
			continue;

		// std::cout << "swap " << *left << " and " << *right << std::endl;
		int remaining_size = (*left).size - (*right).size;
		if (remaining_size == 0)
			std::iter_swap(left, right);
		else
		{
			// split empty element into two, one of size *right and one of size remaining_size
			disk.insert(left, DiskObject(-1, (*right).size));

			// reinitialise iterators
			left = disk.begin() + std::distance(disk.begin(), left) + 1;
			right = disk.begin() + std::distance(disk.begin(), right) + 1;

			// update size
			(*left).size = remaining_size;

			// swap right with first empty
			std::iter_swap(left - 1, right);
		}
		// std::cout << "result of swap: " << disk << std::endl;
	}
}

long calcCheckSum(std::vector<DiskObject> ordered_disk)
{
	std::vector<int>	ods;

	for (DiskObject o : ordered_disk)
	{
		for (int s  = 0; s < o.size; s++)
			ods.push_back(o.id == -1 ? 0 : o.id);
	}

	long check_sum = 0;
	int size = ods.size();
	for (int i = 0; i < size; i++)
	{
		check_sum += ods[i] * i;
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

	std::vector<DiskObject>	v = parseDiskMap(disk_map);

	std::cout << v << std::endl;

	shuffle(v);

	std::cout << v << std::endl;

	std::cout << calcCheckSum(v) << std::endl;

	return (0);
}