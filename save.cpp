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

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <unordered_map>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/


 struct s_array
 {
	 uint32_t matrix[3][3];
};

s_array calculate(uint64_t &board, int moves, int &depth, short &rotates);

unordered_map<uint64_t, s_array> memory;

/*
#define TABLE_SIZE 1<<20

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


void    print_map(uint32_t board[3][3])
{
        cerr << "***\n";
        for (char i = 0; i < 3; i++)
        {
                for (char j = 0; j < 3; j++)
                {
                        cerr << board[i][j] << " ";
                }
                cerr << endl;
        }
        cerr << "***\n";
}


bool is_end_game(uint64_t &board) {
  return ((board & 7) && (board & (7 << 3)) && (board & (7 << 6)) &&
          (board & (7 << 9)) && (board & (7 << 12)) && (board & (7 << 15)) &&
          (board & (7 << 18)) && (board & (7 << 21)) && (board & (7 << 24)));
}

int map_value(uint64_t &board) {
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

void past_2_maps(uint32_t map1[3][3], uint32_t map2[3][3])
{
        map1[0][0] += map2[0][0];
    map1[0][1] += map2[0][1];
    map1[0][2] += map2[0][2];
    map1[1][0] += map2[1][0];
    map1[1][1] += map2[1][1];
    map1[1][2] += map2[1][2];
    map1[2][0] += map2[2][0];
    map1[2][1] += map2[2][1];
    map1[2][2] += map2[2][2];
}

void rm(uint32_t map[3][3])
{
        int save = map[2][2];
        map[2][2] = map[2][1];
        map[2][1] = map[2][0];
        map[2][0] = map[1][2];
        map[1][2] = map[1][1];
        map[1][1] = map[1][0];
        map[1][0] = map[0][2];
        map[0][2] = map[0][1];
        map[0][1] = map[0][0];
        map[0][0] = save;
}

void rrm(uint32_t map[3][3])
{
        int save = map[0][0];
        map[0][0] = map[0][1];
        map[0][1] = map[0][2];
    map[0][2] = map[1][0];
    map[1][0] = map[1][1];
    map[1][1] = map[1][2];
    map[1][2] = map[2][0];
    map[2][0] = map[2][1];
    map[2][1] = map[2][2];
    map[2][2] = save;
}

s_array calculate_cases(s_array &res, uint64_t &copy, uint64_t &board, char &y, char &x, int moves,
                    int &depth, int &cases, short &rotates)
{

  if (x + 1 < 3 && x - 1 >= 0 && ((board >> (y * 9 + (x + 1) * 3)) & 7) &&
      ((board >> (y * 9 + (x - 1) * 3)) & 7) &&
      ((board >> (y * 9 + (x + 1) * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7) <=
          6) {
    copy = board;
    cases++;
    copy |= ((((board >> (y * 9 + (x + 1) * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7))
             << (y * 9 + x * 3));
    copy &= ~(7 << (y * 9 + (x + 1) * 3));
    copy &= ~(7 << (y * 9 + (x - 1) * 3));
    past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
  }
  if (y + 1 < 3 && y - 1 >= 0 && ((board >> ((y + 1) * 9 + x * 3)) & 7) != 0 &&
      ((board >> ((y - 1) * 9 + x * 3)) & 7) != 0 &&
      ((board >> ((y + 1) * 9 + x * 3)) & 7) +
              ((board >> ((y - 1) * 9 + x * 3)) & 7) <=
          6) {
    copy = board;
    cases++;
    copy |= ((((board >> ((y + 1) * 9 + x * 3)) & 7) +
              ((board >> ((y - 1) * 9 + x * 3)) & 7))
             << (y * 9 + x * 3));
    copy &= ~(7 << ((y + 1) * 9 + x * 3));
    copy &= ~(7 << ((y - 1) * 9 + x * 3));
    past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
  }
  ///////////////////////////////////////////////////////////////////////////////////
  if (y + 1 < 3 && x - 1 >= 0 && ((board >> ((y + 1) * 9 + x * 3)) & 7) &&
      ((board >> (y * 9 + (x - 1) * 3)) & 7) &&
      ((board >> ((y + 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7) <=
          6) {
    copy = board;
    cases++;
    copy |= ((((board >> ((y + 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7))
             << (y * 9 + x * 3));
    copy &= ~(7 << ((y + 1) * 9 + x * 3));
    copy &= ~(7 << (y * 9 + (x - 1) * 3));
    past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
  }
  if (x + 1 < 3 && y + 1 < 3 && ((board >> ((y + 1) * 9 + x * 3)) & 7) &&
      ((board >> (y * 9 + (x + 1) * 3)) & 7) &&
      ((board >> ((y + 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x + 1) * 3)) & 7) <=
          6) {
    copy = board;
    cases++;
    copy |= ((((board >> ((y + 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x + 1) * 3)) & 7))
             << (y * 9 + x * 3));
    copy &= ~(7 << (y * 9 + (x + 1) * 3));
    copy &= ~(7 << ((y + 1) * 9 + x * 3));
    past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
  }
  ///////////////////////////////////////////////////////////////////////////////////
  if (x - 1 >= 0 && y - 1 >= 0 && ((board >> ((y - 1) * 9 + x * 3)) & 7) &&
      ((board >> (y * 9 + (x - 1) * 3)) & 7) &&
      ((board >> ((y - 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7) <=
          6) {
    copy = board;
    cases++;
    copy |= ((((board >> ((y - 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7))
             << (y * 9 + x * 3));
    copy &= ~(7 << ((y - 1) * 9 + x * 3));
    copy &= ~(7 << (y * 9 + (x - 1) * 3));
    past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
  }
  if (x + 1 < 3 && y - 1 >= 0 && ((board >> ((y - 1) * 9 + x * 3)) & 7) &&
      ((board >> (y * 9 + (x + 1) * 3)) & 7) &&
      ((board >> ((y - 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x + 1) * 3)) & 7) <=
          6) {
    copy = board;
    cases++;
    copy |= ((((board >> (y * 9 + (x + 1) * 3)) & 7) +
              ((board >> ((y - 1) * 9 + x * 3)) & 7))
             << (y * 9 + x * 3));
    copy &= ~(7 << ((y - 1) * 9 + x * 3));
    copy &= ~(7 << (y * 9 + (x + 1) * 3));
    past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
  }
  ///////////////////////////////////////////////////////////////////////////////////
  if (x + 1 < 3 && x - 1 >= 0 && y + 1 < 3 && y - 1 >= 0 &&
      ((board >> ((y - 1) * 9 + x * 3)) & 7) &&
      ((board >> ((y + 1) * 9 + x * 3)) & 7) &&
      ((board >> (y * 9 + (x + 1) * 3)) & 7) &&
      ((board >> (y * 9 + (x - 1) * 3)) & 7) &&
      ((board >> ((y - 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x + 1) * 3)) & 7) +
              ((board >> ((y + 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7) <=
          6) {
    copy = board;
    cases++;
    copy |= ((((board >> (y * 9 + (x + 1) * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7) +
              ((board >> ((y - 1) * 9 + x * 3)) & 7) +
              ((board >> ((y + 1) * 9 + x * 3)) & 7))
             << (y * 9 + x * 3));
    copy &= ~(7 << ((y - 1) * 9 + x * 3));
    copy &= ~(7 << ((y + 1) * 9 + x * 3));
    copy &= ~(7 << (y * 9 + (x + 1) * 3));
    copy &= ~(7 << (y * 9 + (x - 1) * 3));
    past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (x - 1 >= 0 && y + 1 < 3 && y - 1 >= 0 &&
      ((board >> ((y - 1) * 9 + x * 3)) & 7) &&
      ((board >> ((y + 1) * 9 + x * 3)) & 7) &&
      ((board >> (y * 9 + (x - 1) * 3)) & 7) &&
      ((board >> ((y - 1) * 9 + x * 3)) & 7) +
              ((board >> ((y + 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7) <=
          6) {
    copy = board;
    cases++;
    copy |= ((((board >> ((y + 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7) +
              ((board >> ((y - 1) * 9 + x * 3)) & 7))
             << (y * 9 + x * 3));
    copy &= ~(7 << ((y - 1) * 9 + x * 3));
    copy &= ~(7 << ((y + 1) * 9 + x * 3));
    copy &= ~(7 << (y * 9 + (x - 1) * 3));
    past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
  }
  if (x + 1 < 3 && y + 1 < 3 && y - 1 >= 0 &&
      ((board >> ((y - 1) * 9 + x * 3)) & 7) &&
      ((board >> ((y + 1) * 9 + x * 3)) & 7) &&
      ((board >> (y * 9 + (x + 1) * 3)) & 7) &&
      ((board >> ((y - 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x + 1) * 3)) & 7) +
              ((board >> ((y + 1) * 9 + x * 3)) & 7) <=
          6) {
    copy = board;
    cases++;
    copy |= ((((board >> ((y - 1) * 9 + x * 3)) & 7) +
              ((board >> ((y + 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x + 1) * 3)) & 7))
             << (y * 9 + x * 3));
    copy &= ~(7 << ((y - 1) * 9 + x * 3));
    copy &= ~(7 << ((y + 1) * 9 + x * 3));
    copy &= ~(7 << (y * 9 + (x + 1) * 3));
    past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
  }
  if (x + 1 < 3 && x - 1 >= 0 && y - 1 >= 0 &&
      ((board >> ((y - 1) * 9 + x * 3)) & 7) &&
      ((board >> (y * 9 + (x + 1) * 3)) & 7) &&
      ((board >> (y * 9 + (x - 1) * 3)) & 7) &&
      ((board >> ((y - 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x + 1) * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7) <=
          6) {
    copy = board;
    cases++;
    copy |= ((((board >> (y * 9 + (x + 1) * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7) +
              ((board >> ((y - 1) * 9 + x * 3)) & 7))
             << (y * 9 + x * 3));
    copy &= ~(7 << ((y - 1) * 9 + x * 3));
    copy &= ~(7 << (y * 9 + (x + 1) * 3));
    copy &= ~(7 << (y * 9 + (x - 1) * 3));
    past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
  }
  if (x + 1 < 3 && x - 1 >= 0 && y + 1 < 3 &&
      ((board >> ((y + 1) * 9 + x * 3)) & 7) &&
      ((board >> (y * 9 + (x + 1) * 3)) & 7) &&
      ((board >> (y * 9 + (x - 1) * 3)) & 7) &&
      ((board >> (y * 9 + (x + 1) * 3)) & 7) +
              ((board >> ((y + 1) * 9 + x * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7) <=
          6) {
    copy = board;
    cases++;
    copy |= ((((board >> (y * 9 + (x + 1) * 3)) & 7) +
              ((board >> (y * 9 + (x - 1) * 3)) & 7) +
              ((board >> ((y + 1) * 9 + x * 3)) & 7))
             << (y * 9 + x * 3));
    copy &= ~(7 << ((y + 1) * 9 + x * 3));
    copy &= ~(7 << (y * 9 + (x + 1) * 3));
    copy &= ~(7 << (y * 9 + (x - 1) * 3));
    past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
  }
  return (res);
}

s_array calculate(uint64_t &board, int moves, int &depth, short &rotates)
{
        s_array res;
        if (moves == depth || is_end_game(board))
        {
          res.matrix[0][0] = (board & 7);
          res.matrix[0][1] = ((board >> 3) & 7);
          res.matrix[0][2] = ((board >> 6) & 7);
          res.matrix[1][0] = ((board >> 9) & 7);
          res.matrix[1][1] = ((board >> 12) & 7);
          res.matrix[1][2] = ((board >> 15) & 7);
          res.matrix[2][0] = ((board >> 18) & 7);
          res.matrix[2][1] = ((board >> 21) & 7);
          res.matrix[2][2] = ((board >> 24) & 7);
          return (res);
        }
        uint64_t hashed = board << 8 | moves;
        uint64_t copy = board;
       // print_map(res.matrix);
        #pragma GCC unroll 9
        for (rotates = 0; rotates < 9; rotates++)
        {
                if (memory.find(hashed) != memory.end())
                {
                        res = memory[hashed];
                        //print_map(res.matrix);
                        short r;
                        #pragma GCC unroll 9
                        for (r = 0; r < rotates; r++)
                        {
                                cerr << "hiiiiiiiiiiiiiiiiiiiiiiiiiii\n";
                                rrm(res.matrix);
                               // print_map(res.matrix);
                        }
                        if (r > 0)
                        {
                                copy = 0;
                                copy |= (res.matrix[0][0]);
                                copy |= (res.matrix[0][1] << 3);
                                copy |= (res.matrix[0][2] << 6);
                                copy |= (res.matrix[1][0] << 9);
                                copy |= (res.matrix[1][1] << 12);
                                copy |= (res.matrix[1][2] << 15);
                                copy |= (res.matrix[2][0] << 18);
                                copy |= (res.matrix[2][1] << 21);
                                copy |= (res.matrix[2][2] << 24);
                                hashed = copy << 8 | moves;
                                memory[hashed] = res;
                        }
                        return (res);
                }
                res.matrix[0][0] = (copy & 7);
                res.matrix[0][1] = ((copy >> 3) & 7);
                res.matrix[0][2] = ((copy >> 6) & 7);
                res.matrix[1][0] = ((copy >> 9) & 7);
                res.matrix[1][1] = ((copy >> 12) & 7);
                res.matrix[1][2] = ((copy >> 15) & 7);
                res.matrix[2][0] = ((copy >> 18) & 7);
                res.matrix[2][1] = ((copy >> 21) & 7);
                res.matrix[2][2] = ((copy >> 24) & 7);
               // print_map(res.matrix);
                rm(res.matrix);
                copy = 0;
                copy |= (res.matrix[0][0]);
                copy |= (res.matrix[0][1] << 3);
                copy |= (res.matrix[0][2] << 6);
                copy |= (res.matrix[1][0] << 9);
                copy |= (res.matrix[1][1] << 12);
                copy |= (res.matrix[1][2] << 15);
                copy |= (res.matrix[2][0] << 18);
                copy |= (res.matrix[2][1] << 21);
                copy |= (res.matrix[2][2] << 24);
                hashed = copy << 8 | moves;
        }

        res.matrix[0][0] = 0;
        res.matrix[0][1] = 0;
        res.matrix[0][2] = 0;
        res.matrix[1][0] = 0;
        res.matrix[1][1] = 0;
        res.matrix[1][2] = 0;
        res.matrix[2][0] = 0;
        res.matrix[2][1] = 0;
        res.matrix[2][2] = 0;

  int cases;

  moves++;
  #pragma GCC unroll 3
  for (char i = 0; i < 3; i++)
  {
        #pragma GCC unroll 3
    for (char j = 0; j < 3; j++)
    {
      if (!((board >> (i * 9 + j * 3)) & 7))
      {
        cases = 0;
        calculate_cases(res ,copy, board, i, j, moves, depth, cases, rotates);
        //if (cases)
        //        past_2_maps(res.matrix, res2.matrix);
        if (!cases)
        {
                copy = board;
                copy |= (1 << (i * 9 + j * 3));
                past_2_maps(res.matrix, calculate(copy, moves, depth, rotates).matrix);
        }
      }
    }
  }
  memory[hashed] = res;
  return (res);
}

int main() {
  int depth;
  cin >> depth;
  cin.ignore();

  uint64_t board = 0;
  int cases = 0;
  int maves = 0;
  int value;
  uint64_t final_result = 0;
  s_array map;
  cin >> value; cin.ignore(); board |= (value);
  cin >> value; cin.ignore(); board |= (value << 3);
  cin >> value; cin.ignore(); board |= (value << 6);
  cin >> value; cin.ignore(); board |= (value << 9);
  cin >> value; cin.ignore(); board |= (value << 12);
  cin >> value; cin.ignore(); board |= (value << 15);
  cin >> value; cin.ignore(); board |= (value << 18);
  cin >> value; cin.ignore(); board |= (value << 21);
  cin >> value; cin.ignore(); board |= (value << 24);

  // Write an action using cout. DON'T FORGET THE "<< endl"
  // To debug: cerr << "Debug messages..." << endl;

  // return ((board & 7) * 100000000 +
  // 	((board >> 3) & 7) * 10000000 +
  // 	((board >> 6) & 7) * 1000000 +
  // 	((board >> 9) & 7) * 100000 +
  // 	((board >> 12) & 7) * 10000 +
  // 	((board >> 15) & 7) * 1000 +
  // 	((board >> 18) & 7) * 100 +
  // 	((board >> 21) & 7) * 10 +
  // 	((board >> 24) & 7));

        short rotates = 0;
        map = calculate(board, 0, depth, rotates);
        // for (short r = rotates; r < 9; r++)
        // {
        //         rm(map.matrix);
        // }
        final_result = (final_result + (map.matrix[0][0] * 100000000) % (1<<30)) % (1<<30);
        final_result = (final_result + (map.matrix[0][1] * 10000000) % (1<<30)) % (1<<30);
        final_result = (final_result + (map.matrix[0][2] * 1000000) % (1<<30)) % (1<<30);
        final_result = (final_result + (map.matrix[1][0] * 100000) % (1<<30)) % (1<<30);
        final_result = (final_result + (map.matrix[1][1] * 10000) % (1<<30)) % (1<<30);
        final_result = (final_result + (map.matrix[1][2] * 1000) % (1<<30)) % (1<<30);
        final_result = (final_result + (map.matrix[2][0] * 100) % (1<<30)) % (1<<30);
        final_result = (final_result + (map.matrix[2][1] * 10) % (1<<30)) % (1<<30);
        final_result = (final_result + (map.matrix[2][2]) % (1<<30)) % (1<<30);
        cout << final_result << endl;
}
