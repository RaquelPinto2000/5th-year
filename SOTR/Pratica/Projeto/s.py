with open("base_img.c", "w") as fout:
    for r in range(128):
        fout.write("{")
        for c in range(127):
            fout.write("0x00, ")
        fout.write("0x00},\n")

b1 ="{"
b2 ="}"
with open("gen_functions.c", "w") as fout:
    for r in range(100):
        fout.write(f'uint8_t* gen_{r}_image() {b1} \nuint8_t* img = k_malloc(sizeof(start_image));\nmemcpy(img, &start_image, sizeof(start_image));\n//add functions\nreturn img;\n{b2}\n\n')

with open("switch.c", "w") as fout:
    for r in range(100):
        fout.write(f'case {r}:\n\timage_data = gen_{r}_image();\n\tbreak;\n')