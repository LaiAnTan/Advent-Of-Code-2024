#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <cmath>
#include <chrono>

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i : vector)
		os << i << " ";
	return os;
}

template <typename T, typename U>
std::ostream &operator << (std::ostream &os, const std::map<T, U> &map)
{
	for (auto i : map)
		os << i.first << ": " << i.second << "\n";
	return os;
}

template <typename T, typename U>
std::ostream &operator << (std::ostream &os, const std::unordered_map<T, U> &map)
{
	for (auto i : map)
		os << i.first << ": " << i.second << "\n";
	return os;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &pair)
{
	os << "(" << pair.first << ", " << pair.second << ")";
	return os;
}


long	findNumStones(std::map<long long, long> stones)
{
	long long total = 0;

	for (std::map<long long, long>::iterator it = stones.begin(); it != stones.end(); it++)
		total += (*it).second;

	return (total);
}

void	blink(std::map<long long, long> &stones)
{
	std::map<long long, long>	next_counts;

	for (std::map<long long, long>::iterator it = stones.begin(); it != stones.end(); it++)
	{
		long long num = (*it).first;
		long long count = (*it).second;

		if (num == 0)
		{
			next_counts[1] += count;
			continue;
		}

		int digits = static_cast<int>(std::floor(std::log10(num) + 1));

		if (digits % 2 != 0)
		{
			long long res = num * 2024;
			next_counts[res] += count;
			continue;
		}

		int half = digits / 2;
		std::string ns = std::to_string((*it).first);
		long long fst = std::stoll(ns.substr(0, half));
		long long snd = std::stoll(ns.substr(half));

		// split 2 stones
		next_counts[fst] += count;
		next_counts[snd] += count;

	}
	stones = next_counts;
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

	std::map<long long, long>	stones;

	std::stringstream ss(line);
	int num;
	while (ss >> num)
		stones[num] = 1;
	
	std::cout << "Input: \n" << stones << std::endl;

	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	auto solve_start = high_resolution_clock::now();
	for (int i = 0; i < 75; i++)
		blink(stones);
	auto solve_end = high_resolution_clock::now();
	std::cout << "Total time: " << duration_cast<milliseconds>(solve_end - solve_start).count() << " ms" << std::endl;

	std::cout << findNumStones(stones) << std::endl;

	return (0);
}