#include <iostream>
#include <string>
using namespace std;
char buf[1024];
int main()
{
    // cout << 1;
    while (1)
    {
        cin.getline(buf, 1024);
        string temp = buf;
        cout << temp.substr(0, 10) << endl;
        cout << temp.substr(10);
    }
}