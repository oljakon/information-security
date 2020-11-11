#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <bitset>
#include <string>
#include <array>
#include <algorithm>

using namespace std;

// начальная перестановка IP
int IP[] = {58, 50, 42, 34, 26, 18, 10, 2,
            60, 52, 44, 36, 28, 20, 12, 4,
            62, 54, 46, 38, 30, 22, 14, 6,
            64, 56, 48, 40, 32, 24, 16, 8,
            57, 49, 41, 33, 25, 17, 9,  1,
            59, 51, 43, 35, 27, 19, 11, 3,
            61, 53, 45, 37, 29, 21, 13, 5,
            63, 55, 47, 39, 31, 23, 15, 7};

// конечная перестановка IP-1
int IP_1[] = {40, 8, 48, 16, 56, 24, 64, 32,
              39, 7, 47, 15, 55, 23, 63, 31,
              38, 6, 46, 14, 54, 22, 62, 30,
              37, 5, 45, 13, 53, 21, 61, 29,
              36, 4, 44, 12, 52, 20, 60, 28,
              35, 3, 43, 11, 51, 19, 59, 27,
              34, 2, 42, 10, 50, 18, 58, 26,
              33, 1, 41,  9, 49, 17, 57, 25};

// начальная перестановка В
int B[] = {57, 49, 41, 33, 25, 17, 9,
               1, 58, 50, 42, 34, 26, 18,
              10,  2, 59, 51, 43, 35, 27,
              19, 11,  3, 60, 52, 44, 36,
              63, 55, 47, 39, 31, 23, 15,
               7, 62, 54, 46, 38, 30, 22,
              14,  6, 61, 53, 45, 37, 29,
              21, 13,  5, 28, 20, 12,  4};

// сжимающая перестановка (56 -> 48)
int PC_2[] = {14, 17, 11, 24,  1,  5,
               3, 28, 15,  6, 21, 10,
              23, 19, 12,  4, 26,  8,
              16,  7, 27, 20, 13,  2,
              41, 52, 31, 37, 47, 55,
              30, 40, 51, 45, 33, 48,
              44, 49, 39, 56, 34, 53,
              46, 42, 50, 36, 29, 32};

// сдвиг Si
int S[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// расширяющая перестановка Е
int E[] = {32,  1,  2,  3,  4,  5,
            4,  5,  6,  7,  8,  9,
            8,  9, 10, 11, 12, 13,
           12, 13, 14, 15, 16, 17,
           16, 17, 18, 19, 20, 21,
           20, 21, 22, 23, 24, 25,
           24, 25, 26, 27, 28, 29,
           28, 29, 30, 31, 32,  1};

// S-блоки
int S_BOX[8][4][16] = {
    {
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    },
    {
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
    },
    {
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
    },
    {
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
    },
    {
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
    },
    {
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
    },
    {
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
    },
    {
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
    }
};

// завершающая перестановка P
int P[] = {16,  7, 20, 21,
           29, 12, 28, 17,
            1, 15, 23, 26,
            5, 18, 31, 10,
            2,  8, 24, 14,
           32, 27,  3,  9,
           19, 13, 30,  6,
           22, 11,  4, 25 };

// функция Фейстеля
bitset<32> f(bitset<32> R, bitset<48> k)
{
    // расширяющая перестановка Е
    bitset<48> expandR;
    for(int i=0; i<48; ++i)
        expandR[i] = R[E[i]-1];
    expandR ^= k;

    // преобразование каждого 6-битового блока в 4-битовый
    bitset<32> output;
    for(int i = 0; i < 48; i += 6)
    {
        int a = expandR[i]*2 + expandR[i+5];
        int b = expandR[i+1]*8 + expandR[i+2]*4 + expandR[i+3]*2 + expandR[i+4];
        int num = S_BOX[i/6][a][b];
        bitset<4> block(num);
        for(int j = 0; j < 4; j++)
            output[i*4/6+j] = block[j];
    }

    // перестановка Р
    bitset<32> tmp = output;
    for(int i=0; i<32; i++)
        output[i] = tmp[P[i]-1];
    return output;
}

// генерация ключей ki
array<bitset<48>,16> generateKeys(bitset<56> originalKey)
{
    // 16 ключей по 48 бит
    bitset<56> key;
    for(int i = 0; i < 56; i ++)
        key[i] = originalKey[B[i]-1];

    array<int,28> left, right;
    for(int i = 0; i < 28; i++)
    {
        left[i] = key[i];
        right[i] = key[i+28];
    }

    array<bitset<48>,16> result;

    for(int i = 0; i < 16; i++)
    {
        rotate(left.begin(), left.begin() + 1, left.end());
        rotate(right.begin(), right.begin() + 1, right.end());
        if(S[i] == 2)
        {
            rotate(left.begin(), left.begin() + 1, left.end());
            rotate(right.begin(), right.begin() + 1, right.end());
        }

        bitset<56> LR;
        for(int j = 0; j < 28; j++)
        {
            LR[j] = left[j];
            LR[j+28] = right[j];
        }
        for(int j = 0; j < 48; j++)
            result[i][j] = LR[PC_2[j]-1];
    }
    return result;
}

// flag: false - encrypt; true - decrypt
bitset<64> algorythm(bitset<64> data, array<bitset<48>,16> keys, bool flag)
{
    bitset<32> left, right, tmp;
    bitset<64> copy = data;

    // начальная перестановка
    for(int i = 0; i < 64; i++)
        data[i] = copy[IP[i]-1];

    for(int i = 0; i < 32; i++)
    {
        left[i] = data[i];
        right[i] = data[i+32];
    }

    if(!flag)
        for(int i = 0; i < 16; i++)
        {
            tmp = right;
            right = f(right, keys[i]) ^ left;
            left = tmp;
        }

    else
        for(int i = 15; i >= 0; i--)
        {
            tmp = left;
            left = f(left, keys[i]) ^ right;
            right  = tmp;
        }

    for(int i = 0; i < 32; i++)
    {
        copy[i] = left[i];
        copy[i+32] = right[i];
    }
    
    // конечная перестановка
    for(int i = 0; i < 64; i++)
        data[i] = copy[IP_1[i]-1];
    return data;
}

int main()
{
    ifstream input("data/test.zip", ios::binary );

    vector<unsigned char> buffer(istreambuf_iterator<char>(input), {});

    bitset<56> key(1859312);
    array<bitset<48>,16> keys = generateKeys(key);
    int added_count = 0;
    int blocks_num = buffer.size()/8; // 64 бита = 8 символов из входного потока
    if(buffer.size()%8)
        blocks_num++;
    vector<bitset<64>> blocks;
    blocks.resize(blocks_num);
    for(int i = 0; i < blocks_num; i++)
    {
        bitset<8> subblock;
        for(int j = 0; j < 8; j++)
        {
            if(i*8+j < buffer.size())
                subblock = bitset<8>(buffer[i*8+j]);
            else
            {
                subblock = bitset<8>(0);
                added_count++;
            }
            for(int k = 0; k < 8; k++)
                blocks[i][j*8+k] = subblock[k];
        }
    }

    vector<bitset<64>> encrypted;
    encrypted.resize(blocks_num);
    for(int i = 0; i < blocks_num; i++)
        encrypted[i] = algorythm(blocks[i],keys,0);

    vector<bitset<64>> decrypted;
    decrypted.resize(blocks_num);
    for(int i = 0; i < blocks_num; i++)
        decrypted[i] = algorythm(encrypted[i],keys,1);

    ofstream output ("data/test2.zip", ios::binary );
    for(int i = 0; i < blocks_num; i++)
    {
        bitset<8> tmp;
        for(int j = 0; j < 8; j++) // каждый bitset<8> -> char
        {
            for(int k = 0; k < 8; k++)
                tmp[k] = blocks[i][j*8+k];
            unsigned long itmp = tmp.to_ulong();
            if(i*8+j < (blocks_num*8 - added_count))
                output << (unsigned char)itmp;
        }
    }
}
