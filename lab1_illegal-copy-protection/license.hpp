#ifndef license_hpp
#define license_hpp

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <stdlib.h>
#include <fstream>

#define SUCCESS 0
#define ACTIVATION_ERROR -1
#define FILE_ERROR -2
#define INSTALLATION_ERROR -3

using namespace std;

string get_serial();

#endif /* license_hpp */
