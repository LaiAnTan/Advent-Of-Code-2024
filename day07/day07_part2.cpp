#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i : vector)
		os << i << " ";
	return os;
}

long long concatNumbers(long long num1, long long num2)
{
	return (std::stoll(std::to_string(num1).append(std::to_string(num2))));
}

bool aux(long target, long curr_val, std::vector<int> components)
{
	// base case
	if (components.empty())
		return (target == curr_val);
	
	int next = components[0];

	components.erase(components.begin());

	bool result = aux(target, curr_val * next, components) ||
			aux(target, curr_val + next, components) ||
			aux(target, concatNumbers(curr_val, next), components);

	return (result);
}

int tryPossibleCombinations(long target, std::vector<int> components)
{
	int first = components[0];
	components.erase(components.begin());
	return (aux(target, first, components));
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

	std::vector<std::vector<char> >	layout;

	long long res = 0;

	while (std::getline(data, line))
	{
		long target = std::stol(line.substr(0, line.find(':')));

		std::vector<int>	components;
		std::stringstream ss(line.substr(line.find(":") + 1));
		int num;
		while (ss >> num)
			components.push_back(num);

		// std::cout << target << " = " << components << std::endl;

		if (tryPossibleCombinations(target, components))
			res += target;
	}

	std::cout << res << std::endl;

	return (0);
}