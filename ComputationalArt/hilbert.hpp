#ifndef KMUCS_GRAPHICS_HILBERT_HPP
#define KMUCS_GRAPHICS_HILBERT_HPP

#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <vector>

using namespace std;

struct Point {
		float x;
		float y;
};

void setCoordinate(vector<Point> &points , int max_depth){  //각 Level에 대한 Hilbert curve Algorithm

  points.push_back({.x = -0.5f , .y = -0.5f});
  points.push_back({.x = -0.5f , .y = 0.5f});
  points.push_back({.x = 0.5f , .y = 0.5f});
  points.push_back({.x = 0.5f , .y = -0.5f});

  for(int d = 1; d < max_depth; d++) {
      vector< Point > new_points;
      for(int i = 0; i < points.size(); i += 4){
        if(abs(points[i+1].x - points[i].x) < abs(points[i+1].y - points[i].y)) {
            float length = (points[i+1].y - points[i].y) / 4.0;

            float x = points[i].x;
            float y = points[i].y;
            new_points.push_back({.x = x - length, .y = y - length});
            new_points.push_back({.x = x + length, .y = y - length});
            new_points.push_back({.x = x + length, .y = y + length});
            new_points.push_back({.x = x - length, .y = y + length});

            x = points[i+1].x;
            y = points[i+1].y;
            new_points.push_back({.x = x - length, .y = y - length});
            new_points.push_back({.x = x - length, .y = y + length});
            new_points.push_back({.x = x + length, .y = y + length});
            new_points.push_back({.x = x + length, .y = y - length});

            x = points[i+2].x;
            y = points[i+2].y;
            new_points.push_back({.x = x - length, .y = y - length});
            new_points.push_back({.x = x - length, .y = y + length});
            new_points.push_back({.x = x + length, .y = y + length});
            new_points.push_back({.x = x + length, .y = y - length});

            x = points[i+3].x;
            y = points[i+3].y;
            new_points.push_back({.x = x + length, .y = y + length});
            new_points.push_back({.x = x - length, .y = y + length});
            new_points.push_back({.x = x - length, .y = y - length});
            new_points.push_back({.x = x + length, .y = y - length});
        }

        else {
          float length = (points[i+1].x - points[i].x) / 4.0;

          float x = points[i].x;
          float y = points[i].y;
          new_points.push_back({.x = x - length, .y = y - length});
          new_points.push_back({.x = x - length, .y = y + length});
          new_points.push_back({.x = x + length, .y = y + length});
          new_points.push_back({.x = x + length, .y = y - length});

          x = points[i+1].x;
          y = points[i+1].y;
          new_points.push_back({.x = x - length, .y = y - length});
          new_points.push_back({.x = x + length, .y = y - length});
          new_points.push_back({.x = x + length, .y = y + length});
          new_points.push_back({.x = x - length, .y = y + length});

          x = points[i+2].x;
          y = points[i+2].y;
          new_points.push_back({.x = x - length, .y = y - length});
          new_points.push_back({.x = x + length, .y = y - length});
          new_points.push_back({.x = x + length, .y = y + length});
          new_points.push_back({.x = x - length, .y = y + length});

          x = points[i+3].x;
          y = points[i+3].y;
          new_points.push_back({.x = x + length, .y = y + length});
          new_points.push_back({.x = x + length, .y = y - length});
          new_points.push_back({.x = x - length, .y = y - length});
          new_points.push_back({.x = x - length, .y = y + length});
        }
      }
			points = new_points;
    }
}

Point getRandom(vector<Point> points, int i) {  //Hilbert curve 위에 존재하는 임의의 좌표값을 설정하는 함수
  Point p;

  float random;
  float temp = (float)rand()/RAND_MAX;
  float _x , _y ;

  if(points[i].x == points[i+1].x) {
      float minY = min(points[i].y ,points[i+1].y);
      float maxY = max(points[i].y ,points[i+1].y);

      if(minY < 0) {
        minY += abs(minY);
        maxY += abs(minY);
        random = (minY + temp * (maxY - minY));
        random -= abs(minY);
      }
      else{
        random = (minY + temp * (maxY - minY));
      }
      _x = points[i].x , _y = random;
  }

  else if(points[i].y == points[i+1].y) {
      float minX = min(points[i].x ,points[i+1].x);
      float maxX = max(points[i].x ,points[i+1].x);

      if(minX < 0) {
        minX += abs(minX);
        maxX += abs(minX);
        random = (minX + temp * (maxX - minX));
        random -= abs(minX);
      }
      else {
        random = (minX + temp * (maxX - minX));
      }
      _x = random , _y = points[i].y;
  }
  p.x = _x; p.y = _y;
  return p;
}

#endif // KMUCS_GRAPHICS_HILBERT_HPP
