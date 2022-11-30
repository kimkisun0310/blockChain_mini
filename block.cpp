#include"block.h"

string serializeBlc(Block *block){
    string ser = to_string(block->blockNo);
    ser+=to_string(block->nonce);
    ser+= to_string(block->cnt);
    ser = sha256(ser);
    for(transaction *t : block->txs){
        ostringstream oss;
        oss<<t;
        ser+=oss.str();
        ser=sha256(ser);
    }
    ostringstream oss;
    oss<<block->prevHash;
    ser+=oss.str();
    ser= sha256(ser);
    ser+=block->prevDataHash;
    ser = sha256(ser);
    for(int i=0; i<16; i++){
        ser += block->merkleTree[i];
        ser= sha256(ser);
    }
    return ser;
}

