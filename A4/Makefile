all: 
	@echo "\n"
	@g++ -ld_classic Format_Checker.cpp -o format_checker
	@g++ -ld_classic code.h startup_code.cpp -o code
	@./run.sh alarm.bif records.dat
	@echo "\n"
	@./format_checker
	@echo "\n"

clean:
	-rm code format_checker test format_checker_test
	-rm 2021CS10095_2021CS10566.zip
	@clear

move:
	@rm -rf ./2021CS10095_2021CS10566/*
	@cp writeup.txt compile.sh run.sh startup_code.cpp code.h ./2021CS10095_2021CS10566/
	@zip -rj  2021CS10095_2021CS10566.zip ./2021CS10095_2021CS10566

# test:
# 	@g++ -ld_classic Format_Checker_test.cpp -o format_checker_test
# 	@./run.sh alarm.bif records_gen.dat
# 	@./format_checker_test
# 	@sleep 2
	


# gen:
# 	@g++ -ld_classic code.h startup_code.cpp -o test
# 	@./test alarm.bif records.dat
# 	@sleep 2
# 	@rm test