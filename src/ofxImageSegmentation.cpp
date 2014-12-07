#include "ofxImageSegmentation.h"

#include "image.h"
#include "misc.h"
#include "pnmfile.h"
#include "segment-image.h"

//--------------------------------------------------------------
ofxImageSegmentation::ofxImageSegmentation(){
	sigma = 0.5;
	k = 1000;
	min = 100.;
	createMasks = false;
}

//--------------------------------------------------------------
ofPixels& ofxImageSegmentation::segment(ofPixels& image){

	if(!image.isAllocated()){
		ofLogError("ofxImageSegmentation::segment") << "input image must be allocated";
		return segmentedPixels;
	}
	
	if(!segmentedPixels.isAllocated() || 
		segmentedPixels.getWidth() != image.getWidth() ||
		segmentedPixels.getHeight() != image.getHeight() ||
		segmentedPixels.getImageType() != image.getImageType() )
	{
		segmentedPixels.allocate(image.getWidth(), image.getHeight(), OF_IMAGE_COLOR);
		segmentedMasks.clear();
	}

	image11<rgb> *input = loadPixels(image);	
	image11<rgb> *seg;
	image11<char> **masks;
	numSegments = segment_image(input, sigma, k, min, seg, masks); 
	memcpy(segmentedPixels.getPixels(),seg->data,segmentedPixels.getWidth()*segmentedPixels.getHeight()*segmentedPixels.getBytesPerPixel());
	
	//calculate segment masks
	if(numSegments > 0){
		while(segmentedMasks.size() < numSegments){
			segmentedMasks.push_back(ofPixels());
			segmentedMasks.back().allocate(image.getWidth(), image.getHeight(), OF_IMAGE_GRAYSCALE);
		}
		int bytesPerMask = segmentedMasks[0].getWidth()*segmentedMasks[0].getHeight()*segmentedMasks[0].getBytesPerPixel();
		for(int i = 0; i < numSegments; i++){
			memcpy(segmentedMasks[i].getPixels(),masks[i]->data,bytesPerMask);
		}
	}

	//This is really slow to do, find a way to preserve memory
	delete input;
	delete seg;
	for(int i = 0; i < numSegments; i++){
		delete masks[i];
	}
	delete [] masks;

	return segmentedPixels;
}

//--------------------------------------------------------------
ofPixels& ofxImageSegmentation::getSegmentMask(int segment){
	if(segment >= numSegments){
		ofLogError("ofxImageSegmentation::getSegmentMask") << "segment out of range, max " << numSegments;
	}
	return segmentedMasks[segment];
}

//--------------------------------------------------------------
ofPixels& ofxImageSegmentation::getSegmentedPixels(){
	return segmentedPixels;
}

