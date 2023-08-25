
import os
import subprocess
import re

def extract_number(filename):
    # Extract the number using a regular expression
    match = re.search(r'(\d+)', filename)
    if match:
        # print(int(match.group(1)))
        return int(match.group(1))
    return -1  # Return a default value if no number is found

def custom_sort(filename):
    return extract_number(filename)

input_directory = "./random_testcases/"  # Replace with your input directory path
output_directory = "./random_testcases_output/"  # Replace with your output directory path
cpp_program = "./main"        # Replace with your C++ program's executable path
input_files = [filename for filename in os.listdir(input_directory) if filename.startswith("input")]

input_files = sorted(input_files, key=custom_sort)
with open("out.log", "w") as log_file:  # Open out.log for writing

    for input_filename in input_files:
        input_path = os.path.join(input_directory, input_filename)

        # Extract the index from the input filename (assuming it's in the format "inputX.txt")
        index = input_filename.replace("input", "").replace(".txt", "")

        output_filename = f"output{index}.txt"
        output_path = os.path.join(output_directory, output_filename)

        # Run the C++ program with input and output file arguments
        completed_process = subprocess.run([cpp_program, input_path, output_path], stdout=subprocess.PIPE, text=True)

        # Capture the output and write it to the log file
        output_to_log = completed_process.stdout
        log_file.write(output_to_log)

        print(f"Generated {output_filename}")
