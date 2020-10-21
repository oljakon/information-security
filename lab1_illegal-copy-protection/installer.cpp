#include "license.hpp"

int main()
{
    ofstream outfile ("license.key");
    string result = get_serial();
    
    outfile << result;
    outfile.close();
    
    if (result != "")
    {
        cout << "Program is activated" << endl;
        return SUCCESS;
    }
    else
    {
        cout << "Activation failed" << endl;
        return ACTIVATION_ERROR;
    }

}
