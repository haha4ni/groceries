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

		參數
		wVersion
		Windows 通訊端 DLL 要求呼叫端使用的 Windows 通訊端規格版本。

		wHighVersion
		這個 DLL 可支援的最高 Windows 通訊端規格版本 (編碼如上所示)。 通常，這會與相同wVersion。

		szDescription
		Windows 通訊端 DLL 在以 null 終止之 ASCII 字串複製 Windows Sockets 實作的描述，包括廠商身分識別。 文字 (長度最多 256 個字元) 可以包含任何字元，不過，廠商會被告誡包含控制項和格式化字元：應用程式最可能將此值顯示 (可能以截斷方式) 在狀態訊。

		szSystemStatus
		Windows 通訊端 DLL 在以 null 終止之 ASCII 字串複製相關狀態或組態資訊。 Windows Sockets DLL 的資訊可能會有所幫助使用者或支援人員; 時，才使用這個欄位不應視為的延伸szDescription欄位。

		iMaxSockets
		單一處理序可能會開啟的最大通訊端數目。 Windows 通訊端實作可提供通訊端全域集區以配置到任何處理序，或者也依照通訊端的每個處理序資源進行配置。 數字完全反映 Windows 通訊端 DLL 或網路軟體組態的設定方式。 應用程式撰寫者可以使用這個數字當成 Windows Sockets 實作是否可由應用程式使用的指示。 例如，X Windows 伺服器可能會檢查iMaxSockets第一次啟動︰ 如果少於 8，應用程式會顯示錯誤訊息指示使用者重新設定網路軟體。 (這是情況szSystemStatus文字可能會使用。)很顯然並不保證特定應用程式可以實際配置iMaxSockets通訊端，因為可能有其他使用中的 Windows Sockets 應用程式。

		iMaxUdpDg
		可由 Windows 通訊端應用程式傳送或接收的最大使用者資料包通訊協定 (UDP) 資料包的位元組大小。 如果實作沒有施加限制， iMaxUdpDg為零。 在 Berkeley 通訊端的許多實作中，UDP 資料包上有 8192 位元組的隱含限制 (必要時會分散)。 Windows 通訊端實作可以根據片段重組緩衝區的配置施加限制。 最小值iMaxUdpDg相容 Windows 通訊端實作是 512。 請注意，不論值iMaxUdpDg，不得嘗試傳送大於比最大傳輸單位 (MTU) 的網路廣播的資料包。 (Windows 通訊端 API 不提供機制來尋找 MTU，不過不得少於 512 位元組)。

		lpVendorInfo
		廠商特定資料結構的遠端指標。 這個結構的定義 (如果有提供) 超出 Windows 通訊端規格的範圍。
	*/
	WSADATA wsaData;
	WORD    DLLVersion;
	DLLVersion = MAKEWORD(2, 1);//winsocket-dll version

	// 用 WSAStartup 開始 Winsocket-DLL
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


	// 宣告 socket位址資訊 (不同的通訊,有不同的位址資訊,所以會有不同的資料結構存放這些位址資訊)
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

	// AF_INET        :  表示建立的Socket屬於internet family
	// SOCK_STREAM    :  表示建立的socket是connection-oriented socket
	sConnection = socket(AF_INET, SOCK_STREAM, NULL);

	// 設定位址資訊的資料
	addr.sin_addr.s_addr = inet_addr("140.118.175.159");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NUM);


	// 設定Listen
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


	// 等待連線
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