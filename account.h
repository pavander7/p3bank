// Project Identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include <string>
#include <vector>
#include <unordered_set>
#include "transaction.h"

using namespace std;

class Account {
public:
    Account(string id_in, uint32_t start_bal, uint32_t pin_in, uint64_t timeReg);
    Account();

    string login(string id, uint32_t p, string ip);
    string logout(string ip);

    void send(Transaction &trans, Account* recipient);
    void receive(Transaction &trans);
    
    void fine(uint32_t amount, uint64_t ts);
    void fine(uint32_t amount, uint64_t ts, Account* recipient);
    uint32_t fee(uint32_t amount, uint64_t ts);
    bool canAfford(uint32_t amount);

    bool isOnline();
    bool isOnline(string ip);
    bool exists(uint64_t ts);
    bool loyalty(uint64_t ts);
    void report();

    bool operator< (const Account &other) const;

private:
    string user_id;
    uint32_t balance;
    uint32_t PIN;
    uint64_t REG_TIMESTAMP;
    vector<Transaction*> record_o;
    vector<Transaction*> record_i;
    unordered_set<string> IP_list;
};

uint8_t placeV(uint64_t num, uint8_t p);
void tsOut(uint64_t ts);
Account* findAccount (vector<Account> &data, string id);