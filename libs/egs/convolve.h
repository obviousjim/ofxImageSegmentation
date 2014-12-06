#ifndef CONVOLVE_H
#define CONVOLVE_H

#include <vector>
#include <algorithm>
#include <cmath>
#include "image.h"

/* convolve src with mask.  dst is flipped! */
static void convolve_even(image11<float> *src, image11<float> *dst, 
			  std::vector<float> &mask) {
  int width = src->width();
  int height = src->height();
  int len = mask.size();

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      float sum = mask[0] * imRef(src, x, y);
      for (int i = 1; i < len; i++) {
	sum += mask[i] * 
	  (imRef(src, __max(x-i,0), y) + 
	   imRef(src, __min(x+i, width-1), y));
      }
      imRef(dst, y, x) = sum;
    }
  }
}

/* convolve src with mask.  dst is flipped! */
static void convolve_odd(image11<float> *src, image11<float> *dst, 
			 std::vector<float> &mask) {
  int width = src->width();
  int height = src->height();
  int len = mask.size();

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      float sum = mask[0] * imRef(src, x, y);
      for (int i = 1; i < len; i++) {
	sum += mask[i] * 
	  (imRef(src, __max(x-i,0), y) - 
	   imRef(src, __min(x+i, width-1), y));
      }
      imRef(dst, y, x) = sum;
    }
  }
}

#endif
