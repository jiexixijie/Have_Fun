// sever.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int static const m = 5;
int members = 0;
bool stop = false;
int send_dig = -1;   //用于传递产生的随机数
int finished[m] = {0};    //表示以接收随机数
int quit[m] = { 0 };     //表示中途退出的人
bool wait = true; //表示各线程已经发送完数字的个数
int winner[m];   //游戏结束

std::mutex start;  //比赛开始
std::mutex mutex;  //发送矩阵
std::mutex signal;  //等待随机数产生信号

bool bindsock(SOCKADDR_IN &addr, SOCKET &sever);
void gamestart(SOCKET sever);  //游戏开始 close socket 结束accept
bool Go();                      //手动输入G，则无需等待人齐,直接开始比赛
void Message(SOCKET sock,int n);         //预先准备 ，发送数组
void randmartix(int (&array)[m*m],int n);  //生成随机矩阵 ，发送->client,n:index 加入变量
void sendmartix(SOCKET sock, int n);   //发送矩阵
void show(int const(&array)[m*m]);              //打印数组
void binggo(int const members);                   //游戏函数   members:参见game人数
int rand_digital(int(&martix)[m*m], int n);         //开始产生随机数字   n:以产生的数字
bool findwinner();   //查询是否有winner
void summary(SOCKET sock,int n);
void AllWinner(SOCKET sock);  //发送所有winner

struct Packet       //防止粘包
{
	int len;
	char data[8193];
};

size_t sendn(SOCKET sock,char *buffer,int count,int flags);        //封装send和recv函数 防止粘包和自动memset
size_t recvn(SOCKET sock,char *buffer, int count, int flags);

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsadata;
	WSAStartup(0x0202,&wsadata);
	SOCKET sever = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	SOCKADDR_IN addr;
	int i=bindsock(addr, sever); //初始化sockaddr 绑定 监听
	if (i == false)
	{
		printf("bind error!\r\n");
		system("pause");
		return 0;
	}
	SOCKET sockets[10];  //最多人数
	std::thread threads[10];
	memset(sockets, 0, sizeof(sockets));
	memset(threads, 0, sizeof(threads));

	signal.lock();
	std::thread t(gamestart,sever);
	t.detach();

	while (true)
	{	
		i = sizeof(SOCKADDR_IN);
		sockets[members] = accept(sever, (SOCKADDR*)&addr, &i);
		if (sockets[members] != INVALID_SOCKET)
		{ 
			members++;
			printf("第%d号玩家加入比赛\r\n", members);
			int index = members - 1;
			threads[index] = std::thread(Message,sockets[index],members);
			threads[index].detach();
		}
		else
		{
			if (stop == true)
			{
				printf("正在游戏\r\n");
				break;
			}
			printf("accept fail\r\n");
			break;
		}
	}
	binggo(members);            //开始游戏   参加人数，和对应的socket
	printf("game finish\r\n");
	//最后收尾
	while (members)
	{
		Sleep(100);
	}
	for (int i = 0; i < m; i++)   //delete
	{
		closesocket(sockets[i]);
	}
	WSACleanup();
	system("pause");
	return 0;
}


bool bindsock(SOCKADDR_IN &addr, SOCKET &sever)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int i = bind(sever, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if (i != NOERROR)
	{
		printf("fail to bind\r\n");
		return false;
	}
	i = listen(sever, 64);
	return true;
}

void  gamestart(SOCKET sever)
{
	start.lock();
	for (int i = 0; i < m; i++)  //初始化winner
	{
		winner[i] = -1;
	}
	printf("Wait %d members to join the game\r\n", m);
	printf("********************************\r\n");
	printf("Input \"G\" can start instantly\r\n");
	printf("********************************\r\n");
	while (true)
	{
		if (Go())
		{
			break;
		}
		else if (members == m)    //达到人数上限
		{
			break;
		}
		Sleep(100);
	}
	printf("game start!\r\n");
	stop = true;
	closesocket(sever);
	start.unlock();
	return;
}

bool Go()
{
	if (_kbhit())  //键盘存在输入
	{
		char key = _getch();
		if (members < 2)
		{
			printf("至少要两个人哦！\r\n");
			return false;
		}
		if (key == 'G' || key == 'g')
		{
			return true;
		}
		else
		{
			printf("Input G to start game\r\n");
		}
	}
	return false;
}

void Message(SOCKET sock, int n)    //各SOCKET线程
{
	sendmartix(sock, n);
	int i = 0;
	char buffer[50] = "0";
	//等待接收主线程的随机数字，直到收到binggo结束进程
	//---------------------------产生随机数字 等待binggo unlock signal----------------------------------------------
	while (winner[0]<0)   //直到有人binggo   发送第%d个digital提示,发送digital，收到请求 
	{
		signal.lock();  //等待产生数字信号
		i = sprintf_s(buffer, "number:");
		i = sendn(sock, buffer, i, 0);
		int temp = send_dig;  //防止send_dig被memset
		i = sendn(sock, (char*)&temp, 4, 0);//发送数字
		if (i < 0)
		{
			printf("fail to send %d 号玩家,自动退出\r\n", n);
			members--;
			quit[n - 1] = -1;
			signal.unlock();
			return;
		}
		signal.unlock();
		//需要等待其他玩家开始发送数字，而不是仍在循环判断finish=0，
		//-------------------------------------------------------------
		i = recvn(sock, buffer, sizeof(buffer), 0);
		if (i < 0)
		{
			quit[n - 1] = -1;
			break;
		}
		char win[] = "binggo";
		i = strcmp(win, buffer);
		if (i == 0)
		{
			while (winner[i]>0)   //对winner进行写入
			{
				i++;
			}
			winner[i] = n;
		}
		//-------------一直循环
		wait = true;
		finished[n-1]=1;
		i = 0;
		  //等待下一轮   //大问题-----
		while (!wait)
		{
			Sleep(100);
		}
	}
	//--------------结束-----------------------
	memset(buffer, 0, sizeof(buffer));
	i = sprintf_s(buffer, "game finished!");
	i = sendn(sock, buffer, i, 0);
	//-------------打印------------------------
	summary(sock, n);
	members--;   //结束
	return;
}

void sendmartix(SOCKET sock, int n)
{
	char buffer[] = "Just wait for others to join";
	int i = sendn(sock, buffer, sizeof(buffer), 0);
	if (i < 0)
	{
		printf("向%d号玩家send错误\r\n", n);
	}

	i = sprintf_s(buffer, "%d号玩家你好!", n);
	i = sendn(sock, buffer, i, 0);

	start.lock();   //准备接收game start信号
	start.unlock();

	mutex.lock();

	i = sprintf_s(buffer, "start");
	i = sendn(sock, buffer, i, 0);   //发送比赛开始
	if (i < 0)
	{
		printf("send error\r\n");
		quit[n - 1] = 1;
		mutex.unlock();
		return;
	}

	int martix[m*m] = { 0 };
	randmartix(martix, n);

	i = sendn(sock, (char*)martix, sizeof(martix), 0);  //发送binggo矩阵
	if (i < 0)
	{
		printf("send error\r\n");
		quit[n - 1] = 1;
		mutex.unlock();
		return;
	}
	mutex.unlock();
	return;
}


void randmartix(int (&array)[m*m],int n)
{
	int  base[m*m] = { 0 };
	for (int i = 0; i < m*m; i++)   //初始化
	{
		base[i] = i;
	}
	srand((unsigned)time(NULL)+n);
	int counts = m*m;
	for (int i = 0; i < m*m; i++)
	{
		int index = rand() % (counts);
		array[i] = base[index];
		base[index] = base[counts-1];
		counts--;
	}
	return;
}


void show(int const (&array)[m*m])
{
	for (int i = 0; i < m*m; i++)
	{
		if (i%m == 0)
		{
			printf("\r\n");
		}
		printf("%d ", array[i]);
	}

	return;
}
//-------------------------------------------------------------------
int rand_digital(int (&martix)[m*m],int n)
{
	srand((unsigned)time(NULL));
	int index = rand() % (n);
	int digital = martix[index];
	martix[index] = martix[n - 1];
	Sleep(1000);
	return digital;
}

void binggo(int const members)
{
	int martix[m*m];   //初始化
	for (int i = 0; i < m*m; i++)
	{
		martix[i] = i;
	}
	int n = m*m;

	//创建 线程等待 收到主线程的数组
	while (n>0)
	{
		send_dig = rand_digital(martix, n);   //生成随机数组并发送，直到有有人发送binggo
		printf("send:%d\r\n", send_dig);
		n--;
		signal.unlock();

		int i = 0;
		while (i<members)  //等到全部收到数字后再生成下一个数字
		{
			if (quit[i])  //中途退出
			{
				i++;
			}
			else if (!finished[i])
			{
				Sleep(100);
			}
			else 
			{
				finished[i] = 0;  //清零
				i++;
			}
		}
		wait = false;

		signal.lock();
		if (findwinner())  //是否有人binggo
		{
			//check(); //进行检查 
			signal.unlock();
			break;
		}
		else
		{
			printf("nobody binggo\r\n");
		}
		//检查是否中途全部退出了比赛
		if (!members)
		{
			printf("all quit the game\r\n");
			return;
		}
	}
	return;
}
//-----------------------------------------------------------------
size_t sendn(SOCKET sock, char *buffer, int count, int flags)
{
	if (count < 1)
	{
		printf("can't send\r\n");
		return -1;
	}
	Packet packet;
	memset(&packet, 0, sizeof(packet));
	packet.len = count;
	memcpy(packet.data,buffer,count);
	int i = send(sock, (char*)&packet, count + 4, flags);
	memset(buffer, 0, count);
	return i;
}

size_t recvn(SOCKET sock, char *buffer, int count, int flags)
{
	Packet packet;
	memset(&packet, 0, sizeof(packet));
	memset(buffer, 0, count);
	int i = recv(sock, (char*)&packet.len, 4, 0);
	packet.len = packet.len;
	i = recv(sock, packet.data, packet.len, 0);
	if (i<0)
	{
		printf("revcv error!\r\n");
		return -1;
	}
	else if (packet.len > count)
	{
		printf("can't recv too long!\r\n");
		return -1;
	}
	else
	{
		memcpy(buffer,&packet.data,packet.len);
		return packet.len;
	}
}

bool findwinner()
{
	int i = 0;
	if (winner[i] < 0)
	{
		return false;
	}
	else
	{
		printf("winnner:");
		while (winner[i]>0)
		{
			printf("%d号玩家 ", winner[i]);
			i++;
		}
		printf("\r\n");
		return true;
	}
}

void summary(SOCKET sock,int n)
{
	char buffer[20];
	int i;
	for (int j = 0; winner[j] > 0; j++)
	{
		if (winner[j] == n)                              //如果win 发送 You win
		{
			i = sprintf_s(buffer, "You win!");
			i = sendn(sock, buffer, i, 0);
			AllWinner(sock);   //发送所有winner
			return;
		}
	}
	i = sprintf_s(buffer, "You lose.");             //不然发送You Lose
	i = sendn(sock, buffer, i, 0);
	AllWinner(sock);
	return;
}

void AllWinner(SOCKET sock)
{
	char a[200] = "winer is:";
	int len = strlen(a);
	for (int j = 0; winner[j] > 0; j++)
	{
		char temp[20] = "0";
		int i = sprintf_s(temp, " %d号玩家", winner[j]);
		strcat_s(a, temp);
		len += i;
	}
	len = sendn(sock, a, len, 0);
	return;
}

