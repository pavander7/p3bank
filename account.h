// Project Identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include <string>
#include <vector>
#include <list>
#include "transaction.h"

using namespace std;

uint8_t place(uint64_t num, uint8_t p);

class Account {
public:
    Account(string id_in, uint32_t start_bal, uint32_t pin_in, uint64_t timeReg);

    string login(string id, uint32_t p, size_t ip);
    string logout(size_t ip);

    void send(uint64_t timestamp, Account* recipient, uint32_t amount);
    void receive(uint32_t amount);

private:
    string user_id;
    uint32_t balance;
    uint32_t PIN;
    uint64_t REG_TIMESTAMP;
    vector<Transaction> record;
    list<size_t> IP_list;
};