#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>
#include <climits>

template <typename T, typename U>
std::ostream &operator << (std::ostream &os, const std::map<T, U> &map)
{
	for (auto i : map)
		os << i.first << ": " << i.second << "\n";
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

bool is3Cycle(std::set<std::string> nodes, std::map<std::string, std::set<std::string>> &graph)
{
	// check if edge exist between all nodes.
	if (nodes.size() != 3)
		return (false);

	for (std::string node : nodes)
	{
		auto it = graph.find(node);

		if (it == graph.end())
			return (false);

		for (std::string other : nodes)
		{
			if (node != other && it->second.find(other) == it->second.end())
				return (false);
		}
	}
	return (true);
}

bool cycleHasT(std::set<std::string> cycle)
{
	for (std::string item : cycle)
	{
		if (item[0] == 't')
			return (true);
	}
	return (false);
}

void count3CyclesAux(
	std::string curr_node,
	std::set<std::string> nodes,
	std::set<std::set<std::string>> &cycles,
	std::map<std::string, std::set<std::string>> &graph
)
{
	// base case: we reached path length of 3
	if (nodes.size() == 3)
	{
		if (is3Cycle(nodes, graph) && cycleHasT(nodes))
			cycles.insert(nodes);
		return;
	}

	auto it = graph.find(curr_node);

	if (it == graph.end())
		return;

	for (auto n : it->second)
	{
		if (nodes.find(n) != nodes.end())
			continue;
		nodes.insert(n);
		count3CyclesAux(n, nodes, cycles, graph);
		nodes.erase(n);
	}

	return;
}

int count3Cycles(std::map<std::string, std::set<std::string>> &graph)
{
	std::set<std::set<std::string>> cycles;

	for (auto kv : graph)
		count3CyclesAux(kv.first, {}, cycles, graph);

	std::cout << cycles << std::endl;

	return (cycles.size());
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (-1);

	std::string filename(argv[1]);
	std::fstream data(filename, std::fstream::in);
	std::string line;

	std::map<std::string, std::set<std::string>>	connections;

	while (std::getline(data, line))
	{
		std::string c1 = line.substr(0, line.find('-'));
		std::string c2 = line.substr(line.find('-') + 1);
		
		connections[c1].insert(c2);
		connections[c2].insert(c1);
	}

	std::cout << count3Cycles(connections) << std::endl;

	return (0);
}