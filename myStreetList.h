//
// Created by Emmanuel Abraham on 2019-10-25.
//

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;


vector<string> streetPicker(int sn)
{

    vector<string> street;

    street.push_back("Niger Road");
    street.push_back("Lekki Street");
    street.push_back("Victoria Street");
    street.push_back("Tiamiyu Savage Street");
    street.push_back("Adeola Hopewell Road");
    street.push_back("Emmanuel Road N");
    street.push_back("Emmanuel Road S");
    street.push_back("Columbia Road E");
    street.push_back("Columbia Road W");
    street.push_back("Weber Street");
    street.push_back("Lester Ave");
    street.push_back("Davenport Ave");
    street.push_back("Dunvegan Street");
    street.push_back("Erb Street");
    street.push_back("Waterloo Main E");
    street.push_back("Waterloo Main W");
    street.push_back("Varsity Ave E");
    street.push_back("Serena Ave");
    street.push_back("Oluwatosin Drive");
    street.push_back("Ivy Crescent");
    street.push_back("Varsity Ave W");
    street.push_back("Green Hill Drive");
    street.push_back("Maple Street");
    street.push_back("Columbiana Street");
    street.push_back("Pink Hill Drive");
    street.push_back("Old Abbey Road");
    street.push_back("Back Road");
    street.push_back("Old Street");
    street.push_back("Hope Street");
    street.push_back("Peace Street");
    street.push_back("Joy Road");
    street.push_back("Happiness Road N");
    street.push_back("Happiness Road S");
    street.push_back("Love Road E");
    street.push_back("Love Road W");
    street.push_back("Lee Street");
    street.push_back("Frobisher Ave E");
    street.push_back("Frobisher Ave W");
    street.push_back("Drea Street");
    street.push_back("Bairstow Street");
    street.push_back("Third Intersect");
    street.push_back("First Intersect");
    street.push_back("Williams Road");
    street.push_back("Peters Ave");
    street.push_back("Oluwatobi Hill");
    street.push_back("Towers Crescent");
    street.push_back("University Ave W");
    street.push_back("Red Hills Drive");
    street.push_back("Chester Street");
    street.push_back("Column Street");
    street.push_back("Blue Hills Drive");
    street.push_back("New Abbey Road");
    street.push_back("Heaven Oaks");
    street.push_back("New Haven Street");
    street.push_back("Blockline Street");
    street.push_back("Ojuelegba E");
    street.push_back("Ajegunle E");
    street.push_back("Ojuelegba W");
    street.push_back("Ajegunle W");
    street.push_back("Oshodi N");
    street.push_back("Oshodi S");
    street.push_back("Mile Two");
    street.push_back("Mile Twelve");
    street.push_back("Palm Groove");
    street.push_back("Tellos Place");
    street.push_back("Batavia Place");
    street.push_back("Mile Fourteen");
    street.push_back("Alaba Market");
    street.push_back("Ikeja Along");
    street.push_back("Benin Street");
    street.push_back("Edo Drive");
    street.push_back("Barista Road");
    street.push_back("Kanye Oaks");
    street.push_back("Riise Street");
    street.push_back("Smart Street");
    street.push_back("Hello World");
    street.push_back("Ogunlana Drive");
    street.push_back("Ijebu Street");
    street.push_back("BurnaB Drive");
    street.push_back("Bachelor Road");
    street.push_back("Christmas Oaks");
    street.push_back("Broader Street");
    street.push_back("Duller Street");
    street.push_back("Ijebur Street");
    street.push_back("Burna Drive");
    street.push_back("Bitch Road");
    street.push_back("Christmas Oakers");
    street.push_back("Broad Street");
    street.push_back("Dull Street");
    street.push_back("Bach Road");
    street.push_back("Oyinda Oaks");
    street.push_back("Rachel Street");
    street.push_back("Belle Street");
    street.push_back("Ode Street");
    street.push_back("Wizzy Drive");
    street.push_back("Handsome Road");
    street.push_back("Child Oaks");
    street.push_back("Length Street");
    street.push_back("Spinster Street");
    street.push_back("Tuqa Street");
    street.push_back("Tume Street");
    street.push_back("Duke Street");
    street.push_back("Osqa Street");

    vector<string> streetList;

    int a = 0 +(rand()% (100 - 0 + 1));

    streetList.push_back(street[a]);

    unsigned j = (unsigned int) sn;

    srand(time(NULL));

    while (true)
    {
        int b = 0 + (rand()% (100 - 0 + 1));

        bool test1 = true;
        bool test2;

        while (b)
        {
            if (b != a) {

                if (streetList.size() > 0) {

                    for (unsigned int s = 0; s < streetList.size(); s++) {

                        if (street[b] == streetList[s]) {

                            test1 = 0;

                        } else {

                            test2 = true;

                        }
                    }

                    if (test1 == 0 && test2 == 1) {

                        break;

                    } else if (test1 == true && test2 ==true) {

                        streetList.push_back(street[b]);

                    }

                }
            }
            else
            {
                break;
            }
        }

        if (streetList.size() == j)
        {
            break;
        }
    }
    return streetList;
}
