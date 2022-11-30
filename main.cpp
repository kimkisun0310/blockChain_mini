#include<bits/stdc++.h>
#include"node.h"
#include"sha256.h"
#include<thread>
using namespace std;

map<int, vector<transaction*>> identifierTxHistory;
vector<string> fullNodes;
map<string, FullNode*> hashFN;
map<string, UserNode*> hashUN;
queue<tuple<int, string, transaction*, Block*> > workingQueue;

void init(){
    // TODO : SET TOPOLOGY OF NODES
}

void working(){
    while(1){
        while(!workingQueue.empty()){
            auto &[flag, node, tx, blc] = workingQueue.front();
            workingQueue.pop();
            FullNode *FN = hashFN[node];
            if(flag==0){
                FN->receiveTx(tx);
            }
            else if(flag==1){
                FN->receiveBlc(blc);
            }
            else if(flag==2){
                FN->doMining();
            }
        }
    }
}

void printTxInfo(transaction *tx){
    cout << "trID " << tx->id << "\n";
    cout << "input =  " << tx->input << "\n";
    cout << "modelNo =  " << tx->modelNo << "\n";
    cout << "manufactured date =  " << tx->manufacturedDate << "\n";
    cout << "price =  " << tx->price << "\n";
    cout << "trading date =  " << tx->tradingDate << "\n";
    cout << "others =  " << tx->others << "\n";
}

void printBlockInfo(Block *block){
    if(block->blockNo==0){
        cout << block->blockNo << "\n";
        cout << "HEADER : " << "\n";
        cout << "blockNo : " << block->blockNo <<"\n";
        cout << "prevHash : " << block->prevHash <<"\n";
        cout << "prevDataHash : " << block->prevDataHash <<"\n";
        cout << "transactions : " << "\n";
        for(auto tx : block->txs)cout << tx->id << " ";
        cout << "\n";
    }
    printBlockInfo(block->prevHash);
    cout << block->blockNo << "\n";
    cout << "HEADER : " << "\n";
    cout << "blockNo : " << block->blockNo <<"\n";
    cout << "prevHash : " << block->prevHash <<"\n";
    cout << "prevDataHash : " << block->prevDataHash <<"\n";
    cout << "transactions : " << "\n";
    for(auto tx : block->txs)cout << tx->id << " ";
    cout << "\n";
}

void masterProcess(){
    string input;
    while(cin >> input){
        if(input=="snapshot"){
            string snapType;
            cin >> snapType;
            if(snapType=="myBlockChain"){
                string all;
                cin >> all;
                if(all=="ALL"){
                    for(auto fullNode : fullNodes){
                        FullNode *FN = hashFN[fullNode];
                        cout << fullNode << ": \n";
                        printBlockInfo(FN->myBlockChain);
                    }
                }
            }
            else if(snapType=="trPool"){
                string fi;
                cin >> fi;
                FullNode *fullNode = hashFN[fi];
                int size = fullNode->txPool.size();
                for(int i=0; i<size; i++){
                    transaction *tx = fullNode->txPool.front();
                    fullNode->txPool.pop();
                    printTxInfo(tx);
                    fullNode->txPool.push(tx);
                }
            }
        }
        else if(input=="verifyLastTr"){
            string fi;
            cin >> fi;
            FullNode *fullNode = hashFN[fi];
            if(fullNode->workingBlock->txs.size()>=8){
                transaction *tx = fullNode->workingBlock->txs[7];
                transaction *lastTx = fullNode->fIdentifierTxHistory[tx->id][fullNode->fIdentifierTxHistory.size()-1];
                cout << "fi : " << tx->id << "\n";
                cout << "last transaction's output : " << lastTx->output << "\n";
                cout << fi <<"'s input : " << tx->input << "\n";
                cout << "last transaction's\n";
                cout << "identifier = "  << lastTx->id << " ";
                cout << "modelNo = " << lastTx->modelNo << " ";
                cout << "manufactured date = " << lastTx->manufacturedDate << "\n";
                cout << fi << "'s\n";
                cout << "identifier = "  << tx->id << " ";
                cout << "modelNo = " << tx->modelNo << " ";
                cout << "manufactured date = " << tx->manufacturedDate << "\n";
                // TODO: SIGNIFICANT!
            }

        }
        else if(input=="trace"){
            string identifier, num;
            cin >> identifier >> num;
            int id = stoi(identifier);
            int k = identifierTxHistory[id].size();
            if(num=="ALL") {
                for(int i=k-1; i>=0; i--)printTxInfo(identifierTxHistory[id][i]);
            }
            else{
                int n = stoi(num);
                k = min(k, n);
                for(int i=k-1; i>=k-n; i--)printTxInfo(identifierTxHistory[id][i]);
            }
        }
        else if(input =="Q"){
            // Quit PROGRAM
        }
    }
}


int main() {
    thread _masterThread(masterProcess);
    thread _work(working);
    _work.join();
    _masterThread.join();
    return 0;
}
