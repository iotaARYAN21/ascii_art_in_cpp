#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE2_IMPLEMENTATION
#include "stb_image_resize2.h"

#include <bits/stdc++.h>
using namespace std;

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

vector<vector<vector<float>>> sobel(vector<vector<int>> bright){
    int n = bright.size(),m=bright[0].size();
    int Gx[3][3]={
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };
    int Gy[3][3]={
        {-1,-2,-1},
        {0,0,0},
        {1,2,1}
    };
    vector<vector<float>> gradient(n,vector<float>(m,0));
    vector<vector<float>> dir(n,vector<float>(m,0));
    for(int i=1;i<n-1;i++){
        for(int j=1;j<m-1;j++){
            int sumX=0,sumY=0;
            for(int x=-1;x<=1;x++){
                for(int y=-1;y<=1;y++){
                    int pixel = bright[i+x][j+y];
                    sumX+= pixel*Gx[x+1][y+1];
                    sumY+= pixel*Gy[x+1][y+1];
                }
            }

            float mag = sqrt(sumX*sumX + sumY*sumY);
            if(mag>255)mag=255;
            gradient[i][j]=mag;
            float angle = atan2(sumY,sumX) * 180.0/M_PI;
            dir[i][j]=angle;
        }
    }
    return {gradient,dir};
}

int main() {

    // calculating grayscale which will give brightness value for each pixel using which i wil decide the ascii char and color according to that brightness 
    int height,width,channels;
    unsigned char* img = stbi_load("image.png",&width,&height,&channels,3);
    channels=3;
    int new_height=height/4;
    int new_width = width/4;
    unsigned char* resized_img = new unsigned char[new_height*new_width*3];
    stbir_resize_uint8_linear(
        img,width,height,width*channels,
        resized_img,new_width,new_height,new_width*channels,
        STBIR_RGB
    );

    int size = height * width * 3;
    int cnt=0;

    vector<vector<int>> bright;
    vector<int> temp;

    for(int i=0;i<=size-3;i+=3){
        int r=img[i],g=img[i+1],b=img[i+2];
        int brightness= (r+g+b)/3;
        temp.push_back(brightness);
        // string col = getColor(brightness);
        // char ascii = getASCII(brightness);
        // cout<<col<<ascii<<"\033[0m";
        cnt+=3;
        if(cnt==width*3){
            bright.push_back(temp);
            temp.clear();
            // cout<<endl;
            cnt=0;
        }
    }

    vector<vector<float>> gradient;
    vector<vector<float>> dir;   

    auto res = sobel(bright);

    gradient = res[0], dir=res[1];
    for(int i=0;i<bright.size();i++){
        for(int j=0;j<bright[0].size();j++){
            int grad = (int)gradient[i][j];
            string col = getColor(bright[i][j]);
            char asc  = getASCII(bright[i][j]);
            if(grad>100){
                // edge detected
                int ang = (int)dir[i][j];
                if((ang >= -22 && ang<=22)  || (ang>=158 && ang<=180) || (ang>=-180 && ang<=-158) )cout<<"\033[97m"<<"|"<<"\033[0m";
                else if((ang>=68 && ang<=112) || (ang>=-112 && ang<=-68))cout<<"\033[97m"<<"-"<<"\033[0m";
                else if(ang>=-67 && ang<=-22)cout<<"\033[97m"<<"/"<<"\033[0m";
                else if(ang>=+22 && ang<=67)cout<<"\033[97m"<<"\\"<<"\033[0m";
                else  cout<<col<<asc<<"\033[0m";
            }
            else{
                 cout<<col<<asc<<"\033[0m";

            }
        }
        cout<<endl;
    }

    cout<<endl;
    stbi_write_png("resized.png",new_width,new_height,3,resized_img,new_width*3);





    // vector<string> symbols = {
    //     "─","━","│","┃","┄","┅","┆","┇","┈","┉","┊","┋","┌","┍","┎","┏",
    //     "┐","┑","┒","┓","└","┕","┖","┗","┘","┙","┚","┛","├","┝","┞","┟",
    //     "┠","┡","┢","┣","┤","┥","┦","┧","┨","┩","┪","┫","┬","┭","┮","┯",
    //     "┰","┱","┲","┳","┴","┵","┶","┷","┸","┹","┺","┻","┼","┽","┾","┿",
    //     "╀","╁","╂","╃","╄","╅","╆","╇","╈","╉","╊","╋","╌","╍","╎","╏",
    //     "═","║","╒","╓","╔","╕","╖","╗","╘","╙","╚","╛","╜","╝","╞","╟",
    //     "╠","╡","╢","╣","╤","╥","╦","╧","╨","╩","╪","╫","╬",
    //     "▀","▁","▂","▃","▄","▅","▆","▇","█","▉","▊","▋","▌","▍","▎","▏",
    //     "▐","░","▒","▓","▔","▕","▖","▗","▘","▙","▚","▛","▜","▝","▞","▟",
    //     "■","□","▪","▫","▲","△","▶","▷","▼","▽","◀","◁","◆","◇","◈","◉",
    //     "◊","○","◌","◍","◎","●","◐","◑","◒","◓","◔","◕","◖","◗",
    //     "◘","◙","◚","◛","◜","◝","◞","◟","◠","◡","◢","◣","◤","◥","◦","◧",
    //     "◨","◩","◪","◫","◬","◭","◮","◯",
    //     "★","☆","✦","✧","✩","✪","✫","✬","✭","✮","✯","✰",
    //     "✱","✲","✳","✴","✵","✶","✷","✸","✹","✺","✻","✼","✽","✾","✿",
    //     "❀","❁","❂","❃","❄","❅","❆","❇","❈","❉","❊","❋",
    //     "↑","↓","←","→","↔","↕","↖","↗","↘","↙","↚","↛","↜","↝","↞","↟",
    //     "↠","↡","↢","↣","↤","↥","↦","↧","↨","↩","↪","↫","↬","↭","↮","↯"
    // };


    // int width, height, channels;

    // // Load image
    // unsigned char* img = stbi_load("pokemon.png", &width, &height, &channels, 3); // channels how many values like r,g,b we want 
    // /*
    // if channel = 4 => r g b a
    // if channel = 3 => r g b 
    // the last parameter passed in stbi_load forces to have only 3 channels that's why I used width*3 
    // */
    // if (!img) {
    //     cout << "Failed to load image!" << endl;
    //     return 1;
    // }   
    // channels = 3;
    // // cout<<"Channels: "<<channels<<endl;

    // int new_w = width/4;
    // int new_h = height/4;

    // unsigned char* resized_img = new unsigned char[new_w*new_h*channels];
    // stbir_resize_uint8_linear(
    //     img,width,height,width*channels,
    //     resized_img,new_w,new_h,new_w*channels,
    //     STBIR_RGB
    // );

    // int size = new_w * new_h * 3;
    // // cout<<size<<endl;
    // // cout<<"Printing the pixel values\n";
    // int cnt=0;
    // for (int i = 0; i < size; i++) {
    //     string s = "\033[38;5;";
    //     s+=to_string((int)resized_img[i]);
    //     s+="m";
    //     s+=symbols[(int)resized_img[i]];
    //     s+="\033[0m";
    //     cout<<s;
    //     // cout<<symbols[(int)resized_img[i]];
    //     cnt++;
    //     if(cnt==new_w*3){
    //         cout<<endl;
    //         cnt=0;
    //     }
    //     resized_img[i] = 255 - resized_img[i];
    // }
    // cout<<endl;
    // stbi_write_png("resized.png",new_w,new_h,3,resized_img,new_w*3);
    // // Save output
    // if (!stbi_write_png("output.png", width, height, 3, img, width * 3)) {
    //     cout << "Failed to save output.png" << endl;
    // } else {
    //     cout << "Saved output.png" << endl;
    // }
    
    // stbi_image_free(img);
    return 0;
}
