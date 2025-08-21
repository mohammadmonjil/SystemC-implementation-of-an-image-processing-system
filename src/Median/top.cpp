#include <opencv2/opencv.hpp>
#include <systemc>
#include "inputx.h"
#include "processx.h"
#include "outputx.h"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;
using namespace sc_core;

enum {
        CV_IMWRITE_JPEG_QUALITY=1
};

int sc_main(int argc, char *argv[]) {

    Mat imagein, imageout;
	imagein = imread(argv[1]);
	


	sc_signal<sc_dt::sc_int<24> > pixel1,pixel2;//24 bit signal to move pixels between modules
	sc_signal<bool> start_signal, done1, done2,done3; //signals when each module is finished
	sc_clock clk("clk",10,SC_NS,0.5);

    input in("in");
    in.pixel(pixel1);
	in.done(done1);
	in.start(start_signal);
	in.clk(clk);

    processImage proc("processe");
    proc.clk(clk);
    proc.ready(start_signal);
    proc.donein(done1);
    proc.doneout(done2);
    proc.pixelin(pixel1);
    proc.pixelout(pixel2);

    output out("out");
    out.clk(clk);
    out.pixelin(pixel2);
    out.donein(done2);
    out.doneout(done3);


    in.setImage(imagein); 
    proc.setdim(imagein.rows,imagein.cols);
    out.setMat(imagein.rows,imagein.cols);

	cout << "Starting" << endl;
    while(!done3){ //run simulation till it is finished
		sc_start(100, SC_NS);
	}
    
    imageout = out.getImage(); //retrieve image
	vector<int> params;
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(95);

	imwrite("processed.jpg", imageout, params); //write image to file
    cout << "Finished" << endl;

    return 0;
}
