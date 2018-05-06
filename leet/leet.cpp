//https://simple.wikipedia.org/wiki/Leet  innen vannak a karakterek

#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char* argv[]){
    string fileNev = argv[1];
    ifstream beFile;
    beFile.open(fileNev);

    string leet;
    char temp;
    int i=0;

    while(beFile.get(temp)){
        if(temp==' '){
            leet+=' ';
            continue;
        }

        switch(tolower(temp)){
            case 'a':
                leet+="4";
                break;
            case 'b':
                leet+="8";
                break;
            case 'c':
                leet+="<";
                break;
            case 'd':
                leet+="|)";
                break;
            case 'e':
                leet+="3";
                break;
            case 'f':
                leet+="|=";
                break;
            case 'g':
                leet+="[,";
                break;
            case 'h':
                leet+="|-|";
                break;
            case 'i':
                leet+="1";
                break;
            case 'j':
                leet+="_|";
                break;
            case 'k':
                leet+="|<";
                break;
            case 'l':
                leet+="|_";
                break;
            case 'm':
                leet+="|\\/|";
                break;
            case 'n':
                leet+="|\\";
                break;
            case 'o':
                leet+="0";
                break;
            case 'p':
                leet+="|O";
                break;
            case 'q':
                leet+="O_";
                break;
            case 'r':
                leet+="|2";
                break;
            case 's':
                leet+="5";
                break;
            case 't':
                leet+="7";
                break;
            case 'u':
                leet+="|_|";
                break;
            case 'v':
                leet+="\\/";
                break;
            case 'w':
                leet+="\\/\\/";
                break;
            case 'x':
                leet+="%";
                break;
            case 'y':
                leet+="'/,";
                break;
            case 'z':
                leet+="2";
                break;
            case '0':
                leet+="O";
                break;
            case '1':
                leet+="i";
                break;
            case '2':
                leet+="Z";
                break;
            case '3':
                leet+="E";
                break;
            case '4':
                leet+="A";
                break;
            case '5':
                leet+="S";
                break;
            case '6':
                leet+="b";
                break;
            case '7':
                leet+="T";
                break;
            case '8':
                leet+="B";
                break;
            case '9':
                leet+="P";
                break;
            default:
                leet+=temp;
        }

    }

    cout<<leet;



}