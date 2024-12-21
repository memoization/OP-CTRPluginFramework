# Example command prompt usage: "py pngtorawtoc.py icon.png"

from PIL import Image
import sys
import os

imorg = Image.open(sys.argv[1])
im = imorg.transpose(Image.ROTATE_270)
imorg.close()

pixels = im.convert('RGBA')
pix = list(pixels.getdata())

# Get the base file name (without extension) for the array name
base_name = os.path.splitext(os.path.basename(sys.argv[1]))[0]
array_name = f"{base_name}_array"

# Prepare the output in the desired C array format
array_size = len(pix)

# Start building the C array code
c_array_code = f"static const unsigned char {array_name}[{array_size * 4}] = {{\n"

# Add byte values
hex_values = []
for p in pix:
    # Each pixel (RGBA) will be represented as 4 bytes
    hex_values.append(f"0x{p[3]:02x}")
    hex_values.append(f"0x{p[2]:02x}")
    hex_values.append(f"0x{p[1]:02x}")
    hex_values.append(f"0x{p[0]:02x}")

# Break the hex_values into chunks of 12 bytes
for i in range(0, len(hex_values), 12):
    chunk = hex_values[i:i + 12]
    formatted_chunk = ', '.join(chunk)
    # Add a newline after every 12 bytes
    c_array_code += f"	{formatted_chunk},\n"

# Close the array
c_array_code = c_array_code.rstrip(',\n')
c_array_code += "\n};\n\n"

# Add the length and pointer
c_array_code += f"const int {array_name}_length = {array_size * 4};\n"
c_array_code += f"unsigned char *{base_name} = (unsigned char *){array_name};\n"

output_file = f"{base_name}.c"
with open(output_file, "w") as f:
    f.write(c_array_code)

print(f"c file saved to '{output_file}'")
