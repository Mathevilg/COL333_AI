all:
# part1:
	python3 generateTesetcase.py
	./compile.sh
	./run1.sh test
	-minisat test.satinput test.satoutput &
	sleep 5
	./run2.sh test
	g++ checkGraph.cpp -o checker
	./checker 1 < test.graphs
	python3 makeTestcase.py
	./part2 test2 < test2.graphs
	./checker 2 < test2.graphs

# part2:
# 	g++ part2_check.cpp -o part2_check
# 	./part2_check test2 < test.graphs
# # ./run3.sh test_2


# all:
# 	part2 part1