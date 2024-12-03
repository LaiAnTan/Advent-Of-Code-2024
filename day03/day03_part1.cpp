#include <iostream>
#include <fstream>
#include <sstream>

// parses a number of max 3 digits
int parseNumber(std::string &str)
{
	int i = 0;

	std::string token;

	while (isdigit(str[i]) == true && i < 3)
	{
		token.append(std::string(1, str[i++]));
	}
	str = str.substr(i);
	return (std::stoi(token));
}

std::pair<int, int>	parsePair(std::string &str)
{
	int num1 = 0;
	int num2 = 0;

	if (str[0] != '(')
		return std::make_pair(num1, num2);

	str = str.substr(1);
	num1 = parseNumber(str);


	if (str[0] != ',')
		return std::make_pair(num1, num2);

	str = str.substr(1);
	num2 = parseNumber(str);

	if (str[0] != ')')
		return std::make_pair(0, 0);

	str = str.substr(1);

	return std::make_pair(num1, num2);
}


// parses the first mul(x,y) instruction in the string
// mutates the string, removing the instruction that was parsed
// returns the two numbers x and y in the instruction
std::pair<int, int>	parseInstruction(std::string &str)
{
	size_t start = str.find("mul");

	if (start == std::string::npos)
	{
		str = "";
		return (std::make_pair(0, 0));
	}

	str = str.substr(start + 3);

	std::cout << str << std::endl;

	std::pair<int, int>	p = parsePair(str);

	return (p);
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

	int res = 0;
	while (std::getline(data, line))
	{
		while (line != "")
		{
			std::pair<int, int> p = parseInstruction(line);

			if (p.first == 0 && p.second == 0)
				continue;

			std::cout << p.first << "," << p.second << std::endl;
			res += p.first * p.second;
		}
	}

	std::cout << res << std::endl;

	return (0);
}