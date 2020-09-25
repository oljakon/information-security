#include "license.hpp"
#include <cmath>

using namespace std;

int check_installation()
{
    string current_serial = get_serial();
    current_serial.erase(current_serial.length()-1);

    string actual_serial = "";
    int err = SUCCESS;
    
    ifstream license;
    license.open("license.key");
    if (license.is_open())
    {
        license >> actual_serial;
        
        if (actual_serial != current_serial)
        {
            err = INSTALLATION_ERROR;
        }
    }
    else
    {
        err = FILE_ERROR;
    }
    license.close();
    
    return err;
}

void quadratic_equation()
{
    double a, b, c;
    cout << "Input coefficients of quadratic equation: ";
    cin >> a >> b >> c;

    if (a != 0)
    {
        double D = b * b - 4 * a * c;
        if (D > 0)
        {
            double x1 = (-b + sqrt(D)) / (2 * a);
            double x2 = (-b - sqrt(D)) / (2 * a);
            cout << x1 << " " << x2 << endl;
        }
        else if (D == 0)
        {
            double x = (- b) / (2 * a);
            cout << x << endl;
        }
        else
        {
            cout << "No roots" << endl;;
        }
    }
    else
    {
        if (b != 0)
        {
            double x = - c / b;
            cout << x << endl;
        }
        else
        {
            cout << "No roots" << endl;
        }
    }
}

int main()
{
    int status = check_installation();
    switch (status)
    {
        case SUCCESS:
            printf("Program can be run on this computer\n");
            quadratic_equation();
            break;
        case FILE_ERROR:
            printf("License file not found, program couldn't be run on this computer\n");
            return FILE_ERROR;
        case INSTALLATION_ERROR:
            printf("Program is not installed on this computer\n");
            return INSTALLATION_ERROR;
    }
}
