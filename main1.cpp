#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

int main()
{
	const char* testVideoPath = "rtsp://admin:12345goccia@10.0.0.3:554/h264/ch1/main/av_stream";
	//const char* testVideoPath = "rtsp://admin:12345goccia@10.0.0.3:554//Streaming/Channels/1";
	cv::VideoCapture cap_1(testVideoPath);
	if(!cap_1.isOpened())  // check if we succeeded
	{
		cout<<"error:fail to load camera 1"<<endl;
		return -1;
	}

	const char* testVideoPath1 = "rtsp://admin:12345goccia@10.0.0.4:554/h264/ch1/main/av_stream";
	//const char* testVideoPath1 = "rtsp://admin:12345goccia@10.0.0.4:554//Streaming/Channels/1";
	cv::VideoCapture cap_2(testVideoPath1);
	if(!cap_2.isOpened())  // check if we succeeded
	{
		cout<<"error:fail to load camera 2"<<endl;
		return -1;
	}

	int width_img_1 = (int)cap_1.get(CV_CAP_PROP_FRAME_WIDTH);
	int height_img_1 = (int)cap_1.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout<<"width_img_1:"<<width_img_1 <<",height_img_1:"<<height_img_1<<endl;

	int width_img_2 = (int)cap_2.get(CV_CAP_PROP_FRAME_WIDTH);
	int height_img_2 = (int)cap_2.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout<<"width_img_2:"<<width_img_2<<",height_img_2:"<<height_img_2<<endl;

	cv::Mat origin_image1,origin_image2,resizeImage1,resizeImage2,resizeImage11,resizeImage22, union_image;

  float ratio = 0.5;
	int num = 0;
	CvSize size_1 = cvSize(width_img_1*ratio,height_img_1*ratio); 
	CvSize size_2 = cvSize(width_img_2*ratio,height_img_2*ratio); 
	double fps = 30; 

//	CvVideoWriter *writer_1 = cvCreateVideoWriter("test1.avi",CV_FOURCC('M', 'J', 'P', 'G'),fps,size_1); //´´½¨ÊÓÆµÎÄ¼þ
//	CvVideoWriter *writer_2 = cvCreateVideoWriter("test2.avi",CV_FOURCC('M', 'J', 'P', 'G'),fps,size_2); //´´½¨ÊÓÆµÎÄ¼þ

	while (1)
	{
		cap_1>>origin_image1;

		if(origin_image1.empty())
		{
				continue;
		}

		cap_2>>origin_image2;

		if(origin_image2.empty())
		{
				continue;
		}

		cv::resize(origin_image1,resizeImage1,cv::Size(),ratio,ratio,CV_INTER_CUBIC);
		cv::resize(origin_image2,resizeImage2,cv::Size(),ratio,ratio,CV_INTER_CUBIC);

		cv::flip(resizeImage1,resizeImage11,-1);
		cv::flip(resizeImage2,resizeImage22,-1);

		cv::vconcat(resizeImage11,resizeImage22,union_image);

		//cv::imshow("img1", resizeImage11);
		//cv::imshow("img2", resizeImage22);

		cv::imshow("union_img", union_image);
		char key = cv::waitKey(1);
		if(key =='q')
		{
			cout<<"Have already to end cams!!!"<<endl;
			break;
		}
		//IplImage temp1 = IplImage(resizeImage11);
		//cvWriteFrame(writer_1,&temp1);

		//IplImage temp2 = IplImage(resizeImage22);
		//cvWriteFrame(writer_2,&temp2);

	}
	
	//cvReleaseVideoWriter(&writer_1); 
	//cvReleaseVideoWriter(&writer_2); 

	cv::waitKey(0);
	return 0;
}

