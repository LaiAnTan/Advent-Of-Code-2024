#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i : vector)
		os << i << " ";
	return os;
}

bool isSafe(std::vector<int> levels)
{
	std::vector<int>::iterator	it = levels.begin();

	bool increasing = (*it - *(it + 1)) < 0 ? true : false;

	std::cout << (increasing ? "increase" : "decrease") << std::endl;

	while ((it + 1) != levels.end())
	{
		if ((increasing && !(*it - *(it + 1) >= -3 && *it - *(it + 1) <= -1)) ||
			(!increasing && !(*it - *(it + 1) >= 1 && *it - *(it + 1) <= 3)))
			return (false);

		it++;
	}

	return (true);
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

		std::cout << curr << std::endl;
		if (isSafe(curr))
			total_safe += 1;
	}

	std::cout << total_safe << std::endl;

	return (0);
}