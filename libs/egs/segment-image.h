#ifndef SEGMENT_IMAGE
#define SEGMENT_IMAGE

#include <cstdlib>
#include "image.h"
#include "misc.h"
#include "filter.h"
#include "segment-graph.h"

// random color
rgb random_rgb(){ 
  rgb c;
  double r;
  
  c.r = (uchar)rand();
  c.g = (uchar)rand();
  c.b = (uchar)rand();

  return c;
}

// dissimilarity measure between pixels
static inline float diff(image11<float> *r, image11<float> *g, image11<float> *b,
			 int x1, int y1, int x2, int y2) {
  return sqrt(square(imRef(r, x1, y1)-imRef(r, x2, y2)) +
	      square(imRef(g, x1, y1)-imRef(g, x2, y2)) +
	      square(imRef(b, x1, y1)-imRef(b, x2, y2)));
}

bool maskbit_sort(vector< pair<int,int> > a, vector< pair<int,int> > b){
	return a.size() > b.size();
}
/*
 * Segment an image
 *
 * Returns a color image representing the segmentation.
 *
 * im: image to segment.
 * sigma: to smooth the image.
 * c: constant for treshold function.
 * min_size: minimum component size (enforced by post-processing stage).
 * num_ccs: number of connected components in the segmentation.
 */
int segment_image(image11<rgb> *im, float sigma, float c, int min_size,
			  image11<rgb>*& segmentation, image11<char>**& masks) {
  int width = im->width();
  int height = im->height();

  image11<float> *r = new image11<float>(width, height);
  image11<float> *g = new image11<float>(width, height);
  image11<float> *b = new image11<float>(width, height);

  // smooth each color channel  
  int x,y,i,j;
  for ( y = 0; y < height; y++) {
    for ( x = 0; x < width; x++) {
      imRef(r, x, y) = imRef(im, x, y).r;
      imRef(g, x, y) = imRef(im, x, y).g;
      imRef(b, x, y) = imRef(im, x, y).b;
    }
  }
  image11<float> *smooth_r = smooth(r, sigma);
  image11<float> *smooth_g = smooth(g, sigma);
  image11<float> *smooth_b = smooth(b, sigma);
  delete r;
  delete g;
  delete b;
 
  // build graph
  edge *edges = new edge[width*height*4];
  int num = 0;
  for (y = 0; y < height; y++) {
    for ( x = 0; x < width; x++) {
      if (x < width-1) {
	edges[num].a = y * width + x;
	edges[num].b = y * width + (x+1);
	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y);
	num++;
      }

      if (y < height-1) {
	edges[num].a = y * width + x;
	edges[num].b = (y+1) * width + x;
	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x, y+1);
	num++;
      }

      if ((x < width-1) && (y < height-1)) {
	edges[num].a = y * width + x;
	edges[num].b = (y+1) * width + (x+1);
	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y+1);
	num++;
      }

      if ((x < width-1) && (y > 0)) {
	edges[num].a = y * width + x;
	edges[num].b = (y-1) * width + (x+1);
	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y-1);
	num++;
      }
    }
  }
  delete smooth_r;
  delete smooth_g;
  delete smooth_b;

  // segment
  universe *u = segment_graph(width*height, num, edges, c);
  
  // post process small components
  for ( i = 0; i < num; i++) {
    int a = u->find(edges[i].a);
    int b = u->find(edges[i].b);
    if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
      u->join(a, b);
  }
  delete [] edges;
  
  vector< vector< pair<int,int> > > maskbits_list;
  map<int, int> maskbits_index;
  for (y = 0; y < height; y++) {
    for ( x = 0; x < width; x++) {
      int comp = u->find(y * width + x);
	  if(maskbits_index.find(comp) == maskbits_index.end()){
		maskbits_index[comp] = maskbits_list.size();
		maskbits_list.push_back( vector<pair<int,int>>() );
	  }
	  maskbits_list[ maskbits_index[comp] ].push_back(make_pair(x,y));
    }
  }

 // map<int, vector< pair<int,int> > >::iterator it;
 // for(it = maskbits.begin(); it != maskbits.end(); it++){
	//maskbits_list.push_back(it->second);
 // }
  sort(maskbits_list.begin(), maskbits_list.end(), maskbit_sort);

  //pick random colors for each component
  rgb *colors = new rgb[maskbits_list.size()];
  for (i = 0; i < maskbits_list.size(); i++){
    colors[i] = random_rgb();
  }

  //allocate segmentation images
  segmentation = new image11<rgb>(width, height);
  masks = new image11<char>*[maskbits_list.size()];
  for(int i = 0; i < maskbits_list.size(); i++){
    masks[i] = new image11<char>(width,height);
  }

  //fill images
  for(int i = 0; i < maskbits_list.size(); i++){
	  for(int j = 0; j < maskbits_list[i].size(); j++){
        imRef(segmentation, maskbits_list[i][j].first, maskbits_list[i][j].second) = colors[i];
		imRef(masks[i], maskbits_list[i][j].first, maskbits_list[i][j].second) = 255;
      }
  }

  int num_css = u->num_sets();
  cout << "num comps " << maskbits_list.size() << " num sets " << num_css << endl;
  
  delete [] colors;  
  delete u;

  return num_css;
}

#endif
