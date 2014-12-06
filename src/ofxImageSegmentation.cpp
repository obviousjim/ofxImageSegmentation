#include "ofxImageSegmentation.h"

#include "image.h"
#include "misc.h"
#include "pnmfile.h"
#include "segment-image.h"

ofxImageSegmentation::ofxImageSegmentation(){
	sigma = 0.5;
	k = 1000;
	min = 100.;
}

//-------------------------------------------------------------- KeyBoard Events
ofPixels& ofxImageSegmentation::segment(ofPixels& image){

	if(!image.isAllocated()){
		ofLogError("ofxImageSegmentation::segment") << "input image must be allocated";
	}
	
	if(!segmentedPixels.isAllocated() || 
		segmentedPixels.getWidth() != image.getWidth() ||
		segmentedPixels.getHeight() != image.getHeight() ||
		segmentedPixels.getImageType() != image.getImageType() )
	{
		segmentedPixels.allocate(image.getWidth(), image.getHeight(), OF_IMAGE_COLOR);
	}

	image11<rgb> *input = loadPixels(image);	
	image11<rgb> *seg = segment_image(input, sigma, k, min, &numSegments); 
	for(int y = 0; y < seg->height(); y++){
		for(int x = 0; x < seg->width(); x++){
			int index = segmentedPixels.getPixelIndex(x,y);
			segmentedPixels.getPixels()[index + 0] = imRef(seg,x,y).r;
			segmentedPixels.getPixels()[index + 1] = imRef(seg,x,y).g;
			segmentedPixels.getPixels()[index + 2] = imRef(seg,x,y).b;
		}
	}

	//TODO: process segments into separet images!
	//....

	delete input;
	delete seg;

	//memory leaks...?

	//savePPM(seg, ofToDataPath(string("test_file.ppm"), true).c_str() );
	//printf("got %d components\n", num_ccs);
	return segmentedPixels;
}

ofPixels& ofxImageSegmentation::getSegmentedPixels(){
	return segmentedPixels;
}

