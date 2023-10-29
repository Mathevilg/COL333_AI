all: 
	@echo "\n"
	@g++ -ld_classic Format_Checker.cpp -o format_checker
	@g++ -ld_classic code.h startup_code.cpp -o code
	@./run.sh alarm.bif records.dat
	@echo "\n"
	@./format_checker
	@echo "\n"

clean:
	-rm code format_checker
	-rm 2021CS10095_2021CS10566.zip
	@clear

move:
	@rm -rf ./2021CS10095_2021CS10566/*
	@cp writeup.txt compile.sh run.sh startup_code.cpp code.h ./2021CS10095_2021CS10566/
	@zip -rj  2021CS10095_2021CS10566.zip ./2021CS10095_2021CS10566

test:
	@g++ -ld_classic test.cpp -o test
	@./test
	@sleep 2
	@rm test