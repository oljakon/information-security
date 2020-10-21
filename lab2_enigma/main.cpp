#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>

class Rotor
{
public:
    Rotor(){}
    ~Rotor(){}
    void Generate();
    int Translate(int letter);
    int TranslateBack(int letter);
    void Turn();
    int pos[256];
};

void Rotor::Generate()
{
    for(int i = 0; i < 256; i++)
    {
        pos[i] = i;
    }
    
    for(int i = 0; i < 256; i++)
    {
        int exchange = rand() % 256;
        int tmp = pos[i];
        pos[i] = pos[exchange];
        pos[exchange] = tmp;
    }
}

int Rotor::Translate(int letter)
{
    return pos[letter];
}

int Rotor::TranslateBack(int letter)
{
    return std::distance(pos, std::find(pos, pos + 256, letter)); //index of letter in pos
}

void Rotor::Turn()
{
    int tmp = pos[255];
    for(int i = 255; i > 0; i--)
    {
        pos[i] = pos[i - 1];
    }
    pos[0] = tmp;
}

class Reflector
{
public:
    Reflector(){}
    ~Reflector(){}
    void Generate();
    int Translate(int letter);
    int pos[256];
};

void Reflector::Generate()
{
    for(int i = 0; i < 256; i++)
    {
        pos[i] = i;
    }

    for(int i = 0; i < 256; i++)
    {
        if(pos[i] == i)
        {
            int exchange = rand() % 256;
            while((pos[exchange] != exchange) || (exchange == i))
                exchange = rand() % 256;
            pos[i] = pos[exchange];
            pos[exchange] = i;
        }
    }
}

int Reflector::Translate(int letter)
{
    return pos[letter];
}

class Enigma
{
public:
    Enigma(){}
    ~Enigma(){}
    void Init();
    int* Encode(int* input, int N);
    int* Decode(int* input, int N, Rotor* rotors);
    Rotor* GetRotors();
    Rotor rotors[3];
    Reflector reflector;
};

void Enigma::Init()
{
    rotors[0].Generate();
    rotors[1].Generate();
    rotors[2].Generate();
    reflector.Generate();
}

Rotor* Enigma::GetRotors()
{
    return rotors;
}

int* Enigma::Encode(int* input, int N)
{
    int turncount[] = {0, 0, 0};
    int *res_arr = new int[N];
    for(int i = 0; i < N; i++)
    {
        int symbol = input[i];
        for(int j = 0; j < 3; j++)
        {
            symbol = rotors[j].Translate(symbol);
        }
        symbol = reflector.Translate(symbol);
        for(int j = 2; j >= 0; j--)
        {
            symbol = rotors[j].TranslateBack(symbol);
        }
        res_arr[i] = symbol;
        rotors[0].Turn();
        turncount[0]++;
        if(turncount[0] == 256)
        {
            rotors[1].Turn();
            //turncount[0] = 0;
            turncount[1]++;
        }
        if(turncount[1] == 256)
        {
            rotors[2].Turn();
            //turncount[1] = 0;
            turncount[2]++;
        }
    }
    return res_arr;
}

int* Enigma::Decode(int* input, int N, Rotor* saved_rotors)
{
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 256; j++)
            rotors[i].pos[j] = saved_rotors[i].pos[j];

    return Encode(input, N);
}

int main()
{
    srand(time(NULL));
    Enigma enigma;
    enigma.Init();

    Rotor rotor_state[3];
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 256; j++)
        {
            rotor_state[i].pos[j] = enigma.GetRotors()[i].pos[j];
        }
    }
    
    char* path1 = "data/test";
    struct stat results1;
    stat(path1, &results1);
    long size1 = results1.st_size;
    char* file_contents1 = new char[size1]();
    std::ifstream file1 (path1, std::ios::binary);
    file1.read(file_contents1, size1);

    int *in_arr1 = new int[size1];
    for(int i = 0; i < size1; i++)
        in_arr1[i] = file_contents1[i];

    int* encoded1 = enigma.Encode(in_arr1, size1);
    char* encoded_char1 = new char[size1]();;
    for(int i = 0; i < size1; i++)
        encoded_char1[i] = encoded1[i];
    std::ofstream encoded_file1 ("data/test_encoded", std::ios::binary);
    encoded_file1.write (encoded_char1, size1);

    int* decoded1 = enigma.Decode(encoded1, size1, rotor_state);
    char* decoded_char1 = new char[size1]();;
    for(int i = 0; i < size1; i++)
        decoded_char1[i] = decoded1[i];
    std::ofstream decoded_file1 ("data/test_decoded", std::ios::binary);
    decoded_file1.write (decoded_char1, size1);
    
    
    char* path2 = "data/empty";
    struct stat results2;
    stat(path2, &results2);
    long size2 = results2.st_size;
    char* file_contents2 = new char[size2]();
    std::ifstream file2 (path2, std::ios::binary);
    file2.read(file_contents2, size2);

    int *in_arr2 = new int[size2];
    for(int i = 0; i < size2; i++)
        in_arr2[i] = file_contents2[i];
    
    int* encoded2 = enigma.Encode(in_arr2, size2);
    char* encoded_char2 = new char[size2]();;
    for(int i = 0; i < size2; i++)
        encoded_char2[i] = encoded2[i];
    std::ofstream encoded_file2 ("data/empty_encoded", std::ios::binary);
    encoded_file2.write (encoded_char2, size2);

    int* decoded2 = enigma.Decode(encoded2, size2, rotor_state);
    char* decoded_char2 = new char[size2]();;
    for(int i = 0; i < size2; i++)
        decoded_char2[i] = decoded2[i];
    std::ofstream decoded_file2 ("data/empty_decoded", std::ios::binary);
    decoded_file2.write (decoded_char2, size2);
    
    
    char* path = "data/test.zip";
    struct stat results3;
    stat(path, &results3);
    long size3 = results3.st_size;
    char* file_contents3 = new char[size3]();
    std::ifstream file3 (path, std::ios::binary);
    file3.read(file_contents3, size3);

    int *in_arr3 = new int[size3];
    for(int i = 0; i < size3; i++)
        in_arr3[i] = file_contents3[i] + 128;

    int* encoded3 = enigma.Encode(in_arr3, size3);
    char* encoded_char3 = new char[size3]();;
    for(int i = 0; i < size3; i++)
        encoded_char3[i] = encoded3[i] - 128;
    std::ofstream encoded_file3 ("data/archive_encoded", std::ios::binary);
    encoded_file3.write (encoded_char3, size3);

    int* decoded3 = enigma.Decode(encoded3, size3, rotor_state);
    char* decoded_char3 = new char[size3]();;
    for(int i = 0; i < size3; i++)
        decoded_char3[i] = decoded3[i] - 128;
    std::ofstream decoded_file3 ("data/decoded.zip", std::ios::binary);
    decoded_file3.write (decoded_char3, size3);
}
