#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>


template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i : vector)
		os << i << " ";
	return os;
}

void	blink(std::vector<unsigned long long> &stones)
{

	std::vector<unsigned long long>	blinked;

	for (std::vector<unsigned long long>::iterator it = stones.begin(); it != stones.end(); it++)
	{
		if (*it == 0)
			blinked.push_back(1);
		else
		{
			int digits = static_cast<int>(std::floor(std::log10(*it) + 1));

			if (digits % 2 == 1)
			{
				blinked.push_back(*it * 2024);
				continue;
			}

			std::string ns = std::to_string(*it);

			int half = digits / 2;

			blinked.push_back(std::stoll(ns.substr(0, half)));
			blinked.push_back(std::stoll(ns.substr(half)));
		}
	}
	stones = blinked;
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

	std::getline(data, line);

	std::vector<unsigned long long>	stones;
	std::stringstream ss(line);
	int num;
	while (ss >> num)
		stones.push_back(num);

	std::cout << stones << std::endl;

	for (int i = 0; i < 50; i++)
	{
		std::cout << "Blink " << i + 1 << std::endl;
		blink(stones);
		// std::cout << stones << std::endl;
	}

	std::cout << stones.size() << std::endl;

	return (0);
}