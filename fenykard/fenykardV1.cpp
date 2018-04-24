#include <iostream>
//#include <vector>
#include <stdlib.h>              //atoi
#include <string>
#include <boost/filesystem.hpp>     //recursive_directory_iterator
#include <fstream>              
#include <boost/filesystem/fstream.hpp> //boostos fstream
#include <ctype.h>  //isdigit


using namespace std;
using namespace boost::filesystem;

int pointExtractor(string temp);            //az átadott stringből megszerzi a feladatért járó pontokat

int sumOfPoints(string rootp);             //kiszámolja a max pontot



int main(int argc, char *argv[])
{
    int maxPont=0;
    boost::filesystem::ifstream routes("routes.txt");
    string rootp;
    while(getline(routes,rootp)){
        maxPont+=sumOfPoints(rootp);
    }
        
    cout<<"max elérhető pontok: "<<maxPont<<endl;
    routes.close();
    
                                       
}



int sumOfPoints(string rootp){
    int points=0;
for (recursive_directory_iterator end, dir(rootp); dir != end; dir++ ) {            //rekurzívan végigmegy a megadott út elemein
       //cout << *dir << std::endl;
        if(extension(*dir) ==".props"){                                             //ha props végűt talál
            cout<<*dir<<"---->";                                                    //kiírja az oda vezető utat,
            boost::filesystem::ifstream tempprops(*dir);                            //megnyitja a propst,
            string temp;
            int propsPoints=0;                                                 
            int pe=0;
            while(getline(tempprops, temp)){                                        //az összes benne lévő sorból kinyeri a pontot
                //cout<<*dir<<" "<<temp<<endl;
                pe = pointExtractor(temp);                                  
                points+=pe;                                                        //a pontok összegét menti a points változóba
                propsPoints+=pe;

            }
            cout<<propsPoints<<endl;
            tempprops.close();
        }
            
}
    return points;
}


int pointExtractor(string line){
    string temp="";
    int lineS=line.size()-1;
    //cout<<line<<endl;
    int i=0;
    int lastNumber=0;                                                              
    while(i<=lineS){                                                              //megkeresi a stringben az utolsó számot
        if(isdigit(line.at(i)))
                lastNumber=i;
        i++;

        }

    while(true){                                                                 
        
        
        if(isdigit(line.at(lastNumber))){                                      

            //string cucc=temp;
            temp = line[lastNumber] + temp;                                     //a kétjegyű számok miatt így kell összefűzni
            //cout<<"a szam a vegen:"<<temp<<endl;
            lastNumber--;
        }
        else break;
    }
    int point=atoi(temp.c_str());
    //cout<<point<<endl;
    return point;
}


//g++ fenykardV1.cpp -lboost_system -lboost_filesystem -o fenykardV1
//./fenykardV1