#pragma once

#include "ofMain.h"

class ofxImageSegmentation {
  public:
	ofxImageSegmentation();
	
	float sigma;
	float k;
	int min;

	ofPixels& segment(ofPixels& image);
	ofPixels& getSegmentedPixels();
	int numSegments;

  protected:

	ofPixels segmentedPixels;

};
