#!/bin/bash
# Basic until loop

counter=1
sites='www.ufrj.br www.ucla.edu www.phil.cam.ac.uk www.adelaide.edu.au'


for site in $sites
do
	counter=1
	until [ $counter -gt 10 ]
	do
		traceroute -I $site >> traceroute.txt
		printf "\n\n" >>traceroute.txt
		((counter++))
	done

	echo $site done
	printf "\n\n\n\n" >> traceroute.txt


done
echo All done
