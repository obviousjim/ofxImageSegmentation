#pragma once

#include "ofMain.h"

class ofxImageSegmentation {
  public:
	ofxImageSegmentation();
	
	float sigma;
	float k;
	int min;
	bool createMasks;
	
	ofPixels& segment(ofPixels& image);
	ofPixels& getSegmentedPixels();
	ofPixels& getSegmentMask(int segment);
	int numSegments;

  protected:
	ofPixels segmentedPixels;
	vector<ofPixels> segmentedMasks;
};
