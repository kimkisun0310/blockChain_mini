#include<bits/stdc++.h>
using namespace std;

#ifndef BLOCKCHAIN_TRANSACTION_H
#define BLOCKCHAIN_TRANSACTION_H

struct transaction{
    int id;
    int modelNo;
    int price;
    string input;
    string output;
    string manufacturedDate;
    string tradingDate;
    string others;
    transaction(int tId, int tModelNo, int tPrice, string tInput, string tOutput, string tManufacturedDate, string tTradingDate, string tOthers);
};




#endif //BLOCKCHAIN_TRANSACTION_H
