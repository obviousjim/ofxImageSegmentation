#ifndef IMAGE_H
#define IMAGE_H

#include <cstring>

template <class T>
class image11 {
 public:
  /* create an image */
  image11(const int width, const int height, const bool init = true);

  /* delete an image */
  ~image11();

  /* init an image */
  void init(const T &val);

  /* copy an image */
  image11<T> *copy() const;
  
  /* get the width of an image. */
  int width() const { return w; }
  
  /* get the height of an image. */
  int height() const { return h; }
  
  /* image data. */
  T *data;
  
  /* row pointers. */
  T **access;
  
 private:
  int w, h;
};

/* use imRef to access image data. */
#define imRef(im, x, y) (im->access[y][x])
  
/* use imPtr to get pointer to image data. */
#define imPtr(im, x, y) &(im->access[y][x])

template <class T>
image11<T>::image11(const int width, const int height, const bool init) {
  w = width;
  h = height;
  data = new T[w * h];  // allocate space for image data
  access = new T*[h];   // allocate space for row pointers
  
  // initialize row pointers
  for (int i = 0; i < h; i++)
    access[i] = data + (i * w);  
  
  if (init)
    memset(data, 0, w * h * sizeof(T));
}

template <class T>
image11<T>::~image11() {
  delete [] data; 
  delete [] access;
}

template <class T>
void image11<T>::init(const T &val) {
  T *ptr = imPtr(this, 0, 0);
  T *end = imPtr(this, w-1, h-1);
  while (ptr <= end)
    *ptr++ = val;
}


template <class T>
image11<T> *image11<T>::copy() const {
  image11<T> *im = new image11<T>(w, h, false);
  memcpy(im->data, data, w * h * sizeof(T));
  return im;
}

#endif
