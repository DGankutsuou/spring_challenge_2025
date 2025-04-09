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

#define TABLE_SIZE 1<<20

int    calculate(int board[3][3], int moves, int depth);

unordered_map <uint64_t, int> memory;

/*
struct  s_board
{
    uint64_t map_value = -1;
    int      result = -1;
};

s_board my_memory[TABLE_SIZE];

void    insert(uint64_t key, int res)
{
    int idx = key % (TABLE_SIZE);
    if (my_memory[idx].map_value == -1)
    {
        my_memory[idx].map_value = key;
        my_memory[idx].result = res;
    }
    else
    {
        while (my_memory[idx].map_value != -1)
        {
            idx++;
            if (idx == (TABLE_SIZE))
                idx = 0;
        }
        my_memory[idx].map_value = key;
        my_memory[idx].result = res;
    }
}

int get(uint64_t key)
{
    int idx = key % (TABLE_SIZE);
    int limit = 0;
    while (my_memory[idx].map_value != key)
    {
        if (my_memory[idx].map_value == -1)
            return (-1);
        limit++;
        if (limit == 8)
            return (-1);
        idx++;
        if (idx == TABLE_SIZE)
            idx = 0;
    }
    return (idx);
}
*/

/*
void    print_map(int board[3][3])
{
    cerr << "***\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cerr << board[i][j];
        }
        cerr << endl;
    }
    cerr << "***\n";
}
*/

bool    is_end_game(int board[3][3])
{
    int value = 1;

    value = value * board[0][0] * board[0][1] * board[0][2]
    * board[1][0] * board[1][1] * board[1][2]
    * board[2][0] * board[2][1] * board[2][2];
    return (value);
}

int map_value(int board)
{
    int value = (board & 7);

    value = value * 10 + ((board & (7 << (1 * 3))) >> (1 * 3));
    value = value * 10 + ((board & (7 << (2 * 3))) >> (2 * 3));
    value = value * 10 + ((board & (7 << (1 * 9))) >> (1 * 9));
    value = value * 10 + ((board & (7 << (1 * 9 + 1 * 3))) >> (1 * 9 + 1 * 3));
    value = value * 10 + ((board & (7 << (1 * 9 + 2 * 3))) >> (1 * 9 + 2 * 3));
    value = value * 10 + ((board & (7 << (2 * 9))) >> (2 * 9));
    value = value * 10 + ((board & (7 << (2 * 9 + 1 * 3))) >> (2 * 9 + 1 * 3));
    value = value * 10 + ((board & (7 << (2 * 9 + 2 * 3))) >> (2 * 9 + 2 * 3));
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

void copy_map(int copy[3][3], int board[3][3])
{
    copy[0][0] = board[0][0];
    copy[0][1] = board[0][1];
    copy[0][2] = board[0][2];
    copy[1][0] = board[1][0];
    copy[1][1] = board[1][1];
    copy[1][2] = board[1][2];
    copy[2][0] = board[2][0];
    copy[2][1] = board[2][1];
    copy[2][2] = board[2][2];
}

int    calculate_cases(int copy[3][3], int board[3][3], int y, int x, int moves, int depth, int &cases)
{
    int final_result = 0;

    if (x + 1 < 3 && x - 1 >= 0 && board[y][x + 1] && board[y][x - 1] && board[y][x + 1] + board[y][x - 1] <= 6)
    {
        copy_map(copy, board);
        cases++;
        copy[y][x] = copy[y][x + 1] + copy[y][x - 1];
        copy[y][x + 1] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (1<<30);
    }
    if (y + 1 < 3 && y - 1 >= 0 && board[y + 1][x] != 0 && board[y - 1][x] != 0 && board[y + 1][x] + board[y - 1][x] <= 6)
    {
        copy_map(copy, board);
        cases++;
        copy[y][x] = copy[y + 1][x] + copy[y - 1][x];
        copy[y + 1][x] = 0;
        copy[y - 1][x] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (1<<30);
    }
    ///////////////////////////////////////////////////////////////////////////////////
    if (y + 1 < 3 && x - 1 >= 0 && board[y + 1][x] && board[y][x - 1] && board[y + 1][x] + board[y][x - 1] <= 6)
    {
        copy_map(copy, board);
        cases++;
        copy[y][x] = copy[y + 1][x] + copy[y][x - 1];
        copy[y + 1][x] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (1<<30);
    }
    if (x + 1 < 3 && y + 1 < 3 && board[y + 1][x] && board[y][x + 1] && board[y + 1][x] + board[y][x + 1] <= 6)
    {
        copy_map(copy, board);
        cases++;
        copy[y][x] = copy[y + 1][x] + copy[y][x + 1];
        copy[y + 1][x] = 0;
        copy[y][x + 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (1<<30);
    }
    ///////////////////////////////////////////////////////////////////////////////////
    if (x - 1 >= 0 && y - 1 >= 0 && board[y - 1][x] && board[y][x - 1] && board[y - 1][x] + board[y][x - 1] <= 6)
    {
        copy_map(copy, board);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y][x - 1];
        copy[y - 1][x] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (1<<30);
    }
    if (x + 1 < 3 && y - 1 >= 0 && board[y - 1][x] && board[y][x + 1] && board[y - 1][x] + board[y][x + 1] <= 6)
    {
        copy_map(copy, board);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y][x + 1];
        copy[y - 1][x] = 0;
        copy[y][x + 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (1<<30);
    }
    ///////////////////////////////////////////////////////////////////////////////////
    if (x + 1 < 3 && x - 1 >= 0 && y + 1 < 3 && y - 1 >= 0 && board[y - 1][x] && board[y + 1][x] && board[y][x + 1] && board[y][x - 1] && board[y - 1][x] + board[y][x + 1] + board[y + 1][x] + board[y][x - 1] <= 6)
    {
        copy_map(copy, board);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y][x + 1] + copy[y + 1][x] + copy[y][x - 1];
        copy[y - 1][x] = 0;
        copy[y][x + 1] = 0;
        copy[y + 1][x] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (1<<30);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (x - 1 >= 0 && y + 1 < 3 && y - 1 >= 0 && board[y - 1][x] && board[y + 1][x] && board[y][x - 1] && board[y - 1][x] + board[y + 1][x] + board[y][x - 1] <= 6)
    {
        copy_map(copy, board);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y + 1][x] + copy[y][x - 1];
        copy[y - 1][x] = 0;
        copy[y + 1][x] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (1<<30);
    }
    if (x + 1 < 3 && y + 1 < 3 && y - 1 >= 0 && board[y - 1][x] && board[y + 1][x] && board[y][x + 1] && board[y - 1][x] + board[y][x + 1] + board[y + 1][x] <= 6)
    {
        copy_map(copy, board);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y][x + 1] + copy[y + 1][x];
        copy[y - 1][x] = 0;
        copy[y][x + 1] = 0;
        copy[y + 1][x] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (1<<30);
    }
    if (x + 1 < 3 && x - 1 >= 0 && y - 1 >= 0 && board[y - 1][x] && board[y][x + 1] && board[y][x - 1] && board[y - 1][x] + board[y][x + 1] + board[y][x - 1] <= 6)
    {
        copy_map(copy, board);
        cases++;
        copy[y][x] = copy[y - 1][x] + copy[y][x + 1] + copy[y][x - 1];
        copy[y - 1][x] = 0;
        copy[y][x + 1] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (1<<30);
    }
    if (x + 1 < 3 && x - 1 >= 0 && y + 1 < 3 && board[y + 1][x] && board[y][x + 1] && board[y][x - 1] && board[y][x + 1] + board[y + 1][x] + board[y][x - 1] <= 6)
    {
        copy_map(copy, board);
        cases++;
        copy[y][x] = copy[y][x + 1] + copy[y + 1][x] + copy[y][x - 1];
        copy[y][x + 1] = 0;
        copy[y + 1][x] = 0;
        copy[y][x - 1] = 0;
        final_result = (final_result + calculate(copy, moves, depth)) % (1<<30);
    }
    return (final_result);
}

int    calculate(int board, int moves, int depth)
{
   // print_map(board);
    unsigned long   hashed = hasher(map_value(board), moves + 1);
    if (memory.find(hashed) != memory.end())
    {
        return (memory[hashed]);
    }
    // int idx = get(hashed);
    // if (idx != -1)
    // {
    //     cerr << "memorized_state" << endl;
    //     return (my_memory[idx].result);
    // }
    int final_result = 0;
    int copy[3][3];
    int cases = 0;

    if (is_end_game(board))
        return (map_value(board));
    if (moves == depth)
    {
        return (map_value(board) % (1<<30));
    }
    moves++;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == 0)
            {
                //copy_map(copy, board);
                final_result = (final_result + calculate_cases(copy, board, i, j, moves, depth, cases)) % (1<<30);
                if (cases == 0)
                {
                    board[i][j] = 1;
                    final_result = (final_result + calculate(board, moves, depth)) % (1<<30);
                    board[i][j] = 0;
                }
                cases = 0;
            }
        }
    }
    memory[hashed] = final_result;
    // insert(hashed, final_result);
    return (final_result);
}

int main()
{
    int depth;
    cin >> depth; cin.ignore();

    int board = 0;

    int result = 0;
    int cases = 0;
    int maves = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int value;
            cin >> value; cin.ignore();
            board |= (value<<(j * 3 + i * 9));
            cerr << (board>>(j * 3 + i * 9));
        }
        cerr << endl;
    }
   // cerr << ((board & (7 << (1 * 3 + 1 * 9))) >> (1 * 3 + 1 * 9)) << endl;

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << calculate(board, 0, depth) << endl;
}
