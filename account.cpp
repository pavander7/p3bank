// Project Identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include "account.h"
#include <stdint.h>

using namespace std;

Account::Account(string id_in, uint32_t start_bal, uint32_t pin_in, uint64_t timeReg) : 
                user_id(id_in) , balance(start_bal) , PIN(pin_in) , REG_TIMESTAMP(timeReg) {}

string Account::login(string id, uint32_t p, size_t ip) {
    if (id == this->user_id && p == this->PIN) {
        IP_list.push_back(ip);
        return "User " + id + " logged in.\n";
    } else return "Failed to log in " + id + "\n";
}

string Account::logout(size_t ip) {
    bool found = false;
    for (list<size_t>::iterator q = IP_list.begin(); q != IP_list.end(); q++) {
        if (*q == ip) {
            found = true;
            IP_list.erase(q);
            return "User " + user_id + " logged out.\n";
        }
    }
    if (!found) return "Failed to log out " + user_id + ".\n";
}

void Account::send(uint64_t timestamp, Account* recipient, uint32_t amount) {
    Transaction temp = {user_id, amount, recipient->user_id, timestamp};
    balance -= amount;
    record.push_back(temp);
    recipient->receive(amount);
}

void Account::receive(uint32_t amount) {
    balance += amount;
}