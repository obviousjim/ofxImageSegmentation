#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <cmath>
#include "image.h"
#include "misc.h"
#include "convolve.h"
#include "imconv.h"

#define WIDTH 4.0

/* normalize mask so it integrates to one */
static void normalize(std::vector<float> &mask) {
  int len = mask.size();
  int i;
  float sum = 0;
  for (i = 1; i < len; i++) {
    sum += fabs(mask[i]);
  }
  sum = 2*sum + fabs(mask[0]);
  for (i = 0; i < len; i++) {
    mask[i] /= sum;
  }
}

/* make filters */
#define MAKE_FILTER(name, fun)                                \
static std::vector<float> make_ ## name (float sigma) {       \
  sigma = __max(sigma, 0.01F);			      \
  int len = (int)ceil(sigma * WIDTH) + 1;                     \
  std::vector<float> mask(len);                               \
  for (int i = 0; i < len; i++) {                             \
    mask[i] = fun;                                            \
  }                                                           \
  return mask;                                                \
}

MAKE_FILTER(fgauss, exp(-0.5*square(i/sigma)));

/* convolve image with gaussian filter */
static image11<float> *smooth(image11<float> *src, float sigma) {
  std::vector<float> mask = make_fgauss(sigma);
  normalize(mask);

  image11<float> *tmp = new image11<float>(src->height(), src->width(), false);
  image11<float> *dst = new image11<float>(src->width(), src->height(), false);
  convolve_even(src, tmp, mask);
  convolve_even(tmp, dst, mask);

  delete tmp;
  return dst;
}

/* convolve image with gaussian filter */
image11<float> *smooth(image11<uchar> *src, float sigma) {
  image11<float> *tmp = imageUCHARtoFLOAT(src);
  image11<float> *dst = smooth(tmp, sigma);
  delete tmp;
  return dst;
}

/* compute laplacian */
static image11<float> *laplacian(image11<float> *src) {
  int width = src->width();
  int height = src->height();
  image11<float> *dst = new image11<float>(width, height);  

  for (int y = 1; y < height-1; y++) {
    for (int x = 1; x < width-1; x++) {
      float d2x = imRef(src, x-1, y) + imRef(src, x+1, y) -
	2*imRef(src, x, y);
      float d2y = imRef(src, x, y-1) + imRef(src, x, y+1) -
	2*imRef(src, x, y);
      imRef(dst, x, y) = d2x + d2y;
    }
  }
  return dst;
}

#endif
