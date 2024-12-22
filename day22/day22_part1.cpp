#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>
#include <climits>

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

	ll sum = 0;

	for (ll num : initial_secret_nums)
	{
		for (int i = 0; i < 2000; i++)
			nextSecretNum(num);
		sum += num;
	}
	std::cout << sum << std::endl;

	return (0);
}