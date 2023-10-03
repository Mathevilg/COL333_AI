# Define the input and output file names
input_file_name = "test.graph"
output_file_name = "test2.graph"

# Read the content of the input file
with open(input_file_name, 'r') as input_file:
    lines = input_file.readlines()

# Modify the first line (index 0) with the new content


# Update the content in memory
lines[0] = list(map(int, lines[0].split()))[0:2]
lines[0] = str(lines[0][0]) + " " + str(lines[0][1])  + "\n"

# Write the modified content to the output file
with open(output_file_name, 'w') as output_file:
    output_file.writelines(lines)

print(f"First line changed and saved to '{output_file_name}'")
