/*
 * output.h
 *
 *      Author: Justin Roark
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <systemc>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace sc_core;

SC_MODULE(output){
	Mat I;
	MatIterator_<Vec3b> it;
	MatIterator_<Vec3b> end;
	sc_in<sc_dt::sc_int<24> > pixelin;
    sc_dt::sc_int<24> pixel;
	sc_in_clk clk;
	sc_in<bool> donein;
	sc_out<bool>  doneout;
    int cols, rows;

	void buildImage(); //rebuilds pixels back into an image
	void setMat(int rows, int cols); //gets size of image from top level code
	Mat& getImage(); //returns image

	SC_CTOR(output){
		SC_CTHREAD(buildImage, clk.pos());
		Mat temp;
		I = temp; //initializes temporary image
        doneout.initialize(false);
	}

};

void output::buildImage(){
    int height =0, width =0;
	while(true){

        if (doneout.read()==true){

        //    cout<< "Completed"<<endl;
        }

		else if((donein.read()==true)&&(doneout.read()==false)){
            
            pixel = pixelin.read();

            uchar b = pixel.range(7, 0);
            uchar g = pixel.range(15, 8);
            uchar r = pixel.range(23, 16);

            I.at<Vec3b>(height, width) = Vec3b(b, g, r);
            width++;
            if (width >= cols) {
                width = 0;
                height++;
            }
            if (height >= rows) {
                height = 0;
                doneout.write(true);
            }

        }
		wait();
	}
}

void output::setMat(int row, int col){
    rows = row;
    cols = col;
	Mat temp(rows,cols, CV_8UC3); //sets up image with correct dimensions
	I = temp;
	it = I.begin<Vec3b>();
	end = I.end<Vec3b>();
}

Mat& output::getImage(){
	return I;
}

#endif /* OUTPUT_H_ */
