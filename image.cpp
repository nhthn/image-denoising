#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <iostream>

using namespace cv;
using namespace std;

#define lowThreshold 80
#define highThreshold 140
#define kernel_size 3

int image_width;
int image_height;

int center_weighted_median(Mat *src, Mat *dst, Mat *mask, int k, int wc) ;
int center_weighted_median_pixel(Mat *src, Mat *dst, int x, int y, int k, int wc) ;
int simple_median(Mat *src, Mat *dst, Mat *mask);
int simple_median_pixel(Mat *src, Mat *dst, int x, int y);
int blending(Mat *src_median, Mat* src_gray, Mat* dst, Mat* mask);



int main(int argc, char *argv[]) {
// I declare the variables for image processing.
Mat frame, result_rgb;
vector<Mat> gray(3), blur_gray(3), edge(3), median(3), blend(3);

	// loading RGB image
    //frame = imread("./lighthouse_noisy.png", IMREAD_COLOR );
    frame = imread("./campus_noise.jpg", IMREAD_COLOR );
	image_width = frame.cols;
	image_height = frame.rows;

	split(frame,gray);

	for (int i = 0; i < 3; i++) {

    	//Edge detection
    	blur(gray[i], blur_gray[i], Size(3, 3));
    	Canny(blur_gray[i], edge[i], lowThreshold, highThreshold, kernel_size);
    	blur(edge[i], edge[i], Size(3, 3));

	
		median[i].create(frame.size(), CV_8U);
    	for(int x = 1; x < frame.cols-1; x++) {
    	    for(int y = 1; y < frame.rows-1; y++){
            	//if( (i==0) && (edge[i].at<uchar>(y,x) == 0))
            	if(  (edge[i].at<uchar>(y,x) == 0))
					center_weighted_median_pixel(&gray[i], &median[i], x, y, 3,1);
	}
}

cout << "test" << endl;

		// initialization
		split(frame, gray);
		//blend[i].create(frame.size(), CV_8U);
		blend[i]=gray[i];


    	for(int x = 1; x < frame.cols; x++) {
    	    for(int y = 1; y < frame.rows; y++){
            	//const float e = (float)edge[i].at<uchar>(y, x)/255.f; // const is fine
            	const float e = 0.0;
            	//blend[i].at<uchar>(y, x) = (int)((float)(1 - e) * median[i].at<uchar>(y, x) + (float)e * gray[i].at<uchar>(y, x));
            	blend[i].at<uchar>(y, x) = (int)((float)median[i].at<uchar>(y, x));
        	}
    	}

		//blending(&median[i], &gray[i], &blend[i], &edge[i]);

	}

	merge(blend, result_rgb);

	// You can switch whether to display picture on your screen or not, and to save image to the folder or not.
    string dev("dev");
    if (argc > 1 && dev.compare(argv[1]) == 0) {
        namedWindow("original", CV_WINDOW_AUTOSIZE);
        namedWindow("result", CV_WINDOW_AUTOSIZE);
        namedWindow("edge", CV_WINDOW_AUTOSIZE);

        for (;;) {
            imshow("original", frame);
            imshow("edge", edge[0]);
            //imshow("result", median);
            imshow("result", blend[0]);
            if (waitKey(30) >= 0) {
                break;
            }
        }

    } else {
        imwrite("rgbresult.jpg", result_rgb);
        imwrite("edgeresult.jpg", gray[2]);
    }

    return 0;
}



// It is used for simple_median
int compare(const void* a, const void* b){
    if (*(int*)a > *(int*)b) {
        return 1;
    }
    if (*(int*)a  == *(int*)b) {
        return 0;
    }
    return -1;
}


int center_weighted_median(Mat *src, Mat *dst, Mat *mask, int k, int wc) {
	//initialization
	*dst=*src;

    for (int x = 1; x < image_width; x++) {
        for (int y = 1; y < image_height; y++) {
            // If it is not at edge, let us make simple median
            if(mask->at<uchar>(y,x) == 0)
            	center_weighted_median_pixel(src, dst, x, y, k , wc);
		
        }
	}

	return 0;
}


int center_weighted_median_pixel(Mat *src, Mat *dst, int x, int y, int k, int wc) {

    const int k2 = k * k;
	const int arr_size=k2+wc-1;
    int arr[arr_size];

    int g = 0;
    // We convert from 2dimension array to one array.
    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            arr[g++] = src->at<uchar>(y + j, x + i);
        }
    }
	for(int c=0;c<wc-1;c++)
		arr[g++] = src->at<uchar>(y,x);


    // We identify the center weighted median.
    qsort(arr, k2, sizeof(int), compare);

    dst->at<uchar>(y, x) = arr[k2 / 2];

    return 0;
}


int simple_median(Mat *src, Mat *dst, Mat* mask){
	//initialization
	*dst=*src;

    for (int x = 1; x < image_width; x++) {
        for (int y = 1; y < image_height; y++) {
            // If it is not at edge, let us make simple median
            if(mask->at<uchar>(y,x) == 0)
            	simple_median_pixel(src, dst, x, y);
        }
	}
	return 0;
}

// It carries out the simple_median calculation for each pixel
// simple median assumes k=3 ,wc =1
int simple_median_pixel(Mat *src, Mat *dst, int x, int y) {
	return  center_weighted_median_pixel(src, dst, x, y, 3,1) ; 
}


int blending(Mat *src_median, Mat* src_gray, Mat* dst, Mat* mask){
	// initialization
	*dst = *src_gray;

   	for(int x = 1; x < image_width; x++) {
   	    for(int y = 1; y < image_height; y++){
           	const float e = (float)mask->at<uchar>(y, x) / 255.f;
           	dst->at<uchar>(y, x) = (int)((float)(1 - e) * src_median->at<uchar>(y, x) + (float)e * src_gray->at<uchar>(y, x));
       	}
	}

	return 0;
}


