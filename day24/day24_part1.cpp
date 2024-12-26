#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <bitset>

template <typename S>
std::ostream &operator<<(std::ostream &os, const std::vector<S> &vector)
{
	for (auto i = vector.begin(); i != vector.end(); i++)
		os << *i << "\n";
	return os;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::map<T, U> &map)
{
	for (auto i : map)
		os << i.first << ": " << i.second << "\n";
	return os;
}

class Gate {

	public:
		std::string v1;
		std::string v2;
		std::string op;
		std::string out;
		bool completed;

		Gate(std::string v1, std::string v2, std::string op, std::string out) : v1(v1), v2(v2), op(op), out(out), completed(false) {}
};

std::ostream &operator << (std::ostream &os, const Gate &g)
{
	os << g.v1 << " " << g.op << " " << g.v2 << " -> " << g.out << ": " << (g.completed ? "Finished" : "Not Finished");
	return os;
}

bool allDone(std::vector<Gate> gates)
{
	for (Gate g : gates)
	{
		if (g.completed == false)
			return (false);
	}
	return (true);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (-1);

	std::string filename(argv[1]);
	std::fstream data(filename, std::fstream::in);
	std::string line;

	std::map<std::string, bool>	variables;

	while (std::getline(data, line) && !line.empty())
	{
		std::string name = line.substr(0, line.find(": "));
		bool value = (bool) std::stoi(line.substr(line.find(": ") + 2));
		variables[name] = value;
	}

	std::vector<Gate>	gates;

	while (std::getline(data, line))
	{
		std::string v1 = line.substr(0, line.find(' '));

		line = line.substr(line.find(' ') + 1);
	
		std::string op = line.substr(0, line.find(' '));

		line = line.substr(line.find(' ') + 1);
		std::string v2 = line.substr(0, line.find(' '));
		std::string out = line.substr(line.find("-> ") + 3);
		
		gates.push_back(Gate(v1, v2, op, out));
	}

	std::cout << gates << std::endl;

	while (!allDone(gates))
	{
		for (Gate &g : gates)
		{
			if (variables.find(g.v1) != variables.end() &&
				variables.find(g.v2) != variables.end() && g.completed == false)
			{
				if (g.op == "AND")
						variables[g.out] = variables[g.v1] && variables[g.v2];
				else if (g.op == "OR")
						variables[g.out] = variables[g.v1] || variables[g.v2];
				else if (g.op == "XOR")
						variables[g.out] = variables[g.v1] ^ variables[g.v2];
				g.completed = true;
			}
		}
	}

	long long res = 0;
	std::string s = "";
	for (auto kv : variables)
	{
		if (kv.first[0] != 'z')
			continue;
	
		s.push_back((kv.second == true) ? '1' : '0');

		int position = std::stoi(kv.first.substr(1));

		if (kv.second == true)
			res |= (1 << position); 
		else 
			res &= ~(1 << position);
	}

	std::reverse(s.begin(), s.end());
	std::cout << static_cast<long long>(std::bitset<64>(s).to_ullong()) << std::endl;

	return (0);
}
