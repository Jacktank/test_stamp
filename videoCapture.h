#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <time.h>
#include <thread>
#include <mutex>
#include <string>
#include <chrono>

#define TRYLOCK_MUTEX

class MyVideoCaptures
{
private:
	MyVideoCaptures(){};
private:

	std::vector<std::thread *> vpthrHandle;//
	int nCams;
	bool isExit;

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
 
			isExit = false;//
	
			for (int i = 0; i < nCams; i++)
			{
					std::pair<long long, cv::Mat> tmp;		
					MatsBuffer.push_back(tmp);
		
					v_mutex.push_back(new std::mutex);
			}
			v_cameraAddress = str_camsAdd;
	}

	void getStampImagePair(int id_cam, std::pair<long long, cv::Mat> & p_Mat)
	{
#ifdef	TRYLOCK_MUTEX
	  if (v_mutex[id_cam]->try_lock())
		{
#else
				v_mutex[id_cam]->lock();
#endif
			p_Mat = MatsBuffer[id_cam];		//clone???xuyaoma?
			v_mutex[id_cam]->unlock();//
			//sleep(3);	
#ifdef TRYLOCK_MUTEX
		}
#endif
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
		isExit = true;
		sleep(100);
	}

	void run(int id_cam)
	{
		
		auto start = chrono::system_clock::now();
		
		while (!isExit)
		{		
			cv::Mat tmp, tmp1;

			v_cap[id_cam] >> tmp;

			auto end   = chrono::system_clock::now();
			auto dtime = chrono::duration_cast<chrono::microseconds>(end - start)*chrono::milliseconds::period::num / chrono::milliseconds::period::den;
			if(tmp.empty())
			{
					std::cout<<"capture empty image..."<<std::endl;				
					continue;
			}

			std::cout<<"kkkkkkkkkk"<<id_cam<<std::endl;
			sleep(30);
			cv::resize(tmp, tmp1, cv::Size(), 0.5, 0.5, CV_INTER_LINEAR);


		//	std::stringstream sttr;
		//	sttr<<id_cam;
		//	std::string str;
		//	sttr>>str;
		//	cv::imshow(str,tmp1);
			
#ifdef TRYLOCK_MUTEX
			if (v_mutex[id_cam]->try_lock())
			{
#else
					v_mutex[id_cam]->lock();
#endif
					MatsBuffer[id_cam].second = tmp1;//				
					MatsBuffer[id_cam].first = (long long)dtime.count();
				
					//std::cout << "cameraId:" << id_cam <<" time:"<<MatsBuffer[id_cam].first << std::endl;				
					v_mutex[id_cam]->unlock();				
					//Sleep(100);
#ifdef TRYLOCK_MUTEX
			}
#endif
		}
		std::cout << "close camera thread..." << std::endl;
	}
};

#endif

