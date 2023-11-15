// Project Identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include "account.h"
#include <iostream>
#include <stdint.h>

using namespace std;

Account::Account(string id_in, uint32_t start_bal, uint32_t pin_in, uint64_t timeReg) : 
                user_id(id_in) , balance(start_bal) , PIN(pin_in) , REG_TIMESTAMP(timeReg) {}

Account::Account() {
    balance = 0;
    PIN = 0;
    REG_TIMESTAMP = 0;
}

string Account::login(string id, uint32_t p, string ip) {
    if (id == this->user_id && p == this->PIN) {
        IP_list.insert(ip);
        return "User " + id + " logged in.\n";
    } else return "Failed to log in " + id + ".\n";
}

string Account::logout(string ip) {
    if (IP_list.count(ip) != 0) {
        IP_list.erase(ip);
        return "User " + user_id + " logged out.\n";
    } else return "Failed to log out " + user_id + ".\n";
    return "Failed to log out " + user_id + ".\n";
}

void Account::send(Transaction &trans, Account* recipient) {
    balance -= trans.amount;
    record_o.push_back(&trans);
    recipient->receive(trans);
}

void Account::receive(Transaction &trans) {
    balance += trans.amount;
    record_i.push_back(&trans);
}

bool Account::isOnline() {
    return !IP_list.empty();
}

bool Account::isOnline(string ip) {
    return (IP_list.count(ip) != 0);
}

bool Transaction::operator< (const Transaction &other) const {
    if (this->timestamp != other.timestamp) {
        return (this->timestamp < other.timestamp);
    } else return (this->ID < other.ID);
}

ostream& operator<< (ostream& os, const Transaction &t) {
    os << t.ID << ": " << t.sender + " sent " + to_string(t.amount);
    if (t.amount == 1) os << string(" dollar to ");
    else os << string(" dollars to ");
    os << t.recipient + " at " + to_string(t.timestamp) + ".";
    return os;
}

bool Account::operator< (const Account &other) const {
    return (this->user_id < other.user_id);
}

void Account::fine(uint32_t amount, uint64_t ts) {
    uint32_t fee = this->fee(amount, ts);
    this->balance -= fee;
}

void Account::fine(uint32_t amount, uint64_t ts, Account* recip) {
    uint32_t fee = this->fee(amount, ts);
    this->balance -= (fee/uint32_t(2)); //+ amount%uint32_t(2));
    recip->balance -= (fee/uint32_t(2));
}

uint32_t Account::fee(uint32_t amount, uint64_t ts) {
    uint32_t fee = amount/uint32_t(100);
    fee = min(fee, uint32_t(450));
    fee = max(fee, uint32_t(10));
    if (loyalty(ts)) {
        fee = (fee * 3) / 4;
    } return fee;
}

bool Account::canAfford(uint32_t amount) {
    return (this->balance >= amount);
}

bool Account::loyalty(uint64_t ts) {
    return ((ts - this->REG_TIMESTAMP) >= uint64_t(50000000000));
}

void Account::report() {
    cout << "Customer " << user_id << " account summary:\n"
         << "Balance: $" << to_string(balance) << endl
         << "Total # of transactions: " << to_string(record_i.size() + record_o.size()) << endl
         << "Incoming " << to_string(record_i.size()) << ":";
    for(size_t g = 0; g < size_t(min(int(record_i.size()), 10)); g++) {
        cout << endl << record_i[g];
    } cout << endl << "Outgoing " << to_string(record_o.size()) << ":";
    for(size_t g = 0; g < size_t(min(int(record_o.size()), 10)); g++) {
        cout << endl << record_o[g];
    } cout << endl;
}

void tsOut (uint64_t ts) {
    string number = to_string(ts);
    while (number.size() < 12) {
        number = "0" + number;
    }
    if ((number[0] - 48)*10 + (number[1] - 48) != 0) {
        cout << (number[0] - 48)*10 + (number[1] - 48) << " year";
        if ((number[0] - 48)*10 + (number[1] - 48) == 1) cout << " ";
        else cout << "s ";
    } if ((number[2] - 48)*10 + (number[3] - 48) != 0) {
        cout << (number[2] - 48)*10 + (number[3] - 48) << " month";
        if ((number[2] - 48)*10 + (number[3] - 48) == 1) cout << " ";
        else cout << "s ";
    } if ((number[4] - 48)*10 + (number[5] - 48) != 0) {
        cout << (number[4] - 48)*10 + (number[5] - 48) << " day";
        if ((number[4] - 48)*10 + (number[5] - 48) == 1) cout << " ";
        else cout << "s ";
    } if ((number[6] - 48)*10 + (number[7] - 48) != 0) {
        cout << (number[6] - 48)*10 + (number[7] - 48) << " hour";
        if ((number[6] - 48)*10 + (number[7] - 48) == 1) cout << " ";
        else cout << "s ";
    } if ((number[8] - 48)*10 + (number[9] - 48) != 0) {
        cout << (number[8] - 48)*10 + (number[9] - 48) << " minute";
        if ((number[8] - 48)*10 + (number[9] - 48) == 1) cout << " ";
        else cout << "s ";
    } if ((number[10] - 48)*10 + (number[11] - 48) != 0) {
        cout << (number[10] - 48)*10 + (number[11] - 48) << " second";
        if ((number[10] - 48)*10 + (number[11] - 48) == 1) cout << ".";
        else cout << "s.";
    }
}

bool Account::exists(uint64_t ts) {
    return (ts >= REG_TIMESTAMP);
}