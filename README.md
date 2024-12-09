# BMP to ASCII Art!

**What It Is:**  
This project takes a BMP image and converts it into ASCII art. Dark areas of the image are represented by dense ASCII characters like `@`, while light areas are represented by lighter characters like `.` or spaces.

If you view the ASCII output file on a white background with dark colored text, comment out the greyscale inversion in `ascii_converter.cpp`



**How It Works:**  
1. The program reads a BMP image file.  
2. It converts each pixel to a grayscale value.  
3. Each grayscale value maps to a character in a predefined ASCII gradient.  
4. The resulting ASCII art is printed to the console (or saved to a text file).

**How to Use:**  
1. Compile the code:  
   ```bash
   g++ -o ascii_art main.cpp image_loader.cpp ascii_converter.cpp
   ```  
2. Run the program with a BMP file as input:  
   ```bash
   ./ascii_art ./image-input/car.bmp > ./ascii-output/car.txt
   ```  
3. The ASCII art will be saved to the txt file.