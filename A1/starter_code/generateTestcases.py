# import random
# import subprocess
# import os
# n = 10  # Replace with the number of lines you want to generate
# input_directory = "./../test_cases/"  # Replace with your input directory path
# output_directory = "./../test_cases/"  # Replace with your output directory path
# cpp_program = "./a.out"        # Replace with your C++ program's executable path

# with open("baseForTest.txt", "w") as output_file:
#     for _ in range(n):
#         x = random.randint(2, 20)
#         y = random.randint(x, 20)
#         output_file.write(f"2 {x} {y}\n")
#         input_path = os.path.join(input_directory, "input{}".format(_+5))
        
# input_files = [filename for filename in os.listdir(input_directory) if filename.startswith("input")]

# for input_filename in input_files:
#     input_path = os.path.join(input_directory, input_filename)

#     # Extract the index from the input filename (assuming it's in the format "inputX.txt")
#     index = input_filename.replace("input", "").replace(".txt", "")

#     output_filename = f"output{index}.txt"
#     output_path = os.path.join(output_directory, output_filename)

#     # Run the C++ program with input and output file arguments
#     subprocess.run([cpp_program, input_path, output_path], stdout=subprocess.PIPE)

#     print(f"Generated {output_filename}")




import os
import subprocess
import random

# Replace with your C++ program's executable path
cpp_program = "./a.out"

# Generate 10 sets of 3-sized tuples (2, x, y) where x <= y
input_data = []
for _ in range(10):
    x = random.randint(2, 20)
    y = random.randint(x, 20)
    input_data.append((2, x, y))

# Create an input directory if it doesn't exist
input_directory = "input_data"
os.makedirs(input_directory, exist_ok=True)

# Create input files and run the C++ program
for line_number, tuple_data in enumerate(input_data, start=1):
    input_filename = f"input{line_number}.txt"
    input_path = os.path.join(input_directory, input_filename)

    # Convert tuple data to a string representation
    input_content = " ".join(map(str, tuple_data))

    # Write input content to the input file
    with open(input_path, "w") as input_file:
        input_file.write(input_content)

    output_filename = f"output{line_number}.txt"
    output_path = os.path.join(input_directory, output_filename)

    # Run the C++ program with input and capture the output
    completed_process = subprocess.run([cpp_program], input=input_content, text=True, capture_output=True)

    # Get the captured output
    cpp_output = completed_process.stdout

    # Write the output content to the output file
    with open(output_path, "w") as output_file:
        output_file.write(cpp_output)

    print(f"Generated {input_filename} and {output_filename}")
