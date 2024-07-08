#include <windows.h>
#include <winsock.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <vector>
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
map<char *, int> ipc;
map<string, string> stw;
thread th[10000];
int thi;
void zwc(SOCKET sock, string ip, string number)
{
    char buf[1024] = {0};
    int flag = 0;
    ofstream out;
    ifstream in;
    ip = ip + number;
    while (1)
    {
        int len = recv(sock, buf, sizeof(buf), 0);
        string temp = buf;
        string An = stw[temp.substr(0, 8)];
        string answer = temp.substr(9);
        if (len <= 0)
        {
            cout << "ip:" << ip << "-" << number << " disconnect" << endl;
            return;
        }
        else if (An == EXIT)
        {
            cout << "ip:" << ip << "-" << number << " disconnect" << endl;
            closesocket(sock);
            return;
        }
        else if (An == MKFILE)
        {
            out.open(answer.c_str(), ios::out);
            out.close();
            cout << "ip:" << ip << "-" << number << " create file " << endl;
        }
        else if (An == CATFILE)
        {
            in.open(answer.c_str(), ios::in);
            while (!in.eof())
            {
                getline(in, temp);
                send(sock, temp.c_str(), sizeof(temp), 0);
            }
            in.close();
            send(sock, END, sizeof(END), 0);
        }
        else if (An == OPENFILE)
        {
            if (answer.length >= 4)
            {
                string AOW = stw[answer.substr(0, 1)];
                answer = answer.substr(4);
                if (AOW == "-a")
                {
                    out.open(answer.c_str(), ios::app);
                    cout << "ip:" << ip << "-" << number << " open file with ios::app" << endl;
                }
                else if (AOW == "-w")
                {
                    out.open(answer.c_str(), ios::out);
                    cout << "ip:" << ip << "-" << number << " open file with ios::out" << endl;
                }
            }
        }
    }
    return;
}
int main()
{
    stw["00000"] = EXIT;
    stw["00100"] = MKFILE;
    stw["01000"] = CATFILE;
    stw["01110"] = OPENFILE_A;
    stw["01101"] = OPENFILE_W;
    stw["01101"] = OPENFILE;
    stw["10000"] = CLOSEFILE;
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1060);
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
    bind(sock, (SOCKADDR *)&addr, sizeof(addr));

    listen(sock, 5);
    while (1)
    {
        SOCKADDR_IN client;
        int len = sizeof(client);
        SOCKET temp = accept(sock, (SOCKADDR *)&client, &len);
        char *cip = inet_ntoa(client.sin_addr);
        char buf[1024] = {0};
        recv(temp, buf, sizeof(buf), 0);
        if (((string)buf).substr(0, 4) == "NULL")
        {
            cout << "NULL" << endl;
            cout << "ip:" << cip << "-" << to_string(++ipc[cip]) << " join" << endl;
            char buft[1024];
            strcpy(buft, to_string(ipc[cip]).c_str());
            send(temp, buft, sizeof(buft), 0);
            cout << "give " << (cip + string("-") + to_string(ipc[cip])) << " number" << endl;
            th[++thi] = thread(zwc, temp, cip, to_string(ipc[cip]));
        }
        else
        {
            cout << "ip:" << cip << "-" << buf << " join" << endl;
            th[atoi(buf)] = thread(zwc, temp, cip, string(buf));
        }
    }
    for (int i = 1; i <= thi; i++)
    {
        th[thi].join(); // join会阻塞主线程，直到t线程执行完毕
    }
}