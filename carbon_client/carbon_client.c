#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h>  
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

typedef struct {
    char id[10];
    char password[20];
} userinf;

typedef struct {
    int car;
    int bus;
    int subway;
    int elec;
    int egg;
    int disposable;
    double total;
    struct tm today;
} userdat;

void first_window(userdat* data, userinf* user, SOCKET sock);

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
	userinf user;
	userdat data;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(25565);

    connect(sock, (struct sockaddr*)&server, sizeof(server));

	first_window(&data, &user, sock);

    closesocket(sock);
    WSACleanup();
    return 0;
}


void carbon_emissions_warning(userdat* data)
{

    if (data->total > 1000)
    {
        printf("���: ź�� ���ⷮ�� 1000kg�� �ʰ��߽��ϴ�!\n");
        printf("\n");
    }
    else if (data->total > 500)
    {
        printf("����: ź�� ���ⷮ�� 500kg�� �ʰ��߽��ϴ�!\n");
        printf("\n");
    }
}

void Total_carbon_emissions(userdat* data)
{
    double car_emission = 0.2 * data->car;
    double elec_emission = 0.424 * data->elec;
    double disposable_emission = 0.1 * data->disposable;
    double egg_emission = 0.3 * data->egg;
    double bus_emission = 0.08 * data->bus;
    double subway_emission = 0.04 * data->subway;

    data->total = car_emission + elec_emission + disposable_emission + egg_emission + bus_emission + subway_emission;

    printf("�ڵ���: %d km x 0.2 = %.2f kg\n", data->car, car_emission);
    printf("����: %d km x 0.08 = %.2f kg\n", data->bus, bus_emission);
    printf("����ö: %d km x 0.04 = %.2f kg\n", data->subway, subway_emission);
    printf("����: %d kWh x 0.424 = %.2f kg\n", data->elec, elec_emission);
    printf("��ȸ��ǰ: %d �� x 0.1 = %.2f kg\n", data->disposable, disposable_emission);
    printf("���: %d �� x 0.3 = %.2f kg\n", data->egg, egg_emission);
    printf("\n");
    printf("������ �� ź�� ���ⷮ: %.2f kg CO2\n", data->total);
}

void today_emmission(userdat* data, const char* id, SOCKET sock)
{
    int temp;
    system("cls");
    printf("----������ ź�� ���ڱ� �Է�----\n");

    do {
        printf("���� �ڵ��� �̵��Ÿ�(km): ");
        if (scanf_s("%d", &temp) != 1) {
            while (getchar() != '\n');
            printf("�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���.\n");
            continue;
        }
        if (temp < 0) {
            printf("�̵��Ÿ��� 0 �̻��̾�� �մϴ�.\n");
        }
    } while (temp < 0);
    data->car = temp;

    do {
        printf("���� ���� �̵��Ÿ�(km): ");
        if (scanf_s("%d", &temp) != 1) {
            while (getchar() != '\n');
            printf("�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���.\n");
            continue;
        }
        if (temp < 0) {
            printf("�̵��Ÿ��� 0 �̻��̾�� �մϴ�.\n");
        }
    } while (temp < 0);
    data->bus = temp;

    do {
        printf("���� ����ö �̵��Ÿ�(km): ");
        if (scanf_s("%d", &temp) != 1) {
            while (getchar() != '\n');
            printf("�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���.\n");
            continue;
        }
        if (temp < 0) {
            printf("�̵��Ÿ��� 0 �̻��̾�� �մϴ�.\n");
        }
    } while (temp < 0);
    data->subway = temp;

    do {
        printf("���� ���� ��뷮(kWh): ");
        if (scanf_s("%d", &temp) != 1) {
            while (getchar() != '\n');
            printf("�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���.\n");
            continue;
        }
        if (temp < 0) {
            printf("��뷮�� 0 �̻��̾�� �մϴ�.\n");
        }
    } while (temp < 0);
    data->elec = temp;

    do {
        printf("���� ��ȸ��ǰ ��� ����: ");
        if (scanf_s("%d", &temp) != 1) {
            while (getchar() != '\n');
            printf("�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���.\n");
            continue;
        }
        if (temp < 0) {
            printf("������ 0 �̻��̾�� �մϴ�.\n");
        }
    } while (temp < 0);
    data->disposable = temp;

    do {
        printf("���� ��� �Һ� ����: ");
        if (scanf_s("%d", &temp) != 1) {
            while (getchar() != '\n');
            printf("�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���.\n");
            continue;
        }
        if (temp < 0) {
            printf("������ 0 �̻��̾�� �մϴ�.\n");
        }
    } while (temp < 0);
    data->egg = temp;

    printf("\n");
    printf("�Է� �Ϸ�!\n");
    Total_carbon_emissions(data);
    carbon_emissions_warning(data);
	send(sock, "a", sizeof(char), 0);
	send(sock, (char*)data, sizeof(userdat), 0);
	
}

void cumulative_statistics(const char* id, SOCKET sock)
{
    system("cls");
    int cnt = 0;
	send(sock, "i", sizeof(char), 0);
    recv(sock, (char*)&cnt, sizeof(int), 0);
	if (cnt == 0) {
		printf("�����Ͱ� �����ϴ�.\n");
		return;
	}
	userdat* data = (userdat*)malloc(sizeof(userdat) * cnt);
	if (data == NULL)
	{
		printf("�޸� �Ҵ� ����\n");
		return;
	}
    recv(sock, (char*)data, sizeof(userdat)*cnt, 0);
  
    if (data == NULL) 
    {
        printf("�����͸� �ҷ��� �� �����ϴ�.\n");
        return;
    }
    double sum = 0;
    int maxi = 0, mini = 0;
    for (int i = 0; i < cnt; i++)
    {
        if (data[i].total > data[maxi].total)
        {
            maxi = i;
        }
        if (data[i].total < data[mini].total)
        {
            mini = i;
        }
        sum += data[i].total;
    }

    printf("���ݱ��� �� ź�� ���ⷮ: %.1f kg CO2\n", sum);
    printf("����� ź�� ���ⷮ: %.1f kg CO2\n", sum / cnt);
    printf("���ݱ����� �Ϸ� �ִ� ���ⷮ: %.1f kg CO2 %d�� %d�� %d��\n", data[maxi].total, data[maxi].today.tm_year, data[maxi].today.tm_mon, data[maxi].today.tm_mday);
    printf("���ݱ����� �Ϸ� �ּ� ���ⷮ: %.1f kg CO2 %d�� %d�� %d��\n", data[mini].total, data[mini].today.tm_year, data[mini].today.tm_mon, data[mini].today.tm_mday);

    free(data);
}

void menu(userdat* data, const char* id, SOCKET sock)
{
    while (1)
    {
        system("cls");
        char op;
        printf("=======================================\n");
        printf("* ź�� ���ڱ� ���� ���α׷� *\n");
        printf("=======================================\n");
        printf("1) ������ �Է�\n");
        printf("2) ��� ����\n");
        printf("3) ����\n");
        printf("=======================================\n");
        printf("������ �޴� ��ȣ�� �Է��ϼ���");
        op = _getch();
        printf("\n");
        if (op == '1')
        {
            today_emmission(data, id, sock);
            printf("\n");
            printf("���� Ű�� ������ ���� �޴��� ���ư��ϴ�...\n");

            while (getchar() != '\n'); // �Է� ���� ����

            if (getchar() == '\n')
            {
                continue;
            }

        }
        else if (op == '2')
        {
            cumulative_statistics(id, sock);
            printf("\n");
            printf("���� Ű�� ������ ���� �޴��� ���ư��ϴ�...\n");

            //while (getchar() != '\n'); // �Է� ���� ���� 

            if (getchar() == '\n')
            {
                continue;
            }
        }
        else
        {
            break;
        }

    }
}


void login(userinf* user)
{
    printf("���̵�: ");
    scanf("%10s", user->id);
    printf("��й�ȣ: ");
    scanf("%20s", user->password);
}

void signin(userinf* user)
{
   
    printf("�����̵�: ");
    scanf("%10s", user->id);
    printf("����й�ȣ: ");
    scanf("%20s", user->password);
    
}

void first_window(userdat* data, userinf* user, SOCKET sock)
{
    char sf;
	int cnt = 0;
    while (1)
    {
        system("cls");
        char op;
        printf("1) �α���\n");
        printf("2) ȸ������\n");
        printf("3) ����\n");
   
        op = _getch();
        printf("\n");
        if (op == '1')
        {

            login(user);

			send(sock, "l", sizeof(char), 0);
            send(sock, (char*)user, sizeof(userinf), 0);
			recv(sock, &sf, sizeof(char), 0);
            if (sf == 's')
            {
                printf("�α��� ����\n");
				//recv(sock, (char*)cnt, sizeof(int), 0);
				//recv(sock, (char*)data, sizeof(userdat)*cnt, 0);
                while (getchar() != '\n');
                menu(data, user->id, sock);
                break;
            }
            else if (sf == 'f')
            {
				continue;
                /*
                printf("�α��� ����\n");
                printf("\n");
                printf("���� Ű�� ������ ���� �޴��� ���ư��ϴ�...\n");

                while (getchar() != '\n'); // �Է� ���� ����

                if (getchar() == '\n')
                {
                    continue;
                }
                */
            }
        }
        else if (op == '2')
        {
            signin(user);
            send(sock, "s", sizeof(char), 0);
            send(sock, (char*)user, sizeof(userinf), 0);
            printf("\n");
            printf("���� Ű�� ������ ���� �޴��� ���ư��ϴ�...\n");

            while (getchar() != '\n'); // �Է� ���� ����

            if (getchar() == '\n')
            {
                continue;
            }

        }
        else
        {
            break;
        }
    }
}