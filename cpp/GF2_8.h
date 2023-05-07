//
// Created by simer on 06.05.2023.
//
//#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "string.h"
#ifndef CPP_GF2_8_H
#define CPP_GF2_8_H
using namespace std;
struct Byte{
    bool bit [8];
};

class GF2_8 {
private:
    Byte value;
    static bool Xor(bool a, bool b){
        return a!=b;
    }
    static int compare16(bool* a, bool* b){
        int max=0;
        for (int i = 15; i >= 0; --i)
            if (a[i]==1 && b[i]!=1)
                return i;
            else if (a[i]!= 1 && b[i]==1)
                return -1;
        return 0;
    }
    static Byte intToByte(int a){
        if (a>255 || a<0)
        {
            cout<<"incorrect number";
            throw new exception();
        }
        Byte A;
        for (int i = 0; i < 8; ++i) {
            A.bit[i]=false;
        }
        for (int i = 0; i < 8 && a>0; ++i) {
            A.bit[i]= a%2;
            a=a/2;
        }
        return A;
    }
    static int ByteToInt(Byte A){
        int tmp=1, a=0;
        for (int i = 0; i < 8; ++i) {
            a+=tmp*A.bit[i];
            tmp*=2;
        }
        return a;
    }
    static int curMax(bool* a){
        for (int i = 15; i >= 0; --i)
            if (a[i]) return i;
        return -1;
    }

public:
    GF2_8(int val){Set(val);}
    GF2_8 operator+(const GF2_8& right) const {
        Byte sum ;
        for (int i = 0; i < 8; ++i)
            sum.bit[i]=false;
        for (int i = 0; i < 8; ++i)
           sum.bit[i] = Xor(value.bit[i],right.value.bit[i]);
        GF2_8 ans=0;
        ans.Set(sum);
        return ans;
    }
    GF2_8 operator*(const GF2_8& right) const {
        Byte mplx = {false,false,false,false,false,false,false,false};
        bool algConst[16] = {true,true,false,true,true,false,false,false,true,false,false,false,false,false,false,false};
        bool byte2[16]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
               byte2[i+j]=Xor(value.bit[i]*right.value.bit[j],byte2[i+j]);

        int cur= curMax(byte2);
        while (cur>=8){
            bool tmp[16];
            for (bool & i : tmp)
                i=false;
            for (int i = 8; i >= 0 ; --i)
                tmp[i+cur-8]=algConst[i];
            for (int i = 15; i >= 0; --i)
                byte2[i]=Xor(tmp[i],byte2[i]);
            cur= curMax(byte2);
        }

        for (int i = 0; i < 8; ++i)
            mplx.bit[i]=byte2[i];
        GF2_8 ans=0;
        ans.Set(mplx);
        return ans;
    }
    void Set(Byte a){
        value= a;
    }
    void Set(int a){
        value= intToByte(a);
    }
    int GetInt(){
        return ByteToInt(value);
    }
    Byte GetByte(){
        return value;
    }
    string GetHex(){
        int tmp=1, a=0,b=0;
        for (int i = 0; i < 4; ++i) {
            a+=tmp*value.bit[i];
            tmp*=2;
        }
        tmp=1;
        for (int i = 4; i < 8; ++i) {
            b+=tmp*value.bit[i];
            tmp*=2;
        }
        string hex="";
        if (b>=10)
            hex+= (char)(55+b);
        else
            hex+= (char)(b+48);
        if(a>=10)
            hex+= (char)(55+a);
        else
            hex+= (char)(a+48);
        return  hex;
    }
    void Sbox(bool direction){
        string way;
        if(!direction)
            way="F:\\AES\\cpp\\sbox.txt";
        else way="F:\\AES\\cpp\\sbox_inv.txt";
        ifstream fin;
        fin.open(way);
        char buf[2];
        int a = ByteToInt(value);
        for (int i = 0; i <= a; ++i) {
            fin>>buf[0];
            fin>>buf[1];
          //  fin>>buf[2];
        }
        fin.close();
        int nVal =0;
        if  (buf[0]>96 && buf[0]<103)
            nVal+=((int)buf[0]-87)*16;
        else
            nVal+=((int)buf[0]-48)*16;
        if  (buf[1]>96 && buf[1]<103)
            nVal+=(int)buf[1]-87;
        else
            nVal+=(int)buf[1]-48;
        value = intToByte(nVal);
        return;
    }
};

#endif //CPP_GF2_8_H
