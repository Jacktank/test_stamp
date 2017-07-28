#include "videoCapture.h"

int main(int argc, char* argv[])
{
		int nCams = 3;
		if(argc == 1)
			nCams = 21;
		else
			nCams = atoi(argv[1]);

		std::vector<std::string> vstring;
		for(int i=0;i<nCams;i++)
		{
#if 1
				std::string str;
				std::stringstream sttr;
				sttr<<"./test"<<i<<".avi";		
				sttr>>str;
#else
				std::string str = "./test.avi";
#endif
				vstring.push_back(str);
		}
//    nCams = 3;
//		vstring.push_back(std::string("rtsp://admin:12345goccia@10.0.0.2:554//Streaming/Channels/1"));
//		vstring.push_back(std::string("rtsp://admin:12345goccia@10.0.0.3:554//Streaming/Channels/1"));
//		vstring.push_back(std::string("rtsp://admin:12345goccia@10.0.0.4:554//Streaming/Channels/1"));
//
		MyVideoCaptures* p_cap = MyVideoCaptures::GetInstance();
		p_cap->SetParam(nCams,vstring);

		if(!p_cap->init())
		{
				return -1;
		}

		while(1)
		{
			sleep(3);
		}

		return 0;
}
