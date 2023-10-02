all:
	python3 generateTesetcase.py
	./compile.sh
	./run1.sh test
	-minisat test.satinput test.satoutput
	./run2.sh test
#	./run3.sh test_2
