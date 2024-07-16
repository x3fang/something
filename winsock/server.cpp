#include <windows.h>
#include <winsock.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include "define.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;
map<char *, int> ipc;

thread th[10000];
int thi;
/*
ip+"-"+number+"_"+FILE_NAME
*/
string get_file_name(string ip, string number, string file_name)
{
    string res = ip + "-" + number + "_" + file_name;
    return res;
}

void zwc(SOCKET sock, string ip, string number)
{
    char buf[1024] = {0};
    int flag = 0;
    /*
    flag:
    0: nothing
    1: open file

    */
    ofstream out;
    ifstream in;
    ip = ip + number;
    while (1)
    {
        // cout << 1;
        int len = recv(sock, buf, sizeof(buf), 0);
        string temp = buf;
        cout << temp.substr(0, 5) << endl;
        string An = stw_s[temp.substr(0, 5)];
        cout << An << endl;
        string answer = (temp.length() >= 6 ? temp.substr(6) : "");
        cout << answer << endl;
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
            out.open(get_file_name(ip, number, answer).c_str(), ios::out);
            out.close();
            cout << "ip:" << ip << "-" << number << " create file " << endl;
        }
        else if (An == CATFILE)
        {
            in.open(get_file_name(ip, number, answer).c_str(), ios::in);
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
            in.open(get_file_name(ip, number, answer).c_str(), ios::in);
            while (!in.eof())
            {
                getline(in, temp);
                send(sock, temp.c_str(), sizeof(temp), 0);
            }
            in.close();
            send(sock, END, sizeof(END), 0);
            out.open(get_file_name(ip, number, answer).c_str(), ios::app);
            cout << "ip:" << ip << "-" << number << " open file with mode ios::app" << endl;
            while (1)
            {
                int len = recv(sock, buf, sizeof(buf), 0);
                string answer = buf;
                if (answer == END)
                {
                    out.close();
                    break;
                }
                else if (answer == CLOSEFILE)
                {
                    out.close();
                    cout << "ip:" << ip << "-" << number << " close file" << endl;
                    break;
                }
                else if (answer == SAVEFILE)
                {
                    out.close();
                    out.open(get_file_name(ip, number, answer).c_str(), ios::app);
                    cout << "ip:" << ip << "-" << number << " save file" << endl;
                    break;
                }
                else
                {
                    out << answer;
                }
            }
        }
        else if (An == CLOSEFILE && flag == 1)
        {
            out.close();
            flag = 0;
        }
    }
    return;
}
int main()
{

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