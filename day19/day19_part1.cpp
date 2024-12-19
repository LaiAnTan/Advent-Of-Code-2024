#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <climits>
#include <algorithm>
#include <numeric>

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	os << "[";
	for (auto i = vector.begin(); i != vector.end() - 1; i++)
		os << *i << ", ";
	os << *(vector.end() - 1) << "]";
	return os;
}

template <typename S>
std::ostream &operator << (std::ostream &os, const std::set<S> &set)
{
	os << "{";
	for (auto i = set.begin(); i != set.end(); ++i) {
		if (i != set.begin()) {
			os << ", ";
		}
		os << *i;
	}
	os << "}";
	return os;
}

bool isPrefixOf(std::string prefix, std::string str)
{
	return (str.find(prefix) == 0);
}

bool isPossiblePatternAux(
	std::string	rest,
	std::vector<std::string> curr,
	std::string	pattern,
	std::set<std::string> &towels,
	std::unordered_map<std::string, bool> &memo
)
{
	// base case: rest is empty, check if pattern matches
	if (rest.empty())
		return (std::accumulate(curr.begin(), curr.end(), std::string("")) == pattern);

	if (memo.find(rest) != memo.end())
		return (memo[rest]);

	bool possible = false;

	for (std::string t : towels)
	{
		if (isPrefixOf(t, rest))
		{
			curr.push_back(t);
			if (isPossiblePatternAux(rest.substr(t.size()), curr, pattern, towels, memo))
				possible = true;
			curr.pop_back();
		}
	}

	memo[rest] = possible;
	return (possible);
}

bool isPossiblePattern(std::string pattern, std::set<std::string> &towels, std::unordered_map<std::string, bool> &memo)
{
	std::cout << "Processing " << pattern << std::endl;
	return (isPossiblePatternAux(pattern, {}, pattern, towels, memo));
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (-1);
		
	std::string		filename(argv[1]);
	std::fstream	data(filename, std::fstream::in);
	std::string		line;

	std::set<std::string>	towels;

	std::getline(data, line);

	std::string delim = ", ";
	std::string token;
	size_t pos = 0;

	while (true)
	{
		pos = line.find(delim);
		if (pos == std::string::npos)
			break;
		towels.insert(line.substr(0, pos));
		line = line.substr(pos + delim.length());
	}

	std::cout << towels << std::endl;

	std::vector<std::string> patterns;

	std::getline(data, line);

	while(std::getline(data, line))
		patterns.push_back(line);

	std::cout << patterns << std::endl;
	
	int possible_patterns = 0;

	std::unordered_map<std::string, bool> memo;

	for (std::string p : patterns)
		possible_patterns += isPossiblePattern(p, towels, memo);

	std::cout << possible_patterns << std::endl;

	return (0);
}