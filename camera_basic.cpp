#include <opencv2/opencv.hpp>
#include <raspicam_cv.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace cv;
using namespace raspicam;

Mat frame;

RaspiCam_Cv Camera;

 void Setup ( int argc,char **argv, RaspiCam_Cv &Camera )
  {
    Camera.set ( CAP_PROP_FRAME_WIDTH,  ( "-w",argc,argv,360 ) );
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
	cvtColor(frame, frame, cv::COLOR_BGR2RGB);
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
	
	namedWindow("Frame",WINDOW_KEEPRATIO);
	moveWindow("Frame",700,100);
	resizeWindow("Frame",640,480);
	imshow("Frame",frame);
	
	
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

