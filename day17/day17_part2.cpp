#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <sstream>
#include <cmath>
#include <climits>

template <typename S>
std::ostream &operator<<(std::ostream &os, const std::vector<S> &vector)
{
	os << "[";
	for (auto i = vector.begin(); i != vector.end() - 1; i++)
		os << *i << ", ";
	os << *(vector.end() - 1) << "]";
	return os;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::map<T, U> &map)
{
	for (auto i : map)
		os << i.first << ": " << i.second << "\n";
	return os;
}


std::vector<int> prog(long long a)
{
	std::vector<int> v;
	long long b = 0;
	long long c = 0;

	while (a != 0)
	{
		b = (a % 8) ^ 2;
		c = a >> b;
		b = b ^ 3 ^ c;
		v.push_back(b % 8);
		a = a >> 3;
	}
	return (v);
}

long long findRegisterValAux(long long a, int digits_found, std::vector<int> reversed)
{
	if (digits_found == (int) reversed.size())
		return (a);

	long long res = INT_MIN;

	for (int i = 0; i < 8; i++)
	{
		std::vector<int> out = prog(a * 8 + i);
		if (out.empty())
			continue;
		if (out[0] == reversed[digits_found])
		{
			res = findRegisterValAux(a * 8 + i, digits_found + 1, reversed);
			if (res != INT_MIN)
				return (res);
		}
	}

	return (INT_MIN);
}

long findRegisterVal(std::vector<int> program)
{
	std::reverse(program.begin(), program.end());
	return (findRegisterValAux(0, 0, program));
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (-1);

	std::string filename(argv[1]);
	std::fstream data(filename, std::fstream::in);
	std::string line;

	if (!data.is_open())
		return (-1);

	std::vector<int> prog;

	std::getline(data, line);
	std::getline(data, line);
	std::getline(data, line);
	std::getline(data, line);
	std::getline(data, line);

	std::stringstream ss(line.substr(line.find(":") + 2));
	std::string num;
	while (std::getline(ss, num, ','))
		prog.push_back(std::stoi(num));

	std::cout << findRegisterVal(prog) << std::endl;
}