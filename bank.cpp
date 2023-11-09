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
    vector<Account> registrations;

    while (getline(reg, line)) {
        stringstream s(line);
        string ts, id, p, bal;
        getline(reg, ts, '|');
        getline(reg, id, '|');
        getline(reg, p, '|');
        getline(reg, bal, '|');
        registrations.push_back(Account(id, uint32_t(stoi(bal)), uint32_t(stoi(p)), uint64_t(stoi(ts))));
    }

    vector<Transaction> record;

    string line;
    while(getline(cin, line)) {
        if (line[0] != '#') {
            stringstream s(line);
            string start;
            s >> start;
            if (start == "login") {
                string id, p, ip;
                s >> id >> p >> ip;
                Account* target = findAccount(registrations, id);
                cout << target->login(id, uint32_t(stoi(p)), ip);
            } else if (start == "out") {
                string id, ip;
                s >> id >> ip;
                Account* target = findAccount(registrations, id);
                cout << target->logout(ip);
            } else if (start == "place") {
                string ts, ip, send, recip, amount, exec, share;
                s >> ts >> ip >> send >> recip >> amount >> exec >> share;
                Account* sender = findAccount(registrations, send);
                Account* recipient = findAccount(registrations, recip);
                if ((stoi(exec) - stoi(ts)) > 3 || (stoi(exec) - stoi(ts)) < 0) {
                    cout << "You cannot have an execution date before the current timestamp.\n";
                } else {
                    if(place(sender, recipient, uint32_t(stoi(amount)), uint64_t(stoi(exec)))) {
                        if (share == "o") {
                            sender->fine(uint32_t(stoi(amount)), uint64_t(stoi(exec)));
                        } else sender->fine(uint32_t(stoi(amount)), uint64_t(stoi(exec)), recipient);
                        record.push_back({sender->name(), uint32_t(stoi(amount)), recipient->name(), uint64_t(stoi(exec))});
                    }
                }
            } else if (start == "$$$") {
                break;
            }
        }
    }

    sort(record.begin(), record.end());

    while(getline(cin,line)) {
        stringstream s(line);
        string start;
        s >> start;
        if (start == "l") {
            uint64_t begin, end;
            s >> begin >> end;
            listOut(record, begin, end);
        } else if (start == "r") {
            uint64_t begin, end;
            s >> begin >> end;
            revenue(record, begin, end, registrations);
        } else if (start == "h") {
            string id;
            s >> id;
            findAccount(registrations, id)->report();
        } else if (start == "s") {
            uint64_t ts;
            s >> ts;
            summarize(record, registrations, ts);
        }
    }
}

bool place (Account* s, Account* r, uint32_t amount, uint64_t ts) {
    if (s != nullptr && r != nullptr) {
        if (s->isOnline()) {
            s->send(ts, r, amount);
            return true;
        } else return false;
    } else return false;
}

void listOut(vector<Transaction> &record, uint64_t begin, uint64_t end) {
    uint64_t current = record.front().timestamp;
    size_t i = 0;
    while (current < begin && i < record.size()) {
        current = record[i].timestamp;
        i++;
    } size_t count = 0;
    while (current < end && i < record.size()) {
        cout << record[i];
        i++;
        count++;
    } 
    if (count == 1) {
        cout << "There was " + to_string(count) + " transaction that was placed between time "
             + to_string(begin) + " to " + to_string(end) + ".\n"; 
    } else {
        cout << "There were " + to_string(count) + " transactions that were placed between time "
             + to_string(begin) + " to " + to_string(end) + ".\n"; 
    }
}

void revenue(vector<Transaction> &record, uint64_t begin, uint64_t end, vector<Account> &reg) {
    uint64_t current = record.front().timestamp;
    size_t i = 0;
    while (current < begin && i < record.size()) {
        current = record[i].timestamp;
        i++;
    } uint32_t revenue = 0;
    while (current < end && i < record.size()) {
        Account* sender = findAccount(reg, record[i].sender);
        uint32_t fee = record[i].amount/uint32_t(100);
        fee = min(fee, uint32_t(450));
        fee = max(fee, uint32_t(10));
        if (sender->loyalty(record[i].timestamp)) {
            fee = (fee * 3) / 4;
        } revenue += fee;
        i++;
    } uint64_t interval = end - begin;
    cout << "281bank has collected " << to_string(revenue) << " dollars in fees over ";
    if (place(interval, 0) != 0) cout << to_string(place(interval, 0)) << " years ";
    if (place(interval, 1) != 0) cout << to_string(place(interval, 1)) << " months ";
    if (place(interval, 2) != 0) cout << to_string(place(interval, 2)) << " days ";
    if (place(interval, 3) != 0) cout << to_string(place(interval, 3)) << " hours ";
    if (place(interval, 4) != 0) cout << to_string(place(interval, 4)) << " minutes ";
    if (place(interval, 5) != 0) cout << to_string(place(interval, 5)) << " seconds";
    cout << ".\n";
}

void summarize(vector<Transaction> &record, vector<Account> &reg, uint64_t ts) {
    uint64_t begin, end;
    begin = uint64_t(place(ts, 0)*uint8_t(10000000000));
    end = begin + uint64_t(10000000000);
    cout << "Summary of [" << to_string(begin) << ", " << to_string(end) << "):\n";
    uint64_t current = record.front().timestamp;
    size_t i = 0;
    while (current < begin && i < record.size()) {
        current = record[i].timestamp;
        i++;
    } size_t count = 0;
    uint32_t revenue = 0;
    while (current < end && i < record.size()) {
        cout << record[i];
        Account* sender = findAccount(reg, record[i].sender);
        uint32_t fee = record[i].amount/uint32_t(100);
        fee = min(fee, uint32_t(450));
        fee = max(fee, uint32_t(10));
        if (sender->loyalty(record[i].timestamp)) {
            fee = (fee * 3) / 4;
        } revenue += fee;
        i++;
        count++;
    } 
    cout << "There were a total of " << to_string(count) << " transactions, 281Bank has collected "
         << to_string(revenue) << " dollars in fees.\n";
}