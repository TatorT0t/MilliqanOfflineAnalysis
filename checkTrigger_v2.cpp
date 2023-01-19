/*
1/11/2023 - Make different versions of this code for future change. 

Takes an input text file from the code:
    TFile * f = new TFile("Run591.root")
    TTree * t = (TTree *) f->Get("t;1")
    .> input_file_name.log
    t->Scan("tTrigger:chan:column:row:layer:area:", "tTrigger==2");

Code checks of each "Row" (e.g. channel) has at least 3 layers hit, and somewhat of a line
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include "string"
#include <ctype.h>
using namespace std;

int file_main()
{
    ifstream testFile("t4.log");
    string line;

    //initialize nessassary variables:
    int run = 0;
    int area = 0; //to determinroote last entry
    int chan[74] = {-1};
    int col[74] = {0};
    int row[74]= {0};
    int layer[74]= {0};
    int total = 0;
    int pass_layer = 0;
    int pass_line = 0;
    int last_index = -1;

    //initialize layer conditionals:
    bool layer0 = false;
    bool layer1 = false;
    bool layer2 = false;
    bool layer3 = false;
    bool passed_layer= false;


    //initilize line condition variables:
    bool passed_line_row  = false;
    bool passed_line_col = false;

    while(getline(testFile, line)){

        //get the info out of line
        int info[8] = {0};                                  // all the info from the line
        string curr_num = "";
        int info_index = 0;
        for(int i = 1; i < line.length(); i++){
            if (isdigit(line[i])){
                curr_num = curr_num + line[i];
            }
            else if(isdigit(line[i-1])){
                    if(info_index < 8){
                        info[info_index] = stoi(curr_num);
                        curr_num = "";
                    }
                    info_index++;
            }
        }

        //input data into the variables
        if(info[1]==0){ //checks if this is the first instance
            //check if the run is right
            if(run != 0){
                cout << "\n\n...checking if run "<< run << " is right \n";
                //layer argument
                layer0 = false;
                layer1 = false;
                layer2 = false;
                layer3 = false;

                for(int i=0; i<74; i++){
                    if(chan[i] >= 0){
                        //cout << "chanel: "<< chan[i] << endl;
                        if(layer[i] == 0){layer0=true;}
                        else if(layer[i] == 1){layer1=true;}
                        else if(layer[i]==2){layer2=true;}
                        else if(layer[i]==3){layer3=true;}
                        else{cout << "ERROR! FALSE LAYER!! \n\n";}
                    }
                }
                cout << "layer condition: \t";
                if(int(layer0) + int(layer1) + int(layer2) + int(layer3) > 2){cout << "PASSED" << endl; passed_layer=true; pass_layer++;}
                else{cout << "FAILED"<<endl;}
                total++;

                //line condition:
                if(passed_layer && last_index > 0){
                    if(layer0){ //first layer triggered
                        if(layer3){ //last layer triggered
                            for(int i = 0; i <= last_index; i++){
                                if(layer[i]==0){ //check if any line formed with one point in the first
                                    for(int j=i; j <= last_index; j++){
                                        if(layer[j]==1 || layer[j]==2){ //assumes there could be some background activity happending (only three points have to form a line)
                                            for(int k=j; k<=last_index; k++){
                                                if(layer[k]==3){
                                                    if(row[i]<=row[k]){
                                                        if(row[j]>=row[i] && row[j]<=row[k]){passed_line_row=true;}
                                                    }
                                                    else{
                                                        if(row[j]<=row[i] && row[j] >= row[k]){passed_line_row=true;}
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else if (layer2){ //second to last layer triggered
                            for(int i = 0; i <= last_index; i++){
                                if(layer[i]==0){ //check if any line formed with one point in the first
                                    for(int j=i; j <= last_index; j++){
                                        if(layer[j]==1){
                                            for(int k=j; k<=last_index; k++){
                                                if(layer[k]==2){
                                                    if(row[i]<=row[k]){
                                                        if(row[j]>=row[i] && row[j]<=row[k]){passed_line_row=true;}
                                                    }
                                                    else{
                                                        if(row[j]<=row[i] && row[j] >= row[k]){passed_line_row=true;}
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else{ cout << "ERROR:layer trigger!";}
                    }
                    else if(layer1){ //first layer not triggered
                        for(int i = 0; i <= last_index; i++){
                            if(layer[i]==1){ //check if any line formed with one point in the first
                                for(int j=i; j <= last_index; j++){
                                    if(layer[j]==2){
                                        for(int k=j; k<=last_index; k++){
                                            if(layer[k]==3){
                                                if(row[i]<=row[k]){
                                                    if(row[j]>=row[i] && row[j]<=row[k]){passed_line_row=true;}
                                                }
                                                else{
                                                    if(row[j]<=row[i] && row[j] >= row[k]){passed_line_row=true;}
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else{cout << "ERROR:layer trigger!";}
                }

                if(passed_layer && last_index > 0){
                    if(layer0){ //first layer triggered
                        if(layer3){ //last layer triggered
                            for(int i = 0; i <= last_index; i++){
                                if(layer[i]==0){ //check if any line formed with one point in the first
                                    for(int j=i; j <= last_index; j++){
                                        if(layer[j]==1 || layer[j]==2){ //assumes there could be some background activity happending (only three points have to form a line)
                                            for(int k=j; k<=last_index; k++){
                                                if(layer[k]==3){
                                                    if(col[i]<=col[k]){
                                                        if(col[j]>=col[i] && col[j]<=col[k]){passed_line_col=true;}
                                                    }
                                                    else{
                                                        if(col[j]<=col[i] && col[j] >= col[k]){passed_line_col=true;}
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else if (layer2){ //second to last layer triggered
                            for(int i = 0; i <= last_index; i++){
                                if(layer[i]==0){ //check if any line formed with one point in the first
                                    for(int j=i; j <= last_index; j++){
                                        if(layer[j]==1){
                                            for(int k=j; k<=last_index; k++){
                                                if(layer[k]==2){
                                                    if(col[i]<=col[k]){
                                                        if(col[j]>=col[i] && col[j]<=col[k]){passed_line_col=true;}
                                                    }
                                                    else{
                                                        if(col[j]<=col[i] && col[j] >= col[k]){passed_line_col=true;}
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else{ cout << "ERROR:layer trigger!";}
                    }
                    else if(layer1){ //first layer not triggered
                        for(int i = 0; i <= last_index; i++){
                            if(layer[i]==1){ //check if any line formed with one point in the first
                                for(int j=i; j <= last_index; j++){
                                    if(layer[j]==2){
                                        for(int k=j; k<=last_index; k++){
                                            if(layer[k]==3){
                                                if(col[i]<=col[k]){
                                                    if(col[j]>=col[i] && col[j]<=col[k]){passed_line_col=true;}
                                                }
                                                else{
                                                    if(col[j]<=col[i] && col[j] >= col[k]){passed_line_col=true;}
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                //will look if colomns right. could have done it in same loop, but did not think of it so too late. Easier to just copy and paste



                cout<< "line condition:   \t";
                if(passed_line_row && passed_line_col){ cout << "PASSED" << endl; pass_line++;}
                else{cout << "FAILED" << endl;}


                /* //line argument
                bool line_argument = true;
                for(int i = 1; i <74; i++){
                    if(chan[i] > 0){
                        if(abs(row[i]-row[i-1]) > 1 && abs(col[i]-col[i-1]) > 1) {line_argument = false;}
                    }
                }
                cout << "line condition: \t";
                if(line_argument){cout << "PASSED\n";}
                else{cout<<"FAILED\n";}             
 */
            }
            //resets everything;
            for(int i=0; i<74; i++){
                layer[i]=0;
                chan[i]=-1;
                col[i]=0;
                row[i]=0;
            }
            run = 0;
            passed_line_row=false;
            passed_line_col=false;
            passed_layer = false;
            
        }
        if(info[7] > 0 && info[3] != 74){
            run = info[0];
            //cout << info[1] << endl;
            chan[info[1]] = info[3];
            col[info[1]] = info[4];
            row[info[1]] = info[5];
            layer[info[1]] = info[6];
            last_index = info[1];
        }
    }

    cout << endl << "Total:\t" << total << endl;
    cout << "Pass Layer:\t" << pass_layer << endl;
    cout << "Pass Line: \t" << pass_line << endl;
    return 1;
}
