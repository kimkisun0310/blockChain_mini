#include<bits/stdc++.h>
#include"transaction.h"
#include"sha256.h"
using namespace std;


struct Block{
public:
    int blockNo;
    struct Block* prevHash;
    string prevDataHash;
    int nonce = 0;
    int cnt = 0;
    vector<transaction*> txs;
    string merkleTree[16];
};

string serializeBlc(Block *block);


#ifndef BLOCKCHAIN_BLOCK_H
#define BLOCKCHAIN_BLOCK_H

#endif //BLOCKCHAIN_BLOCK_H
