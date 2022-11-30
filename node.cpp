#include "node.h"

extern queue<tuple<int, string, transaction*, Block*> > workingQueue;
extern map<int, vector<transaction*>> identifierTxHistory;


void FullNode::initBlc() {
    Block *newBlc;
    workingBlock = newBlc;
    workingBlock->blockNo=myBlockChain->blockNo+1;
    workingBlock->nonce=0;
    workingBlock->cnt=0;
    workingBlock->prevHash = myBlockChain;
    workingBlock->prevDataHash = serializeBlc(myBlockChain);
    if(txPool.size()>=8){
        for(int i=0; i<8; i++){
            transaction* nTr = txPool.front();
            txPool.pop();
            workingBlock->txs.push_back(nTr);
        }
        isWorking = true;
        makeMerkle(1);
        doMining();
    }
}

int FullNode::increaseNonce() {
    if(workingBlock->nonce==INT_MAX)return -1;
    else workingBlock->nonce++;
    return workingBlock->nonce;
}

string FullNode::makeMerkle(int idx) {
    if(idx<8) return workingBlock->merkleTree[idx] = sha256(makeMerkle(idx*2) + makeMerkle(idx*2+1));
    else{
        transaction* p = workingBlock->txs[idx];
        const char* tx2s = reinterpret_cast<const char*>(&p); //serialize transaction
        return workingBlock->merkleTree[idx] = sha256(tx2s);
    }
}

void FullNode::changeTx() {
    transaction* newTr = txPool.front();
    txPool.pop();
    transaction* oldTr = workingBlock->txs[(workingBlock->cnt)%8];
    txPool.push(oldTr);
    workingBlock->txs[(workingBlock->cnt)%8] = newTr;
    workingBlock->cnt++;
    updateMerkle((workingBlock->cnt-1)%8);
}

string FullNode::updateMerkle(int idx) {
    if(idx>1)return workingBlock->merkleTree[idx/2] = sha256(sha256(workingBlock->merkleTree[idx/2*2])+ sha256(workingBlock->merkleTree[idx/2*2+1]));
}

void FullNode::receiveTx(transaction *tr) {
    if(txCheck[tr])return;
    if(validateTX(tr)){
        txPool.push(tr);
        txCheck[tr] = true;
        transferTX(tr);
        bool isDone = false;
        if(isWorking){
            doMining();
        }
        else if(!isWorking && txPool.size()>=8){
            for(int i=0; i<8; i++){
                transaction* nTr = txPool.front();
                txPool.pop();
                workingBlock->txs.push_back(nTr);
            }
            isWorking = true;
            makeMerkle(1);
            doMining();
        }
    }
}

void FullNode::receiveBlc(Block* block){
    if(blcCheck[block])return;
    if(validateBlc(block)){
        blcCheck[block] = true;
        transferBlc(block);
        if(block->blockNo>myBlockChain->blockNo){
            myBlockChain = block;
            initBlc();
            isWorking = false;
        }
    }
};


bool FullNode::validateTX(transaction *tr) {
    int tId = tr->id;
    int size = fIdentifierTxHistory[tId].size();
    if(fIdentifierTxHistory[tId][size-1]->output != tr->input)return false;
    if(fIdentifierTxHistory[tId][size-1]->id != tr->id)return false;
    if(fIdentifierTxHistory[tId][size-1]->modelNo != tr->modelNo)return false;
    if(fIdentifierTxHistory[tId][size-1]->manufacturedDate != tr->manufacturedDate)return false;
    // TODO: NEED TO IMPLEMENT CHECK VALIDATE SIGNIFICANT USING ECDSA
    return true;
}

bool FullNode::validateBlc(Block *block) {
    string res = serializeBlc(workingBlock);
    for(int i=0; i<4; i++)if(res[i]!=0)return false;
    if(block->blockNo!=0){
        Block *before = block->prevHash;
        while(block->blockNo!=0){
            for(auto tx : block->txs)if(!validateTX(tx))return false;
            if(serializeBlc(before)!=block->prevDataHash)return false;
            if(before->blockNo!=block->blockNo-1)return false;
            block = block->prevHash;
            before = block->prevHash;
        }
    }
    for(auto tx : block->txs)if(validateTX(tx))return false;
    return true;
}

void FullNode::transferTX(transaction *tr) {
    for(auto nxt : adj){
        workingQueue.push({0, nxt, tr, workingBlock});
    }
}

void FullNode::transferBlc(Block *block) {
    transaction* tr;
    for(auto nxt : adj){
        workingQueue.push({1, nxt, tr, block});
    }
}

void FullNode::doMining() {
    bool flag = true;
    transaction *tmp;
    int nonce = increaseNonce();
    if(nonce==-1){
        workingBlock->nonce = 0;
        if(!txPool.empty()){
            changeTx();
        }
        else{
            workingQueue.push({2, name, tmp, workingBlock});
            return;
        }
    }
    string res = serializeBlc(workingBlock);
    for(int i=0; i<4; i++){
        if(res[i]!='0')flag=false;
    }
    if(flag)doneMining();
    else{
        workingQueue.push({2, name, tmp, workingBlock});
    }
}

void FullNode::doneMining(){
    // TODO: NEED TO IMPLEMENT DO SIGNIFICANT USING ECDSA
    for(auto tx : workingBlock->txs){
        fIdentifierTxHistory[tx->id].push_back(tx);
        identifierTxHistory[tx->id].push_back(tx);
    }
    myBlockChain = workingBlock;
    transferBlc(workingBlock);
    initBlc();
}

void UserNode::transferTX(transaction *tr) {
    Block *block;
    workingQueue.push({0, parent, tr, block});
}