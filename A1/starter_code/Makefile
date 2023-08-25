 all: main.cpp ./models/localSearch.cpp
	  g++ -o main main.cpp ./models/localSearch.cpp

generateTest:
	python3 generateTestcases.py

generateOutput: 
	 python3 generateOutput.py


test:
	python3 generateTestcases.py
	python3 generateOutput.py
# generateTest
#  all: main.cpp SportsLayout.cpp
# 	  g++ -o main main.cpp SportsLayout.cpp