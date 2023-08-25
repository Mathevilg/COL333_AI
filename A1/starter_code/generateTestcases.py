import os
import subprocess
import random

# Replace with your C++ program's executable path
subprocess.run(["g++ -o genTestcase generateRandomTest.cpp"], shell=True)
cpp_program = "./genTestcase"

# Generate 10 sets of 3-sized tuples (2, x, y) where x <= y
input_data = []
for _ in range(10):
    x = random.randint(2, 20)
    y = random.randint(x, 20)
    input_data.append((2, x, y))

# Create an output directory if it doesn't exist
output_directory = "random_testcases"
os.makedirs(output_directory, exist_ok=True)

# Create input files and run the C++ program
for line_number, tuple_data in enumerate(input_data, start=1):
    input_filename = f"input{line_number}.txt"
    input_path = os.path.join(output_directory, input_filename)

    # Convert tuple data to a string representation
    input_content = " ".join(map(str, tuple_data))

    # Write input content to the input file
    with open(input_path, "w") as input_file:
        input_file.write(input_content)

    # Run the C++ program with input and capture the output
    completed_process = subprocess.run([cpp_program], input=input_content, text=True, capture_output=True)

    # Get the captured output
    cpp_output = completed_process.stdout

    # Write the output content to the output file with the same name as input file
    with open(os.path.join(output_directory, input_filename), "w") as output_file:
        output_file.write(cpp_output)

    print(f"Generated {input_filename}")
