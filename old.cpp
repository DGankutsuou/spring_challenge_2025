#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <cstdint>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int    calculate(int map[3][3], int moves, int depth);

unordered_map <uint64_t, int> memory;

int my_memory[1<<20];

void    insert(uint64_t key, int value)
{
    my_memory[key] = value;
}

void    print_map(int map[3][3])
{
    cerr << "***\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
           cerr << map[i][j];
        }
       cerr << endl;
    }
    cerr << "***\n";
}

bool    is_end_game(int map[3][3])
{
    int value = 1;

    value = value * map[0][0] * map[0][1] * map[0][2]
    * map[1][0] * map[1][1] * map[1][2]
    * map[2][0] * map[2][1] * map[2][2];
    return (value);
}

int map_value(int map[3][3])
{
    int value = 0;

    value = value * 10 + map[0][0];
    value = value * 10 + map[0][1];
    value = value * 10 + map[0][2];
    value = value * 10 + map[1][0];
    value = value * 10 + map[1][1];
    value = value * 10 + map[1][2];
    value = value * 10 + map[2][0];
    value = value * 10 + map[2][1];
    value = value * 10 + map[2][2];
    return (value);
}

uint64_t hasher(int value, int key)
{
    uint64_t h;
    uint64_t k;

    h = static_cast<uint64_t>(value);
    k = static_cast<uint64_t>(key);
    h = h * 0xDEADBEEFDEADBEEFULL + k * 0x123456789ABCDEFULL;
    h ^= (h >> 33);
    h *= 0xff51afd7ed558ccdULL;
    h ^= (h >> 33);
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= (h >> 33);
    return (h);
}

void copy_map(int copy[3][3], int map[3][3])
{
    copy[0][0] = map[0][0];
    copy[0][1] = map[0][1];
    copy[0][2] = map[0][2];
    copy[1][0] = map[1][0];
    copy[1][1] = map[1][1];
    copy[1][2] = map[1][2];
    copy[2][0] = map[2][0];
    copy[2][1] = map[2][1];
    copy[2][2] = map[2][2];
}

int    calculate_cases(int copy[3][3], int map[3][3], int y, int x, int moves, int depth, int &cases)
{
    int final_result = 0;

    if (x + 1 < 3 && x - 1 >= 0 && map[y][x + 1] && map[y][x - 1] && map[y][x + 1] + map[y][x - 1] <= 6)
    {
        copy_map(copy, map);
        cases++;
        copy[y][x] = copy[y][x + 1] + copy[y][x - 1];
        copy[y][x + 1] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (int)pow(2, 30);
    }
    if (y + 1 < 3 && y - 1 >= 0 && map[y + 1][x] != 0 && map[y - 1][x] != 0 && map[y + 1][x] + map[y - 1][x] <= 6)
    {
        copy_map(copy, map);
        cases++;
        copy[y][x] = copy[y + 1][x] + copy[y - 1][x];
        copy[y + 1][x] = 0;
        copy[y - 1][x] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (int)pow(2, 30);
    }
    ///////////////////////////////////////////////////////////////////////////////////
    if (y + 1 < 3 && x - 1 >= 0 && map[y + 1][x] && map[y][x - 1] && map[y + 1][x] + map[y][x - 1] <= 6)
    {
        copy_map(copy, map);
        cases++;
        copy[y][x] = copy[y + 1][x] + copy[y][x - 1];
        copy[y + 1][x] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (int)pow(2, 30);
    }
    if (x + 1 < 3 && y + 1 < 3 && map[y + 1][x] && map[y][x + 1] && map[y + 1][x] + map[y][x + 1] <= 6)
    {
        copy_map(copy, map);
        cases++;
        copy[y][x] = copy[y + 1][x] + copy[y][x + 1];
        copy[y + 1][x] = 0;
        copy[y][x + 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (int)pow(2, 30);
    }
    ///////////////////////////////////////////////////////////////////////////////////
    if (x - 1 >= 0 && y - 1 >= 0 && map[y - 1][x] && map[y][x - 1] && map[y - 1][x] + map[y][x - 1] <= 6)
    {
        copy_map(copy, map);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y][x - 1];
        copy[y - 1][x] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (int)pow(2, 30);
    }
    if (x + 1 < 3 && y - 1 >= 0 && map[y - 1][x] && map[y][x + 1] && map[y - 1][x] + map[y][x + 1] <= 6)
    {
        copy_map(copy, map);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y][x + 1];
        copy[y - 1][x] = 0;
        copy[y][x + 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (int)pow(2, 30);
    }
    ///////////////////////////////////////////////////////////////////////////////////
    if (x + 1 < 3 && x - 1 >= 0 && y + 1 < 3 && y - 1 >= 0 && map[y - 1][x] && map[y + 1][x] && map[y][x + 1] && map[y][x - 1] && map[y - 1][x] + map[y][x + 1] + map[y + 1][x] + map[y][x - 1] <= 6)
    {
        copy_map(copy, map);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y][x + 1] + copy[y + 1][x] + copy[y][x - 1];
        copy[y - 1][x] = 0;
        copy[y][x + 1] = 0;
        copy[y + 1][x] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (int)pow(2, 30);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (x - 1 >= 0 && y + 1 < 3 && y - 1 >= 0 && map[y - 1][x] && map[y + 1][x] && map[y][x - 1] && map[y - 1][x] + map[y + 1][x] + map[y][x - 1] <= 6)
    {
        copy_map(copy, map);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y + 1][x] + copy[y][x - 1];
        copy[y - 1][x] = 0;
        copy[y + 1][x] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (int)pow(2, 30);
    }
    if (x + 1 < 3 && y + 1 < 3 && y - 1 >= 0 && map[y - 1][x] && map[y + 1][x] && map[y][x + 1] && map[y - 1][x] + map[y][x + 1] + map[y + 1][x] <= 6)
    {
        copy_map(copy, map);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y][x + 1] + copy[y + 1][x];
        copy[y - 1][x] = 0;
        copy[y][x + 1] = 0;
        copy[y + 1][x] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (int)pow(2, 30);
    }
    if (x + 1 < 3 && x - 1 >= 0 && y - 1 >= 0 && map[y - 1][x] && map[y][x + 1] && map[y][x - 1] && map[y - 1][x] + map[y][x + 1] + map[y][x - 1] <= 6)
    {
        copy_map(copy, map);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y][x + 1] + copy[y][x - 1];
        copy[y - 1][x] = 0;
        copy[y][x + 1] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (int)pow(2, 30);
    }
    if (x + 1 < 3 && x - 1 >= 0 && y + 1 < 3 && map[y + 1][x] && map[y][x + 1] && map[y][x - 1] && map[y][x + 1] + map[y + 1][x] + map[y][x - 1] <= 6)
    {
        copy_map(copy, map);
        cases++;
        copy[y][x] = copy[y][x + 1] + copy[y + 1][x] + copy[y][x - 1];
        copy[y][x + 1] = 0;
        copy[y + 1][x] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (int)pow(2, 30);
    }
    return (final_result);
}

int    calculate(int map[3][3], int moves, int depth)
{
   // print_map(map);
    unsigned long   hashed = hasher(map_value(map), moves + 1);
    if (memory.find(hashed) != memory.end())
    {
        return (memory[hashed]);
    }
    int final_result = 0;
    int copy[3][3];
    int cases = 0;

    if (is_end_game(map))
        return (map_value(map));
    if (moves == depth)
    {
        return (map_value(map) % (int)pow(2, 30));
    }
    moves++;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (map[i][j] == 0)
            {
                //copy_map(copy, map);
                final_result = (final_result + calculate_cases(copy, map, i, j, moves, depth, cases)) % (int)pow(2, 30);
                if (cases == 0)
                {
                    map[i][j] = 1;
                    final_result = (final_result + calculate(map, moves, depth)) % (int)pow(2, 30);
                    map[i][j] = 0;
                }
                cases = 0;
            }
        }
    }
    memory[hashed] = final_result;
    return (final_result);
}

int main()
{
    int depth;
    cin >> depth; cin.ignore();

    int map[3][3];

    int result = 0;
    int cases = 0;
    int maves = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int value;
            cin >> value; cin.ignore();
            map[i][j] = value;
        }
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << calculate(map, 0, depth) << endl;
}
