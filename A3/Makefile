part1:
	python3 generateTesetcase.py
	./compile.sh
	./run1.sh test
	-minisat test.satinput test.satoutput
	./run2.sh test
	g++ checkGraph.cpp -o checker_1
	./checker_1 < test.graphs


part2:
#	./run3.sh test_2


all:
	part1
