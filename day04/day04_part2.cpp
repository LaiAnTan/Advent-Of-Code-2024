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

int	findMASinShapeOfX(std::vector<std::vector<char>> matrix)
{
	int mas_in_shape_of_x = 0;
	int rows = matrix.size();
	int cols = matrix[0].size();

	// ignore A's at the edge of the board
	for (int y = 1; y < rows - 1; y++)
	{
		for (int x = 1; x < cols - 1; x++)
		{
			if (matrix[y][x] == 'A')
			{
				int tx = x - 1;
				int dx = x + 1;
				int ty = y - 1;
				int dy = y + 1;

				if ((matrix[ty][tx] == 'M' && matrix[dy][dx] == 'S') || (matrix[ty][tx] == 'S' && matrix[dy][dx] == 'M'))
				{
					if ((matrix[ty][dx] == 'M' && matrix[dy][tx] == 'S') || (matrix[ty][dx] == 'S' && matrix[dy][tx] == 'M'))
					{
						mas_in_shape_of_x++;
					}
				}
			}
		}
	}

	return (mas_in_shape_of_x);
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

	std::cout << findMASinShapeOfX(matrix) << std::endl;

	return (0);
}