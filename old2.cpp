#undef _GLIBCXX_DEBUG                // disable run-time bound checking, etc
#pragma GCC optimize("Ofast,inline") // Ofast = O3,fast-math,allow-store-data-races,no-protect-parens

#ifndef __POPCNT__ // not march=generic
#endif

#pragma GCC target("bmi,bmi2,lzcnt,popcnt")                      // bit manipulation
#pragma GCC target("movbe")                                      // byte swap
#pragma GCC target("aes,pclmul,rdrnd")                           // encryption
#pragma GCC target("avx,avx2,f16c,fma,sse3,ssse3,sse4.1,sse4.2") // SIMD
#pragma GCC optimize "Ofast,unroll-loops,omit-frame-pointer,inline"
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")
#pragma GCC target("rdrnd", "popcnt", "avx", "bmi2")

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <cstdint>
using namespace std;

int calculate(uint64_t &board, int moves, int &depth);

//#define TABLE_SIZE 1<<20

struct  s_board
{
	uint64_t map_value = -1;
	int      result = -1;
};

struct s_array
{
	int board[3][3];
};

unordered_map<uint64_t, int> memory;

//s_board my_memory[TABLE_SIZE];

// void    insert(uint64_t key, int res)
// {
// 	int idx = key % (TABLE_SIZE);
// 	if (my_memory[idx].map_value == -1)
// 	{
// 		my_memory[idx].map_value = key;
// 		my_memory[idx].result = res;
// 	}
// 	else
// 	{
// 		while (my_memory[idx].map_value != -1)
// 		{
// 			idx++;
// 			if (idx == (TABLE_SIZE))
// 				idx = 0;
// 		}
// 		my_memory[idx].map_value = key;
// 		my_memory[idx].result = res;
// 	}
// }

// int get(uint64_t key)
// {
// 	int idx = key % (TABLE_SIZE);
// 	int limit = 0;
// 	while (my_memory[idx].map_value != key)
// 	{
// 		if (my_memory[idx].map_value == -1)
// 			return (-1);
// 		limit++;
// 		if (limit == 8)
// 			return (-1);
// 		idx++;
// 		if (idx == TABLE_SIZE)
// 			idx = 0;
// 	}
// 	return (idx);
// }

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

bool is_end_game(uint64_t &board)
{
	return ((board & 7) && (board & (7 << 3)) && (board & (7 << 6)) &&
			(board & (7 << 9)) && (board & (7 << 12)) && (board & (7 << 15)) &&
			(board & (7 << 18)) && (board & (7 << 21)) && (board & (7 << 24)));
}

int map_value(uint64_t &board)
{
	int value = (board & 7);

	value = value * 10 + ((board >> 3) & 7);
	value = value * 10 + ((board >> 6) & 7);
	value = value * 10 + ((board >> 9) & 7);
	value = value * 10 + ((board >> 12) & 7);
	value = value * 10 + ((board >> 15) & 7);
	value = value * 10 + ((board >> 18) & 7);
	value = value * 10 + ((board >> 21) & 7);
	value = value * 10 + ((board >> 24) & 7);
	return (value);
}

/*
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
*/

int calculate_cases(uint64_t &copy, uint64_t &board, int &y, int &x, int moves, int &depth, int &cases)
{
	int final_result = 0;

	if (x + 1 < 3 && x - 1 >= 0 && ((board >> (y * 9 + (x + 1) * 3)) & 7) && ((board >> (y * 9 + (x - 1) * 3)) & 7) && ((board >> (y * 9 + (x + 1) * 3)) & 7) + ((board >> (y * 9 + (x - 1) * 3)) & 7) <= 6)
	{
		copy = board;
		cases++;
		copy |= ((((board>>(y * 9 + (x + 1) * 3)) & 7) + ((board>>(y * 9 + (x - 1) * 3)) & 7))<<(y * 9 + x * 3));
		copy &= ~(7 << (y * 9 + (x + 1) * 3));
		copy &= ~(7 << (y * 9 + (x - 1) * 3));
		final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
	}
	if (y + 1 < 3 && y - 1 >= 0 && ((board >> ((y + 1) * 9 + x * 3)) & 7) != 0 && ((board >> ((y - 1) * 9 + x * 3)) & 7) != 0 && ((board >> ((y + 1) * 9 + x * 3)) & 7) + ((board >> ((y - 1) * 9 + x * 3)) & 7) <= 6)
	{
		copy = board;
		cases++;
		copy |= ((((board>>((y + 1) * 9 + x * 3)) & 7) + ((board>>((y - 1) * 9 + x * 3)) & 7))<<(y * 9 + x * 3));
		copy &= ~(7 << ((y + 1) * 9 + x * 3));
		copy &= ~(7 << ((y - 1) * 9 + x * 3));
		final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
	}
	///////////////////////////////////////////////////////////////////////////////////
	if (y + 1 < 3 && x - 1 >= 0 && ((board >> ((y + 1) * 9 + x * 3)) & 7) && ((board >> (y * 9 + (x - 1) * 3)) & 7) && ((board >> ((y + 1) * 9 + x * 3)) & 7) + ((board >> (y * 9 + (x - 1) * 3)) & 7) <= 6)
	{
		copy = board;
		cases++;
		copy |= ((((board>>((y + 1) * 9 + x * 3)) & 7) + ((board>>(y * 9 + (x - 1) * 3)) & 7))<<(y * 9 + x * 3));
		copy &= ~(7 << ((y + 1) * 9 + x * 3));
		copy &= ~(7 << (y * 9 + (x - 1) * 3));
		final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
	}
	if (x + 1 < 3 && y + 1 < 3 && ((board >> ((y + 1) * 9 + x * 3)) & 7) && ((board >> (y * 9 + (x + 1) * 3)) & 7) && ((board >> ((y + 1) * 9 + x * 3)) & 7) + ((board >> (y * 9 + (x + 1) * 3)) & 7) <= 6)
	{
		copy = board;
		cases++;
		copy |= ((((board>>((y + 1) * 9 + x * 3)) & 7) + ((board>>(y * 9 + (x + 1) * 3)) & 7))<<(y * 9 + x * 3));
		copy &= ~(7 << (y * 9 + (x + 1) * 3));
		copy &= ~(7 << ((y + 1) * 9 + x * 3));
		final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
	}
	///////////////////////////////////////////////////////////////////////////////////
	if (x - 1 >= 0 && y - 1 >= 0 && ((board >> ((y - 1) * 9 + x * 3)) & 7) && ((board >> (y * 9 + (x - 1) * 3)) & 7) && ((board >> ((y - 1) * 9 + x * 3)) & 7) + ((board >> (y * 9 + (x - 1) * 3)) & 7) <= 6)
	{
		copy = board;
		cases++;
		copy |= ((((board>>((y - 1) * 9 + x * 3)) & 7) + ((board>>(y * 9 + (x - 1) * 3)) & 7))<<(y * 9 + x * 3));
		copy &= ~(7 << ((y - 1) * 9 + x * 3));
		copy &= ~(7 << (y * 9 + (x - 1) * 3));
		final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
	}
	if (x + 1 < 3 && y - 1 >= 0 && ((board >> ((y - 1) * 9 + x * 3)) & 7) && ((board >> (y * 9 + (x + 1) * 3)) & 7) && ((board >> ((y - 1) * 9 + x * 3)) & 7) + ((board >> (y * 9 + (x + 1) * 3)) & 7) <= 6)
	{
		copy = board;
		cases++;
		copy |= ((((board>>(y * 9 + (x + 1) * 3)) & 7) + ((board>>((y - 1) * 9 + x * 3)) & 7))<<(y * 9 + x * 3));
		copy &= ~(7 << ((y - 1) * 9 + x * 3));
		copy &= ~(7 << (y * 9 + (x + 1) * 3));
		final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
	}
	///////////////////////////////////////////////////////////////////////////////////
	if (x + 1 < 3 && x - 1 >= 0 && y + 1 < 3 && y - 1 >= 0 && ((board >> ((y - 1) * 9 + x * 3)) & 7) && ((board >> ((y + 1) * 9 + x * 3)) & 7) && ((board >> (y * 9 + (x + 1) * 3)) & 7) && ((board >> (y * 9 + (x - 1) * 3)) & 7) && ((board >> ((y - 1) * 9 + x * 3)) & 7) + ((board >> (y * 9 + (x + 1) * 3)) & 7) + ((board >> ((y + 1) * 9 + x * 3)) & 7) + ((board >> (y * 9 + (x - 1) * 3)) & 7) <= 6)
	{
		copy = board;
		cases++;
		copy |= ((((board>>(y * 9 + (x + 1) * 3)) & 7) + ((board>>(y * 9 + (x - 1) * 3)) & 7) + ((board>>((y - 1) * 9 + x * 3)) & 7) + ((board>>((y + 1) * 9 + x * 3)) & 7))<<(y * 9 + x * 3));
		copy &= ~(7 << ((y - 1) * 9 + x * 3));
		copy &= ~(7 << ((y + 1) * 9 + x * 3));
		copy &= ~(7 << (y * 9 + (x + 1) * 3));
		copy &= ~(7 << (y * 9 + (x - 1) * 3));
		final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (x - 1 >= 0 && y + 1 < 3 && y - 1 >= 0 && ((board >> ((y - 1) * 9 + x * 3)) & 7) && ((board >> ((y + 1) * 9 + x * 3)) & 7) && ((board >> (y * 9 + (x - 1) * 3)) & 7) && ((board >> ((y - 1) * 9 + x * 3)) & 7) + ((board >> ((y + 1) * 9 + x * 3)) & 7) + ((board >> (y * 9 + (x - 1) * 3)) & 7) <= 6)
	{
		copy = board;
		cases++;
		copy |= ((((board>>((y + 1) * 9 + x * 3)) & 7) + ((board>>(y * 9 + (x - 1) * 3)) & 7) + ((board>>((y - 1) * 9 + x * 3)) & 7))<<(y * 9 + x * 3));
		copy &= ~(7 << ((y - 1) * 9 + x * 3));
		copy &= ~(7 << ((y + 1) * 9 + x * 3));
		copy &= ~(7 << (y * 9 + (x - 1) * 3));
		final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
	}
	if (x + 1 < 3 && y + 1 < 3 && y - 1 >= 0 && ((board >> ((y - 1) * 9 + x * 3)) & 7) && ((board >> ((y + 1) * 9 + x * 3)) & 7) && ((board >> (y * 9 + (x + 1) * 3)) & 7) && ((board >> ((y - 1) * 9 + x * 3)) & 7) + ((board >> (y * 9 + (x + 1) * 3)) & 7) + ((board >> ((y + 1) * 9 + x * 3)) & 7) <= 6)
	{
		copy = board;
		cases++;
		copy |= ((((board>>((y - 1) * 9 + x * 3)) & 7) + ((board>>((y + 1) * 9 + x * 3)) & 7) + ((board>>(y * 9 + (x + 1) * 3)) & 7))<<(y * 9 + x * 3));
		copy &= ~(7 << ((y - 1) * 9 + x * 3));
		copy &= ~(7 << ((y + 1) * 9 + x * 3));
		copy &= ~(7 << (y * 9 + (x + 1) * 3));
		final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
	}
	if (x + 1 < 3 && x - 1 >= 0 && y - 1 >= 0 && ((board >> ((y - 1) * 9 + x * 3)) & 7) && ((board >> (y * 9 + (x + 1) * 3)) & 7) && ((board >> (y * 9 + (x - 1) * 3)) & 7) && ((board >> ((y - 1) * 9 + x * 3)) & 7) + ((board >> (y * 9 + (x + 1) * 3)) & 7) + ((board >> (y * 9 + (x - 1) * 3)) & 7) <= 6)
	{
		copy = board;
		cases++;
		copy |= ((((board>>(y * 9 + (x + 1) * 3)) & 7) + ((board>>(y * 9 + (x - 1) * 3)) & 7) + ((board>>((y - 1) * 9 + x * 3)) & 7))<<(y * 9 + x * 3));
		copy &= ~(7 << ((y - 1) * 9 + x * 3));
		copy &= ~(7 << (y * 9 + (x + 1) * 3));
		copy &= ~(7 << (y * 9 + (x - 1) * 3));
		final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
	}
	if (x + 1 < 3 && x - 1 >= 0 && y + 1 < 3 && ((board >> ((y + 1) * 9 + x * 3)) & 7) && ((board >> (y * 9 + (x + 1) * 3)) & 7) && ((board >> (y * 9 + (x - 1) * 3)) & 7) && ((board >> (y * 9 + (x + 1) * 3)) & 7) + ((board >> ((y + 1) * 9 + x * 3)) & 7) + ((board >> (y * 9 + (x - 1) * 3)) & 7) <= 6)
	{
		copy = board;
		cases++;
		copy |= ((((board>>(y * 9 + (x + 1) * 3)) & 7) + ((board>>(y * 9 + (x - 1) * 3)) & 7) + ((board>>((y + 1) * 9 + x * 3)) & 7))<<(y * 9 + x * 3));
		copy &= ~(7 << ((y + 1) * 9 + x * 3));
		copy &= ~(7 << (y * 9 + (x + 1) * 3));
		copy &= ~(7 << (y * 9 + (x - 1) * 3));
		final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
	}
	return (final_result);
}

int calculate(uint64_t &board, int moves, int &depth)
{
	if (moves == depth || is_end_game(board))
	{
		return (map_value(board));
	}
	uint64_t hashed = board<<8 | moves;
	if (memory.find(hashed) != memory.end())
	{
		return (memory[hashed]);
	}
	int final_result = 0;
	uint64_t copy;
	int cases;

	moves++;
        #pragma GCC unroll 3
	for (int i = 0; i < 3; i++)
	{
                #pragma GCC unroll 3
		for (int j = 0; j < 3; j++)
		{
			if (!((board >> (i * 9 + j * 3)) & 7))
			{
                cases = 0;
				final_result = (final_result + calculate_cases(copy, board, i, j, moves, depth, cases)) % (1 << 30);
				if (cases == 0)
				{
					copy = board;
					copy |= (1 << (i * 9 + j * 3));
                    final_result = (final_result + calculate(copy, moves, depth)) % (1 << 30);
				}
			}
		}
	}
	memory[hashed] = final_result;
	return (final_result);
}

int main()
{
	int depth;
	cin >> depth;
	cin.ignore();

	uint64_t board = 0;
	int cases = 0;
	int maves = 0;
	int value;
	cin >> value; cin.ignore(); board |= (value);
	cin >> value; cin.ignore(); board |= (value<<3);
	cin >> value; cin.ignore(); board |= (value<<6);
	cin >> value; cin.ignore(); board |= (value<<9);
	cin >> value; cin.ignore(); board |= (value<<12);
	cin >> value; cin.ignore(); board |= (value<<15);
	cin >> value; cin.ignore(); board |= (value<<18);
	cin >> value; cin.ignore(); board |= (value<<21);
	cin >> value; cin.ignore(); board |= (value<<24);

	// Write an action using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	cout << calculate(board, 0, depth) << endl;
}
