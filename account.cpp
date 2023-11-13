// Project Identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include "account.h"
#include <iostream>
#include <stdint.h>

using namespace std;

Account::Account(string id_in, uint32_t start_bal, uint32_t pin_in, uint64_t timeReg) : 
                user_id(id_in) , balance(start_bal) , PIN(pin_in) , REG_TIMESTAMP(timeReg) {}

string Account::login(string id, uint32_t p, string ip) {
    if (id == this->user_id && p == this->PIN) {
        IP_list.push_back(ip);
        return "User " + id + " logged in.\n";
    } else return "Failed to log in " + id + "\n";
}

string Account::logout(string ip) {
    bool found = false;
    for (list<string>::iterator q = IP_list.begin(); q != IP_list.end(); q++) {
        if (*q == ip) {
            found = true;
            IP_list.erase(q);
            return "User " + user_id + " logged out.\n";
        }
    }
    if (!found) return "Failed to log out " + user_id + ".\n";
    return "Failed to log out " + user_id + ".\n";
}

void Account::send(uint64_t timestamp, Account* recipient, uint32_t amount, size_t ID) {
    Transaction temp = {user_id, amount, recipient->user_id, timestamp, ID};
    balance -= amount;
    record_o.push_back(temp);
    recipient->receive(timestamp, this, amount, ID);
}

void Account::receive(uint64_t timestamp, Account* sender, uint32_t amount, size_t ID) {
    Transaction temp = {sender->user_id, amount, user_id, timestamp, ID};
    balance += amount;
    record_i.push_back(temp);
}

string Account::name() {
    return user_id;
}

bool Account::isOnline() {
    return !IP_list.empty();
}

Account* findAccount (vector<Account> &data, string id) {
    for (size_t q = 0; q < data.size(); q++) {
        if (data[q].name() == id) return &data[q];
    } return nullptr;
}

bool Transaction::operator< (const Transaction &other) const {
    return (this->timestamp < other.timestamp);
}

ostream& operator<< (ostream& os, const Transaction &t) {
    os << t.sender + " sent " + to_string(t.amount);
    if (t.amount == 1) os << string("dollar to ");
    else os << string("dollars to ");
    os << t.recipient + " at " + to_string(t.timestamp) + ".\n";
    return os;
}

bool Account::operator< (const Account &other) const {
    return (this->user_id < other.user_id);
}

void Account::fine(uint32_t amount, uint64_t ts) {
    uint32_t fee = amount/uint32_t(100);
    fee = min(fee, uint32_t(450));
    fee = max(fee, uint32_t(10));
    if ((ts - this->REG_TIMESTAMP) > uint64_t(50000000000)) {
        fee = (fee * 3) / 4;
    } 
    this->balance -= fee;
}

void Account::fine(uint32_t amount, uint64_t ts, Account* recip) {
    uint32_t fee = amount/uint32_t(100);
    fee = min(fee, uint32_t(450));
    fee = max(fee, uint32_t(10));
    if (loyalty(ts)) {
        fee = (fee * 3) / 4;
    } 
    this->balance -= (fee/uint32_t(2) + amount%uint32_t(2));
    recip->balance -= (fee/uint32_t(2));
}

bool Account::loyalty(uint64_t ts) {
    return ((ts - this->REG_TIMESTAMP) >= uint64_t(50000000000));
}

uint8_t placeV(uint64_t num, uint8_t p) {
    uint8_t val = 0;
    val = (num/uint64_t(10^(10-2*int(p))))%100;
    return val;
}

void Account::report() {
    cout << "Customer " << user_id << " account summary:\n"
         << "Balance: $" << to_string(balance) << endl
         << "Total # of transactions: " << to_string(record_i.size() + record_o.size()) << endl
         << "Incoming " << to_string(record_i.size()) << ":";
    for(size_t g = 0; g < record_i.size(); g++) {
        cout << endl << record_i[g];
    } cout << endl << "Outgoing " << to_string(record_o.size()) << ":";
    for(size_t g = 0; g < record_o.size(); g++) {
        cout << endl << record_o[g];
    }
}

void tsOut (uint64_t ts) {
    uint8_t g = 0;
    while (placeV(ts, g) == 0 && g <= 5) {
        g++;
    }
    switch (g) {
        case 0 :
            cout << to_string(placeV(ts, 0)) << " year";
            if (placeV(ts, 0) == 1) cout << " ";
            else cout << "s ";
            cout << to_string(placeV(ts, 1)) << " month";
            if (placeV(ts, 1) == 1) cout << " ";
            else cout << "s ";
            cout << to_string(placeV(ts, 2)) << " day";
            if (placeV(ts, 2) == 1) cout << " ";
            else cout << "s ";
            cout << to_string(placeV(ts, 3)) << " hour";
            if (placeV(ts, 3) == 1) cout << " ";
            else cout << "s ";
            cout << to_string(placeV(ts, 4)) << " minute";
            if (placeV(ts, 4) == 1) cout << " ";
            else cout << "s ";
            break;
        case 1 :
            cout << to_string(placeV(ts, 1)) << " month";
            if (placeV(ts, 1) == 1) cout << " ";
            else cout << "s ";
            cout << to_string(placeV(ts, 2)) << " day";
            if (placeV(ts, 2) == 1) cout << " ";
            else cout << "s ";
            cout << to_string(placeV(ts, 3)) << " hour";
            if (placeV(ts, 3) == 1) cout << " ";
            else cout << "s ";
            cout << to_string(placeV(ts, 4)) << " minute";
            if (placeV(ts, 4) == 1) cout << " ";
            else cout << "s ";
            break;
        case 2 :
            cout << to_string(placeV(ts, 2)) << " day";
            if (placeV(ts, 2) == 1) cout << " ";
            else cout << "s ";
            cout << to_string(placeV(ts, 3)) << " hour";
            if (placeV(ts, 3) == 1) cout << " ";
            else cout << "s ";
            cout << to_string(placeV(ts, 4)) << " minute";
            if (placeV(ts, 4) == 1) cout << " ";
            else cout << "s ";
            break;
        case 3 :
            cout << to_string(placeV(ts, 3)) << " hour";
            if (placeV(ts, 3) == 1) cout << " ";
            else cout << "s ";
            cout << to_string(placeV(ts, 4)) << " minute";
            if (placeV(ts, 4) == 1) cout << " ";
            else cout << "s ";
            break;
        case 4 :
            cout << to_string(placeV(ts, 4)) << " minute";
            if (placeV(ts, 4) == 1) cout << " ";
            else cout << "s ";
            break;
        default :
            break;
    } cout << to_string(placeV(ts, 5)) << " second";
    if (placeV(ts, 5) == 1) cout << ".\n";
    else cout << "s.\n";
}