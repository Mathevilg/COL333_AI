all:
# compile
	g++ -ld_classic Format_Checker.cpp -o format_checker
	g++ -ld_classic startup_code.cpp -o code
# run
	./run.sh alarm.bif records.dat
# check
	./format_checker 
# all working !! 
	echo "\n\n\nall working\nformat checked ...."

clean:
	-rm code format_checker
	-rm 2021CS10095_2021CS10566.zip
	clear

move:
	rm -rf ./2021CS10095_2021CS10566/*
	cp writeup.txt compile.sh run.sh startup_code.cpp ./2021CS10095_2021CS10566/
	zip -rj  2021CS10095_2021CS10566.zip ./2021CS10095_2021CS10566