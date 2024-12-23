#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <climits>

template <typename S>
std::ostream &operator<<(std::ostream &os, const std::vector<S> &vector)
{
	for (auto i = vector.begin(); i != vector.end(); i++)
		os << *i << " ";
	return os;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::map<T, U> &map)
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


typedef long long ll;

void mix(ll &secret, ll val)
{
	secret = secret ^ val;
}

void prune(ll &secret)
{
	secret = secret % 16777216;
}

void nextSecretNum(ll &secret)
{
	mix(secret, secret * 64);
	prune(secret);
	mix(secret, std::floor(secret / 32));
	prune(secret);
	mix(secret, secret * 2048);
	prune(secret);
}

// brute force slow as hell lmao
int findBestTimetoSell(std::vector<std::pair<std::vector<int>, std::vector<int>>> price_diff_pairs)
{
	// sequence, profit
	std::map<std::vector<int>, int> diff_map;
	std::set<std::vector<int>> diff_set;

	for (auto &pair : price_diff_pairs)
	{
		auto &diff = pair.second;

		for (size_t i = 0; i < diff.size() - 3; i++)
			diff_set.insert(std::vector<int>(diff.begin() + i, diff.begin() + i + 4));
	}

	std::cout << diff_set.size() << " unique sequences" << std::endl;

	int max_win = 0;

	for (const auto &diff_seq : diff_set)
	{
		int occur_count = 0;
		for (const auto &pair : price_diff_pairs)
		{
			const auto &diffs = pair.second;
			const auto &prices = pair.first;

			auto it = std::search(diffs.begin(), diffs.end(), diff_seq.begin(), diff_seq.end());

			if (it != diffs.end())
			{
				diff_map[diff_seq] += *(prices.begin() + std::distance(diffs.begin(), it) + 4);
				occur_count++;
			}
		}
		
		if (diff_map[diff_seq] > max_win)
		{
			max_win = diff_map[diff_seq];
			std::cout << diff_seq << " occuring " << occur_count << " times => " << max_win << std::endl;
		}
	}

	return (max_win);
}

int findBestTimetoSellButBetter(std::vector<std::pair<std::vector<int>, std::vector<int>>> price_diff_pairs)
{
	std::vector<std::map<std::vector<int>, int>> diff_maps(price_diff_pairs.size());
	
	// for each sequence, count how many occurences

	for (size_t i = 0; i < price_diff_pairs.size(); i++)
	{
		auto curr_prices = price_diff_pairs[i].first;
		auto curr_diffs = price_diff_pairs[i].second;

		for (auto cd = curr_diffs.begin(); cd != curr_diffs.end() - 3; cd++)
		{
			std::vector<int> seq(cd, cd + 4);

			// if found in map, skip
			if (diff_maps[i].find(seq) != diff_maps[i].end())
				continue;
			diff_maps[i][seq] = *(curr_prices.begin() + std::distance(curr_diffs.begin(), cd + 4));
		}
	}

	// combine all maps
	std::map<std::vector<int>, int> main;

	for (auto map : diff_maps)
	{
		for (auto kv : map)
			main[kv.first] += kv.second;
	}

	int max_win = 0;

	for (auto kv : main)
		max_win = (kv.second > max_win) ? kv.second : max_win;

	return (max_win);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (-1);

	std::string filename(argv[1]);
	std::fstream data(filename, std::fstream::in);
	std::string line;

	std::vector<ll> initial_secret_nums;

	while (std::getline(data, line))
		initial_secret_nums.push_back(std::stoll(line));

	std::vector<std::pair<std::vector<int>, std::vector<int>>> price_diff_pairs;

	for (ll num : initial_secret_nums)
	{
		std::vector<int> prices;
		std::vector<int> diffs;
		for (int i = 0; i < 2000; i++)
		{
			nextSecretNum(num);
			prices.push_back(num % 10);
		}
		for (std::vector<int>::iterator pt = prices.begin(); pt != (prices.end() - 1); pt++)
			diffs.push_back(*(pt + 1) - *pt);

		price_diff_pairs.push_back({prices, diffs});
	}

	std::cout << findBestTimetoSellButBetter(price_diff_pairs) << std::endl;

	return (0);
}