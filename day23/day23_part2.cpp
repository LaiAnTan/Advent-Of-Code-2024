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


void bronKerboschAux(
	std::set<std::string> curr_max_clique,
	std::set<std::string> &largest_max_clique,
	std::set<std::string> x,
	std::set<std::string> vertex_set,
	std::map<std::string, std::set<std::string>> &graph
)
{
	if (vertex_set.empty() && x.empty())
	{
		if (curr_max_clique.size() > largest_max_clique.size())
			largest_max_clique = curr_max_clique;
		return;
	}

	std::set<std::string> vertex_set_copy = vertex_set;

	for (std::string vertex: vertex_set_copy)
	{
		std::set<std::string> next_max_clique = curr_max_clique;
		next_max_clique.insert(vertex);

		std::set<std::string> new_vertex_set;
		std::set<std::string> new_x;

		std::set_intersection(vertex_set.begin(), vertex_set.end(),
			graph[vertex].begin(), graph[vertex].end(),
			std::inserter(new_vertex_set, new_vertex_set.begin()));
		
		std::set_intersection(x.begin(), x.end(),
			graph[vertex].begin(), graph[vertex].end(),
			std::inserter(new_x, new_x.begin()));

		bronKerboschAux(next_max_clique, largest_max_clique, new_x, new_vertex_set, graph);
		
		vertex_set.erase(vertex);
		x.insert(vertex);
	}
}

std::set<std::string> bronKerbosch(std::map<std::string, std::set<std::string>> &graph)
{
	std::set<std::string> largest_max_clique = {};
	std::set<std::string> vertex_set = {};

	for (auto kv : graph)
		vertex_set.insert(kv.first);

	bronKerboschAux({}, largest_max_clique, {}, vertex_set, graph);

	return (largest_max_clique);
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

	std::set<std::string> max_clique = bronKerbosch(connections);

	std::string password = "";

	std::vector<std::string> v(max_clique.begin(), max_clique.end());

	for (auto it = v.begin(); it != v.end() - 1; it++)
		password += *it + ",";
	password += *(v.end() - 1);

	std::cout << password << std::endl;

	return (0);
}