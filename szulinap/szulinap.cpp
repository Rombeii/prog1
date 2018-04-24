#include <iostream>
#include <boost/date_time.hpp>

using namespace std;
using namespace boost::gregorian;

int main( ){
    cout<<"Add meg a születési dátumodat! (yyyy.mm.dd)"<<endl;
    string szulstring;
    cin>>szulstring;

    //int ev=atoi(szulstring.substr(0,4).c_str());
    int honap=atoi(szulstring.substr(5,2).c_str());
    int nap=atoi(szulstring.substr(8,2).c_str());

    //cout<<ev<<" "<<honap<<" "<<nap<<endl;

    date szul3(3000,honap,nap);


    switch(szul3.day_of_week()){
        case boost::date_time::Monday :
            cout<<"A születésnapod 3000-ben hétfőre esik!"<<endl;
            break;
        case boost::date_time::Tuesday :
            cout<<"A születésnapod 3000-ben keddre esik!"<<endl;
            break;
        case boost::date_time::Wednesday :
            cout<<"A születésnapod 3000-ben szerdára esik!"<<endl;
           break;
        case boost::date_time::Thursday :
            cout<<"A születésnapod 3000-ben csütörtökre esik!"<<endl;
            break;
        case boost::date_time::Friday :
            cout<<"A születésnapod 3000-ben péntekre esik!"<<endl;
            break;
        case boost::date_time::Saturday :
            cout<<"A születésnapod 3000-ben szombatra esik!"<<endl;
            break;
        case boost::date_time::Sunday :
            cout<<"A születésnapod 3000-ben vasárnapra esik!"<<endl;
            break;


    }

}
