#include <iostream>

using namespace std;

int main( ){

int a=12;
int b=8;

cout<<"Változók cseréje xorral: "<<endl;
cout<<"A változók eredetileg: "<<a<<"\t"<<b<<endl;

a=a^b;
b=b^a;
a=a^b;

cout<<"A változók csere után: "<<a<<"\t"<<b<<endl;

cout<<"\nVáltozók cseréje különbséggel: "<<endl;
cout<<"A változók eredetileg: "<<a<<"\t"<<b<<endl;

a=a+b;
b=a-b;
a=a-b;

cout<<"A változók csere után: "<<a<<"\t"<<b<<endl;

cout<<"\nVáltozók cseréje szorzattal: "<<endl;
cout<<"A változók eredetileg: "<<a<<"\t"<<b<<endl;

a=a*b;
b=a/b;
a=a/b;

cout<<"A változók csere után: "<<a<<"\t"<<b<<endl;



return 0;
}