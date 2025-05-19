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
    int elec;
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
        printf("경고: 탄소 배출량이 1000kg을 초과했습니다!\n");
    }
    else if (data->total > 500)
    {
        printf("주의: 탄소 배출량이 500kg을 초과했습니다!\n");
    }
}

void Total_carbon_emissions(userdat* data)
{
    double car_emission = 0.2 * data->car;
    double elec_emission = 0.424 * data->elec;
    double disposable_emission = 0.1 * data->disposable;

    data->total = car_emission + elec_emission + disposable_emission;

    printf("자동차: %d km x 0.2 = %.2f kg\n", data->car, car_emission);
    printf("전기: %d kWh x 0.424 = %.2f kg\n", data->elec, elec_emission);
    printf("일회용품: %d 개 x 0.1 = %.2f kg\n", data->disposable, disposable_emission);
    printf("\n");
    printf("총 배출량 %.2f + %.2f + %.2f = %.2f kg CO2\n", car_emission, elec_emission, disposable_emission, data->total);
}

void today_emmission(userdat* data, const char* id, SOCKET sock)
{
    printf("----오늘의 탄소 발자국 입력----\n");
    printf("오늘 자동차 이동거리(km): ");
    scanf_s("%d", &(data->car));
    printf("오늘 전기 사용량(kWh): ");
    scanf_s("%d", &(data->elec));
    printf("오늘 일회용품 사용 개수: ");
    scanf_s("%d", &(data->disposable));
    printf("\n");
    printf("입력 완료!\n");
    Total_carbon_emissions(data);
    printf("\n");
    carbon_emissions_warning(data);
	send(sock, "a", sizeof(char), 0);
	send(sock, (char*)data, sizeof(userdat), 0);
	
}

void cumulative_statistics(const char* id, SOCKET sock)
{
    int cnt = 0;
	send(sock, "i", sizeof(char), 0);
    recv(sock, (char*)&cnt, sizeof(int), 0);
	if (cnt == 0) {
		printf("데이터가 없습니다.\n");
		return;
	}
	userdat* data = (userdat*)malloc(sizeof(userdat) * cnt);
	if (data == NULL)
	{
		printf("메모리 할당 실패\n");
		return;
	}
    recv(sock, (char*)data, sizeof(userdat)*cnt, 0);
  
    if (data == NULL) 
    {
        printf("데이터를 불러올 수 없습니다.\n");
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

    printf("지금까지 총 탄소 배출량: %.1f kg CO2\n", sum);
    printf("일평균 탄소 배출량: %.1f kg CO2\n", sum / cnt);
    printf("지금까지의 하루 최대 배출량: %.1f kg CO2 %d년 %d월 %d일\n", data[maxi].total, data[maxi].today.tm_year, data[maxi].today.tm_mon, data[maxi].today.tm_mday);
    printf("지금까지의 하루 최소 배출량: %.1f kg CO2 %d년 %d월 %d일\n", data[mini].total, data[mini].today.tm_year, data[mini].today.tm_mon, data[mini].today.tm_mday);

    free(data);
}

void menu(userdat* data, const char* id, SOCKET sock)
{
    while (1)
    {
        system("cls");
        int op;
        printf("=======================================\n");
        printf("* 탄소 발자국 추적 프로그램 *\n");
        printf("=======================================\n");
        printf("1) 데이터 입력\n");
        printf("2) 요약 보기\n");
        printf("3) 종료\n");
        printf("=======================================\n");
        printf("선택할 메뉴 번호를 입력하세요 -> ");
        scanf_s("%d", &op);
        printf("\n");
        if (op == 1)
        {
            today_emmission(data, id, sock);
            printf("\n");
            printf("엔터 키를 누르면 메인 메뉴로 돌아갑니다...\n");

            while (getchar() != '\n'); // 입력 버퍼 비우기

            if (getchar() == '\n')
            {
                continue;
            }

        }
        else if (op == 2)
        {
            cumulative_statistics(id, sock);
            printf("\n");
            printf("엔터 키를 누르면 메인 메뉴로 돌아갑니다...\n");

            while (getchar() != '\n'); // 입력 버퍼 비우기 

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
    printf("아이디: ");
    scanf("%10s", user->id);
    printf("비밀번호: ");
    scanf("%20s", user->password);
}

void signin(userinf* user)
{
   
    printf("새아이디: ");
    scanf("%10s", user->id);
    printf("새비밀번호: ");
    scanf("%20s", user->password);
    
}

void first_window(userdat* data, userinf* user, SOCKET sock)
{
    char sf;
	int cnt = 0;
    while (1)
    {
        system("cls");
        int op;
        printf("1) 로그인\n");
        printf("2) 회원가입\n");
        printf("3) 종료\n");
        printf("->");
        scanf("%d", &op);
        printf("\n");
        if (op == 1)
        {

            login(user);

			send(sock, "l", sizeof(char), 0);
            send(sock, (char*)user, sizeof(userinf), 0);
			recv(sock, &sf, sizeof(char), 0);
            if (sf == 's')
            {
                printf("로그인 성공\n");
				//recv(sock, (char*)cnt, sizeof(int), 0);
				//recv(sock, (char*)data, sizeof(userdat)*cnt, 0);
                menu(data, user->id, sock);
                break;
            }
            else if (sf == 'f')
            {
                printf("로그인 실패\n");
                printf("\n");
                printf("엔터 키를 누르면 메인 메뉴로 돌아갑니다...\n");

                while (getchar() != '\n'); // 입력 버퍼 비우기

                if (getchar() == '\n')
                {
                    continue;
                }
            }
        }
        else if (op == 2)
        {
            signin(user);
            send(sock, "s", sizeof(char), 0);
            send(sock, (char*)user, sizeof(userinf), 0);
            printf("\n");
            printf("엔터 키를 누르면 메인 메뉴로 돌아갑니다...\n");

            while (getchar() != '\n'); // 입력 버퍼 비우기

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