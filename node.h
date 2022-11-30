#include<bits/stdc++.h>
#include"transaction.h"
#include"block.h"
#include"sha256.h"
using namespace std;

class FullNode;
class UserNode;


using namespace std;

class FullNode{
public:
    map<int, vector<transaction*>> fIdentifierTxHistory;
    vector<string> adj;
    string name;
    Block* myBlockChain;
    Block* workingBlock;
    queue<transaction*> txPool;
    map<transaction*, bool> txCheck;
    map<Block*, bool> blcCheck;
    bool isWorking=false;


    void initBlc();
    int increaseNonce(); // doMining시에 이용
    string makeMerkle(int idx); // 처음 merkle 트리 만드는 용도
    void changeTx(); // tx 변경해서 다른거 넣을때
    string updateMerkle(int idx); // tx 변경하고 update용도
    bool validateTX(transaction* tr); // 나한테 온 tx이 제대로 된 것인지 확인
    bool validateBlc(Block* block);
    void receiveTx(transaction* tr); // 나한테 온 tx 처리
    void receiveBlc(Block* block);
    void transferTX(transaction *tr); // 인접한 노드들에 전달
    void transferBlc(Block* block);
    void doMining(); // 채굴확인
    void doneMining();
};

class UserNode{
public:
    string parent;
    void transferTX(transaction *tr); // 부모 노드에 전달
};


#ifndef BLOCKCHAIN_NODE_H
#define BLOCKCHAIN_NODE_H

#endif //BLOCKCHAIN_NODE_H
