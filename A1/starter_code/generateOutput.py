

# import os
# import subprocess

# input_directory = "./../test_cases/"  # Replace with your input directory path
# output_directory = "./../test_cases/"  # Replace with your output directory path
# cpp_program = "./main"        # Replace with your C++ program's executable path
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
# # In this version of the script, the subprocess.run command runs your C++ program with the input and output file paths as command-line arguments. Make sure your C++ program handles command-line arguments appropriately using argc and argv.





import os
import subprocess

input_directory = "./random_testcases/"  # Replace with your input directory path
output_directory = "./random_testcases_output/"  # Replace with your output directory path
cpp_program = "./main"        # Replace with your C++ program's executable path
input_files = [filename for filename in os.listdir(input_directory) if filename.startswith("input")]

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
