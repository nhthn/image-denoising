#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int simple_median(Mat *src, int x, int y);


Mat frame, gray, blur_gray, edge, canny;
Mat median(512, 768, CV_8U, Scalar(0, 0, 255));
Mat blend(512, 768, CV_8U, Scalar(0, 0, 255));

int main(int argc, char *argv[]) {

    frame = imread("./noisy_lighthouse.png", 1);

    gray.zeros(frame.cols, frame.rows, CV_8U);

    cvtColor(frame, gray, CV_BGR2GRAY);


    //Edge detection
    blur(gray, blur_gray, Size(3, 3));
    //Canny( blur_gray, edge, lowThreshold, lowThreshold*ratio, kernel_size );
    Canny(blur_gray, edge, 80, 140, 3);
    blur(edge, edge, Size(3, 3));

    median = gray;

    for (int x = 1; x < frame.cols; x++) {
        for (int y = 1; y < frame.rows; y++) {
            // If it is not at edge, let us make simple median
            //if(edge.at<uchar>(y,x) == 0)
            simple_median(&gray, x, y);
        }
    }

    for(int x = 1; x < frame.cols; x++) {
        for(int y = 1; y < frame.rows; y++){
            const int e = edge.at<uchar>(y, x)/255;
            blend.at<uchar>(y, x) = (int)((double)(1 - e) * median.at<uchar>(y, x) + (double)e * edge.at<uchar>(y, x));
        }
    }

    string dev("dev");
    if (argc > 1 && dev.compare(argv[1]) == 0) {
        namedWindow("original", CV_WINDOW_AUTOSIZE);
        namedWindow("result", CV_WINDOW_AUTOSIZE);
        namedWindow("edge", CV_WINDOW_AUTOSIZE);

        for (;;) {
            imshow("original", frame);
            imshow("edge", edge);
            //imshow("result", median);
            imshow("result", blend);
            if (waitKey(30) >= 0) {
                break;
            }
        }
    } else {
        imwrite("edge.jpg", edge);
        imwrite("result.jpg", blend);
    }

    return 0;
}


int compare(const void* a, const void* b){
    if (*(int*)a > *(int*)b) {
        return 1;
    }
    if (*(int*)a  == *(int*)b) {
        return 0;
    }
    return -1;
}

int simple_median(Mat *src, int x, int y) {

    //static int counter=0;
    const int k = 3;
    const int k2 = k * k;
    int array[k2] = {2, 4, 1, 5, 2, 5, 3, 6, 6};

    int g = 0;
    // We convert from 2dimension array to one array
    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            array[g] = src->at<uchar>(y + j, x + i);
            g++;
        }
    }

    // We identify the simple median.
    qsort(array, k2, sizeof(int), compare);

    median.at<uchar>(y, x) = array[k2 / 2];

    return 0;
}
