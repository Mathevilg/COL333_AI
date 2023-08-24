import random
import subprocess
import os
n = 10  # Replace with the number of lines you want to generate
input_directory = "./../test_cases/"  # Replace with your input directory path
output_directory = "./../test_cases/"  # Replace with your output directory path
cpp_program = "./a.out"        # Replace with your C++ program's executable path

with open("baseForTest.txt", "w") as output_file:
    for _ in range(n):
        x = random.randint(2, 20)
        y = random.randint(x, 20)
        output_file.write(f"2 {x} {y}\n")
        input_path = os.path.join(input_directory, "input{}".format(_+5))
        
input_files = [filename for filename in os.listdir(input_directory) if filename.startswith("input")]

for input_filename in input_files:
    input_path = os.path.join(input_directory, input_filename)

    # Extract the index from the input filename (assuming it's in the format "inputX.txt")
    index = input_filename.replace("input", "").replace(".txt", "")

    output_filename = f"output{index}.txt"
    output_path = os.path.join(output_directory, output_filename)

    # Run the C++ program with input and output file arguments
    subprocess.run([cpp_program, input_path, output_path], stdout=subprocess.PIPE)

    print(f"Generated {output_filename}")