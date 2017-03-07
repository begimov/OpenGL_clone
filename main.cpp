#include "tgaimage.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
//     for (float t=0.; t<1.; t+=.01) {
//         int x = x0*(1.-t) + x1*t;
//         int y = y0*(1.-t) + y1*t;
//         image.set(x, y, color);
//     }
// }

// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
//     for (int x=x0; x<=x1; x++) {
//         float t = (x-x0)/(float)(x1-x0);
//         int y = y0*(1.-t) + y1*t;
//         image.set(x, y, color);
//     }
// }

void printInitialCoordinates(int x0, int y0, int x1, int y1) {
  std::cout << "\n";
  std::cout << "INITIAL COORDINATES:" << "\n";
  std::cout << "X0: " << x0 << " " << "Y0: " << y0 << "\n";
  std::cout << "X1: " << x1 << " " << "Y1: " << y1 << "\n";
  std::cout << "\n";
}

void printTempCoordinates(int x0, int y0, int x1, int y1, bool steep) {
  std::cout << "\n";
  std::cout << "TEMPORARY COORDINATES:" << "\n";
  std::cout << "X0: " << x0 << " " << "Y0: " << y0 << "\n";
  std::cout << "X1: " << x1 << " " << "Y1: " << y1 << "\n";
  std::cout << "STEEP: " << steep << "\n";
  std::cout << "\n";
}

void printCoordinates(int x, int y) {
  std::cout << "X: " << x << " " << "Y: " << y << "\n";
  std::cout << "\n";
}

// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
//     printInitialCoordinates(x0, y0, x1, y1);
//     bool steep = false;
//     if (std::abs(x0-x1)<std::abs(y0-y1)) { // if the line is steep, we transpose the image
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         steep = true;
//     }
//     if (x0>x1) { // make it left−to−right
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }
//     printTempCoordinates(x0, y0, x1, y1, steep);
//     for (int x=x0; x<=x1; x++) {
//         float t = (x-x0)/(float)(x1-x0);
//         int y = y0*(1.-t) + y1*t;
//         if (steep) {
//             printCoordinates(y, x);
//             image.set(y, x, color); // if transposed, de−transpose
//         } else {
//             printCoordinates(x, y);
//             image.set(x, y, color);
//         }
//     }
// }

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    printInitialCoordinates(x0, y0, x1, y1);
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    printTempCoordinates(x0, y0, x1, y1, steep);
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            printCoordinates(y, x);
            image.set(y, x, color);
        } else {
            printCoordinates(x, y);
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    // line(1, 1, 2, 2, image, white);
    // line(13, 20, 80, 40, image, white);
    line(20, 13, 40, 100, image, red);
    // line(80, 40, 13, 20, image, red);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}
