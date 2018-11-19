#include <iostream>
using namespace std;

char* encrypt(char* pstr,int keylength){
    const int n=strlen(pstr);
    for(int i=0;i<n;i++){
        if(pstr[i]<='z'&&pstr[i]>='a'){
        pstr[i]=pstr[i]+keylength%26;
        if(pstr[i]<'a'||pstr[i]>'z'){
            if(keylength<0) pstr[i]+=26;
            else   pstr[i]-=26;
        }
        }
        if(pstr[i]<='Z'&&pstr[i]>='A'){
        pstr[i]=pstr[i]+keylength%26;
        if(pstr[i]<'A'||pstr[i]>'Z'){
            if(keylength<0) pstr[i]+=26;
            else   pstr[i]-=26;
        }
        }
    }
    return pstr;
}
