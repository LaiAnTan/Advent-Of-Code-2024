#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// parses a number of max 3 digits
int parseNumber(std::string &str)
{
	int i = 0;

	std::string token;

	while (isdigit(str[i]) == true && i < 3)
		i++;

	int num = std::stoi(str.substr(0, i));
	str = str.substr(i);
	return (num);
}

std::pair<int, int>	parsePair(std::string &str)
{
	int num1 = 0;
	int num2 = 0;

	if (str[0] != '(')
		return std::make_pair(0, 0);

	str = str.substr(1);
	num1 = parseNumber(str);


	if (str[0] != ',')
		return std::make_pair(0, 0);

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
std::pair<int, int>	parseMul(std::string &str)
{
	size_t start = str.find("mul");

	if (start == std::string::npos)
	{
		str = "";
		return (std::make_pair(0, 0));
	}

	str = str.substr(start + 3);

	std::pair<int, int>	p = parsePair(str);

	return (p);
}

// obtains the first segment of a string, starting with 'do()' and ending with 'don't()'
// stuff before "do()" is ignored.
// removes the segment from the original string.
std::string	segmentString(std::string &str)
{
	size_t	start = str.find("do()");
	std::string segment = "";

	if (start == std::string::npos)
	{
		str = "";
		return ("");
	}
	size_t	end = str.find("don't()", start);
	if (end == std::string::npos) // no "don't()" or "don't() before "do()"
	{
		segment = str.substr(start + 4);
		str = "";
	}
	else
	{
		segment = str.substr(start + 4, end - (start + 4));
		str = str.substr(end + 7);
	}

	return (segment);
}

std::string firstSegment(std::string &str)
{
	size_t pos =  str.find("don't()");
	std::string segment = "";

	if (pos == std::string::npos)
	{
		segment = str;
		str = "";
	}
	else
	{
		segment = str.substr(0, pos);
		str = str.substr(pos + 7);
	}

	return (segment);
}

int handleSegment(std::string &segment)
{
	int res = 0;

	while (segment != "")
	{
		std::pair<int, int> p = parseMul(segment);

		if (p.first == 0 && p.second == 0)
			continue;

		res += p.first * p.second;
	}

	return (res);
}


int main(int argc, char **argv)
{

	if (argc != 2)
			return (-1);
		
	std::string		filename(argv[1]);
	std::fstream	data(filename, std::fstream::in);
	std::string		line;
	std::string		input;

	if (!data.is_open())
		return (-1);

	int res = 0;

	// !! this is important
	while (std::getline(data, line))
		input.append(line);

	std::string segment = firstSegment(input);
	std::cout << segment << "\n" << std::endl;
	res += handleSegment(segment);

	while (input != "")
	{
		segment = segmentString(input);
		std::cout << segment << "\n" << std::endl;
		
		if (!segment.empty())
			res += handleSegment(segment);
	}

	std::cout << res << std::endl;

	return (0);
}