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
	clear

