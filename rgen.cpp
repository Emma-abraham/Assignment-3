//
// Created by Emmanuel Abraham on 2019-10-22.
//

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include<unistd.h>
#include <fcntl.h>
#include "myStreetList.h"


using namespace std;

vector<string> streetList;

void graphInOut(int street[100][500][2], int sn, int count[100], int remove) {

    int j, k;

    if (streetList.size() != 0) {
        // Remove the existing street details
        for(j = 0; j < remove; j++) {
            cout << "r" << " \"" << streetList[j] << "\"" << endl;
        }

        streetList.clear();

    }

    if (streetList.size() == 0) {
        // Randomize street from the mystreetList.h header file

        streetList = streetPicker(sn);
        for (j = 0; j < sn; j++) {

            cout << "a" << " \"" << streetList[j] << "\" ";

            for (k = 0; k < count[j]; k++) {

                cout << "(" << street[j][k][0] << "," << street[j][k][1] << ")";

            }

            cout << endl;

        }
        // give the g command

        cout << 'g' << endl;
    }
}

void setCount(int count[100]) {
    int j;

    for (j = 0; j < 100; j++) {
        count[j]=0;
    }
}

int randomizeInput(int x , int y) {
    int Z;
    static int fd = -1;
    char* next_random_value;
    int values_to_read;
    unsigned random_value;

    if (fd == -1) {
        fd = open("/dev/urandom", O_RDONLY);
    }

    next_random_value = (char*) &random_value;
    values_to_read = sizeof(random_value);

    do {
        int bytes_read;
        bytes_read = read(fd, next_random_value, values_to_read);
        values_to_read -= bytes_read;
        next_random_value += values_to_read;
    } while (values_to_read > 0);

    Z = x + (random_value %(y - x + 1));

    return Z;
}

int intersect(int p1[],int p2[],int p3[],int p4[]) {
    float a,b,c;
    float a1,c1;
    float x1 = (float) p1[0];
    float x2 = (float) p2[0];
    float x3 = (float) p3[0];
    float x4 = (float) p4[0];
    float y1 = (float) p1[1];
    float y2 = (float) p2[1];
    float y3 = (float) p3[1];
    float y4 = (float) p4[1];
    float y5;


    if (x3 == x4 && y3 == y4) {
        return 0;
    }

    if ((x1 == x3 && y1 == y3 && x2 == x4 && y2 == y4)||(x1 == x4 && y1 == y4 && x2 == x3 && y2 == y3)) {
        return 0;
    }

    if (x1 == x2 && x1 == x3 && x1 == x4) {

        if ((y1 < y3 && y3 < y2) || (y2 < y3 && y3 < y1) || (y1 < y4 && y4 < y2) || (y2 < y4 && y4 < y1) ||
            (y3 < y1 && y1 < y4)  || (y4 < y1 && y1 < y3) || (y3 < y2 && y2 < y4) || (y4 < y2 && y2 < y3))
        {
            return 0;
        }
    }

    else {

        a1 = (x1 - x2);
        c1 = (x3 - x4);

        if (a1 != 0 && c1 != 0) {
            a = (y1 - y2) / a1;
            b = y1 - a * x1;
            c = (y3 - y4) / c1;

            if (a == 0.0 && c == 0.0) {
                if((x1 < x3 && x3 < x2) || (x2 < x3 && x3 < x1) || (x1 < x4 && x4 < x2) || (x2 < x4 && x4 < x1) ||
                   (x3 < x1 && x1 < x4) || (x4 < x1 && x1 < x3) || (x3 < x2 && x2 < x4) || (x4 < x2 && x2 < x3))
                {
                    return 0;
                }
            }
            else if(a == c) {
                y5 = a * x3 + b;

                if (y3 == y5 && ((y1 < y3 && y3 < y2) || (y2 < y3 && y3 < y1) || (y1 < y4 && y4 < y2) || (y2 < y4 && y4 < y1)
                                 || (y3 < y1 && y1 < y4) || (y4 < y1 && y1 < y3) || (y3 < y2 && y2 < y4) || (y4 < y2 && y2 < y3)))
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}


int main(int argc, char* argv[]) {

    int s_default = 10;      // -s statement [2,k]
    int n_default = 5;       // -n statement [1,k]
    int l_default = 5;       // -l statement [5,k]
    int c_default = 20;      // -c statement [-k,k]

    int s_s, s_n, s_l, cX, cY;
    int street[100][500][2] = {};

    int i, j, k;
    int m, n, t;

    int errorQty = 0;
    int temp[2];
    int checkresult;
    int count[100] = {0};
    int remove = 0;

    if(argc > 1) {

        int arg_value;

        for(i = 1 ; i < argc-1; i++) {

            arg_value = atoi(argv[i+1]); // converting the input string to obtain integers

            if(strcmp(argv[i], "-s") == 0) {

                if(arg_value < 2) {

                    cerr << "Error: the number of streets should not be less than 2" << endl;
                    return 0;

                }
                else if (arg_value > 100) {

                    cerr << "Error: the number of streets should not be greater than 100" << endl;
                    return 0;

                }
                else {

                    arg_value = s_default;

                }

            }

            else if(strcmp(argv[i], "-n") == 0) {

                if(arg_value < 1){

                    cerr << "Error: the number of line segments should not be less than 1" << endl;
                    return 0;

                }
                else if (arg_value > 100) {

                    cerr << "Error: the number of line segments should not be greater than 100" << endl;
                    return 0;
                }
                else {

                   arg_value = n_default;

                }
            }


            else if(strcmp(argv[i], "-l") == 0) {

                if(arg_value < 5){

                    cerr << "Error: Wait time should not be less than 5 seconds" <<endl;
                    return 0;

                }
                else {

                    arg_value = l_default;

                }
            }

            else if(strcmp(argv[i], "-c") == 0) {

                 arg_value = c_default;

            }
        }
    }

    s_l = randomizeInput(5, l_default);

    while (true) {

        s_s = randomizeInput(2,s_default);

        j = 0; //street number

        setCount(count); // will keep count array to zero (resets)

        errorQty = 0;

        while(j < s_s) {

            s_n = randomizeInput(2, s_default+1);

            k = 0; //street point

            while(k < s_n) {

                if (errorQty >= 25) {
                    cerr << "Error: The program has failed to generate valid input for 25 simultaneous attempts" << endl;
                    return 0;
                }

                cX = randomizeInput(-c_default,c_default);
                cY = randomizeInput(-c_default,c_default);

                temp[0] = cX;
                temp[1] = cY;

                // same points
                if (k > 0) {
                    if(street[j][k-1][0] == temp[0] && street[j][k-1][1] == temp[1]) {

                        errorQty++;
                        continue;

                    }
                }

                //
                for (n = 0; n < k-1 && k > 0; n++) {

                    checkresult = intersect(street[j][n], street[j][n+1], street[j][k-1], temp);

                    if (checkresult == 0) {
                        errorQty++;
                        break;
                    }
                }

                //
                if (n < k-1 && k > 0) {
                    continue;
                }

                //
                t = 0;
                for (m = 0; m < j && k > 0; m++){

                    for (n = 0; n < count[t] - 1; n++) {

                        checkresult = intersect(street[m][n],street[m][n+1],street[j][k-1],temp);

                        if(checkresult == 0){
                            errorQty++;
                            break;
                        }
                    }

                    //
                    if (n < count[t] - 1) {
                        break;
                    }

                    t++;

                }

                //
                if(m < j && k > 0) {
                    continue;
                }

                street[j][k][0] = cX;
                street[j][k][1] = cY;
                count[j]++;
                k++;

            }

            j++;

        }

        graphInOut(street,s_s,count,remove);
        cout.flush();

        usleep(s_l * 1000000);

        remove = s_s;
    }
    return 0;
}
