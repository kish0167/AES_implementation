//
// Created by simer on 06.05.2023.
//

#include "GF2_8.h"
#include <vector>
#ifndef AES_BLOCK128_H
#define AES_BLOCK128_H

class block128 {
private:
    vector<vector<GF2_8>> byteMatrix;
    vector<GF2_8> getRow(int n){
        if(n>4 || n<0) {cout<<"wrong size of row"<<endl; return byteMatrix[0];}
        return byteMatrix[n];
    }
    void setRow(int n, vector<GF2_8> row){
        if(row.size()!=4) {cout<<"wrong size of row"<<endl; return;}
        byteMatrix[n]=row;
    }
    vector<GF2_8> gFunc(block128 lastKey,int round){
        if(round<1 || round>10) {cout<<"wrong round number"<<endl;}
        int RC[10] ={1,2,4,8,16,32,64,128,27,54};
        vector<GF2_8> newRow = lastKey.getRow(3);
        GF2_8 tmp=newRow[0];
        for (int i = 0; i < 3; ++i)
            newRow[i]=newRow[i+1];
        newRow[3]=tmp;
        for (int i = 0; i < 4; ++i)
            newRow[i].Sbox(0);
        GF2_8 rc = RC[round];
        newRow[0] = newRow[0] + rc;
        return newRow;
    }
public:
    block128(){
        vector<GF2_8> zero;
        GF2_8 tmp=0;
        for (int i = 0; i < 4; ++i) {
            zero.push_back(tmp);
        }
        for (int i = 0; i < 4; ++i) {
            byteMatrix.push_back(zero);
        }
    }
    void SetMatrix(vector<GF2_8> input){
        if (input.size()!=16) {cout<<"wrong array size"; return;}
        vector<vector<GF2_8>> tmp;
        for (int i = 0; i < 4; ++i){
            vector<GF2_8> row;
            for (int j = 0; j < 4; ++j)
                row.push_back(input[i*4+j]);
            tmp.push_back(row);
        }
        byteMatrix=tmp;
    }
    void SetMatrix(vector<vector<GF2_8>> input){
        if (input.size()!=4 || input[0].size()!=4 || input[1].size()!=4 || input[2].size()!=4 || input[3].size()!=4)
        {cout<< "wrong matrix size"; return;}
        byteMatrix = input;
    }
    vector<vector<GF2_8>> GetMatrix(){
        return byteMatrix;
    }
    vector<GF2_8> GetRow(){
        vector<GF2_8> tmp;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                tmp.push_back(byteMatrix[i][j]);
        return tmp;
    }
    void print(){
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                cout<<byteMatrix[i][j].GetHex()<<" ";
            }
            cout<<endl;
        }
    }
    vector<block128> KeyExpansion(){
        vector<block128> expandedKey;
        expandedKey.push_back(*this);
        block128 rKey, lastKey = *this;
        for (int i = 1; i < 11; ++i) {
            vector<GF2_8> sum;
            for (int k = 0; k < 4; ++k){
                GF2_8 a=0,b=0;
                a=lastKey.getRow(0)[k];
                b= gFunc(lastKey,i)[k];
                sum.push_back(a+b);
            }
            rKey.setRow(0,sum);
            for (int j = 1; j < 4; ++j) {
                for (int k = 0; k < 4; ++k)
                    sum[k]=lastKey.getRow(j)[k] + rKey.getRow(j-1)[k];
                rKey.setRow(j,sum);
            }
            lastKey=rKey;
            expandedKey.push_back(rKey);
        }
        return expandedKey;
    }
    void addRoundKey(block128 roundKey){
        block128 sum;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                sum.byteMatrix[i][j] = this->byteMatrix[i][j]+roundKey.byteMatrix[i][j];
        this->byteMatrix = sum.byteMatrix;
    }
    void subBytes(){
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                this->byteMatrix[i][j].Sbox(0);
    }
    void invSubBytes(){
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                this->byteMatrix[i][j].Sbox(1);
    }
    void shiftRows(){
        block128 shifted;
        for (int i = 0; i < 4; ++i) {
            vector<GF2_8> tmp = this->getRow(i);
            for (int j = 0; j < 4; ++j)
                shifted.byteMatrix[i][j]=tmp[(i+j)%4];
        }
        this->byteMatrix = shifted.byteMatrix;
    }
    void invShiftRows(){
        block128 shifted;
        for (int i = 0; i < 4; ++i) {
            vector<GF2_8> tmp = this->getRow(i);
            for (int j = 0; j < 4; ++j)
                shifted.byteMatrix[i][j]=tmp[(j+(4-i))%4];
        }
        this->byteMatrix = shifted.byteMatrix;
    }
    void mixColumns(){
        block128 predefine;
        predefine.byteMatrix[0][0].Set(2);
        predefine.byteMatrix[0][1].Set(3);
        predefine.byteMatrix[0][2].Set(1);
        predefine.byteMatrix[0][3].Set(1);
        predefine.byteMatrix[1][0].Set(1);
        predefine.byteMatrix[1][1].Set(2);
        predefine.byteMatrix[1][2].Set(3);
        predefine.byteMatrix[1][3].Set(1);
        predefine.byteMatrix[2][0].Set(1);
        predefine.byteMatrix[2][1].Set(1);
        predefine.byteMatrix[2][2].Set(2);
        predefine.byteMatrix[2][3].Set(3);
        predefine.byteMatrix[3][0].Set(3);
        predefine.byteMatrix[3][1].Set(1);
        predefine.byteMatrix[3][2].Set(1);
        predefine.byteMatrix[3][3].Set(2);
        block128 mixed;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    GF2_8 tmp =0;
                    tmp = predefine.byteMatrix[i][k]*this->byteMatrix[k][j];
                    mixed.byteMatrix[i][j]=mixed.byteMatrix[i][j]+tmp;
                }
            }
        }
        this->byteMatrix=mixed.byteMatrix;
    }
    void invMixColumns(){
        block128 predefine;
        predefine.byteMatrix[0][0].Set(14);
        predefine.byteMatrix[0][1].Set(11);
        predefine.byteMatrix[0][2].Set(13);
        predefine.byteMatrix[0][3].Set(9);
        predefine.byteMatrix[1][0].Set(9);
        predefine.byteMatrix[1][1].Set(14);
        predefine.byteMatrix[1][2].Set(11);
        predefine.byteMatrix[1][3].Set(13);
        predefine.byteMatrix[2][0].Set(13);
        predefine.byteMatrix[2][1].Set(9);
        predefine.byteMatrix[2][2].Set(14);
        predefine.byteMatrix[2][3].Set(11);
        predefine.byteMatrix[3][0].Set(11);
        predefine.byteMatrix[3][1].Set(13);
        predefine.byteMatrix[3][2].Set(9);
        predefine.byteMatrix[3][3].Set(14);
        block128 mixed;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    GF2_8 tmp =0;
                    tmp = predefine.byteMatrix[i][k]*this->byteMatrix[k][j];
                    mixed.byteMatrix[i][j]=mixed.byteMatrix[i][j]+tmp;
                }
            }
        }
        this->byteMatrix=mixed.byteMatrix;
    }
    void createByHand(){
        int x[16];
        GF2_8 tmp =0;
        vector<GF2_8> row;
        cout<<"put 4x4 matrix here:\n";
        for (int i = 0; i < 16; ++i) {
            cin>>x[i];
            tmp.Set(x[i]);
            row.push_back(tmp);
        }
        this->SetMatrix(row);
        cout<<endl;
    }

};
#endif //AES_BLOCK128_H
