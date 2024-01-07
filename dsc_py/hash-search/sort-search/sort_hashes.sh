#!/bin/bash

# Define the input file name and output file name
input_file="../hashes.txt"
output_file="../hashes.txt"

# Generate a random input file with unsorted data (you can replace this with your own data)
# For example, to create a file with 1000 random integers:
# for i in {1..1000}; do echo $RANDOM; done > $input_file

# Start measuring execution time
# start_time=$(date +%s.%N)

# Sort the input file and save the sorted output to a new file
sort -o "$output_file" "$input_file"  

# End measuring execution time
# end_time=$(date +%s.%N)

# Calculate the elapsed time in seconds
# elapsed_time=$(echo "$end_time - $start_time")

# Print the sorting time
# echo "Sorting time: $elapsed_time seconds"