#include <iostream>

#include "Configuration.h"

using namespace std;

int main()
{
    Configuration::getInstance().Initialize("iptables.conf");
    cout << "Hello, World!" << endl;
    return 0;
}