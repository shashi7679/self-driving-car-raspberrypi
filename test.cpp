#include <opencv2/opencv.hpp>
#include <raspicam_cv.h>
#include <iostream>
#include <chrono>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;
using namespace raspicam;



RaspiCam_Cv Camera;

CascadeClassifier Stop_Cascade;
Mat frame,frame_Stop, RoI_Stop, gray_Stop;
vector<Rect> Stop;


 void Setup ( int argc,char **argv, RaspiCam_Cv &Camera )
  {
    Camera.set ( CAP_PROP_FRAME_WIDTH,  ( "-w",argc,argv,400 ) );
    Camera.set ( CAP_PROP_FRAME_HEIGHT,  ( "-h",argc,argv,240 ) );
    Camera.set ( CAP_PROP_BRIGHTNESS, ( "-br",argc,argv,50 ) );
    Camera.set ( CAP_PROP_CONTRAST ,( "-co",argc,argv,50 ) );
    Camera.set ( CAP_PROP_SATURATION,  ( "-sa",argc,argv,50 ) );
    Camera.set ( CAP_PROP_GAIN,  ( "-g",argc,argv ,50 ) );
    Camera.set ( CAP_PROP_FPS,  ( "-fps",argc,argv,10));

}

void Capture()
{
	Camera.grab();
	Camera.retrieve(frame);
	cvtColor(frame, frame_Stop, COLOR_BGR2RGB);
	cvtColor(frame,frame,COLOR_BGR2RGB);
}

void Stop_detection()
{
    if(!Stop_Cascade.load("//home//pi//Desktop//STOP Sign//Stop_cascade.xml"))
    {
	printf("Unable to open stop cascade file");
    }
    RoI_Stop = frame_Stop(Rect(0,0,400,240));
    cvtColor(RoI_Stop, gray_Stop, COLOR_RGB2GRAY);
    equalizeHist(gray_Stop, gray_Stop);
    Stop_Cascade.detectMultiScale(gray_Stop, Stop);
    
    for(int i=0; i<Stop.size(); i++)
    {
	Point P1(Stop[i].x, Stop[i].y);
	Point P2(Stop[i].x + Stop[i].width, Stop[i].x + Stop[i].height);
	
	rectangle(RoI_Stop, P1, P2, Scalar(0, 0, 255), 2);
	putText(RoI_Stop, "Stop Sign", P1, FONT_HERSHEY_PLAIN, 1,  Scalar(0, 0, 255, 255), 2);
	
	
	
    }
}

int main(int argc,char **argv)
{
	
	Setup(argc, argv, Camera);
	cout<<"Connecting to camera"<<endl;
	if(!Camera.open())
	{	cout<<"Failed to connect"<<endl;
		return -1;
	}
	cout<<"Camera ID:-"<<Camera.getId()<<endl;
	while(1)
	{
	auto start = std::chrono::steady_clock::now();	
	Capture();
	Stop_detection();
	
	namedWindow("Frame",WINDOW_KEEPRATIO);
	moveWindow("Frame",700,100);
	resizeWindow("Frame",640,480);
	imshow("Frame",frame);
	
	namedWindow("Stop Sign", WINDOW_KEEPRATIO);
    moveWindow("Stop Sign", 1280, 580);
    resizeWindow("Stop Sign", 640, 480);
    imshow("Stop Sign", RoI_Stop);
	
	
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	float t = elapsed_seconds.count();
	int FPS = 1/t;
	cout<<"\n FPS:-"<<FPS;
	imshow("Frame",frame);
	waitKey(1);
	}
	return 0;
	
}

