#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int simple_median();

int main(int, char**) {
/*
    Mat frame=imread("resources/example.jpg");
    namedWindow("edges",1);
    for(;;) {
        imshow("edges", frame);
        if(waitKey(30) >= 0) break;
    }
*/


simple_median();
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}


int compare(const void *a, const void* b){
if(*(int*)a  > *(int*)b)
	return 1;
if(*(int*)a  == *(int*)b)
	return 0;
	return -1;
}

int simple_median(){

const int k =3;
const int k2 = k*k;
	int array[k2]={2,4,1,5,2,5,3,6,6};
	qsort(array, k2, sizeof(int), compare);

/*
	for(int i=0;i<9;i++)
		cout << array[i] << endl;
*/

		cout << array[(k2)/2] << endl;

	return 0; }

