#include <iostream>
#include <vector>
#include <stdlib.h>                         //atoi
#include <string>   
#include <boost/filesystem.hpp>             //recursive_directory_iterator
#include <boost/filesystem/fstream.hpp>     //boostos fstream
#include <boost/program_options.hpp>
#include <fstream>              
#include <ctype.h>                          //isdigit
#include <algorithm>                        //sort

using namespace std;
using namespace boost::filesystem;          //recursive iteratornál

struct task{
    string name;
    int points;
};

struct student{
    string name;
    int points;
};

struct dbline{
    string name;
    string task;
    string date;
};

int counter=0;
vector<task> tasks;
vector<student> students;
vector<dbline> dblines;
vector<string> props;




int pointExtractor(string temp);            //az átadott stringből levágja a feladatért járó pontokat

int sumOfPoints(string rootp);              //kiszámolja a max pontot, feltölti a props vektort

void dblinesFiller(string csvnev);          //feltölti a dblines vektort az adatbázis adataival

void tasksNameFiller();                     //feltölti a task.name-t, levágja a props vektorból a [nev].props végződést, az elejére "./-t, a végére
                                            //"-t illeszt

void studentNameFiller();                   //feltölti a students vektort az emberek nevével

void pointCounter();                        //kiszámolja a pontokat emberenként

bool comparePoints(const student &a, const student &b);                       //ha már ki van számolva minden emberre a pont, ennek segítségével lehet sorbarakni őket





int main(int argc, char* argv[])
{

    boost::program_options::options_description desc("Options");

    desc.add_options()
        ("tasks", "Kiirja a feladatokat, az értük járó pontokat, és a max. elérhető pontot")
        ("list", "Kilistázza az embereket, és a pontjaikat")
        ("roots,r", boost::program_options::value<vector<string>> (), "Gyökerek helye")
        ("database,db", boost::program_options::value<string> (), "Adatbázis helye");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);



    //if(vm.count(root))


    int maxPont=0;
    boost::filesystem::ifstream routes("routes.txt");
    string rootp;
    while(getline(routes,rootp)){                                       //a props összes sorára meghívjuk
        maxPont+=sumOfPoints(rootp);
    }
    
    if(vm.count("tasks")){
        for(int i=0; i<tasks.size(); i++){                              //kiirja azokat, amikre pont adható 
            cout<<props[i]<<"--->"<<tasks[i].points<<endl;
        }                                    
          cout<<"max elérhető pont: "<<maxPont<<endl;  

    }
    
    
    if(vm.count("list") && vm.count("database")){

        string csvnev=vm["database"].as<string>();
        dblinesFiller(csvnev);          
        //cout<<dblines.size();
        studentNameFiller();
        tasksNameFiller();
        pointCounter();

        sort(students.begin(), students.end(), comparePoints);

        for(int i=0; i<students.size();i++){
            if(students[i].points!=0)
                cout<<students[i].name<<"---->"<<students[i].points<<endl;
        }


    }

        
}



int sumOfPoints(string rootp){
    int points=0;
    for (recursive_directory_iterator end, dir(rootp); dir != end; dir++ ) {            //rekurzívan végigmegy a megadott út elemein
           //cout << *dir << std::endl;
            if(extension(*dir) ==".props"){                                             //ha props végűt talál
                //cout<<*dir<<"---->";                                                    //kiírja az oda vezető utat,
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
               //cout<<propsPoints<<endl;
               tasks.push_back(task());
               props.push_back(path(*dir).string());
               //tasks[counter].name=path(*dir).string();
               tasks[counter].points=propsPoints;
               counter++;
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

void dblinesFiller(string csvnev){
    int i=0;
    boost::filesystem::ifstream csv(csvnev);
    string tempName;
    string tempTask;
    string tempDate;
    while(getline(csv, tempName, ',')){
        getline(csv, tempTask, ',');
        getline(csv, tempDate, '\n');
        dblines.push_back(dbline());
        dblines[i].name=tempName;
        dblines[i].task=tempTask;
        dblines[i].date=tempDate;
        i++;


    }
}

void tasksNameFiller(){
    for(int i=0; i<props.size(); i++){                             //végigmegy a props vektor összes elemén
        int lastBSlash=0;
        for(int j=0; j<props[i].size(); j++){
            if(props[i][j]=='/')
                lastBSlash=j;
        }
        tasks[i].name="\"./"+props[i].substr(0,lastBSlash)+"\"";

    }

}

void studentNameFiller(){
    counter=0;

    for(int i=0; i<dblines.size(); i++){
        bool exists=false;
        for(int j=0; j<students.size(); j++){
            if(dblines[i].name==students[j].name)
                exists = true;

        }
        
        if(exists==false){
            students.push_back(student());
            students[counter].name=dblines[i].name;
            counter++;

        }
            


    }

}

void pointCounter(){
    for (int i=0; i<dblines.size(); i++){
        for(int j=0; j<students.size(); j++){
            if(dblines[i].name==students[j].name){       
                for(int k=0; k<tasks.size();k++){
                    if(dblines[i].task==tasks[k].name){
                        students[j].points+=tasks[k].points;
                    }
                }
            }
        }
    }        
}

bool comparePoints(const student &a, const student &b){
    return a.points > b.points;
}
//g++ fenykardV2.cpp -lboost_system -lboost_filesystem -o fenykardV2
//./fenykardV2 db-2018-03-10.csv