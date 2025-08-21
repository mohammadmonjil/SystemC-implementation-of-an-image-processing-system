#ifndef PROCESS_H_
#define PROCESS_H_

#include <systemc>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace sc_core;
using namespace std;

SC_MODULE(processImage){
    sc_in<sc_dt::sc_int<24>> pixelin;
    sc_out<sc_dt::sc_int<24>> pixelout;
    sc_in<bool> donein, ready;
    sc_out<bool> doneout;
    sc_in_clk clk;
    sc_dt::sc_int<24> pixel;
    int count;
    int cols, rows;

    void processing();
    void setdim(int row, int col);

    SC_CTOR(processImage){
        SC_CTHREAD(processing, clk.pos());
        doneout.initialize(false);
        pixelout.initialize(0);
        count = 0;
    }
};

void processImage::setdim(int row, int col){
    rows = row;
    cols = col;
}

void processImage :: processing(){
    Mat gray, grad_x, grad_y, abs_grad_x, abs_grad_y, filtered;
    Mat frame( rows,cols, CV_8UC3);
    MatIterator_<Vec3b> it;
	MatIterator_<Vec3b> end;
    int height = 0, width = 0, count =0;
    bool send2output = false, finished = false;

    while(true){
        if(finished){

        }
        else if ((doneout == true)&&(send2output==true)){

            sc_dt::sc_int<24> temp;
			temp.range(7,0) = (*it)[0]; //copy RGB values into 24 bit int
			temp.range(15,8) = (*it)[1];
			temp.range(23,16) = (*it)[2];
			pixelout.write(temp);
			++it;

            if(it==end){
                send2output = false;
                finished = true;
            } 
        }

        else if((donein==false)&&(ready==true)){
            pixel = pixelin.read();
    
            uchar b = pixel.range(7, 0);
            uchar g = pixel.range(15, 8);
            uchar r = pixel.range(23, 16);
            frame.at<Vec3b>(height,width) = Vec3b(b, g, r);
            
            width++;
            if (width >= cols) {
                width = 0;
                height++;
            }
            if (height >= rows) {
                height = 0;
               
            }
        }

        else if ((donein == true)&&(ready==false)){

            // Apply median filter to input image using OpenCV's built-in function
            int kernel_size = 7;
            cv::medianBlur(frame, filtered, kernel_size);
        
	        it = filtered.begin<Vec3b>(); //sets Iterator for image
	        end = filtered.end<Vec3b>();
           // cv::imwrite("img.jpg",filtered);
            send2output = true;
            doneout.write(true);
        }

        wait();
    }

}

#endif /* PROCESS_H_ */