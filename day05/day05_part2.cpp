#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i : vector)
		os << i << " ";
	return os;
}

bool isValidUpdateOrdering(std::map<int, std::vector<int>> tab, std::vector<int> ordering)
{
	int size = ordering.size();

	for (int i = 0; i < (size - 1); i++)
	{
		std::vector<int>::iterator it = ordering.begin() + i;
		std::map<int, std::vector<int>>::iterator	vt = tab.find(*it);

		if (vt == tab.end())
				return (false);

		for (std::vector<int>::iterator jt = it + 1; jt != ordering.end(); jt++)
		{
			if (std::find(vt->second.begin(), vt->second.end(), *jt) == vt->second.end())
				return (false);
		}
	}
	return (true);
}

void reorder(std::map<int, std::vector<int>> tab, std::vector<int> &ordering)
{
	int size = ordering.size();

	for (int i = 0; i < (size - 1); i++)
	{
		std::vector<int>::iterator it = ordering.begin() + i;
		std::map<int, std::vector<int>>::iterator	vt = tab.find(*it);

		// if we cant find a rule with the specified key, the only possible explanation is that the number will be at the last position of the ordering
		if (vt == tab.end())
		{
			int value = *it;
			ordering.erase(it);
			ordering.push_back(value);
			i--;
			continue;
		}

		for (std::vector<int>::iterator jt = it + 1; jt != ordering.end(); jt++)
		{
			// if we cannot find a key | value , it means that the value and key should swap places
			if (std::find(vt->second.begin(), vt->second.end(), *jt) == vt->second.end())
				std::iter_swap(it, jt);
		}
	}
}

int main(int argc, char **argv)
{

	if (argc != 2)
			return (-1);
		
	std::string		filename(argv[1]);
	std::fstream	data(filename, std::fstream::in);
	std::string		line = "";

	if (!data.is_open())
		return (-1);

	std::map<int, std::vector<int>>	tab;

	std::getline(data, line);
	
	while (!line.empty())
	{
		int delim_pos = line.find('|');

		int key = std::stoi(line.substr(0, delim_pos));
		int val = std::stoi(line.substr(delim_pos + 1));

		std::map<int, std::vector<int>>::iterator	it = tab.find(key);

		if (it != tab.end())
			it->second.push_back(val);
		else
			tab[key] = std::vector<int>{val};

		std::getline(data, line);
	}

	std::cout << "map built" << std::endl;
	for (auto it : tab)
	{
		std::cout << "key: " << it.first << " val: " << it.second << std::endl;
	}

	int res = 0;

	while (std::getline(data, line))
	{
		std::vector<int>	curr;
		std::stringstream	ss(line);
		std::string			token;

		while (std::getline(ss, token, ','))
			curr.push_back(std::stoi(token));

		std::cout << "Curr: " << curr << std::endl;

		if (isValidUpdateOrdering(tab, curr))
			continue;
		
		while (!isValidUpdateOrdering(tab, curr))
			reorder(tab, curr);

		std::cout << "Curr after fixes: " << curr << std::endl;

		res += *(curr.begin() + (curr.size() / 2));
	}

	std::cout << res << std::endl;

	return (0);
}