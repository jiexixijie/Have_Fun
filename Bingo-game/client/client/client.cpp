// client.cpp : 定义控制台应用程序的入口点。
//


//长时间不连接会为全0
#include "stdafx.h"

int static const m = 5;
int martix[m][m] = { 0 };
int check[m][m] = { 0 };
int times = 0;

struct Packet       //防止粘包
{
	int len ;
	char data[8193];
};
size_t sendn(SOCKET sock, char *buffer, int count, int flags);        //封装send和recv函数 防止粘包和自动memset
size_t recvn(SOCKET sock, char *buffer, int count, int flags);

bool game(SOCKET sock, int digital);  //开始确认     
bool binggo();//判断是否binggo
bool digcheck();   //对角线检查
bool colcheck();  //列检查
bool rowcheck();  //行检查
void show(int const (&array)[m][m]);  //显示矩阵

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsadata;
	WSAStartup(0x0202, &wsadata);

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = 0;

	SOCKADDR_IN dst;
	dst.sin_family = AF_INET;
	dst.sin_port = htons(1234);
	dst.sin_addr.s_addr = inet_addr("127.0.0.1");

	int i = bind(client, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if (i < 0)
	{
		printf("fail to bind\r\n");
		system("pause");
		return 0;
	}
	i = connect(client, (SOCKADDR*)&dst, sizeof(SOCKADDR_IN));
	if (i < 0)
	{
		printf("can't connect sever\r\n");
		system("pause");
		return 0;
	}
	char buffer[8193];
	while (true)
	{
		memset(buffer, 0, sizeof(buffer));
		i = recvn(client, buffer, 8192, 0);
		if (i < 0)
		{
			printf("fail to recv\r\n");
			system("pause");
			return 0;
		}
		buffer[i] = '\0';
		printf("%s\r\n", buffer);
		char start[] = "start";
		i = strcmp(buffer, start);
		if (i == 0)
		{
			break;
		}
	}
	//i = recvn(client, buffer, 8192, 0);
	//buffer[i] = '\0';
	//printf("%s\r\n", buffer);
	//游戏开始  开始接收数组
	i = recvn(client,(char*)martix,sizeof(martix),0);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			printf("%d ", martix[i][j]);
		}
		printf("\r\n");
	}
	int digital;   //接收的数字
	//接收语句，接收数字，发送是否binggo
	while (true)
	{
		i = recvn(client, buffer, sizeof(buffer), 0);    //语句number: 或game finished
		if (i < 0)
		{
			system("pause");
			return 0;
		}
		printf("%s", buffer);
		char finish[] = "game finished!";
		if (strcmp(finish, buffer) == 0)
		{
			i = recvn(client, buffer, sizeof(buffer), 0);          //You win 或 You lose
			printf("\r\n%s\r\n", buffer);
			i = recvn(client, buffer, sizeof(buffer), 0);           //打印所有winner
			printf("%s\r\n", buffer);
			break;
		}
		i = recvn(client, (char*)&digital, sizeof(digital), 0);  //数字
		if (i < 0)
		{
			system("pause");
			return 0;
		}
		printf("%d\r\n",digital);
		times++;
		game(client,digital);
	}


	closesocket(client);
	WSACleanup();
	system("pause");
	return 0;
}


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
	memcpy(packet.data, buffer, count);
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
		//printf("revcv error!\r\n");
		return -1;
	}
	else if (packet.len > count)
	{
		printf("can't recv too long!\r\n");
		return -1;
	}
	else
	{
		memcpy(buffer, &packet.data, packet.len);
		return packet.len;
	}
}

bool game(SOCKET sock,int digital)
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (martix[i][j] == digital)
			{
				check[i][j] = 1;
				break;
			}
		}
	}
	if (binggo())//检查是否成列，行，斜
	{
		char buffer[] = "binggo";
		int i = sendn(sock,buffer,sizeof(buffer),0);
		printf("binggo\r\n");
		show(check);
		return true;
	}
	else
	{
		char buffer[] = "continue";
		int i = sendn(sock, buffer, sizeof(buffer), 0);
		printf("continue\r\n");
		show(check);
		return false;
	}
}

bool binggo()
{
	if (times < m)
	{
		return false;
	}
	if (digcheck())//对角线检查
	{
		return true;
	}
	else if (colcheck())//列检查
	{
		return true;
	}
	else if (rowcheck())//行检查
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool digcheck()
{
	int sum1 = 0; //左右对角线
	int sum2 = 0;
	for (int i = 0; i < m; i++)
	{
		if (check[i][i] == 1)
		{
			sum1++;
		}
		if (check[i][m-1-i] == 1)
		{
			sum2++;
		}
	}
	if (sum1 == m || sum2 == m)
	{
		return true;
	}
	return false;
}

bool colcheck()
{
	for (int i = 0; i < m; i++)
	{
		int sum = 0;
		for (int j = 0; j < m; j++)
		{
			if (check[i][j] == 1)
			{
				sum++;
			}
			else
			{
				break;
			}
		}
		if (sum == m)
		{
			return true;
		}
	}
	return false;
}

bool rowcheck()
{
	for (int i = 0; i < m; i++)
	{
		int sum = 0;
		for (int j = 0; j < m; j++)
		{
			if (check[j][i] == 1)
			{
				sum++;
			}
			else
			{
				break;
			}
		}
		if (sum == m)
		{
			return true;
		}
	}
	return false;
}

void show(int const (&array)[m][m])
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			printf("%d ", array[i][j]);
		}
		printf("\r\n");
	}

	return;
}