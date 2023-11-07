// Project Identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include <string>

using namespace std;

struct Transaction {
    string sender;
    uint32_t amount;
    string recipient;
    uint64_t timestamp;
};