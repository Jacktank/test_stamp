#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include "common.h"
#include <thread>
#include <sys/time.h>
#include <mutex>
#include <string>
#include <sstream>

class MyVideoCaptures
{
private:
	MyVideoCaptures(){};
private:

	std::vector<std::thread *> vpthrHandle;//
	int nCams;
	//bool isExit;

	std::vector<std::string> v_cameraAddress;
	
	std::vector<cv::VideoCapture> v_cap;
	std::vector<std::pair<long long ,cv::Mat>> MatsBuffer;

public:

	std::vector<std::mutex*> v_mutex;

	~MyVideoCaptures()
	{
		for (int i = 0; i < nCams; i++)
		{			
			vpthrHandle[i]->detach();
			delete v_mutex[i];			
		}

		MatsBuffer.clear();

	}

	static MyVideoCaptures * GetInstance()    
  {    
     static MyVideoCaptures instance;     
     return &instance;    
  } 

	void SetParam(int num_cams,std::vector<std::string>& str_camsAdd)
	{
			nCams = num_cams;
 
			//isExit = false;//
	
			for (int i = 0; i < nCams; i++)
			{
					std::pair<long long, cv::Mat> tmp;		
					MatsBuffer.push_back(tmp);
		
					v_mutex.push_back(new std::mutex);
			}
			v_cameraAddress = str_camsAdd;
	}

	bool init()//
	{
		if (v_cameraAddress.size() != nCams)
		{
			return false;
		}
		for (int i = 0; i < nCams; i++)
		{			
			cv::VideoCapture cap(v_cameraAddress[i]);
			if (!cap.isOpened())
			{
				std::cout << "fail to open the cam" << std::endl;
				return false;
			}
			v_cap.push_back(cap);

		}
		
		for (int i = 0; i < nCams; i++)
		{
				std::cout<<"start camera thread..."<<i<<std::endl;	
				std::thread *pth = new std::thread(&MyVideoCaptures::run, this,i);
				vpthrHandle.push_back(pth);
		}
		
		return true;
	}

	void StopThread()
	{		
		//isExit = true;
		sleep(100);
	}

	void run(int id_cam)
	{
		bool isExit = false;
		timeval starttime,endtime;
		gettimeofday(&starttime,0);

		int count = 0;
		while (!isExit)
		{		
			cv::Mat tmp, tmp1 ,tmp2;

			v_cap[id_cam] >> tmp;
			if(tmp.empty())
			{
					std::cout<<"fail to get image..."<<id_cam<<std::endl;
					continue;
			}
			
			//double msec = v_cap[id_cam].get(CV_CAP_PROP_POS_MSEC);
			//std::cout<<"Thread No:"<<id_cam<<" Frame stamp:"<<msec<<" no: "<<count<<" times: "<<timeuse<<std::endl;

			if(tmp.empty())
			{
					std::cout<<"capture empty image..."<<std::endl;				
					continue;
			}

			//cv::resize(tmp, tmp1, cv::Size(), 0.5, 0.5, CV_INTER_LINEAR);
			//cv::flip(tmp1,tmp2,-1);

			if(count == 800)
				isExit = true;
			
		//	std::stringstream sttr;
		//	sttr<<"./img/img"<<id_cam<<"/Image_"<<id_cam<<"_"<<count<<"_"<<(int)msec<<".jpg";
		//	std::string str;
		//	sttr>>str;
		//
		//cv::imwrite(str,tmp2);

			count++;
		}

		gettimeofday(&endtime,0);
    double timeuse = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
    timeuse /=1000;//除以1000则进行毫秒计时，如果除以1000000则进行秒级别计时，如果除以1则进行微妙级别计时

		std::cout << "close camera thread: "<<id_cam<<" Frames to get: "<<count<<" average times used for one frame: "<<timeuse/800.0<<" ms"<<std::endl;	
	}
};

#endif

