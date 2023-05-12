/*
1/11/2023 - Make different versions of this code for future change. 

important commands: 
 - scp klarina@tau.physics.ucsb.edu:/net/cms26/cms26r0/milliqan/Run3Offline/v30/*879*.root ./
 - hadd -k Run679v2.root MilliQan_Run879.*.root

Takes an input text file from the code:
    TFile * f = new TFile("Run879v2.root")
    TTree * t = (TTree *) f->Get("t;1")
    t->SetScanField(0)
    .> r879_4v2.log
    t->Scan("tTrigger:chan:column:row:layer:area:fileNumber:event", "tTrigger==4");

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

string gfile_name = "r879_4v2.log";
const int array_size = 1000;
bool print_details = false;


int checkTrigger2(string file_name)
{
    ifstream testFile(file_name);
    string line;

    //initialize nessassary variables:
    int run = 0;
    int area = 0; //to determinroote last entry


    //size variables
    int chan[array_size] = {-1};
    int col[array_size] = {0};
    int row[array_size]= {0};
    int layer[array_size]= {0};
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
        int info[8] = {0}; 
                                       
        // all the info from the line
        string curr_num = "";
        int info_index = 0;
        for(int i = 1; i < line.length(); i++){
            if (isdigit(line[i])){
                curr_num = curr_num + line[i];
            }
            else if(isdigit(line[i-1])){
                    if(info_index < 10){
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
                //cout << "\n\n...checking if run "<< run << " is right \n";
                //layer argument
                layer0 = false;
                layer1 = false;
                layer2 = false;
                layer3 = false;

                for(int i=0; i<array_size; i++){
                    if(chan[i] >= 0){
                        
                        //cout << "chanel: "<< chan[i] << endl;
                        if(layer[i] == 0){layer0=true;}
                        else if(layer[i] == 1){layer1=true;}
                        else if(layer[i]==2){layer2=true;}
                        else if(layer[i]==3){layer3=true;}
                        else if(layer[i]==4){}
                        else{
                            cout << layer[i] <<": ERROR! FALSE LAYER!! \n\n";
                        }
                    }
                }
                
                if(int(layer0) + int(layer1) + int(layer2) + int(layer3) > 2){
                    passed_layer=true; 
                    pass_layer++;
                }
                else{
                    if(print_details){
                        cout << "Run: " << run << endl;
                        cout << "layer condition: \t";
                        cout << "FAILED"<<endl;
                    }
                }
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
                                                        //i == first layer, j == second layer, k == third layer
                                                        if(row[j]>=row[i] && row[j]<=row[k]){passed_line_row=true;}
                                                        else if(abs(row[j]-row[i])<=1 && abs(row[j]-row[k])<=1 ){passed_line_row=true;}
                                                    }
                                                    else{
                                                        if(row[j]<=row[i] && row[j] >= row[k]){passed_line_row=true;}
                                                        else if(abs(row[j]-row[i])<=1 && abs(row[j]-row[k])<=1 ){passed_line_row=true;}
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
                                                        else if(abs(row[j]-row[i])<=1 && abs(row[j]-row[k])<=1 ){passed_line_row=true;}
                                                    }
                                                    else{
                                                        if(row[j]<=row[i] && row[j] >= row[k]){passed_line_row=true;}
                                                        else if(abs(row[j]-row[i])<=1 && abs(row[j]-row[k])<=1 ){passed_line_row=true;}
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
                                                    else if(abs(row[j]-row[i])<=1 && abs(row[j]-row[k])<=1 ){passed_line_row=true;}
                                                }
                                                else{
                                                    if(row[j]<=row[i] && row[j] >= row[k]){passed_line_row=true;}
                                                    else if(abs(row[j]-row[i])<=1 && abs(row[j]-row[k])<=1 ){passed_line_row=true;}
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
                                                        else if(abs(col[j]-col[i])<=1 && abs(col[j]-col[k])<=1 ){passed_line_col=true;}
                                                    }
                                                    else{
                                                        if(col[j]<=col[i] && col[j] >= col[k]){passed_line_col=true;}
                                                        else if(abs(col[j]-col[i])<=1 && abs(col[j]-col[k])<=1 ){passed_line_col=true;}
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
                                                        else if(abs(col[j]-col[i])<=1 && abs(col[j]-col[k])<=1 ){passed_line_col=true;}
                                                    }
                                                    else{
                                                        if(col[j]<=col[i] && col[j] >= col[k]){passed_line_col=true;}
                                                        else if(abs(col[j]-col[i])<=1 && abs(col[j]-col[k])<=1 ){passed_line_col=true;}
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
                                                    else if(abs(col[j]-col[i])<=1 && abs(col[j]-col[k])<=1 ){passed_line_col=true;}
                                                }
                                                else{
                                                    if(col[j]<=col[i] && col[j] >= col[k]){passed_line_col=true;}
                                                    else if(abs(col[j]-col[i])<=1 && abs(col[j]-col[k])<=1 ){passed_line_col=true;}
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


                
                if(passed_line_row && passed_line_col){ 
                    pass_line++;
                }
                else{
                    if(print_details){
                        if(passed_layer){
                            cout << "layer condition: \t PASSED" << endl;
                        }
                        cout<< "line condition:   \t";
                        cout << "FAILED" << endl << endl;
                    }
                }

            }
            //resets everything;
            for(int i=0; i<array_size; i++){
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
    if(print_details){
        cout << "Pass Layer:\t" << pass_layer << endl;
        cout << "Pass Line: \t" << pass_line << endl;
    }
    cout << double(pass_line)/total*100 << " percent" << endl;
    return 1;
}

int checkTrigger1(string file_name)
{
    ifstream testFile(file_name);
    string line;

    //initialize nessassary variables:
    int run = 0;
    int area = 0; //to determinroote last entry: pretty sure never used the variable, but too scared to delete


    //size variables
    int chan[array_size] = {-1};
    int col[array_size] = {0};
    int row[array_size]= {0};
    int layer[array_size]= {0};
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
                //cout << "\n\n...checking if run "<< run << " is right \n";
                //layer argument
                layer0 = false;
                layer1 = false;
                layer2 = false;
                layer3 = false;

                for(int i=0; i<array_size; i++){
                    if(chan[i] >= 0){
                        //cout << "chanel: "<< chan[i] << endl;
                        if(layer[i] == 0){layer0=true;}
                        else if(layer[i] == 1){layer1=true;}
                        else if(layer[i]==2){layer2=true;}
                        else if(layer[i]==3){layer3=true;}
                        else if(layer[i]==4){}
                        else{cout << "ERROR! FALSE LAYER!! \n\n";}
                    }
                }
                
                if(int(layer0) + int(layer1) + int(layer2) + int(layer3) > 3){
                    //cout << "PASSED" << endl; 
                    passed_layer=true; 
                    pass_layer++;
                }
                else{
                    if(print_details){
                        cout << "Run: " << run << endl;
                        cout << "layer condition: \t";
                        cout << "FAILED"<<endl;
                    }
                }
                total++;

            }
            //resets everything;
            for(int i=0; i<array_size; i++){
                layer[i]=0;
                chan[i]=-1;
                col[i]=0;
                row[i]=0;
            }
            run = 0;
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
    if(print_details){
        cout << "Pass Layer:\t" << pass_layer << endl;
    }
    cout << double(pass_layer)/total*100 << " percent" << endl;
    return 1;
}

int checkTrigger3(string file_name)
{
    ifstream testFile(file_name);

    //ifstream testFile("r607_3.log");


    string line;

    //constant variables:
    

    //initialize nessassary variables:
    int run = 0;
    int area = 0; //to determinroote last entry: pretty sure never used the variable, but too scared to delete


    //size variables
    int chan[array_size] = {-1};
    int col[array_size] = {0};
    int row[array_size]= {0};
    int layer[array_size]= {0};
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
                //cout << "\n\n...checking if run "<< run << " is right \n";
                //layer argument
                layer0 = false;
                layer1 = false;
                layer2 = false;
                layer3 = false;

                for(int i=0; i<array_size; i++){
                    if(chan[i] >= 0){
                        //cout << "chanel: "<< chan[i] << endl;
                        if(layer[i] == 0){layer0=true;}
                        else if(layer[i] == 1){layer1=true;}
                        else if(layer[i]==2){layer2=true;}
                        else if(layer[i]==3){layer3=true;}
                        else if(layer[i]==4){}
                        else{cout << "ERROR! FALSE LAYER!! \n\n";}
                    }
                }
                
                if(layer0 && (layer2 || layer3)){
                    //cout << "PASSED" << endl; 
                    passed_layer = true; 
                    pass_layer++;
                }
                else if(layer1 && layer3){
                    //cout << "PASSED" << endl; 
                    passed_layer=true; 
                    pass_layer++;
                }
                else{
                    if(print_details){
                        cout << "Run: " << run << endl;
                        cout << "layer condition: \t";
                        cout << "FAILED"<<endl;
                    }
                }

                total++;

            }
            //resets everything;
            for(int i=0; i<array_size; i++){
                layer[i]=0;
                chan[i]=-1;
                col[i]=0;
                row[i]=0;
            }
            run = 0;
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
    if(print_details){
        cout << "Pass Layer:\t" << pass_layer << endl;
    }
    cout << double(pass_layer)/total*100 << "percent" << endl;
    return 1;
}

int checkTrigger4(string file_name)
{
    ifstream testFile(file_name);

    //ifstream testFile("r607_3.log");


    string line;

    //constant variables:
    

    //initialize nessassary variables:
    int run = 0;
    int area = 0; //to determinroote last entry: pretty sure never used the variable, but too scared to delete


    //size variables
    int chan[array_size] = {-1};
    int col[array_size] = {0};
    int row[array_size]= {0};
    int layer[array_size]= {0};
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
                //cout << "\n\n...checking if run "<< run << " is right \n";
                //layer argument
                layer0 = false;
                layer1 = false;
                layer2 = false;
                layer3 = false;

                for(int i=0; i<array_size; i++){
                    if(chan[i] >= 0){
                        //cout << "chanel: "<< chan[i] << endl;
                        if(layer[i] == 0){layer0=true;}
                        else if(layer[i] == 1){layer1=true;}
                        else if(layer[i]==2){layer2=true;}
                        else if(layer[i]==3){layer3=true;}
                        else if(layer[i]==4){}
                        else{cout << "ERROR! FALSE LAYER!! \n\n";}
                    }
                }
                
                if( (layer0 && layer1) || (layer1 && layer2) || (layer2 && layer3) ){
                    //cout << "PASSED" << endl; 
                    passed_layer = true; 
                    pass_layer++;
                }
                else{
                    if(print_details){
                        cout << "Run: " << run << endl;
                        cout << "layer condition: \t";
                        cout << "FAILED"<<endl;
                    }
                }

                total++;

            }
            //resets everything;
            for(int i=0; i<array_size; i++){
                layer[i]=0;
                chan[i]=-1;
                col[i]=0;
                row[i]=0;
            }
            run = 0;
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
    if(print_details){
        cout << "Pass Layer:\t" << pass_layer << endl;
    }
    cout << double(pass_layer)/total*100 << " percent" << endl;
    return 1;
}

int scan_all(string file_name){
    cout << "Trigger 1:"<< endl;
    int temp = checkTrigger1(file_name);
    cout << endl<<"Trigger 2:" << endl;
    temp = checkTrigger2(file_name);
    cout << endl << "Trigger 3:" << endl;
    temp = checkTrigger3(file_name);
    cout << endl << "Trigger 4:" << endl;
    temp = checkTrigger4(file_name);

    return 1;
}

int scan_all(){
    cout << "Trigger 1:"<< endl;
    int temp = checkTrigger1(gfile_name);
    cout <<"Trigger 2:" << endl;
    temp = checkTrigger2(gfile_name);
    cout << "Trigger 3:" << endl;
    temp = checkTrigger3(gfile_name);
    cout << "Trigger 4:" << endl;
    temp = checkTrigger4(gfile_name);

    return 1;
}

void all_files(){
    string file_name = "r879_1v2.log";
    cout << endl << "Trigger 1 enabled: " << endl;
    scan_all(file_name);

    file_name = "r879_2v2.log";
    cout << endl << "Trigger 2 enabled: " << endl;
    scan_all(file_name);

    file_name = "r879_3v2.log";
    cout << endl << "Trigger 3 enabled: " << endl;
    scan_all(file_name);

    file_name = "r879_4v2.log";
    cout << endl << "Trigger 4 enabled: " << endl;
    scan_all(file_name);
}