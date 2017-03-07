#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
Model *model = NULL;
const int width  = 800;
const int height = 800;

void line(Vec2i t0, Vec2i t1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(t0.x-t1.x)<std::abs(t0.y-t1.y)) {
        std::swap(t0.x, t0.y);
        std::swap(t1.x, t1.y);
        steep = true;
    }
    if (t0.x>t1.x) {
        std::swap(t0, t1);
    }
    int dx = t1.x-t0.x;
    int dy = t1.y-t0.y;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = t0.y;
    for (int x=t0.x; x<=t1.x; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (t1.y>t0.y?1:-1);
            error2 -= dx*2;
        }
    }
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    if (t0.y==t1.y && t0.y==t2.y) return; // degenerate triangles
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height;
        Vec2i A = t0 + (t2-t0)*alpha;
        Vec2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, t0.y+i, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}

void writeImgToFile(TGAImage image) {
  image.flip_vertically();
  image.write_tga_file("output.tga");
}

TGAImage renderToWireframe() {
  TGAImage image(width, height, TGAImage::RGB);
  for (int i=0; i<model->nfaces(); i++) {
      std::vector<int> face = model->face(i);
      for (int j=0; j<3; j++) {
          Vec3f v0 = model->vert(face[j]);
          Vec3f v1 = model->vert(face[(j+1)%3]);
          int x0 = (v0.x+1.)*width/2.;
          int y0 = (v0.y+1.)*height/2.;
          int x1 = (v1.x+1.)*width/2.;
          int y1 = (v1.y+1.)*height/2.;
          Vec2i p0 = Vec2i(x0, y0);
          Vec2i p1 = Vec2i(x1, y1);
          line(p0, p1, image, white);
      }
  }
  return image;
}

TGAImage renderToRandomColoredTriangles() {
  TGAImage image(width, height, TGAImage::RGB);
  for (int i=0; i<model->nfaces(); i++) {
      std::vector<int> face = model->face(i);
      Vec2i screen_coords[3];
      for (int j=0; j<3; j++) {
          Vec3f world_coords = model->vert(face[j]);
          screen_coords[j] = Vec2i((world_coords.x+1.)*width/2., (world_coords.y+1.)*height/2.);
      }
      triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(rand()%255, rand()%255, rand()%255, 255));
  }
  return image;
}

TGAImage renderToIlluminatedTriangles() {
  TGAImage image(width, height, TGAImage::RGB);
  Vec3f light_dir(0,0,-1);
  for (int i=0; i<model->nfaces(); i++) {
      std::vector<int> face = model->face(i);
      Vec2i screen_coords[3];
      Vec3f world_coords[3];
      for (int j=0; j<3; j++) {
          Vec3f v = model->vert(face[j]);
          screen_coords[j] = Vec2i((v.x+1.)*width/2., (v.y+1.)*height/2.);
          world_coords[j]  = v;
      }
      Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
      n.normalize();
      float intensity = n*light_dir;
      if (intensity>0) {
          triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
      }
  }
  return image;
}

int main(int argc, char** argv) {

    model = new Model("obj/african_head.obj");
    writeImgToFile(renderToWireframe());
    // writeImgToFile(renderToRandomColoredTriangles());
    // writeImgToFile(renderToIlluminatedTriangles());
    delete model;
    return 0;
}
