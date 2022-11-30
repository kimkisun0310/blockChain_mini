#include "transaction.h"


transaction::transaction(int tId, int tModelNo, int tPrice, string tInput, string tOutput, string tManufacturedDate, string tTradingDate, string tOthers){
    id = tId;
    modelNo = tModelNo;
    price = tPrice;
    input = tInput;
    output = tOutput;
    manufacturedDate = tManufacturedDate;
    tradingDate = tTradingDate;
    others = tOthers;
}

