#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <string>

using namespace std;


#define        PORT_NUM        (9527)

int main()
{

	/*
		struct WSAData
		{
			WORD            wVersion;
			WORD            wHighVersion;
			char            szDescription[WSADESCRIPTION_LEN+1];
			char            szSystemStatus[WSASYSSTATUS_LEN+1];
			unsigned short    iMaxSockets;
			unsigned short    iMaxUdpDg;
			char FAR*        lpVendorInfo;
		};

		�Ѽ�
		wVersion
		Windows �q�T�� DLL �n�D�I�s�ݨϥΪ� Windows �q�T�ݳW�檩���C

		wHighVersion
		�o�� DLL �i�䴩���̰� Windows �q�T�ݳW�檩�� (�s�X�p�W�ҥ�)�C �q�`�A�o�|�P�ۦPwVersion�C

		szDescription
		Windows �q�T�� DLL �b�H null �פ ASCII �r��ƻs Windows Sockets ��@���y�z�A�]�A�t�Ө����ѧO�C ��r (���׳̦h 256 �Ӧr��) �i�H�]�t����r���A���L�A�t�ӷ|�Q�i�|�]�t����M�榡�Ʀr���G���ε{���̥i��N������� (�i��H�I�_�覡) �b���A�T�C

		szSystemStatus
		Windows �q�T�� DLL �b�H null �פ ASCII �r��ƻs�������A�βպA��T�C Windows Sockets DLL ����T�i��|�������U�ϥΪ̩Τ䴩�H��; �ɡA�~�ϥγo����줣������������szDescription���C

		iMaxSockets
		��@�B�z�ǥi��|�}�Ҫ��̤j�q�T�ݼƥءC Windows �q�T�ݹ�@�i���ѳq�T�ݥ��춰�ϥH�t�m�����B�z�ǡA�Ϊ̤]�̷ӳq�T�ݪ��C�ӳB�z�Ǹ귽�i��t�m�C �Ʀr�����ϬM Windows �q�T�� DLL �κ����n��պA���]�w�覡�C ���ε{�����g�̥i�H�ϥγo�ӼƦr�� Windows Sockets ��@�O�_�i�����ε{���ϥΪ����ܡC �Ҧp�AX Windows ���A���i��|�ˬdiMaxSockets�Ĥ@���ҰʡJ �p�G�֩� 8�A���ε{���|��ܿ��~�T�����ܨϥΪ̭��s�]�w�����n��C (�o�O���pszSystemStatus��r�i��|�ϥΡC)����M�ä��O�үS�w���ε{���i�H��ڰt�miMaxSockets�q�T�ݡA�]���i�঳��L�ϥΤ��� Windows Sockets ���ε{���C

		iMaxUdpDg
		�i�� Windows �q�T�����ε{���ǰe�α������̤j�ϥΪ̸�ƥ]�q�T��w (UDP) ��ƥ]���줸�դj�p�C �p�G��@�S���I�[����A iMaxUdpDg���s�C �b Berkeley �q�T�ݪ��\�h��@���AUDP ��ƥ]�W�� 8192 �줸�ժ����t���� (���n�ɷ|����)�C Windows �q�T�ݹ�@�i�H�ھڤ��q���սw�İϪ��t�m�I�[����C �̤p��iMaxUdpDg�ۮe Windows �q�T�ݹ�@�O 512�C �Ъ`�N�A���׭�iMaxUdpDg�A���o���նǰe�j���̤j�ǿ��� (MTU) �������s������ƥ]�C (Windows �q�T�� API �����Ѿ���ӴM�� MTU�A���L���o�֩� 512 �줸��)�C

		lpVendorInfo
		�t�ӯS�w��Ƶ��c�����ݫ��СC �o�ӵ��c���w�q (�p�G������) �W�X Windows �q�T�ݳW�檺�d��C
	*/
	WSADATA wsaData;
	WORD    DLLVersion;
	DLLVersion = MAKEWORD(2, 1);//winsocket-dll version

	// �� WSAStartup �}�l Winsocket-DLL
	int err = WSAStartup(DLLVersion, &wsaData);
	if (err != 0)
	{
		// Tell the user that we could not find a usable Winsock DLL.
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 1)
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return 1;
	}
	else
	{
		printf("The Winsock 2.1 dll was found okay\n");
	}


	// �ŧi socket��}��T (���P���q�T,�����P����}��T,�ҥH�|�����P����Ƶ��c�s��o�Ǧ�}��T)
	/*
	/*
	 *
	 * Socket address, internet style.
		struct sockaddr_in
		{
				short   sin_family;
				u_short sin_port;
				struct  in_addr sin_addr;
				char    sin_zero[8];
		};
		#ifndef s_addr

	 * Internet address (old style... should be updated)

		struct in_addr
		{
			union
			{
					struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
					struct { u_short s_w1,s_w2; } S_un_w;
					u_long S_addr;
			} S_un;
		}
	*/
	SOCKADDR_IN    addr;
	int addrlen = sizeof(addr);

	// Create socket
	SOCKET sListen;        //listening for an incoming connection
	SOCKET sConnection;        //oerating if a connection was found

	// AF_INET        :  ��ܫإߪ�Socket�ݩ�internet family
	// SOCK_STREAM    :  ��ܫإߪ�socket�Oconnection-oriented socket
	sConnection = socket(AF_INET, SOCK_STREAM, NULL);

	// �]�w��}��T�����
	addr.sin_addr.s_addr = inet_addr("140.118.175.159");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NUM);


	// �]�wListen
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	if (sListen == INVALID_SOCKET)
	{
		printf("socket function failed with error : %u \n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	// Bind the socket
	int iResult = bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	if (iResult == SOCKET_ERROR)
	{
		printf("Bind failed with error : %u \n", WSAGetLastError());
		closesocket(sListen);
		WSACleanup();
		return 1;
	}
	//SOMAXCONN: listening without any limit
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("listen function failed with error: %d \n", WSAGetLastError());
		closesocket(sListen);
		WSACleanup();
		return 1;

	}
	else
		printf("Listening on socket...\n");


	// ���ݳs�u
	SOCKADDR_IN clientAddr;
	while (1)
	{
		cout << "Waitting for connect... " << endl;
		if (sConnection = accept(sListen, (SOCKADDR*)&clientAddr, &addrlen))
		{
			cout << "a connection was found." << endl;
			printf("Server : got a connection from : %s\n", inet_ntoa(addr.sin_addr));

			//Send message to client 
			const char *sendbuf = "sending data test";
			printf("Send buf to client (0x%x) \n", &sendbuf);

			//----------------------
			// Send an initial buffer
			iResult = send(sConnection, sendbuf, (int)strlen(sendbuf), 0);
			if (iResult == SOCKET_ERROR)
			{

				printf("send failed with error :%d \n", WSAGetLastError());
				closesocket(sConnection);
				WSACleanup();
				return 1;
			}
		}
	}

	getchar();
	getchar();
}