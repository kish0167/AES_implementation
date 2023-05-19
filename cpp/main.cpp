#include <iostream>
#include "GF2_8.h"
#include "GF2_8.cpp"
#include "block128.h"
#include "AES.h"
#include "string.h"
using namespace std;
void encryptText(){
    block128 key;
    GF2_8 buf =0;
    vector<block128> message;
    cout<<"enter text\n";
    string text,trash;
    getline(cin,trash);
    getline(cin,text);
    int length = text.size()-text.size()%16+16;
    for (int i = 0; i < (length-16)/16; ++i) {
        vector<GF2_8> part16;
        block128 temp;
        for (int j = 0; j < 16; ++j) {
            buf.Set((int)text[i*16+j]);
            part16.push_back(buf);
        }
        temp.SetMatrix(part16);
        message.push_back(temp);
    }
    vector<GF2_8> part16;
    block128 temp;
    for (int i = 0; i < text.size()%16; ++i) {
        buf.Set((int)text[text.size()-text.size()%16+i]);
        part16.push_back(buf);
    }
    buf.Set(32);
    for (int i = 0; i < 16-text.size()%16; ++i)
        part16.push_back(buf);
    temp.SetMatrix(part16);
    message.push_back(temp);
    cout<<"enter the key\n";
    key.createByHand();
    cout<<"your message:\n";
    for (int i = 0; i < message.size(); ++i) {
        AES x;
        x.setData(message[i]);
        x.encrypt(key);
        message[i]=x.getData();
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k)
                cout<<x.getData().GetMatrix()[j][k].GetHex();
        cout<<endl;
    }
}
void decryptText(){
    cout<<"enter message and 'end' at the end\n";
    string message, t="0";
    cin>>t;
    while(t!="end") {
        message+=t;
        cin >> t;
    }
    block128 buf;
    GF2_8 temp=0;
    vector<block128> text;
    for (int i = 0; i < message.size()/32; ++i) {
        vector<GF2_8> row16;
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                int a=0,b=0,c=0;
                a=(int)message[i*32+j*8+k*2];
                b=(int)message[i*32+j*8+k*2+1];
                if(a>64 && a <71) c+=(a-55)*16;
                else c+=(a-48)*16;
                if(b>64 && b<71) c+=(b-55);
                else c+=(b-48);
                temp.Set(c);
                row16.push_back(temp);
            }
        }
        buf.SetMatrix(row16);
        text.push_back(buf);
    }
    cout<<"enter the key\n";
    block128 key;
    key.createByHand();
    for (int i = 0; i < text.size(); ++i) {
        AES x;
        x.setData(text[i]);
        x.decrypt(key);
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                int c=x.getData().GetMatrix()[j][k].GetInt();
                cout<<(char)c;
            }
        }
    }
}
int main() {
//    GF2_8 A=0;
//    for (int i = 0; i < 256; ++i) {
//       A.Set(i);
//       cout<<"\twhen x\""<<A.GetHex()<<"\" => output8 <= x\"";
//       A.Sbox(0);
//       cout<<A.GetHex()<<"\";\n";
//    }
    int c=0;
    while(c!=3){
        cout<<"\n\n?\n1 encrypt\n2 decrypt\n3 stop\n";
        cin>>c;
        if (c==1)
            encryptText();
        else if(c==2)
            decryptText();
    }
//    GF2_8 a=1, two = 2;
//    for (int i = 0; i < 12; ++i) {
//        cout<<a.GetHex()<<endl;
//        a=a*two;
//    }
//    GF2_8 a=1,x=5;
//    vector<GF2_8> A,F;
//    for (int i = 0; i < 15; ++i)
//        A.push_back(a);
//    A.push_back(x);
//    F.push_back(x);
//    for (int i = 1; i < 16; ++i)
//        F.push_back(a);
//    block128 b,v;
//    b.SetMatrix(A);
//    v.SetMatrix(F);
//    AES c;
//    c.setData(b);
//    c.print();
//    c.encrypt(v);
//    c.print();
    return 0;
}
