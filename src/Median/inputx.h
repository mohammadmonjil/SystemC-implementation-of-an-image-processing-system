/*
 * input.h
 *
 *      Author: Justin Roark
 */

#ifndef INPUT_H_
#define INPUT_H_

#include <systemc>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace sc_core;
using namespace std;
// using std::cout;
// using std::endl;

SC_MODULE(input){
	Mat I;
	MatIterator_<Vec3b> it;
	MatIterator_<Vec3b> end;
	sc_out<sc_dt::sc_int<24> > pixel;
	sc_in_clk clk;
	sc_out<bool> start, done;
	int count;

	void nextPixel();
	void setImage(Mat& imagein); //allows image to be set in top level code

	SC_CTOR(input){
		SC_CTHREAD(nextPixel, clk.pos());
		done.initialize(false);
		start.initialize(false);
		Mat x;
		I = x; //temporary initialization
		count = 0;
	}

};

void input::nextPixel(){
	while(true){
		if (it == end){
			done.write(true);
			start.write(false);
		}
		else {
			start.write(true);
			// cout << "Fifo running "<< start.read() << endl;
			sc_dt::sc_int<24> temp;
			temp.range(7,0) = (*it)[0]; //copy RGB values into 24 bit int
			temp.range(15,8) = (*it)[1];
			temp.range(23,16) = (*it)[2];
			pixel.write(temp);
			++it;
			count++;
			// cout << "sending pixel values:"<< pixel.read() << endl;
			//cout <<"in: "<< temp.range(7,0) << ", " << temp.range(15,8) << ", " << temp.range(23,16) << endl;
		}
		wait();
	}
}

void input::setImage(Mat& imagein){
	I = imagein; //sets image
	it = I.begin<Vec3b>(); //sets Iterator for image
	end = I.end<Vec3b>();
	// cout << "Image set" << endl;
}


#endif /* INPUT_H_ */
