#!/bin/bash

# Set the variables for the remote server and password
server="klarina@tau.physics.ucsb.edu"
read -p "Run name: " myRun
read -p "Run number saved (can just be string): " runNum



# Start ROOT
root -l <<EOF

TFile *f = new TFile("$myRun");
TTree *t = (TTree *)f->Get("t;1");
t->SetScanField(0);

.> r${runNum}_1v2.log
t->Scan("tTrigger:chan:column:row:layer:area:fileNumber:event", "tTrigger==1");
.q

EOF

root -l <<EOF

TFile *f = new TFile("$myRun");
TTree *t = (TTree *)f->Get("t;1");
t->SetScanField(0);

.> r${runNum}_2v2.log
t->Scan("tTrigger:chan:column:row:layer:area:fileNumber:event", "tTrigger==2");
.q

EOF

root -l <<EOF

TFile *f = new TFile("$myRun");
TTree *t = (TTree *)f->Get("t;1");
t->SetScanField(0);

.> r${runNum}_4v3.log
t->Scan("tTrigger:chan:column:row:layer:area:fileNumber:event", "tTrigger==3");
.q

EOF

root -l <<EOF

TFile *f = new TFile("$myRun");
TTree *t = (TTree *)f->Get("t;1");
t->SetScanField(0);

.> r${runNum}_4v2.log
t->Scan("tTrigger:chan:column:row:layer:area:fileNumber:event", "tTrigger==4");
.q

EOF

