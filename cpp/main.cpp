#include <iostream>
#include "GF2_8.h"
#include "GF2_8.cpp"
#include "block128.h"
#include "AES.h"
#include "string.h"
using namespace std;
int main() {
    AES block;
    block128 key;
    cout<<"enter key\n";
    key.createByHand();
    cout<<"enter data block\n";
    block.create();
    block.encrypt(key);
    cout<<"\nencrypted data:\n";
    block.print();
    return 0;
}
