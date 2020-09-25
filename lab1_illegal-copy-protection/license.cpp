#include "license.hpp"

using namespace std;

string get_serial()
{
    char buffer[128];
    string result = "";
    
    FILE* pipe = popen("ioreg -rd1 -c IOPlatformExpertDevice | awk '/IOPlatformUUID/ { split($0, line, \"\\\"\"); printf(\"%s\\n\", line[4]); }'", "r");
    if (!pipe)
    {
        throw runtime_error("popen() failed");
    }
    
    while (fgets(buffer, sizeof buffer, pipe) != NULL)
    {
        result += buffer;
    }
    
    pclose(pipe);
    return result;
}
