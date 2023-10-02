all:
# part1:
	python3 generateTesetcase.py
	./compile.sh
	./run1.sh test
	echo "Starting minisat"
	-minisat test.satinput test.satoutput &
	sleep 5
	./run2.sh test
	g++ checkGraph.cpp -o checker_1
	./checker_1 1 < test.graphs
	g++ part2_check.cpp -o part2_check
	./part2_check test2 < test.graphs
	./checker_1 2 < test.graphs

# part2:
# 	g++ part2_check.cpp -o part2_check
# 	./part2_check test2 < test.graphs
# # ./run3.sh test_2


# all:
# 	part2 part1