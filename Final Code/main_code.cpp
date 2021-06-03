#include <opencv2/opencv.hpp>
#include <raspicam_cv.h>
#include <iostream>
#include <chrono>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <wiringPi.h>

using namespace std;
using namespace cv;
using namespace raspicam;



RaspiCam_Cv Camera;
Point2f Source[] ={Point2f(80,130),Point2f(350,130),Point2f(30,200),Point2f(400,200)}; 
Point2f Destination[] = {Point2f(100,0),Point2f(280,0),Point2f(100,240), Point2f(280,240)};

int LeftLanePos, RightLanePos, frameCenter, laneCenter, Result,laneEnd,dist_Stop;

stringstream ss;

CascadeClassifier Stop_Cascade;
Mat frame,frame_Stop,Matrix,frame_pres,frame_Grey,frame_Thres,frame_Edge,frame_final,frame_finalDupli,frame_finalDupli_1;

Mat ROI_Lane,ROI_Lane_End,gray_Stop,ROI_Stop;
vector<Rect> Stop;
vector<int> hist_Lane;
vector<int> hist_Lane_End;



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
void Perspective()
{	
	line(frame,Source[0],Source[1],Scalar(0,0,255),2);
	line(frame,Source[1],Source[3],Scalar(0,0,255),2);
	line(frame,Source[3],Source[2],Scalar(0,0,255),2);
	line(frame,Source[2],Source[0],Scalar(0,0,255),2);
	
	Matrix = getPerspectiveTransform(Source, Destination);
	warpPerspective(frame, frame_pres, Matrix, Size(400,240));
}
void Threshold()
{
	cvtColor(frame_pres,frame_Grey,COLOR_RGB2GRAY);
	inRange(frame_Grey, 150, 240, frame_Thres);
	Canny(frame_Grey,frame_Edge,100,500,3,false);
	add(frame_Thres,frame_Edge,frame_final);
	cvtColor(frame_final,frame_final,COLOR_GRAY2RGB);
	cvtColor(frame_final,frame_finalDupli,COLOR_RGB2BGR);
	cvtColor(frame_final,frame_finalDupli_1,COLOR_RGB2BGR);
	
}
void Histogram()
{
	hist_Lane.resize(400);
	hist_Lane.clear();
	for(int i=0;i<400;i++)
	{	ROI_Lane = frame_finalDupli(Rect(i,140,1,100));
		divide(255,ROI_Lane,ROI_Lane);
		hist_Lane.push_back((int)(sum(ROI_Lane)[0]));
	}
	hist_Lane_End.resize(400);
	hist_Lane_End.clear();
	for(int i=0;i<400;i++)
	{	ROI_Lane_End = frame_finalDupli_1(Rect(i,0,1,240));
		divide(255,ROI_Lane_End,ROI_Lane_End);
		hist_Lane_End.push_back((int)(sum(ROI_Lane_End)[0]));
	}
	laneEnd = sum(hist_Lane_End)[0];
	cout<<"Lane END = "<<laneEnd<<endl;
}
void Lane_Finder()
{
	
    vector<int>:: iterator LeftPtr;
    LeftPtr = max_element(hist_Lane.begin(), hist_Lane.begin() + 180);
    LeftLanePos = distance(hist_Lane.begin(), LeftPtr); 
    
    vector<int>:: iterator RightPtr;
    RightPtr = max_element(hist_Lane.begin() +180, hist_Lane.end());
    RightLanePos = distance(hist_Lane.begin(), RightPtr);
    
    line(frame_final, Point2f(LeftLanePos, 0), Point2f(LeftLanePos, 240), Scalar(0, 255,0), 2);
    line(frame_final, Point2f(RightLanePos, 0), Point2f(RightLanePos, 240), Scalar(0,255,0), 2); 
}
void Lane_Center()
{
    laneCenter = (RightLanePos-LeftLanePos)/2 +LeftLanePos;
    frameCenter = 195;
    
    line(frame_final, Point2f(laneCenter,0), Point2f(laneCenter,240), Scalar(0,255,0), 3);
    line(frame_final, Point2f(frameCenter,0), Point2f(frameCenter,240), Scalar(255,0,0), 3);

    Result = laneCenter-frameCenter;
}
void Stop_detection()
{	
	if(!Stop_Cascade.load("//home//pi//Desktop//Final Code//STOP_cascade.xml"))
	{	
		cout<<"Unable to open Cascade Classifier!!!"<<endl;
	}
	ROI_Stop = frame_Stop(Rect(0,0,200,140));
	cvtColor(ROI_Stop, gray_Stop, COLOR_RGB2GRAY);
	equalizeHist(gray_Stop, gray_Stop);
	Stop_Cascade.detectMultiScale(gray_Stop, Stop);
	for(int i=0; i<Stop.size(); i++)
	{
	Point P1(Stop[i].x, Stop[i].y);
	Point P2(Stop[i].x + Stop[i].width, Stop[i].x + Stop[i].height);
	
	rectangle(ROI_Stop, P1, P2, Scalar(0, 0, 255), 2);
	putText(ROI_Stop, "Stop Sign", P1, FONT_HERSHEY_PLAIN, 1,  Scalar(0, 0, 255, 255), 2);
	dist_Stop = (-0.45454)*(P2.x-P1.x) + 46.818;
	ss.str(" ");
	ss.clear();
	ss<<"D = "<<dist_Stop<<"(cm)";
	putText(ROI_Stop, ss.str(), Point2f(1,130), 0,1, Scalar(0,0,255), 2);
	
	}
}

void Capture()
{
	Camera.grab();
	Camera.retrieve(frame);
	cvtColor(frame, frame_Stop, COLOR_BGR2RGB);
	cvtColor(frame,frame,COLOR_BGR2RGB);
}


int main(int argc,char **argv)
{
	wiringPiSetup();
	pinMode(21,OUTPUT);
	pinMode(22,OUTPUT);
	pinMode(23,OUTPUT);
	pinMode(24,OUTPUT);
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
	Perspective();
	Threshold();
	Histogram();
	Lane_Finder();
	Lane_Center();
	Stop_detection();
	if (laneEnd > 3000)
	{
		digitalWrite(21, 1);
		digitalWrite(22, 1);    //decimal = 7
		digitalWrite(23, 1);
		digitalWrite(24, 0);
		cout<<"Lane End"<<endl;
	}
	if(dist_Stop>0 && dist_Stop<20)
	{
		digitalWrite(21,0);
		digitalWrite(22,0);	//Decimal value 8
		digitalWrite(23,0);
		digitalWrite(24,1);
		cout<<"Stop Sign"<<endl;
		dist_Stop=0;
		goto Stop_skip;
	}
	if(Result==0)
	{	digitalWrite(21,0);
		digitalWrite(22,0);	//Decimal value 0
		digitalWrite(23,0);
		digitalWrite(24,0);
		cout<<"Forward"<<endl;
	}
	else if(Result>0 && Result<10)
	{	digitalWrite(21,1);
		digitalWrite(22,0);	//Decimal value 1
		digitalWrite(23,0);
		digitalWrite(24,0);
		cout<<"Right_Slight"<<endl;
	}
	else if(Result >=10 && Result <20)
	{	digitalWrite(21,0);
		digitalWrite(22,1);	//Decimal value 2
		digitalWrite(23,0);
		digitalWrite(24,0);
		cout<<"Right_Medium"<<endl;
	}
	else if(Result>20)
	{	digitalWrite(21,1);
		digitalWrite(22,1);	//Decimal value 3
		digitalWrite(23,0);
		digitalWrite(24,0);
		cout<<"Right_Extreme"<<endl;
	}
	else if(Result<0 && Result>-10)
	{	digitalWrite(21,0);
		digitalWrite(22,0);	//Decimal value 4
		digitalWrite(23,1);
		digitalWrite(24,0);
		cout<<"Left_Slight"<<endl;
	}
	else if(Result <=-10 && Result >-20)
	{	digitalWrite(21,1);
		digitalWrite(22,0);	//Decimal value 5
		digitalWrite(23,1);
		digitalWrite(24,0);
		cout<<"Left_Medium"<<endl;
	}
	else if(Result<-20)
	{	digitalWrite(21,0);
		digitalWrite(22,1);	//Decimal value 6
		digitalWrite(23,1);
		digitalWrite(24,0);
		cout<<"Left_Extreme"<<endl;
	}
	Stop_skip:
		
	if (laneEnd > 4000)
	{
	ss.str(" ");
	ss.clear();
	ss<<" Lane End";
	putText(frame, ss.str(), Point2f(1,50), 0,1, Scalar(255,0,0), 2);
    
	}
	else if (Result == 0)
	{
	ss.str(" ");
	ss.clear();
	ss<<"Result = "<<Result<<" Move Forward";
	putText(frame, ss.str(), Point2f(1,50), 0,1, Scalar(0,0,255), 2);
    
	}
    
	else if (Result > 0)
	{
	ss.str(" ");
	ss.clear();
	ss<<"Result = "<<Result<<" Move Right";
	putText(frame, ss.str(), Point2f(1,50), 0,1, Scalar(0,0,255), 2);
    
	}
     
	else if (Result < 0)
	{
	ss.str(" ");
	ss.clear();
	ss<<"Result = "<<Result<<" Move Left";
	putText(frame, ss.str(), Point2f(1,50), 0,1, Scalar(0,0,255), 2);
    
	}
	
	namedWindow("Frame",WINDOW_KEEPRATIO);
	moveWindow("Frame",0,100);
	resizeWindow("Frame",640,480);
	imshow("Frame",frame);
	
	namedWindow("Perspective", WINDOW_KEEPRATIO);
	moveWindow("Perspective", 640, 100);
	resizeWindow("Perspective", 640, 480);
	imshow("Perspective", frame_pres);
    
	namedWindow("Threshold", WINDOW_KEEPRATIO);
	moveWindow("Threshold", 1280, 100);
	resizeWindow("Threshold", 640, 480);
	imshow("Threshold", frame_final);
	
	namedWindow("Stop Sign", WINDOW_KEEPRATIO);
	moveWindow("Stop Sign", 1280, 580);
	resizeWindow("Stop Sign", 640, 480);
	imshow("Stop Sign", ROI_Stop);

    
   

	
	
		
	
	
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

