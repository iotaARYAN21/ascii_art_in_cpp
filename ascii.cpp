#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE2_IMPLEMENTATION
#include "stb_image_resize2.h"

#include <bits/stdc++.h>
using namespace std;

int main() {

    // unordered_map<int,char> mp;
    // mp.reserve(256);
    // for(int i=0;i<256;i++){
    //     mp[i]=static_cast<char>(i);
    // }

    vector<string> symbols = {
        "─","━","│","┃","┄","┅","┆","┇","┈","┉","┊","┋","┌","┍","┎","┏",
        "┐","┑","┒","┓","└","┕","┖","┗","┘","┙","┚","┛","├","┝","┞","┟",
        "┠","┡","┢","┣","┤","┥","┦","┧","┨","┩","┪","┫","┬","┭","┮","┯",
        "┰","┱","┲","┳","┴","┵","┶","┷","┸","┹","┺","┻","┼","┽","┾","┿",
        "╀","╁","╂","╃","╄","╅","╆","╇","╈","╉","╊","╋","╌","╍","╎","╏",
        "═","║","╒","╓","╔","╕","╖","╗","╘","╙","╚","╛","╜","╝","╞","╟",
        "╠","╡","╢","╣","╤","╥","╦","╧","╨","╩","╪","╫","╬",
        "▀","▁","▂","▃","▄","▅","▆","▇","█","▉","▊","▋","▌","▍","▎","▏",
        "▐","░","▒","▓","▔","▕","▖","▗","▘","▙","▚","▛","▜","▝","▞","▟",
        "■","□","▪","▫","▲","△","▶","▷","▼","▽","◀","◁","◆","◇","◈","◉",
        "◊","○","◌","◍","◎","●","◐","◑","◒","◓","◔","◕","◖","◗",
        "◘","◙","◚","◛","◜","◝","◞","◟","◠","◡","◢","◣","◤","◥","◦","◧",
        "◨","◩","◪","◫","◬","◭","◮","◯",
        "★","☆","✦","✧","✩","✪","✫","✬","✭","✮","✯","✰",
        "✱","✲","✳","✴","✵","✶","✷","✸","✹","✺","✻","✼","✽","✾","✿",
        "❀","❁","❂","❃","❄","❅","❆","❇","❈","❉","❊","❋",
        "↑","↓","←","→","↔","↕","↖","↗","↘","↙","↚","↛","↜","↝","↞","↟",
        "↠","↡","↢","↣","↤","↥","↦","↧","↨","↩","↪","↫","↬","↭","↮","↯"
    };


    int width, height, channels;

    // Load image
    unsigned char* img = stbi_load("image.png", &width, &height, &channels, 3); // channels how many values like r,g,b we want 
    /*
    if channel = 4 => r g b a
    if channel = 3 => r g b 
    the last parameter passed in stbi_load forces to have only 3 channels that's why I used width*3 
    */
    if (!img) {
        cout << "Failed to load image!" << endl;
        return 1;
    }   
    channels = 3;
    // cout<<"Channels: "<<channels<<endl;

    int new_w = width/4;
    int new_h = height/4;

    unsigned char* resized_img = new unsigned char[new_w*new_h*channels];
    stbir_resize_uint8_linear(
        img,width,height,width*channels,
        resized_img,new_w,new_h,new_w*channels,
        STBIR_RGB
    );

    int size = new_w * new_h * 3;
    // cout<<size<<endl;
    // cout<<"Printing the pixel values\n";
    int cnt=0;
    for (int i = 0; i < size; i++) {
        cout<<symbols[(int)resized_img[i]];
        cnt++;
        if(cnt==new_w*3){
            cout<<endl;
            cnt=0;
        }
        resized_img[i] = 255 - resized_img[i];
    }
    cout<<endl;
    stbi_write_png("resized.png",new_w,new_h,3,resized_img,new_w*3);
    // Save output
    if (!stbi_write_png("output.png", width, height, 3, img, width * 3)) {
        cout << "Failed to save output.png" << endl;
    } else {
        cout << "Saved output.png" << endl;
    }

    stbi_image_free(img);
    return 0;
}
