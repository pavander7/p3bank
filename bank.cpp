// Project Identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include <algorithm>
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include "account.h"

using namespace std;

void listOut(vector<Transaction> &record, uint64_t begin, uint64_t end) {
    uint64_t current = record.front().timestamp;
    size_t i = 0;
    while (current < begin && i < record.size()) {
        i++;
        current = record[i].timestamp;
    } size_t count = 0;
    while (current < end && i < record.size()) {
        cout << record[i] << endl;
        i++;
        count++;
        current = record[i].timestamp;
    }
    if (count == 1) {
        cout << "There was " + to_string(count) + " transaction that was placed between time "
             + to_string(begin) + " to " + to_string(end) + ".\n"; 
    } else {
        cout << "There were " + to_string(count) + " transactions that were placed between time "
             + to_string(begin) + " to " + to_string(end) + ".\n"; 
    }
}

void revenue(vector<Transaction> &record, uint64_t begin, uint64_t end, unordered_map<string,Account> &reg) {
    uint64_t current = record.front().timestamp;
    size_t i = 0;
    while (current < begin && i < record.size()) {
        i++;
        current = record[i].timestamp;
    }
    uint32_t revenue = 0;
    while (current < end && i < record.size()) {
        uint32_t fee = record[i].amount/uint32_t(100);
        fee = min(fee, uint32_t(450));
        fee = max(fee, uint32_t(10));
        if (reg[record[i].sender].loyalty(record[i].timestamp)) {
            fee = (fee * 3) / 4;
        } revenue += fee;
        i++;
        current = record[i].timestamp;
    } 
    uint64_t interval = end - begin;
    if (revenue == 1) {
        cout << "281Bank has collected " << revenue << " dollar in fees over ";
    } else {
        cout << "281Bank has collected " << revenue << " dollars in fees over ";
    }
    tsOut(interval);
    cout << endl;
}

void summarize(vector<Transaction> &record, unordered_map<string,Account> &reg, uint64_t ts) {
    uint64_t begin, end;
    begin = ts / uint64_t(1000000) * uint64_t(1000000);
    end = begin + uint64_t(1000000);
    cout << "Summary of [" << to_string(begin) << ", " << to_string(end) << "):\n";
    uint64_t current = record.front().timestamp;
    size_t i = 0;
    while (current < begin && i < record.size()) {
        i++;
        current = record[i].timestamp;
    } size_t count = 0;
    uint32_t revenue = 0;
    while (current < end && i < record.size()) {
        cout << record[i] << endl;
        uint32_t fee = record[i].amount/uint32_t(100);
        fee = min(fee, uint32_t(450));
        fee = max(fee, uint32_t(10));
        if (reg[record[i].sender].loyalty(record[i].timestamp)) {
            fee = (fee * 3) / 4;
        } revenue += fee;
        i++;
        count++;
        current = record[i].timestamp;
    } 
    if (count == 1) {
        cout << "There was a total of " << to_string(count) << " transaction, 281Bank has collected ";
    } else {
        cout << "There were a total of " << to_string(count) << " transactions, 281Bank has collected ";
    } if (revenue == 1) {
        cout << to_string(revenue) << " dollar in fees.\n";
    } else {
        cout << to_string(revenue) << " dollars in fees.\n";
    }
}

uint64_t clean(string ts) {
    string result;
    for (size_t c = 0; c < ts.size(); c++) {
        if (ts[c] != ':') {
            result.push_back(ts[c]);
        }
    } 
    return uint64_t(strtoull(result.c_str(), nullptr, 10));
}

int main (int argc, char* argv[]) {
    //getoptlong time
    ios_base::sync_with_stdio(false);

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
    unordered_map<string, Account> registrations;

    if (!reg.is_open()) {
        cerr << "Registration file failed to open.\n";
        exit(1);
    }

    while (getline(reg, line)) {
        stringstream s(line);
        string ts, id, p, bal;
        getline(s, ts, '|');
        getline(s, id, '|');
        getline(s, p, '|');
        getline(s, bal, '|');
        uint64_t time = clean(ts);
        registrations[id] = Account(id, uint32_t(stoi(bal)), uint32_t(stoi(p)), time);
    }

    vector<Transaction> record;
    priority_queue<Transaction> buffer;

    size_t TransNum = 0;
    uint64_t currentTime = 0;
    while(getline(cin, line)) {
        if (line[0] != '#') {
            stringstream s(line);
            string start;
            s >> start;
            if (start == "login") {
                string id, p, ip, temp;
                s >> id >> p >> ip;
                temp = registrations[id].login(id, uint32_t(stoi(p)), ip);
                if (verbose) cout << temp;
            } else if (start == "out") {
                string id, ip, temp;
                s >> id >> ip;
                temp = registrations[id].logout(ip);
                if (verbose) cout << temp;
            } else if (start == "place") {
                string ts, ip, send, recip, amount, exec, share;
                s >> ts >> ip >> send >> recip >> amount >> exec >> share;
                uint64_t timestamp = clean(ts);
                uint64_t execTime = clean(exec);
                if (execTime < timestamp) {
                    cerr << "Invalid decreasing timestamp in 'place' command.\n";
                    exit(1);
                } if (timestamp < currentTime || execTime < currentTime) {
                    cerr << "You cannot have an execution date before the current timestamp.\n";
                    exit(1);
                } currentTime = timestamp;
                if ((execTime - timestamp) > uint64_t(3000000)) {
                    if (verbose) cout << "Select a time less than three days in the future.\n";
                } else if (registrations.count(send) == 0) {
                    if (verbose) cout << "Sender " << send << " does not exist.\n";
                } else if (registrations.count(recip) == 0) {
                    if (verbose) cout << "Recipient " << recip << " does not exist.\n";
                } else if (!registrations[send].exists(execTime) || !registrations[recip].exists(execTime)) {
                    if (verbose) cout << "At the time of execution, sender and/or recipient have not registered.\n";
                } else if (!registrations[send].isOnline()) {
                    if (verbose) cout << "Sender " << send << " is not logged in.\n";
                } else if (!registrations[send].isOnline(ip)) {
                    if (verbose) cout << "Fraudulent transaction detected, aborting request.\n";
                } else {
                    Transaction temp = {send, uint32_t(stoi(amount)), recip, execTime, TransNum, share};
                    if (!buffer.empty()) {
                        while (buffer.top().timestamp <= timestamp) {
                            uint32_t sum_send = 0;
                            uint32_t sum_recip = 0;
                            if (buffer.top().so == "o") {
                                sum_send = buffer.top().amount + registrations[buffer.top().sender].fee(buffer.top().amount, buffer.top().timestamp);
                            } else {
                                uint32_t f = registrations[buffer.top().sender].fee(buffer.top().amount, buffer.top().timestamp);
                                sum_send = sum_send + buffer.top().amount + f/uint32_t(2);
                                sum_recip = f/uint32_t(2);
                            } if (registrations[buffer.top().sender].canAfford(sum_send) && registrations[buffer.top().recipient].canAfford(sum_recip)) { //check for funds
                                Transaction trans = buffer.top();
                                registrations[buffer.top().sender].send(trans, &registrations[buffer.top().recipient]);
                                if (buffer.top().so == "o") {
                                    registrations[buffer.top().sender].fine(buffer.top().amount, buffer.top().timestamp);
                                } else registrations[buffer.top().sender].fine(buffer.top().amount, buffer.top().timestamp, &registrations[buffer.top().recipient]);
                                record.push_back(buffer.top());
                                if (verbose) cout << "Transaction executed at " << buffer.top().timestamp << ": $" << buffer.top().amount << " from "
                                    << buffer.top().sender << " to " << buffer.top().recipient <<  ".\n";
                            } else {
                                if (verbose) cout << "Insufficient funds to process transaction " << buffer.top().ID << ".\n";
                            } 
                            buffer.pop();
                            if (buffer.empty()) {
                                break;
                            }
                        }
                    }
                    buffer.push(temp);
                    if (verbose) cout << "Transaction placed at " << timestamp << ": $" << amount << " from " 
                             << send << " to " << recip << " at " << execTime << ".\n";
                    TransNum++;
                }
            } else if (start == "$$$") {
                while (!buffer.empty()) {
                    uint32_t sum_send = 0;
                    uint32_t sum_recip = 0;
                    if (buffer.top().so == "o") {
                        sum_send = buffer.top().amount + registrations[buffer.top().sender].fee(buffer.top().amount, buffer.top().timestamp);
                    } else {
                        uint32_t f = registrations[buffer.top().sender].fee(buffer.top().amount, buffer.top().timestamp);
                        sum_send = sum_send + buffer.top().amount + f/uint32_t(2);
                        sum_recip = f/uint32_t(2);
                    } if (registrations[buffer.top().sender].canAfford(sum_send) && registrations[buffer.top().sender].canAfford(sum_recip)) { //check for funds
                        Transaction trans = buffer.top();
                        registrations[buffer.top().sender].send(trans, &registrations[buffer.top().recipient]);
                        if (buffer.top().so == "o") {
                        registrations[buffer.top().sender].fine(buffer.top().amount, buffer.top().timestamp);
                        } else registrations[buffer.top().sender].fine(buffer.top().amount, buffer.top().timestamp, &registrations[buffer.top().recipient]);
                        record.push_back(buffer.top());
                        if (verbose) cout << "Transaction executed at " << buffer.top().timestamp << ": $" << buffer.top().amount << " from "
                            << buffer.top().sender << " to " << buffer.top().recipient <<  ".\n";
                    } else {
                        if (verbose) cout << "Insufficient funds to process transaction " << buffer.top().ID << ".\n";
                    } 
                    buffer.pop();
                }
                break;
            } else {
                cerr << "Error: Reading from cin has failed\n";
                exit(1);
            }
        }
    }

    sort(record.begin(), record.end());

    while(getline(cin,line)) {
        stringstream s(line);
        string start;
        s >> start;
        if (start == "l") {
            string b, e, junk;
            uint64_t begin, end;
            getline(s, junk, ' ');
            getline(s, b, ' ');
            getline(s, e, ' ');
            begin = clean(b);
            end = clean(e);
            //cout << begin << " " << end << endl;
            listOut(record, begin, end);
        } else if (start == "r") {
            string b, e, junk;
            uint64_t begin, end;
            getline(s, junk, ' ');
            getline(s, b, ' ');
            getline(s, e, ' ');
            begin = clean(b);
            end = clean(e);
            //cout << begin << " " << end << endl;
            revenue(record, begin, end, registrations);
        } else if (start == "h") {
            string id;
            s >> id;
            if (registrations.count(id) == 0) {
                cout << "User " << id << " does not exist.\n";
            } else {
                registrations[id].report();
            }
        } else if (start == "s") {
            string ts;
            uint64_t time;
            s >> ts;
            time = clean(ts);
            summarize(record, registrations, time);
        } else {
            cerr << "Error: Reading from cin has failed\n";
            exit(1);
        }
    }
}