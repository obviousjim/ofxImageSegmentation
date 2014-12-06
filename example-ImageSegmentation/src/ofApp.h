#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxImageSegmentation.h"

class ofApp : public ofBaseApp{

  public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	
	ofVideoGrabber videoInput;
	ofxImageSegmentation segmentation;
	ofImage segmentedImage;

	ofxPanel gui;
	ofxSlider<float> sigma;
	ofxSlider<float> k;
	ofxSlider<int> min_size;

};
