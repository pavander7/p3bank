// Project Identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include <algorithm>
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include "account.h"

using namespace std;

int main (int argc, char* argv[]) {
    //getoptlong
    //getoptlong time
    ios_base::sync_with_stdio(false);

    //cout << "good morning \n";

    static struct option long_options[] = {
    {"help",        no_argument,        NULL,  'h'},
    {"verbose",     no_argument,        NULL,  'v'},
    {"file",        no_argument,        NULL,  'f'},
    {0,             0,                  NULL,  0, }
    };
    int option_index = 0;

    bool verbose = false;
    bool file = false;
    string filename;

    int c = getopt_long(argc, argv, "hvf:", long_options, &option_index);
    while (c != -1) {
        switch(c) {
            case 'h' :
                cout << "help message";
                exit(0);
                break;
            case 'v' :
                verbose = true;
                break;
            case 'f' :
                file = true;
                filename = optarg;
                break;
        }
        c = getopt_long(argc, argv, "hvf:", long_options, &option_index);
    }

    if (!file) {
        cout << "registrations not specified\n";
        return 1;
    }

    ifstream reg(filename);
    string line;
    list<Account> registrations;

    while (getline(reg, line)) {
        stringstream s(line);
        string ts, id, p, bal;
        getline(reg, ts, '|');
        getline(reg, id, '|');
        getline(reg, p, '|');
        getline(reg, bal, '|');
        registrations.push_back(Account(id, uint32_t(stoi(bal)), uint32_t(stoi(p)), uint64_t(stoi(ts))));
    }

    string line;
    while(getline(cin, line)) {
        if (line[0] != '#') {
            stringstream s(line);
            string start;
            s >> start;
            if (start == "login") {
                string id, p, ip;
                s >> id >> p >> ip;
            } else if (start == "out") {
                string id, ip;
                s >> id >> ip;
            } else if (start == "place") {
                string ts, ip, send, recip, amount, exec;
                s >> ts >> ip >> send >> recip >> amount >> exec;
            } else if (start == "$$$") {
                break;
            }
        }
    }
}