#include <iostream>
#include <vector>
#include <fstream>

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	for (auto i : vector)
		os << i;
	return os;
}

int	findXMAS(std::vector<std::vector<char>> matrix)
{
	int xmas = 0;
	std::vector<char> word = {'M', 'A', 'S'};
	std::vector<std::pair<int, int>>	directions = {
		{-1, 0}, {1, 0}, {0, -1}, {0, 1},
		{-1, -1}, {-1, 1}, {1, -1}, {1, 1}
	};

	int rows = matrix.size();
	int cols = matrix[0].size();

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			if (matrix[y][x] == 'X')
			{
				for (std::pair<int, int> d : directions)
				{
					bool found = true;

					for (size_t i = 0; i < 3; i++)
					{
						int nx = x + (d.first * (i + 1));
						int ny = y + (d.second * (i + 1));

						if (ny < 0 || ny >= rows || nx < 0 || nx >= cols || matrix[ny][nx] != word[i])
						{
							found = false;
							break;
						}
					}

					if (found)
						xmas++;
				}
			}
		}
	}

	return (xmas);
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

	std::vector<std::vector<char> >	matrix;

	while (std::getline(data, line))
		matrix.push_back(std::vector<char>(line.begin(), line.end()));

	// for (auto i = matrix.begin(); i != matrix.end(); i++)
	// 	std::cout << *i << std::endl;

	std::cout << findXMAS(matrix) << std::endl;

	return (0);
}