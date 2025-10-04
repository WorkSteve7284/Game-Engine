
""" Python script to add escape sequences to YAML files. """

import sys
import os
from pathlib import Path

def escape_file(og_file, target_path):
    """ Add the escape sequences and write to a header file. """
    try:
        with open(og_file, 'r', encoding="utf-8") as file:
            try:
                with open(target_path, 'w', encoding="utf-8") as header:
                    print("Processing file \"", og_file, "\"")
                    header.write("R\"object(\n")
                    header.write(file.read())
                    header.write("\n)object\";")
            except FileNotFoundError:
                print(f"Failed to open or create file \"{target_path}\"!")
    except FileNotFoundError:
        print(f"File \"{arg}\" does not exist!")

# Check if target directory exists, and if not create nessecary directories
if not os.path.exists("src/prefabs/headers"):
    os.makedirs("src/prefabs/headers")

# Process all files passed in through CMake
for arg in sys.argv[1:]:
    escape_file(arg, f"src/prefabs/headers/{Path(arg).stem}.hpp")
