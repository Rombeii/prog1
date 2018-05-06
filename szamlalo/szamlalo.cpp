#include <iostream>
#include <fstream>
#include <string>


using namespace std;





int main(int argc, char* argv[]){

    string fileNev = argv[1];
    ifstream beFile;
    beFile.open(fileNev);

    int betuszam=0;
    int szoszam=0;
    int sorszam=0;
    int szamszam=0;



    char temp;
    while(beFile.get(temp)){
        if(isalpha(temp))
            betuszam++;
        if(temp==' ' || temp=='\t'  || temp=='\n')
            szoszam++;
        if(temp=='\n')
            sorszam++;
        if(isdigit(temp))
            szamszam++;
    }

    cout<<"A megadott fileban "<<betuszam<<" db betű, "<<szoszam<< " db szo, "<<sorszam<<" db sor, és "<<szamszam<<" db szám van."<<endl;

    beFile.close();

    return 0;
}