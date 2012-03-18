#!/bin/bash

outputfile=$2
echo "" > $outputfile

numnodes=$1

workloads=( 300 200 100 50 10 )
sleeptimes=( "20" "25" "30" "35" "40" "45" "50" "55" "60" )



for (( i = 0 ; i < 9 ; i++ ))
do
	for (( j = 0 ; j < 5 ; j++ ))
	do
		echo "Running (sleep: ${sleeptimes[$i]}; workload: ${workloads[$j]})"

		php apis.php 1 ${sleeptimes[$i]}
                php apis.php 2 ${sleeptimes[$i]}
                php apis.php 3 ${sleeptimes[$i]}

		php generate_test.php 1000 ${workloads[$j]} $numnodes
		sleep 2
		
		results=`php get_DB_results.php $numnodes`
		echo -e "${sleeptimes[$i]}\t${workloads[$j]}\t$results" >> $outputfile
	done
done

