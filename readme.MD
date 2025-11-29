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
        - can think of removing the background from the image and then working on it  but then the edges of the art is not that good . also stb_image not supports bg_remover . I will have to use then OpenCV

# Day 4
Added color to the symbols in the terminal using ANSII escape sequences . Used the extended 256 color variant as all the values in the image[] lie between 0 to 255 . Now the ascii art main part is looking more distinctive from the rest of the chars.
    Syntax:
        \033[38;5;<color_code>m

    eg
        echo -e "\033[38;5;196mThis is bright RED (256-color)\033[0m"
        echo -e "\033[38;5;82mThis is neon GREEN\033[0m"

# Day 5
Trying to assign color based on the rgb value of each pixel 
for each pixel value i will try to assign an ascii value and a particular color depending on the brightness of the pixel as brightness influence the ascii char and color

We are concerned about how light and dark a pixel is so we are using brightness value (convert to greyscale)

if brightness is low  => use darker color and a particular symbol 
elif brightness is high => use brighter color 

# Day 6
To better represent the image I converted the rgb values to grayscale brightness (as now aiming to depict the bright and low part of the image with differenet colors and ascii chars) . For this first I converted the rgb values (img[i],img[i+1],img[i+2]) to brightness using approximate formula

    brightness = (r+g+b)/3;
    NOTE: the actual formula is different from it as in real images the proportion of green is more than red and blue

    then for darker brightness I used different symbol and ascii and similarly for brighter 

    string getColor(int brightness){
        if(brightness < 51)return "\033[30m";  // black
        else if(brightness < 101)return "\033[31m"; // red
        else if(brightness < 151)return "\033[33m"; // yellow
        else if(brightness<201)return "\033[36m"; // cyan
        return "\033[97m"; // white
    }

    char getASCII(int brightness){
        string chars = "@%#*+=-:. ";
        int index = brightness * (chars.size()-1)/255;
        return chars[index];
    }

Then for each pixel I printed the ascii along with its color

Now looking on the output , the ascii art at most of the boundaries of it is getting merged with the background . So here was a need to decide the edges of the image .
After searching I found that some ascii art used **Sobel operator** for edge detection in image which is easy to use .

First of all we need to know what classifies as an edge 
    a location around which there is sudden large change in the brightness(pixel value) -> we call it region of edge
    Gradient of a function => the rate and direction of the greatest change in the function
    so here gradient means => how much brightness is changing
        If a pixel’s brightness changes a lot from one side to the other → strong gradient → edge.
        If brightness hardly changes → weak gradient → no edge.

For this first I created a matrix of the above brightness values .
then I passed it to a function which will decided which char should be replaced with an edge char.
the possible edge chars are "| \ / -"

sobel uses two 3*3 matrices .
Gx[3][3] -> to determine edge in horizontal direction
Gy[3][3] -> to determine edge in vertical direction

Now the question is -> How do we use it to assign edges in ascii art

    The logic is as follows:
        for each pixel (i,j) we find 9 values of brightness
        (i-1,j-1) (i-1,j) (i-1,j+1)
        (i,j-1)   (i,j)   (i,j+1)
        (i+1,j-1) (i+1,j) (i+1,j+1)

we multiply these values with all 9 values of Gx and Gy separately and add to sum variables 
sumX and sumY
these two variables are used to calculate the gradient
grad[i][j] = sqrt(sumX*sumX + sum*sumY)

if(grad[i][j]>255)grad[i][j]=255 as it is the highest possible value

Note: for all (i,j) it is not possible to calculate the 9 values so we set 0 for the one for which it is not possible.

Now we need a threshold value that will decide whether we want it to be edge or not
I took threshold = 100
if grad[i][j] > 100 =>  the char is replaced with an edge char

Now we need to know which direction the edge will face
for this we use angle = sumY/sumX and convert it in degree
depending upon the different angle value I am deciding the direction of the edge


Ultimately I am returning the gradient matrix and the angle matrix and for each brightness value I am checking whether to print an edge or not


        



