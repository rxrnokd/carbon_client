#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h>  
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 25565

typedef struct {
    char id[10];
    char password[20];
} userinf;

typedef struct {
    int car;
    int gas;
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
    server.sin_port = htons(PORT);

    connect(sock, (struct sockaddr*)&server, sizeof(server));

	first_window(&data, &user, sock);

    closesocket(sock);
    WSACleanup();
    return 0;
}


void carbon_emissions_warning(userdat* data)
{
    if (data->car > 20)
    {
        printf("경고: 자동차 이동거리가 20km를 초과했습니다! 대중교통을 이용해주세요\n");
    }
    if (data->gas > 10)
    {
        printf("경고: 가스 사용량이 10m^3을 초과했습니다! 보일러를 절약해주세요\n");
    }

    if (data->elec > 30)
    {
        printf("경고: 전기 사용량이 30kWh을 초과했습니다! 불필요한 전기 차단 해주세요\n");
    }

    if (data->disposable > 5)
    {
        printf("경고: 일회용품 사용 개수가 5개를 초과했습니다! 텀블러, 장바구니 들고 다녀 주세요\n");
    }
    if (data->total > 500)
    {
        printf("경고: 탄소 배출량이 500kg을 초과했습니다!\n");
        printf("\n");
    }
    else if (data->total > 250)
    {
        printf("주의: 탄소 배출량이 250kg을 초과했습니다!\n");
        printf("\n");
    }
}

void Total_carbon_emissions(userdat* data)
{
    double car_emission = 0.2 * data->car;
    double elec_emission = 0.424 * data->elec;
    double disposable_emission = 0.1 * data->disposable;
    double egg_emission = 0.3 * data->egg;
    double gas_emission = 0.08 * data->gas;
 

    data->total = car_emission + elec_emission + disposable_emission + egg_emission + gas_emission;

    printf("자동차: %d km x 0.2 = %.2f kg\n", data->car, car_emission);
    printf("가스: %d m^3 x 0.08 = %.2f kg\n", data->gas, gas_emission);
    printf("전기: %d kWh x 0.424 = %.2f kg\n", data->elec, elec_emission);
    printf("일회용품: %d 개 x 0.1 = %.2f kg\n", data->disposable, disposable_emission);
    printf("계란: %d 개 x 0.3 = %.2f kg\n", data->egg, egg_emission);
    printf("\n");
    printf("오늘의 총 탄소 배출량: %.2f kg CO2\n", data->total);
}

void today_emmission(userdat* data, const char* id, SOCKET sock)
{
    int temp;
    system("cls");
    printf("----오늘의 탄소 발자국 입력----\n");

    do {
        printf("오늘 자동차 이동거리(km): ");
        if (scanf_s("%d", &temp) != 1) {
            while (getchar() != '\n');
            printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
            continue;
        }
        if (temp < 0) {
            printf("이동거리는 0 이상이어야 합니다.\n");
        }
    } while (temp < 0);
    data->car = temp;

    do {
        printf("오늘 가스 사용량(m^3): ");
        if (scanf_s("%d", &temp) != 1) {
            while (getchar() != '\n');
            printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
            continue;
        }
        if (temp < 0) {
            printf("사용량은 0 이상이어야 합니다.\n");
        }
    } while (temp < 0);
    data->gas = temp;

    do {
        printf("오늘 전기 사용량(kWh): ");
        if (scanf_s("%d", &temp) != 1) {
            while (getchar() != '\n');
            printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
            continue;
        }
        if (temp < 0) {
            printf("사용량은 0 이상이어야 합니다.\n");
        }
    } while (temp < 0);
    data->elec = temp;

    do {
        printf("오늘 일회용품 사용 개수: ");
        if (scanf_s("%d", &temp) != 1) {
            while (getchar() != '\n');
            printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
            continue;
        }
        if (temp < 0) {
            printf("개수는 0 이상이어야 합니다.\n");
        }
    } while (temp < 0);
    data->disposable = temp;

    do {
        printf("오늘 계란 소비 개수: ");
        if (scanf_s("%d", &temp) != 1) {
            while (getchar() != '\n');
            printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
            continue;
        }
        if (temp < 0) {
            printf("개수는 0 이상이어야 합니다.\n");
        }
    } while (temp < 0);
    data->egg = temp;

    printf("\n");
    printf("입력 완료!\n");
    Total_carbon_emissions(data);
    carbon_emissions_warning(data);
	send(sock, "a", sizeof(char), 0);
	send(sock, (char*)data, sizeof(userdat), 0);
	
}

void draw_bar(int value, int max_value, int width)
{
    int bar_length = (int)((double)value / max_value * width);
    for (int i = 0; i < bar_length; i++) {
        printf("#");
    }
    for (int i = bar_length; i < width; i++) {
        printf(" ");
    }
}

char* get_day(int day)
{
    switch (day) {
    case 0: return "일요일";
    case 1: return "월요일";
    case 2: return "화요일";
    case 3: return "수요일";
    case 4: return "목요일";
    case 5: return "금요일";
    case 6: return "토요일";
    default: return "알 수 없음";
    }
}

void graph_by_weekday(userdat* data, int cnt)
{
    double weekday_total[7] = { 0 };
    double max_total = 0;

    for (int i = 0; i < cnt; i++) {
        int wday = data[i].today.tm_wday; 
        weekday_total[wday] = data[i].total;
        if (weekday_total[wday] > max_total)
            max_total = weekday_total[wday];
    }

    printf("\n[요일별 탄소 배출량 그래프]\n");
    for (int i = 0; i < 7; i++) {
        printf("%s: ", get_day(i));
        draw_bar((int)weekday_total[i], (int)max_total == 0 ? 1 : (int)max_total, 50);
        printf(" %.1f kg CO2 \n", weekday_total[i]);
    }
}

void cumulative_statistics(const char* id, SOCKET sock)
{
    system("cls");
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

    graph_by_weekday(data, cnt);

    free(data);
}

void menu(userdat* data, const char* id, SOCKET sock)
{
    while (1)
    {
        system("cls");
        char op;
        printf("=======================================\n");
        printf("* 탄소 발자국 추적 프로그램 *\n");
        printf("=======================================\n");
        printf("1) 데이터 입력\n");
        printf("2) 요약 보기\n");
        printf("3) 종료\n");
        printf("=======================================\n");
        printf("선택할 메뉴 번호를 입력하세요");
        op = _getch();
        printf("\n");
        if (op == '1')
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
        else if (op == '2')
        {
            cumulative_statistics(id, sock);
            printf("\n");
            printf("엔터 키를 누르면 메인 메뉴로 돌아갑니다...\n");

            //while (getchar() != '\n'); // 입력 버퍼 비우기 

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
        char op;
        printf("1) 로그인\n");
        printf("2) 회원가입\n");
        printf("3) 종료\n");
   
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
                printf("로그인 성공\n");
				//recv(sock, (char*)cnt, sizeof(int), 0);
				//recv(sock, (char*)data, sizeof(userdat)*cnt, 0);
                while (getchar() != '\n');
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
        else if (op == '2')
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