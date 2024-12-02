#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char **argv) {

	if (argc != 2)
		return (-1);
	
	std::string		filename(argv[1]);
	std::fstream	data(filename, std::fstream::in);
	std::string		line;
	std::vector<int>	l1, l2;
	int		sum = 0;

	if (!data.is_open())
		return (-1);
	
	while (std::getline(data, line))
	{
		l1.push_back(std::stoi(line.substr(0, line.find(' '))));
		l2.push_back(std::stoi(line.substr(line.rfind(' ') + 1)));
	}

	std::sort(l1.begin(), l1.end());
	std::sort(l2.begin(), l2.end());

	std::vector<int>::iterator	i1(l1.begin());
	std::vector<int>::iterator	i2(l2.begin());

	while (i1 != l1.end())
	{
		sum += std::abs(*i1 - *i2);
		i1++;
		i2++;
	}

	std::cout << sum << std::endl;

	return (0);
}