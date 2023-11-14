// Project Identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include "account.h"
#include <iostream>
#include <stdint.h>

using namespace std;

Account::Account(string id_in, uint32_t start_bal, uint32_t pin_in, uint64_t timeReg) : 
                user_id(id_in) , balance(start_bal) , PIN(pin_in) , REG_TIMESTAMP(timeReg) {}

Account::Account(const Account &other) : user_id(other.user_id) , balance(other.balance) , 
                PIN(other.PIN) , REG_TIMESTAMP(other.REG_TIMESTAMP) {}

Account::Account() {
    user_id = "";
    balance = 0;
    PIN = 0;
    REG_TIMESTAMP = 0;
}

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
    Transaction temp = {user_id, amount, recipient->user_id, timestamp, ID, ""};
    balance -= amount;
    record_o.push_back(temp);
    recipient->receive(timestamp, this, amount, ID);
}

void Account::receive(uint64_t timestamp, Account* sender, uint32_t amount, size_t ID) {
    Transaction temp = {sender->user_id, amount, user_id, timestamp, ID, ""};
    balance += amount;
    record_i.push_back(temp);
}

string Account::name() {
    return user_id;
}

bool Account::isOnline() {
    return !IP_list.empty();
}

bool Account::isOnline(string ip) {
    for (list<string>::iterator q = IP_list.begin(); q != IP_list.end(); q++) {
        if (*q == ip) return true;
    } return false;
}

Account* findAccount (vector<Account> &data, string id) {
    for (size_t q = 0; q < data.size(); q++) {
        if (data[q].name() == id) return &data[q];
    } return nullptr;
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
    this->balance -= (fee/uint32_t(2) + amount%uint32_t(2));
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

uint8_t placeV(uint64_t num, uint8_t p) {
    string number = to_string(num);
    cout << p << endl;
    while (number.size() < 12) {
        number = "0" + number;
    }
    cout << number.size() << " length (12 expected)" << endl;
    uint8_t val = uint8_t(number[uint8_t(2)*p]*uint8_t(10) + number[uint8_t(2)*p + uint8_t(1)]);
    cout << num << " place " << p << " = " << val << endl;
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
    } cout << endl;
}

void tsOut (uint64_t ts) {
    string number = to_string(ts);
    if (number.size()%2 == 1) {
        number = "0" + number;
    } 
    if (number.size() == 12) {
        cout << (number[0] - 48)*10 + (number[1] - 48) << " year";
        if ((number[0] - 48)*10 + (number[1] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[2] - 48)*10 + (number[3] - 48) << " month";
        if ((number[2] - 48)*10 + (number[3] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[4] - 48)*10 + (number[5] - 48) << " day";
        if ((number[4] - 48)*10 + (number[5] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[6] - 48)*10 + (number[7] - 48) << " hour";
        if ((number[6] - 48)*10 + (number[7] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[8] - 48)*10 + (number[9] - 48) << " minute";
        if ((number[8] - 48)*10 + (number[9] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[10] - 48)*10 + (number[11] - 48) << " second";
        if ((number[10] - 48)*10 + (number[11] - 48) == 1) cout << ".";
        else cout << "s.";
    } else if (number.size() == 10) {
        cout << (number[0] - 48)*10 + (number[1] - 48) << " month";
        if ((number[0] - 48)*10 + (number[1] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[2] - 48)*10 + (number[3] - 48) << " day";
        if ((number[2] - 48)*10 + (number[3] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[4] - 48)*10 + (number[5] - 48) << " hour";
        if ((number[4] - 48)*10 + (number[5] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[6] - 48)*10 + (number[7] - 48) << " minute";
        if ((number[6] - 48)*10 + (number[7] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[8] - 48)*10 + (number[9] - 48) << " second";
        if ((number[8] - 48)*10 + (number[9] - 48) == 1) cout << ".";
        else cout << "s.";
    } else if (number.size() == 8) {
        cout << (number[0] - 48)*10 + (number[1] - 48) << " day";
        if ((number[0] - 48)*10 + (number[1] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[2] - 48)*10 + (number[3] - 48) << " hour";
        if ((number[2] - 48)*10 + (number[3] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[4] - 48)*10 + (number[5] - 48) << " minute";
        if ((number[4] - 48)*10 + (number[5] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[6] - 48)*10 + (number[7] - 48) << " second";
        if ((number[6] - 48)*10 + (number[7] - 48) == 1) cout << ".";
        else cout << "s.";
    } else if (number.size() == 6) {
        cout << (number[0] - 48)*10 + (number[1] - 48) << " hour";
        if ((number[0] - 48)*10 + (number[1] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[2] - 48)*10 + (number[3] - 48) << " minute";
        if ((number[2] - 48)*10 + (number[3] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[4] - 48)*10 + (number[5] - 48) << " second";
        if ((number[4] - 48)*10 + (number[5] - 48) == 1) cout << " ";
        else cout << "s ";
    } else if (number.size() == 4) {
        cout << (number[0] - 48)*10 + (number[1] - 48) << " minute";
        if ((number[0] - 48)*10 + (number[1] - 48) == 1) cout << " ";
        else cout << "s ";
        cout << (number[2] - 48)*10 + (number[3] - 48) << " second";
        if ((number[2] - 48)*10 + (number[3] - 48) == 1) cout << ".";
        else cout << "s.";
    } else {
        cout << (number[0] - 48)*10 + (number[1] - 48) << " second";
        if ((number[0] - 48)*10 + (number[1] - 48) == 1) cout << ".";
        else cout << "s.";
    } 
}

bool Account::exists(uint64_t ts) {
    return (ts >= REG_TIMESTAMP);
}