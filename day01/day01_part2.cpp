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
	std::vector<int>	l1;
	std::unordered_map<int, int>	counts;
	int		sim_score = 0;

	if (!data.is_open())
		return (-1);
	
	while (std::getline(data, line))
	{
		l1.push_back(std::stoi(line.substr(0, line.find(' '))));
		
		int num = std::stoi(line.substr(line.rfind(' ') + 1));

		std::unordered_map<int, int>::iterator	it = counts.find(num);

		if (it != counts.end())
		{
			it->second += 1;
		}
		else
		{
			counts[num] = 1;
		}

		if (it != counts.end())
			std::cout << it->first << ", " << it->second << std::endl;
	}

	std::vector<int>::iterator	i1(l1.begin());

	std::cout << "Finished building hash table" << std::endl;

	while (i1 != l1.end())
	{
		std::unordered_map<int, int>::iterator	it = counts.find(*i1);

		sim_score += *i1 * (it != counts.end() ? it->second : 0);
		i1++;
	}

	std::cout << sim_score << std::endl;

	return (0);
}