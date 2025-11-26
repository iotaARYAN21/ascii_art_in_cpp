for compiling with cImg
g++ main.cpp -o app -O2 -lX11

OR

g++ ascii.cpp -o app -O2



## Example program 

#include "CImg.h"
using namespace cimg_library;

int main() {
    CImg<unsigned char> img("input.png");

    cimg_forXY(img, x, y) {
        img(x,y,0) = 255 - img(x,y,0);  // R
        img(x,y,1) = 255 - img(x,y,1);  // G
        img(x,y,2) = 255 - img(x,y,2);  // B
    }

    img.save("output.png");
}



# Day 1

Started looking for methods to process and manipulate pixels in image in C++ language. Focusing on a method which could allowing
me to manipulate pixels by taking least amount of storage .  I came across methods like **openCV** , **cImg** but they both required some additional decoders to be downloaded in order to process images . Then I came across **stb_image** this one works by just downloading its header file i.e no need for additional downloads

Understood how it stores input image 
It takes three input variables for height , width and channel
the image pixels is stored in an char* array
each pixel has R, G, B values that means channel for each pixel value is 3
if width=3 => there are 3 pixels in one row => total 3*channel values in one row
each pixel value ranges between 0 - 255

# Day 2
Created a mapping for all the pixel values (0-255) so that when I print it on the terminal , it may resemble the image layout and the distinction of it on the high end (i.e not in detail yet)
write now not clear why printing width*channel number of values in one row is not able to print the image correctly


# Day 3
Identified the bug of why the image was not getting printed correctly
Since in the char* [] in which we are storing the image we are limiting the channels to 3 (R,G,B) only so the number of values in 1 row 
will be width*3 (earlier I was using channel value to multiply whose value was 4 (R,G,B,A))
Since the number of characters are large so needed to zoom out to max in order to show the ascii art 
Thought of way to decrease the size so that output is visible in one window size 
Searched for method but not found any good method . Ultimately came across resizing function available inside stbi_image which can reduce 
the height and width of the image . Then it can be used to print the ascii art

# TODO
while resizing how to make sure the parts of the main content not get cropped.
    possible solution: 
        - can think of removing the background from the image and then working on it  but then the edges of the art is not that good . also stb_image not supports          bg_remover . I will have to use then OpenCV

# Day 4
Added color to the symbols in the terminal using ANSII escape sequences . Used the extended 256 color variant as all the values in the image[] lie between 0 to 255 . Now the ascii art main part is looking more distinctive from the rest of the chars.
Syntax:
    \033[38;5;<color_code>m

eg
    echo -e "\033[38;5;196mThis is bright RED (256-color)\033[0m"
    echo -e "\033[38;5;82mThis is neon GREEN\033[0m"



