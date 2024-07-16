#include <windows.h>
#include <winsock.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <conio.h>
#include "define.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;
char buf[1024];
int flag;
WSADATA wsaData;
SOCKET s;
SOCKADDR_IN addr;
void init()
{
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1060);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(s, (SOCKADDR *)&addr, sizeof(addr));
    ifstream in("number.txt", ios::in);
    if (!in)
    {
        send(s, "NULL", strlen("NULL"), 0);
        recv(s, buf, sizeof(buf), 0);
        ofstream out("number.txt", ios::out);
        out << buf;
        out.close();
    }
    else
    {
        in.getline(buf, 1024);
        send(s, buf, strlen(buf), 0);
    }
    in.close();
}
int main()
{
    // cout << 1;

    init();
    while (1)
    {
        cin.getline(buf, 1024);
        string temp = buf;
        string sends;
        
        if (temp.length() < 8)
        {
            cout << "ERROR NOT FIND IT" << endl;
        }
        // cout << An;
        string An = temp.substr(0, 8);
        if (temp == EXIT)
        {
            send(s, EXIT, strlen(EXIT), 0);
            closesocket(s);
            break;
        }
        else if (An == CATFILE && temp.length() >= 9)
        {
            sends = stw_c[An] + temp.substr(9);
            send(s, sends.c_str(), sends.length(), 0);
            string recvs;
            while (recvs != END)
            {
                recv(s, buf, sizeof(buf), 0);
                recvs = buf;
                if (recvs != END)
                    cout << recv;
            }
        }
        else if (An == OPENFILE && temp.length() >= 9)
        {
            sends = stw_c[An] + temp.substr(9);
            send(s, sends.c_str(), sends.length(), 0);
            string recvs;
            system("cls");
            while (recvs != END)
            {
                recv(s, buf, sizeof(buf), 0);
                recvs = buf;
                if (recvs != END)
                    cout << recv;  
            }
            while (1)
            {
                char now = getch();
                if (now == 8)
                {
                    cout << "\b \b";
                    send(s, "\b \b", sizeof("\b \b"), 0);
                }
                else if (now == 24)
                {
                    send(s, CLOSEFILE, sizeof(CLOSEFILE), 0);
                    break;
                }
                else if (now == 15)
                {
                    send(s, SAVEFILE, sizeof(SAVEFILE), 0);
                }
                else
                {
                    cout << now;
                    send(s, (const char *)now, sizeof(now), 0);
                }
            }
            send(s, END, strlen(END), 0);
        }
        else if (stw_c[An] != "")
        {
            sends = stw_c[An] +
                    (temp.length() >= 9 ? " " + temp.substr(9) : "");
            send(s, sends.c_str(), sends.length(), 0);
        }
        else
        {
            cout << "ERROR NOT FIND IT" << endl;
        }
    }
    // return 0;
}