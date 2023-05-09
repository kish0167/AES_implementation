//
// Created by simer on 06.05.2023.
//

#ifndef CPP_AES_H
#define CPP_AES_H
#include "GF2_8.h"
#include "block128.h"

class AES {

public:
    AES(){
        block128 a;
        this->setData(a);
    }
    void setData(block128 a){
        data = a;
    }
    void encrypt(block128 key){
        block128 result;
        result=data;
        vector<block128> keySchedule;
        keySchedule = key.KeyExpansion();
       for (int i = 0; i < 10; ++i) {
            result.subBytes();
            result.shiftRows();
            result.mixColumns();
            result.addRoundKey(keySchedule[i]);
        }
        result.subBytes();
        result.shiftRows();
        result.addRoundKey(keySchedule[10]);
        data=result;
    }
    void decrypt(block128 key){
        block128 result;
        result=data;
        vector<block128> keySchedule;
        keySchedule = key.KeyExpansion();
        result.addRoundKey(keySchedule[10]);
        result.invShiftRows();
        result.invSubBytes();
        for (int i = 9; i >= 0; --i) {
            result.addRoundKey(keySchedule[i]);
            result.invMixColumns();
            result.invShiftRows();
            result.invSubBytes();
        }
        data=result;
    }
    void create(){
        block128 a;
        a.createByHand();
        this->setData(a);
    }
    void print(){
        data.print();
    }
    block128 getData(){
        return data;
    }
private:
    block128 data;
};


#endif //CPP_AES_H
