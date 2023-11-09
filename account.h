// Project Identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include <string>
#include <vector>
#include <list>
#include "transaction.h"

using namespace std;

class Account {
public:
    Account(string id_in, uint32_t start_bal, uint32_t pin_in, uint64_t timeReg);

    string login(string id, uint32_t p, string ip);
    string logout(string ip);

    void send(uint64_t timestamp, Account* recipient, uint32_t amount);
    void receive(uint64_t timestamp, Account* sender, uint32_t amount);
    void fine(uint32_t amount, uint64_t ts);
    void fine(uint32_t amount, uint64_t ts, Account* recipient);

    string name();
    bool isOnline();
    bool loyalty(uint64_t ts);
    void report();

    bool operator< (const Account &other) const;

private:
    string user_id;
    uint32_t balance;
    uint32_t PIN;
    uint64_t REG_TIMESTAMP;
    vector<Transaction> record_o;
    vector<Transaction> record_i;
    list<string> IP_list;
};

uint8_t place(uint64_t num, uint8_t p);
Account* findAccount (vector<Account> &data, string id);