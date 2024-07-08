#include <windows.h>
#include <winsock.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <map>

#define EXIT ":to_exit"
#define MKFILE ":mk_file"
#define OPENFILE ":op_file"
#define CLOSEFILE ":cl_file"
#define CATFILE ":ct_file"
#define OPENFILE_A ":op_file -a"
#define OPENFILE_W ":op_file -w"
#define END "\r\nEND\r\n"

#pragma comment(lib, "ws2_32.lib")
using namespace std;
char buf[1024];
map<string, string> stw;
WSADATA wsaData;
SOCKET s;
SOCKADDR_IN addr;
void init()
{
    stw[EXIT] = "00000";
    stw[MKFILE] = "00100";
    stw[CATFILE] = "01000";
    stw[OPENFILE_A] = "01110";
    stw[OPENFILE_W] = "01101";
    stw[OPENFILE] = "01101";
    stw[CLOSEFILE] = "10000";
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
        string sends ;
        // cout << temp.substr(0, 8);
        if (temp == EXIT)
        {
            send(s, EXIT, strlen(EXIT), 0);
            closesocket(s);
            break;
        }
        else if (temp.substr(0, 8) == OPENFILE && temp.length() >= 12)
        {
            sends = stw[temp.substr(0, 11)] + " " + temp.substr(12);
            send(s, sends.c_str(), sends.length(), 0);
        }
        if (temp.substr(0, 8) == CATFILE_D)
        {
            sends = stw[temp.substr(0, 11)] + " " + temp.substr(12);
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
        else if (stw[temp.substr(0, 8)] != "")
        {
            sends = stw[temp.substr(0, 8)] + (temp.length() >= 9 ? " " + temp.substr(9) : "");
            send(s, sends.c_str(), sends.length(), 0);
        }
        else
        {
            cout << "ERROR NOT FIND IT" << endl;
        }
    }
    // return 0;
}