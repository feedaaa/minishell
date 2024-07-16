import os

def concatenate_code_files(directory_path, output_file):
    with open(output_file, 'w') as outfile:
        for root, dirs, files in os.walk(directory_path):
            for file in files:
                if file.endswith('.c'):
                    file_path = os.path.join(root, file)
                    with open(file_path, 'r') as infile:
                        outfile.write(f"# File: {file}\n")
                        outfile.write(infile.read())
                        outfile.write("\n\n")

directory_path = '/Users/ffidha/Desktop/mebranch'
output_file = '/Users/ffidha/Desktop/mebranch/all_files.c'

concatenate_code_files(directory_path, output_file)

# this is just a python script for putting the content from all the files in this directory to one single file.