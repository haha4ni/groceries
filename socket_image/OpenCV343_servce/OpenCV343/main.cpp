#include <opencv2/opencv.hpp>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable: 4996)
#include <WinSock2.h>
#include <iostream>
#include <string>

using namespace std;


#define        PORT_NUM        (9527)

cv::Mat img;
int main()
{
	img = cv::imread("50767698_604653473289909_3987410215556349952_n.jpg", CV_LOAD_IMAGE_COLOR);
	img.data;

	string confirm;
	char message[200];

	//開始 Winsock-DLL
	int r;
	WSAData wsaData;
	WORD DLLVersion;
	DLLVersion = MAKEWORD(2, 1);
	r = WSAStartup(DLLVersion, &wsaData);

	//宣告給 socket 使用的 sockadder_in 結構
	SOCKADDR_IN addr;

	int addlen = sizeof(addr);

	//設定 socket
	SOCKET sConnect;
	//AF_INET: internet-family
	//SOCKET_STREAM: connection-oriented socket
	sConnect = socket(AF_INET, SOCK_STREAM, NULL);

	//設定 addr 資料
	addr.sin_addr.s_addr = inet_addr("140.118.175.159");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NUM);

	cout << "connect to server?[Y] or [N]" << endl;
	cin >> confirm;

	if (confirm == "N")
	{
		exit(1);
	}
	else {
		if (confirm == "Y")
		{
			connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));

			Sleep(100);
			//接收 server 端的訊息
			/*
			ZeroMemory(message, 200);
			r = recv(sConnect, message, sizeof(message), 0);
			cout << message << endl;*/
			

			vector<uchar>buff;
			vector<int> param = vector<int>(2);
			param[0] = CV_IMWRITE_JPEG_QUALITY;
			param[1] = 95; // default(95) 0 - 100
			cv::imencode(".jpg", img, buff, param); // encode img1 to jpeg format
			int imgSize = (int)buff.size();

			//cv::Mat img_decode = cv::imdecode(buff, CV_LOAD_IMAGE_COLOR);
			//imshow("pic", img_decode);
			//cv::waitKey(0);


			int iResult = send(sConnect, (const char*)&imgSize, 4, 0);
			Sleep(100);

			int iResult2 = send(sConnect, (const char*)buff.data(), imgSize, 0);
			//設定 closesocket 時，不經過 TIME-WAIT 過程,直接關閉socket
			//BOOL bDontLinger = FALSE;
			//setsockopt(sConnect,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(BOOL));

			//若之後不再使用，可用 closesocket 關閉連線
			closesocket(sConnect);

			getchar();
			getchar();
		}
	}

}

