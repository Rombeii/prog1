#include <iostream>
#include <vector>
#include <stdlib.h>                         //atoi
#include <string>   
#include <boost/filesystem.hpp>             //recursive_directory_iterator
#include <boost/filesystem/fstream.hpp>     //boostos fstream
#include <boost/date_time.hpp>              //az adatbázis dátumainak kezeléséhez
#include <boost/program_options.hpp>
#include <fstream>              
#include <ctype.h>                          //isdigit
#include <algorithm>                        //sort



using namespace std;
using namespace boost::filesystem;          //recursive iteratornál
using namespace boost::gregorian;           //date előtt



struct day{
    vector<int> points;                    //a pont ami jár érte
    vector<int> prob;                      //a valószínűsége annak, hogy megkapja azt a pontot
    vector<string> task;
};

struct task{
    string name;                           //a feladat neve
    int points;                            //a feladatért járó pont
};

struct student{
    string name;                           //a hallgató neve
    int points;                            //a hallgató összpontja
    day days[7];                           //a hét napjai
    int grade;                             //az elért jegye
    int notZero;                           //
};

struct dbline{
    string name;                           //a hallgató neve
    string task;                           //a feladat neve
    string date;                           //a dátum, mikor be lett küldve
};

    
    
    
    



int pointExtractor(string temp);                                                                      //az átadott stringből levágja a feladatért járó pontokat

int sumOfPoints(string rootp, int &counter, vector<task> &tasks, vector<string> &props);              //kiszámolja a max pontot, feltölti a props vektort és a tasks vektor points részét

void dblinesFiller(string csvnev, vector<dbline> &dblines);                                           //feltölti a dblines vektort az adatbázis adataival

void tasksNameFiller(vector<task> &tasks, vector<string> &props);                                     //feltölti a task.name-t, levágja a props vektorból a [nev].props végződést, az elejére "./-t, a végére "-t illeszt

void studentNameFiller(vector<student> &students, vector<dbline> &dblines);                           //feltölti a students vektort az emberek nevével

void pointCounter(vector<task> &tasks, vector<student> &students, vector<dbline> &dblines);           //kiszámolja a pontokat emberenként

bool comparePoints(const student &a, const student &b);                                               //ha már ki van számolva minden emberre a pont, ennek segítségével lehet sorbarakni őket

bool compareNames(const student &a, const student &b);                                                //abc sorrendbe segít rendezni

bool compareGrades(const student &a, const student &b);

void probabilityCounter(vector<student> &students, vector<task> &tasks, vector<dbline> &dblines);     //a studentben a day-eket feltölti                                                                          //kiszámolja napra szabva a

int dayOfWeek(string date);                                                                           //eldönti, hogy milyen nap, és annak megfelelően számot ad vissza(1-Hétfő, 2-Kedd)

void studentDayFiller(vector<student> &students, vector<task> &tasks, int num, int i, int k);

void simulation(vector<student> &students, int vNumberOfDays []);

int randomNumber(int min, int max);

int main(int argc, char* argv[])
{   
    int counter=0;
    vector<task> tasks;
    vector<student> students;
    vector<dbline> dblines;
    vector<string> props;
    vector<string> routes;
    routes.push_back("uCity/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Példák/Előadás");
    routes.push_back("uCity/Debrecen/Szórakozás/Könyv/Ismeretterjesztő/Informatika");
    routes.push_back("uCity/Debrecen/Oktatás/Informatika/Programozás/Tankönyv olvasás");




    boost::program_options::options_description desc("Options");

    desc.add_options()
        ("tasks", "Kiirja a feladatokat, az értük járó pontokat, és a max. elérhető pontot")
        ("list", "Kilistázza az embereket, és a pontjaikat")
        ("roots,r", boost::program_options::value<vector<string>> ())//->default_value(routes))
        ("database,db", boost::program_options::value<string> (), "Adatbázis helye")
        ("simul", "Szimulál egy új sorrendet")
        ("grade", "Kiirja a laboron elért jegyet");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);



    //if(vm.count(root))

    if ( vm.count ( "roots" ) ){
            routes = vm["roots"].as<vector<string>>();
            
    }


    int maxPont=0;
    
    for(int i=0; i<routes.size(); i++){
        maxPont+=sumOfPoints(routes[i], counter, tasks, props);
    }                           
        
    

    
    if(vm.count("tasks")){
        for(int i=0; i<tasks.size(); i++){                              //kiirja azokat, amikre pont adható 
            cout<<props[i]<<"--->"<<tasks[i].points<<endl;
        }                                    
          cout<<"max elérhető pont: "<<maxPont<<endl;  

    }
    
    
    if(vm.count("list") && vm.count("database")){

        string csvnev=vm["database"].as<string>();
        dblinesFiller(csvnev, dblines);          
        studentNameFiller(students, dblines);
        tasksNameFiller(tasks, props);
        pointCounter(tasks, students, dblines);

        sort(students.begin(), students.end(), comparePoints);

        for(int i=0; i<students.size();i++){
            if(students[i].points!=0)
                cout<<students[i].name<<"---->"<<students[i].points<<endl;
        }
    }
    

    if(vm.count("simul") && vm.count("database")){
        srand((unsigned int)time(NULL));          

        string csvnev=vm["database"].as<string>();
        dblinesFiller(csvnev, dblines);          
        studentNameFiller(students, dblines);
        tasksNameFiller(tasks, props);
        

        int  vNumberOfDays[7];                       //ez a vektor tárolja, hogy egyes napokból hány fordul elő a szimuláció alatt
        for(int i=0; i<7; i++){
            vNumberOfDays[i]=0;
        }
        

        int sYear;
        int sMonth;
        int sDay;
        int eYear;
        int eMonth;
        int eDay;


        sYear=atoi(dblines[0].date.substr(1,4).c_str());
        sMonth=atoi(dblines[0].date.substr(6,2).c_str());
        sDay=atoi(dblines[0].date.substr(9,2).c_str());

        eYear=atoi(dblines[dblines.size()-1].date.substr(1,4).c_str());         
        eMonth=atoi(dblines[dblines.size()-1].date.substr(6,2).c_str());        
        eDay=atoi(dblines[dblines.size()-1].date.substr(9,2).c_str());

        date begin_dt(sYear,sMonth,sDay);                                       //az első bejegyzés dátuma
        date end_dt(eYear, eMonth, eDay);                                       //az utolsó bejegyzés dátuma



        for(day_iterator iter=begin_dt; iter!=end_dt; ++iter ){                 //megszámolja, hogy egyes napokból hány van
            switch(iter->day_of_week()){
                case boost::date_time::Monday :
                    vNumberOfDays[0]+=1;
                    break;
                case boost::date_time::Tuesday :
                    vNumberOfDays[1]+=1;
                    break;
                case boost::date_time::Wednesday :
                    vNumberOfDays[2]+=1;
                    break;
                case boost::date_time::Thursday :
                    vNumberOfDays[3]+=1;
                    break;
                case boost::date_time::Friday :
                    vNumberOfDays[4]+=1;
                    break;
                case boost::date_time::Saturday :
                    vNumberOfDays[5]+=1;
                    break;
                case boost::date_time::Sunday :
                    vNumberOfDays[6]+=1;
                    break;
            } 
        }
        
        //for(int i=0; i<7; i++)
        //    cout<<vNumberOfDays[i]<<endl;
        probabilityCounter(students, tasks, dblines);

        /*for(int k=0; k<students.size();k++){
            for(int i=0; i<7;i++){
                for(int j=0; j<students[k].days[i].prob.size();j++){
                    cout<<students[k].name<<"\n"<<students[k].days[i].task[j]<<"\n"<<students[k].days[i].points[j]<<"\n"<<students[k].days[i].prob[j]<<"\n"<<i<<endl<<endl;
                }
            }

        }*/
        
        simulation(students, vNumberOfDays);
        sort(students.begin(), students.end(), comparePoints);
        for(int i=0; i<students.size(); i++){
            if(students[i].points!=0)
                cout<<students[i].name<<" "<<students[i].points<<endl;
        }
    }
        
    if(vm.count("grade") && vm.count("database")){
        string csvnev=vm["database"].as<string>();
        dblinesFiller(csvnev, dblines);          
        studentNameFiller(students, dblines);
        tasksNameFiller(tasks, props);
        for(int i=0; i<students.size(); i++){
            int first=0;
            int second=0;
            int third=0;
            for(int j=0; j<dblines.size(); j++){
                if(students[i].name==dblines[j].name){
                    if(dblines[j].task.find("./City/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Labor/Védés/I/")!= string::npos){
                        if(dblines[j].task.find("elégtelen")!= string::npos){
                            if(1>first)
                                first=1;
                        }
                        if(dblines[j].task.find("elégséges")!= string::npos){
                            if(2>first)
                                first=2;
                        }
                        if(dblines[j].task.find("közepes")!= string::npos){
                            if(3>first)
                                first=3;
                        }
                        if(dblines[j].task.find("jó")!= string::npos){
                            if(4>first)
                                first=4;
                        }
                        if(dblines[j].task.find("jeles")!= string::npos){
                            if(5>first)
                                first=5;
                        }
                    }
                    if(dblines[j].task.find("./City/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Labor/Védés/II/")!= string::npos){
                        if(dblines[j].task.find("elégtelen")!= string::npos){
                            if(1>second)
                                second=1;
                        }
                        if(dblines[j].task.find("elégséges")!= string::npos){
                            if(2>second)
                                second=2;
                        }
                        if(dblines[j].task.find("közepes")!= string::npos){
                            if(3>second)
                                second=3;
                        }
                        if(dblines[j].task.find("jó")!= string::npos){
                            if(4>second)
                                second=4;
                        }
                        if(dblines[j].task.find("jeles")!= string::npos){
                            if(5>second)
                                second=5;
                        }
                    }
                    if(dblines[j].task.find("./City/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Labor/Védés/III/")!= string::npos){
                        if(dblines[j].task.find("elégtelen")!= string::npos){
                            if(1>third)
                                third=1;
                        }
                        if(dblines[j].task.find("elégséges")!= string::npos){
                            if(2>third)
                                third=2;
                        }
                        if(dblines[j].task.find("közepes")!= string::npos){
                            if(3>third)
                                third=3;
                        }
                        if(dblines[j].task.find("jó")!= string::npos){
                            if(4>third)
                                third=4;
                        }
                        if(dblines[j].task.find("jeles")!= string::npos){
                            if(5>third)
                                third=5;
                        }
                    }
                }
            }
            int notZero=0;
        if(first!=0)
            notZero++;
        if(second!=0)
            notZero++;
        if(third!=0)
            notZero++;
        students[i].notZero=notZero;
        if(notZero!=0){
            
            double temp=((double)first+(double)second+(double)third)/(double)notZero;
            //cout<<temp<<endl;
            students[i].grade=(int)temp;
            //cout<<students[i].name<<" #"<<notZero<<" "<<students[i].grade<<endl;
            //cout<<first<<" "<<second<<" "<<third<<" "<<notZero<<endl;
        }
        }
        
        sort(students.begin(), students.end(), compareGrades);

        for(int i=0; i<students.size();i++){
            if(students[i].notZero!=0)
                cout<<students[i].name<<" #"<<students[i].notZero<<" "<<students[i].grade<<endl;
        }

        
    }
}

int sumOfPoints(string rootp, int &counter, vector<task> &tasks, vector<string> &props ){
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

void dblinesFiller(string csvnev, vector<dbline> &dblines){
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

void tasksNameFiller(vector<task> & tasks, vector<string> &props){
    for(int i=0; i<props.size(); i++){                             //végigmegy a props vektor összes elemén
        int lastBSlash=0;
        for(int j=0; j<props[i].size(); j++){
            if(props[i][j]=='/')
                lastBSlash=j;
        }
        tasks[i].name="\"./"+props[i].substr(0,lastBSlash)+"\"";

    }

}

void studentNameFiller(vector<student> &students, vector<dbline> &dblines){
    int j=0;

    for(int i=0; i<dblines.size(); i++){
        bool exists=false;
        for(int j=0; j<students.size(); j++){
            if(dblines[i].name==students[j].name)
                exists = true;

        }
        
        if(exists==false){
            students.push_back(student());
            students[j].name=dblines[i].name;
            j++;

        }
            


    }

}

void pointCounter(vector<task> &tasks, vector<student> &students, vector<dbline> &dblines){
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

bool compareNames(const student &a, const student &b){
    return a.name < b.name;
}

bool compareGrades(const student &a, const student &b){
    return a.grade > b.grade;
}
    
void probabilityCounter(vector<student> &students, vector<task> &tasks, vector<dbline> &dblines){
    for(int i=0; i<students.size(); i++){ 
        //int counter=0;                     
        for(int j=0; j<dblines.size(); j++){                //emberenként végigmegy az adatbázis sorain
            if(students[i].name==dblines[j].name){          
                for(int k=0; k<tasks.size(); k++){          //ha megtalálja a nevet, megnézi, hogy a tevékenységért jár-e pont
                    if(dblines[j].task==tasks[k].name){     //ha ját érte pont, akkor megnézi milyen napon xelte be, ++prob, és a pontot is feladja 
                        switch(dayOfWeek(dblines[j].date)){ //lekérdezzük, hogy milyen nap volt
                            case(0) :
                                studentDayFiller(students, tasks, 0, i, k);
                                break;
                            case(1) :
                                studentDayFiller(students, tasks, 1, i, k);
                                break;
                            case(2) :
                                studentDayFiller(students, tasks, 2, i, k);
                                break;
                            case(3) :
                                studentDayFiller(students, tasks, 3, i, k);
                                break;
                            case(4) :
                                studentDayFiller(students, tasks, 4, i, k);
                                break;
                            case(5) :
                                studentDayFiller(students, tasks, 5, i, k);
                                break;
                            case(6) :
                                studentDayFiller(students, tasks, 6, i, k);


                        }  
                            




                    }      
                }
            }
        }
    }

}

int dayOfWeek(string adate){
    int aYear;
    int aMonth;
    int aDay;


    aYear=atoi(adate.substr(1,4).c_str());
    aMonth=atoi(adate.substr(6,2).c_str());
    aDay=atoi(adate.substr(9,2).c_str());

    date adate_dt(aYear,aMonth,aDay);

    switch(adate_dt.day_of_week()){
        case boost::date_time::Monday :
            return 0;
        case boost::date_time::Tuesday :
            return 1;
        case boost::date_time::Wednesday :
            return 2;
        case boost::date_time::Thursday :
            return 3;
         case boost::date_time::Friday :
            return 4;
        case boost::date_time::Saturday :
            return 5;
        case boost::date_time::Sunday :
            return 6;
            } 
}

void studentDayFiller(vector<student> &students, vector<task> &tasks, int num, int i, int k){
    bool exists=false;
    int where;
    for(int l=0; l<students[i].days[num].task.size(); l++){
        if(tasks[k].name==students[i].days[num].task[l]){
            exists=true;
            where=l;
            //cout<<"kapta"<<endl;
        }
    }
    if(exists==true)
        students[i].days[num].prob[where]+=1;

    if(exists==false){
        students[i].days[num].prob.push_back(1);
        students[i].days[num].task.push_back(tasks[k].name);
        students[i].days[num].points.push_back(tasks[k].points);
        //counter++;
        //cout<<tasks[k].name<<" "<<tasks[k].points<<endl;
                                    }
    
                                
}

void simulation(vector<student> &students, int vNumberOfDays[]){
    for(int i=0; i<students.size(); i++){
        students[i].points=0;
        for(int j=0; j<7; j++){
            for(int k=0; k<students[i].days[j].prob.size(); k++){
                for(int l=0; l<vNumberOfDays[j]; l++){
                    if(randomNumber(1, vNumberOfDays[j]+1)<=students[i].days[j].prob[k]){
                        students[i].points+=students[i].days[j].points[k];
                    }
                }
            }

        }
    }
}

int randomNumber(int min, int max){
    return min+ rand()%(max-min);
}

//g++ fenykardV3.cpp -lboost_system -lboost_filesystem lboost_program_options -lboost_date_time -o fenykardV3
//./fenykardV3 --tasks
//./fenykardV3 --list --database db-2018-03-10.csv
//./fenykardV3 --simul --database db-2018-03-10.csv