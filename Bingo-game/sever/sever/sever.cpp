// sever.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

int static const m = 5;
int members = 0;
bool stop = false;
int send_dig = -1;   //���ڴ��ݲ����������
int finished[m] = {0};    //��ʾ�Խ��������
int quit[m] = { 0 };     //��ʾ��;�˳�����
bool wait = true; //��ʾ���߳��Ѿ����������ֵĸ���
int winner[m];   //��Ϸ����

std::mutex start;  //������ʼ
std::mutex mutex;  //���;���
std::mutex signal;  //�ȴ�����������ź�

bool bindsock(SOCKADDR_IN &addr, SOCKET &sever);
void gamestart(SOCKET sever);  //��Ϸ��ʼ close socket ����accept
bool Go();                      //�ֶ�����G��������ȴ�����,ֱ�ӿ�ʼ����
void Message(SOCKET sock,int n);         //Ԥ��׼�� ����������
void randmartix(int (&array)[m*m],int n);  //����������� ������->client,n:index �������
void sendmartix(SOCKET sock, int n);   //���;���
void show(int const(&array)[m*m]);              //��ӡ����
void binggo(int const members);                   //��Ϸ����   members:�μ�game����
int rand_digital(int(&martix)[m*m], int n);         //��ʼ�����������   n:�Բ���������
bool findwinner();   //��ѯ�Ƿ���winner
void summary(SOCKET sock,int n);
void AllWinner(SOCKET sock);  //��������winner

struct Packet       //��ֹճ��
{
	int len;
	char data[8193];
};

size_t sendn(SOCKET sock,char *buffer,int count,int flags);        //��װsend��recv���� ��ֹճ�����Զ�memset
size_t recvn(SOCKET sock,char *buffer, int count, int flags);

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsadata;
	WSAStartup(0x0202,&wsadata);
	SOCKET sever = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	SOCKADDR_IN addr;
	int i=bindsock(addr, sever); //��ʼ��sockaddr �� ����
	if (i == false)
	{
		printf("bind error!\r\n");
		system("pause");
		return 0;
	}
	SOCKET sockets[10];  //�������
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
			printf("��%d����Ҽ������\r\n", members);
			int index = members - 1;
			threads[index] = std::thread(Message,sockets[index],members);
			threads[index].detach();
		}
		else
		{
			if (stop == true)
			{
				printf("������Ϸ\r\n");
				break;
			}
			printf("accept fail\r\n");
			break;
		}
	}
	binggo(members);            //��ʼ��Ϸ   �μ��������Ͷ�Ӧ��socket
	printf("game finish\r\n");
	//�����β
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
	for (int i = 0; i < m; i++)  //��ʼ��winner
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
		else if (members == m)    //�ﵽ��������
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
	if (_kbhit())  //���̴�������
	{
		char key = _getch();
		if (members < 2)
		{
			printf("����Ҫ������Ŷ��\r\n");
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

void Message(SOCKET sock, int n)    //��SOCKET�߳�
{
	sendmartix(sock, n);
	int i = 0;
	char buffer[50] = "0";
	//�ȴ��������̵߳�������֣�ֱ���յ�binggo��������
	//---------------------------����������� �ȴ�binggo unlock signal----------------------------------------------
	while (winner[0]<0)   //ֱ������binggo   ���͵�%d��digital��ʾ,����digital���յ����� 
	{
		signal.lock();  //�ȴ����������ź�
		i = sprintf_s(buffer, "number:");
		i = sendn(sock, buffer, i, 0);
		int temp = send_dig;  //��ֹsend_dig��memset
		i = sendn(sock, (char*)&temp, 4, 0);//��������
		if (i < 0)
		{
			printf("fail to send %d �����,�Զ��˳�\r\n", n);
			members--;
			quit[n - 1] = -1;
			signal.unlock();
			return;
		}
		signal.unlock();
		//��Ҫ�ȴ�������ҿ�ʼ�������֣�����������ѭ���ж�finish=0��
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
			while (winner[i]>0)   //��winner����д��
			{
				i++;
			}
			winner[i] = n;
		}
		//-------------һֱѭ��
		wait = true;
		finished[n-1]=1;
		i = 0;
		  //�ȴ���һ��   //������-----
		while (!wait)
		{
			Sleep(100);
		}
	}
	//--------------����-----------------------
	memset(buffer, 0, sizeof(buffer));
	i = sprintf_s(buffer, "game finished!");
	i = sendn(sock, buffer, i, 0);
	//-------------��ӡ------------------------
	summary(sock, n);
	members--;   //����
	return;
}

void sendmartix(SOCKET sock, int n)
{
	char buffer[] = "Just wait for others to join";
	int i = sendn(sock, buffer, sizeof(buffer), 0);
	if (i < 0)
	{
		printf("��%d�����send����\r\n", n);
	}

	i = sprintf_s(buffer, "%d��������!", n);
	i = sendn(sock, buffer, i, 0);

	start.lock();   //׼������game start�ź�
	start.unlock();

	mutex.lock();

	i = sprintf_s(buffer, "start");
	i = sendn(sock, buffer, i, 0);   //���ͱ�����ʼ
	if (i < 0)
	{
		printf("send error\r\n");
		quit[n - 1] = 1;
		mutex.unlock();
		return;
	}

	int martix[m*m] = { 0 };
	randmartix(martix, n);

	i = sendn(sock, (char*)martix, sizeof(martix), 0);  //����binggo����
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
	for (int i = 0; i < m*m; i++)   //��ʼ��
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
	int martix[m*m];   //��ʼ��
	for (int i = 0; i < m*m; i++)
	{
		martix[i] = i;
	}
	int n = m*m;

	//���� �̵߳ȴ� �յ����̵߳�����
	while (n>0)
	{
		send_dig = rand_digital(martix, n);   //����������鲢���ͣ�ֱ�������˷���binggo
		printf("send:%d\r\n", send_dig);
		n--;
		signal.unlock();

		int i = 0;
		while (i<members)  //�ȵ�ȫ���յ����ֺ���������һ������
		{
			if (quit[i])  //��;�˳�
			{
				i++;
			}
			else if (!finished[i])
			{
				Sleep(100);
			}
			else 
			{
				finished[i] = 0;  //����
				i++;
			}
		}
		wait = false;

		signal.lock();
		if (findwinner())  //�Ƿ�����binggo
		{
			//check(); //���м�� 
			signal.unlock();
			break;
		}
		else
		{
			printf("nobody binggo\r\n");
		}
		//����Ƿ���;ȫ���˳��˱���
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
			printf("%d����� ", winner[i]);
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
		if (winner[j] == n)                              //���win ���� You win
		{
			i = sprintf_s(buffer, "You win!");
			i = sendn(sock, buffer, i, 0);
			AllWinner(sock);   //��������winner
			return;
		}
	}
	i = sprintf_s(buffer, "You lose.");             //��Ȼ����You Lose
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
		int i = sprintf_s(temp, " %d�����", winner[j]);
		strcat_s(a, temp);
		len += i;
	}
	len = sendn(sock, a, len, 0);
	return;
}

