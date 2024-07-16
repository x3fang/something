#ifndef DEFINE_H
#define DEFINE_H
#define EXIT ":to_exit"
#define MKFILE ":mk_file"
#define OPENFILE ":op_file"
#define SAVEFILE ":se_file"
#define CLOSEFILE ":cl_file"
#define CATFILE ":ct_file"
#define END "\r\nEND\r\n"
#include <map>
#include <string>
using namespace std;
map<string, string> stw_c = {
    {EXIT, "00000"},
    {MKFILE, "00100"},
    {CATFILE, "01000"},
    {OPENFILE, "01101"}};
map<string, string> stw_s = {
    {"00000",EXIT},
    {"00100",MKFILE},
    {"01000",CATFILE},
    {"01101",OPENFILE}
};
// stw[EXIT] = "00000";
// stw[MKFILE] = "00100";
// stw[CATFILE] = "01000";
// stw[OPENFILE] = "01101";
#endif