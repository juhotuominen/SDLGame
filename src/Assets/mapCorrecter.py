try:
    with open('correctSizeMap_TileLayer1.csv', 'r') as file:
        # Read all lines from the file
        lines = file.readlines()

    # Modify the numbers as needed
    for i in range(len(lines)):
        # Split the line into individual values
        values = lines[i].strip().split(',')
        
        # Check and format each number in the values
        for j in range(len(values)):
            if values[j].isdigit() and int(values[j]) < 10:
                # Format the number as 2 digits (00-09)
                values[j] = f'{int(values[j]):02}'

        # Join the modified values and add a newline character
        lines[i] = ','.join(values) + '\n'

    # Open the same file in write mode to overwrite its contents
    with open('correctSizeMap_TileLayer1.csv', 'w') as file:
        # Write the modified lines back to the file
        file.writelines(lines)
        print("done")
except Exception as e:
    print(f"An error occurred: {e}")

