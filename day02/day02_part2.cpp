#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <set>

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i : vector)
		os << i << " ";
	return os;
}

bool isAlreadySafe(std::vector<int> vec)
{
	std::vector<int>::iterator	it = vec.begin();

	bool increasing = (*it - *(it + 1)) < 0 ? true : false;

	while ((it + 1) != vec.end())
	{
		if ((increasing && !(*it - *(it + 1) >= -3 && *it - *(it + 1) <= -1)) ||
			(!increasing && !(*it - *(it + 1) >= 1 && *it - *(it + 1) <= 3)))
			return (false);

		it++;
	}

	return (true);
}

bool isSafeWithout(std::vector<int> vec, int i)
{
	vec.erase(vec.begin() + i);
	return (isAlreadySafe(vec));
}

bool isSafe(std::vector<int> levels)
{
	if (isAlreadySafe(levels))
		return (true);
	
	for (int i = 0; i < (int) levels.size(); ++i)
	{
		if (isSafeWithout(levels, i))
			return (true);
	}
	return (false);
}

int main(int argc, char **argv) {

	if (argc != 2)
		return (-1);
	
	std::string		filename(argv[1]);
	std::fstream	data(filename, std::fstream::in);
	std::string		line;
	int		total_safe = 0;

	if (!data.is_open())
		return (-1);
	
	while (std::getline(data, line))
	{
		std::vector<int>	curr;
		std::stringstream ss(line);
		int num;
		while (ss >> num)
			curr.push_back(num);

		if (isSafe(curr))
			total_safe += 1;
	}

	std::cout << total_safe << std::endl;

	return (0);
}