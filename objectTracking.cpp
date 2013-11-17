#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/extensions/XTest.h>
#include <iostream>
#include <unistd.h>

using namespace cv;
//initial min and max HSV filter values.
//these will be changed using trackbars

/*int H_MIN = 35;
int H_MAX = 55;
int S_MIN = 29;
int S_MAX = 120;
int V_MIN = 90;
int V_MAX = 181;*/





int H_MIN = 25;
int H_MAX = 109;
int S_MIN = 85;
int S_MAX = 121;
int V_MIN = 125;
int V_MAX = 165;
//default capture width and height
const int FRAME_WIDTH = 1080;
const int FRAME_HEIGHT = 960;
int CAM_WIDTH;
int CAM_HEIGHT;
int CAM_RES;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 10*10;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

//Screen pointer
Display *pdsp = XOpenDisplay(NULL);
Screen *pscr = NULL;
Window root = XDefaultRootWindow(pdsp);

int prevx = 0;
int prevy = 0;

int bluecount = 10;

bool blueDetected = false;
bool calib = false;
bool stillBlue = false;

int redcount = 10;
bool redDetected = false;
bool stillRed = false;
int height;
int width;

XEvent event;



int getScreenSize(int *w, int *h)
{

	pscr = DefaultScreenOfDisplay(pdsp);
	*w = pscr->width;
	*h = pscr->height;
	return 0;
}



void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed





}
string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}
void createTrackbars(){
	//create window for trackbars


    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}
void drawObject(int x, int y,Mat &frame){

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!

    //UPDATE:JUNE 18TH, 2013
    //added 'if' and 'else' statements to prevent
    //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

	circle(frame,Point(x,y),20,Scalar(0,255,0),2);
    if(y-25>0)
    line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
    line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
    line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
    line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

	putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

}
void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);
	//erode(thresh,thresh,erodeElement);


	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
	dilate(thresh, thresh,dilateElement);
	dilate(thresh, thresh,dilateElement);
	/*dilate(thresh, thresh,dilateElement);
	dilate(thresh, thresh,dilateElement);*/
	


}
void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed){

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
                	//if(abs(prevx - x) > 5 || abs(prevy - y) > 5){
                		if(!calib){
                			width = moment.m10;
                			height = moment.m01;}
						x = moment.m10/area;
						y = moment.m01/area;
					//}
					objectFound = true;
					refArea = area;
				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
				//draw object location on screen
				drawObject(x,y,cameraFeed);}

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}


void mouseClick(void)
{
	//event up top moved
	XTestFakeButtonEvent(pdsp, 1, true, CurrentTime);
	XTestFakeButtonEvent(pdsp, 1, false, CurrentTime);
	XFlush(pdsp);
	
}

void rightClick(void)
{
	XTestFakeButtonEvent(pdsp, 3, true, CurrentTime);
	XTestFakeButtonEvent(pdsp, 3, false, CurrentTime);
	XFlush(pdsp);
}


void checkBlue(int &x, int &y, Mat bThreshold, Mat &cameraFeed)
{
	Mat temp;
	bThreshold.copyTo(temp);



	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	double refArea = 0;
	if(hierarchy.size() > 0)
	{
		int numObjects = hierarchy.size();
		if(numObjects <2)
		{
			for(int index = 0; index >= 0; index = hierarchy[index][0])
			{
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				if(area > 5)
				{
					blueDetected = true;
				}
				else blueDetected = false;
			}
			if(blueDetected)
			{
				if(!stillBlue)
				{	
					mouseClick();
					printf("Click\n");
					//event.type = ButtonRelease;
					//event.xbutton.state = 0x100;
					XFlush(pdsp);
					stillBlue = true;
				}
				if(stillBlue){
					bluecount--;
				}
				if(bluecount == 0)
				{
					stillBlue = false;
					bluecount = 10;
				}
				blueDetected = false;
				usleep(100000);
			}
		}
	}
}

void checkRed(int &x, int &y, Mat rThreshold, Mat cameraFeed)
{
	Mat temp;
	rThreshold.copyTo(temp);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	double refArea = 0;
	if(hierarchy.size() > 0)
	{
		int numObjects = hierarchy.size();
		if(numObjects < 2)
		{
			for(int index = 0; index >= 0; index = hierarchy[index][0])
			{
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				if(area > 5)
				{
					redDetected = true;
				}
				else redDetected = false;
			}
			if(redDetected)
			{
				if(!stillRed)
				{
					rightClick();
					printf("Right\n");
					//event.type = ButtonRelease;
					//event.xbutton.state = 0x100;
					XFlush(pdsp);
					stillRed = true;
				}
				if(stillRed)
					redcount--;
				if(redcount == 0)
				{
					stillRed = false;
					redcount = 10;
				}
				redDetected = false;
				usleep(100000);
			}
		}
	}
}


int main(int argc, char* argv[])
{

	//get screen resolution
	int sw, sh;
	getScreenSize(&sw, &sh);

	//some boolean variables for different functionality within this
	//program
    bool trackObjects = true;
    bool useMorphOps = true;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	Mat blue;
	Mat red;
	//matrix storage for binary threshold image
	Mat threshold;
	Mat bThreshold;
	Mat rThreshold;
	//x and y values for the location of the object
	int x=0, y=0;
	//create slider bars for HSV filtering
	//createTrackbars();
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);

	//get height and width of capture frame
	CAM_WIDTH = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	CAM_HEIGHT = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	CAM_RES = CAM_HEIGHT * CAM_WIDTH;
	//printf("%d\n", CAM_RES);

	//get screen/webcam ratio
	float ratiox = ((float)sw)/CAM_WIDTH;
	float ratioy = ((float)sh)/CAM_HEIGHT;
	

	//resize image to map to screen
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, sh);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, sw);

	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while(1){
		//YCbCr Mins
	CvScalar min = cvScalar(H_MIN, S_MIN, V_MIN);

	//YCbCr Maxs
	CvScalar max = cvScalar(H_MAX, S_MAX, V_MAX);
		//store image to matrix
		capture.read(cameraFeed);
		cv::flip(cameraFeed,cameraFeed,1);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2YCrCb);
		cvtColor(cameraFeed, blue, COLOR_BGR2YCrCb);
		cvtColor(cameraFeed, red, COLOR_BGR2YCrCb);
		//HSV = cameraFeed;
		//inRange(cameraFeed,(Scalar)min, (Scalar)max, HSV);
		//filter HSV image between values and store filtered image to
		//threshold matrix
		inRange(HSV,(Scalar)min, (Scalar)max,threshold);
		inRange(blue, Scalar(4,100,95), Scalar(55,130,130), bThreshold);
		inRange(red, Scalar(73,100,104), Scalar(102,154,125), rThreshold);
		//perform morphological operations on thresholded image to eliminate noise
		//and emphasize the filtered object(s)
		//if(useMorphOps)
		morphOps(threshold);
		morphOps(bThreshold);
		morphOps(rThreshold);
		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		//if(abs(prevx - x) > 5 || abs(prevy - y) > 5)
			trackFilteredObject(x,y,threshold,cameraFeed);
			checkBlue(x, y, bThreshold, cameraFeed);
			checkRed(x, y, rThreshold, cameraFeed);

		//Move mouse
		int mx = x;
		int my = y;

		prevx = x;
		prevy = y;

		if(!calib)
		{

			
			//printf("%f:%f\n", width, height);
			//(assert(width!=0 && height!= 0 )); 
			ratiox = ((float)sw)/(CAM_WIDTH-(width));
			ratioy = ((float)sh)/(CAM_HEIGHT-(height));
			capture.set(CV_CAP_PROP_FRAME_WIDTH, (CAM_WIDTH - width) * ratiox);
			capture.set(CV_CAP_PROP_FRAME_HEIGHT, (CAM_HEIGHT - height) * ratioy);
			calib = true;
		}
		
		if(abs(prevx - x) < 2 && abs(prevy - y) < 3){
		 	XWarpPointer(pdsp, None, root, 0, 0, 0, 0, mx, my);
		XFlush(pdsp);}
		
		prevx = x;
		prevy = y;

		//show frames 
		//imshow(windowName2,threshold);
		//imshow(windowName,cameraFeed);
		//imshow(windowName1,HSV);
		

		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		
		char c = cvWaitKey(33);
   		if( c == 27 ) break;
	}




	XCloseDisplay(pdsp);

	return 0;
}
