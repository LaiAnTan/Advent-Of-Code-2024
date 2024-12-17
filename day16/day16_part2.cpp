#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <algorithm>
#include <climits>
#include <unistd.h>
#include <termios.h>

typedef std::pair<int, int> Coordinate;
typedef std::pair<int, int> Direction;

template <typename S>
std::ostream &operator<<(std::ostream &os, const std::vector<S> &vector)
{
    for (auto i = vector.begin(); i != vector.end(); i++)
        os << *i;
    return os;
}

template <typename S>
std::ostream &operator<<(std::ostream &os, const std::set<S> &set)
{
    for (auto i = set.begin(); i != set.end(); i++)
        os << *i << " ";
    return os;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &pair)
{
    os << "(" << pair.first << ", " << pair.second << ")";
    return os;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::map<T, U> &map)
{
    for (auto i : map)
        os << i.first << ": " << i.second << "\n";
    return os;
}

Coordinate findTilePos(char tile, std::vector<std::vector<char>> maze)
{
    int rows = maze.size();
    int cols = maze[0].size();

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            if (maze[y][x] == tile)
                return (std::make_pair(x, y));
        }
    }
    return (std::make_pair(-1, -1));
}

// helper to return the lowest cost turning 'from' -> 'to'
int turnCost(Direction from, Direction to)
{
    // up, left, down, right
    std::vector<Direction> ds = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    if (from == to)
        return (0);

    int from_idx = std::find(ds.begin(), ds.end(), from) - ds.begin();
    int to_idx = std::find(ds.begin(), ds.end(), to) - ds.begin();

    return (std::max(std::abs(from_idx - to_idx), std::abs(to_idx - from_idx)) * 1000);
}

std::vector<Direction> getLeftRightTurns(Direction curr_dir)
{
    if (curr_dir == Direction{0, -1} || curr_dir == Direction{0, 1})
        return {Direction{-1, 0}, Direction{1, 0}};
    if (curr_dir == Direction{-1, 0} || curr_dir == Direction{1, 0})
        return {Direction{0, 1}, Direction{0, -1}};
    return {}; // Should not reach here
}

void printMaze(const std::vector<std::vector<char>> &maze)
{
    // Move the cursor up by the number of rows in the maze
    std::cout << "\033[2J\033[1;1H";

    for (const std::vector<char> &row : maze)
    {
        for (char cell : row)
        {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    std::flush(std::cout);
    usleep(10000);
}

struct CompareDistance
{
    bool operator()(const std::pair<int, std::pair<Coordinate, Direction>> &a,
                    const std::pair<int, std::pair<Coordinate, Direction>> &b)
    {
        return (a.first > b.first); // Min-heap based on the first element of the pair
    }
};

int totalTilesUsedInlowestCostPaths(Coordinate start, Direction start_dir, Coordinate end, std::vector<std::vector<char>> maze)
{
    // up, down, left, right
    std::vector<Direction> ds = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    std::map<std::pair<Coordinate, Direction>, int> dist;

    // modify dijkstras to save all possible shortest paths
    std::map<std::pair<Coordinate, Direction>, std::vector<std::vector<Coordinate>>> paths;
    std::priority_queue<
        std::pair<int, std::pair<Coordinate, Direction>>,
        std::vector<std::pair<int, std::pair<Coordinate, Direction>>>,
        CompareDistance>
        pq;

    for (int y = 0; y < (int) maze.size(); y++)
    {
        for (int x = 0; x < (int) maze[0].size(); x++)
        {
            if (maze[y][x] != '#')
            {
                Coordinate pos = std::make_pair(x, y);
                for (Direction d : ds)
                {
                    dist[{pos, d}] = INT_MAX;
                    paths[{pos, d}] = {};
                }
            }
        }
    }

    dist[{start, start_dir}] = 0;
    paths[{start, start_dir}] = {{start}};

    pq.push({0, {start, start_dir}});

    while (!pq.empty())
    {
        auto [curr_dist, curr_pair] = pq.top();
        auto [curr_pos, curr_dir] = curr_pair;
        pq.pop();

        // cost is curr tile + turn + 1
        // find neighbours - neighbours are: next tile + same tile but diff direction
        std::vector<std::pair<int, std::pair<Coordinate, Direction>>> neighbours;

        // 1. adjacent tile neighbours
        Coordinate next = std::make_pair(curr_pos.first + curr_dir.first, curr_pos.second + curr_dir.second);

        if (maze[next.second][next.first] != '#')
            neighbours.push_back({1, {next, curr_dir}});

        // 2. rotational neighbours

        for (Direction d : getLeftRightTurns(curr_dir))
            neighbours.push_back({1000, {curr_pos, d}});

        for (std::pair<int, std::pair<Coordinate, Direction>> n : neighbours)
        {
            int dist_thru_curr = curr_dist + n.first;
            Coordinate n_pos = n.second.first;
            // Direction n_dir = n.second.second;

            // if found shorter path, change all paths to n.second to the shortest path(s)
            if (dist_thru_curr < dist[n.second])
            {
                dist[n.second] = dist_thru_curr;

                paths[n.second] = {};
                for (std::vector<Coordinate> path : paths[curr_pair])
                {
                    std::vector<Coordinate> new_path = path;
                    new_path.push_back(n_pos);
                    paths[n.second].push_back(new_path);
                }
                pq.push({dist[n.second], n.second});
            }
            // if found path of same length, append it to the list of path(s)
            else if (dist_thru_curr == dist[n.second])
            {
                for (std::vector<Coordinate> path : paths[curr_pair])
                {
                    std::vector<Coordinate> new_path = path;
                    new_path.push_back(n_pos);
                    paths[n.second].push_back(new_path);
                }
            }
        }
    }

    std::vector<std::vector<Coordinate>> paths_to_end;
    
    for (Direction d : ds) {
        if (paths.find({end, d}) != paths.end()) {
            for (const auto& path : paths[{end, d}]) {
                paths_to_end.push_back(path);
            }
        }
    }

    if (paths_to_end.empty())
        return (0);

    std::sort(paths_to_end.begin(), paths_to_end.end(),
        [](std::vector<Coordinate> a, std::vector<Coordinate> b) { return (a.size() < b.size()); });

    int shortest = paths_to_end[0].size();

    std::set<Coordinate>    tiles_used;

    for (auto it = paths_to_end.begin(); it != paths_to_end.end() && (int) (*it).size() == shortest; it++)
        tiles_used.insert((*it).begin(), (*it).end());

    return (tiles_used.size());
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (-1);

    std::string filename(argv[1]);
    std::fstream data(filename, std::fstream::in);
    std::string line;

    if (!data.is_open())
        return (-1);

    std::vector<std::vector<char>> maze;

    while (std::getline(data, line))
        maze.push_back(std::vector<char>(line.begin(), line.end()));

    for (std::vector<char> row : maze)
        std::cout << row << std::endl;

    Coordinate start_pos = findTilePos('S', maze);
    Coordinate end_pos = findTilePos('E', maze);
    Direction start_dir = {1, 0};

    std::cout << "Start pos: " << start_pos << std::endl;
    std::cout << "End pos: " << end_pos << std::endl;

    std::cout << totalTilesUsedInlowestCostPaths(start_pos, start_dir, end_pos, maze) << std::endl;

    return (0);
}