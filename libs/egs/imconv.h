/* image conversion */

#ifndef CONV_H
#define CONV_H

#include <climits>
#include "image.h"
#include "imutil.h"
#include "misc.h"

#define	RED_WEIGHT	0.299
#define GREEN_WEIGHT	0.587
#define BLUE_WEIGHT	0.114

static image11<uchar> *imageRGBtoGRAY(image11<rgb> *input) {
  int width = input->width();
  int height = input->height();
  image11<uchar> *output = new image11<uchar>(width, height, false);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      imRef(output, x, y) = (uchar)
	(imRef(input, x, y).r * RED_WEIGHT +
	 imRef(input, x, y).g * GREEN_WEIGHT +
	 imRef(input, x, y).b * BLUE_WEIGHT);
    }
  }
  return output;
}

static image11<rgb> *imageGRAYtoRGB(image11<uchar> *input) {
  int width = input->width();
  int height = input->height();
  image11<rgb> *output = new image11<rgb>(width, height, false);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      imRef(output, x, y).r = imRef(input, x, y);
      imRef(output, x, y).g = imRef(input, x, y);
      imRef(output, x, y).b = imRef(input, x, y);
    }
  }
  return output;  
}

static image11<float> *imageUCHARtoFLOAT(image11<uchar> *input) {
  int width = input->width();
  int height = input->height();
  image11<float> *output = new image11<float>(width, height, false);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      imRef(output, x, y) = imRef(input, x, y);
    }
  }
  return output;  
}

static image11<float> *imageINTtoFLOAT(image11<int> *input) {
  int width = input->width();
  int height = input->height();
  image11<float> *output = new image11<float>(width, height, false);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      imRef(output, x, y) = imRef(input, x, y);
    }
  }
  return output;  
}

static image11<uchar> *imageFLOATtoUCHAR(image11<float> *input, 
				       float min, float max) {
  int width = input->width();
  int height = input->height();
  image11<uchar> *output = new image11<uchar>(width, height, false);

  if (max == min)
    return output;

  float scale = UCHAR_MAX / (max - min);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      uchar val = (uchar)((imRef(input, x, y) - min) * scale);
      imRef(output, x, y) = bound(val, (uchar)0, (uchar)UCHAR_MAX);
    }
  }
  return output;
}

static image11<uchar> *imageFLOATtoUCHAR(image11<float> *input) {
  float min, max;
  min_max(input, &min, &max);
  return imageFLOATtoUCHAR(input, min, max);
}

static image11<long> *imageUCHARtoLONG(image11<uchar> *input) {
  int width = input->width();
  int height = input->height();
  image11<long> *output = new image11<long>(width, height, false);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      imRef(output, x, y) = imRef(input, x, y);
    }
  }
  return output;  
}

static image11<uchar> *imageLONGtoUCHAR(image11<long> *input, long min, long max) {
  int width = input->width();
  int height = input->height();
  image11<uchar> *output = new image11<uchar>(width, height, false);

  if (max == min)
    return output;

  float scale = UCHAR_MAX / (float)(max - min);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      uchar val = (uchar)((imRef(input, x, y) - min) * scale);
      imRef(output, x, y) = bound(val, (uchar)0, (uchar)UCHAR_MAX);
    }
  }
  return output;
}

static image11<uchar> *imageLONGtoUCHAR(image11<long> *input) {
  long min, max;
  min_max(input, &min, &max);
  return imageLONGtoUCHAR(input, min, max);
}

static image11<uchar> *imageSHORTtoUCHAR(image11<short> *input, 
					short min, short max) {
  int width = input->width();
  int height = input->height();
  image11<uchar> *output = new image11<uchar>(width, height, false);

  if (max == min)
    return output;

  float scale = UCHAR_MAX / (float)(max - min);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      uchar val = (uchar)((imRef(input, x, y) - min) * scale);
      imRef(output, x, y) = bound(val, (uchar)0, (uchar)UCHAR_MAX);
    }
  }
  return output;
}

static image11<uchar> *imageSHORTtoUCHAR(image11<short> *input) {
  short min, max;
  min_max(input, &min, &max);
  return imageSHORTtoUCHAR(input, min, max);
}

#endif
