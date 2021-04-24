#include <iostream>
#include "base/byte_codec.h"
using namespace std;
using namespace basic;
int main(){
    uint64_t number=8000000000;
    char buffer[8];
    for(uint64_t i=0;i<number;i++){
        uint64_t a=i;
        uint64_t b=0;    
        DataWriter writer(buffer,sizeof(uint64_t));
        writer.WriteVarInt62(a);
        DataReader reader(buffer,sizeof(uint64_t));
        bool success=reader.ReadVarInt62(&b);
        if(a!=b){
            std::cout<<"error "<<a<<" "<<b<<std::endl;
            break;
        }
    }
    return 0;
}
