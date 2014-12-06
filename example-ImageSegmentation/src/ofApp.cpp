#include "ofApp.h"

//-------------------------------------------------------------- setup();
void ofApp::setup(){

    ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);

	gui.setup("SEGMENTATION PARAMETERS");
	gui.add(sigma.setup("SIGMA", segmentation.sigma, 0, 2.0));
	gui.add(k.setup("K", segmentation.k, 0, 500));
	gui.add(min_size.setup("MIN SIZE", segmentation.min, 0, 50));
	
	videoInput.initGrabber(320,240);
}

//-------------------------------------------------------------- update();
void ofApp::update(){

	segmentation.sigma = sigma;
	segmentation.k = k;
	segmentation.min = min_size;

	videoInput.update();
	if(videoInput.isFrameNew()){
		segmentation.segment(videoInput.getPixelsRef());
		segmentedImage.setFromPixels(segmentation.getSegmentedPixels());
		segmentedImage.update();
	}
}

//-------------------------------------------------------------- draw();
void ofApp::draw(){
	videoInput.draw(0,0);
	if(segmentedImage.isAllocated()){
		segmentedImage.draw(videoInput.getWidth(),0);
	}

	gui.draw();
}

//-------------------------------------------------------------- KeyBoard Events
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}
