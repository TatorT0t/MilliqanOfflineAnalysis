#!/bin/bash

# Set the variables for the remote server and password
server="klarina@tau.physics.ucsb.edu"
read -p "Run number: " runNum
read -p "Run name saved: " runName

# Set the source file and destination path for the scp colsmmand
source_file="/net/cms26/cms26r0/milliqan/Run3Offline/v*/MilliQan_Run${runNum}.*.root"
echo $source_file
destination_path="./"

# Spawn the scp command
scp -r "$server:$source_file" "$destination_path"


rm $runName
hadd -k $runName MilliQan_Run$runNum.*.root
rm MilliQan_Run$runNum.*.root

