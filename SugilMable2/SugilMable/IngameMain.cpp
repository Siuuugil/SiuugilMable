#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <functional>


#define MAX_LOADSTRING 100

// 전역 변수
extern HINSTANCE hInst;
WCHAR szIngameWindowClass[MAX_LOADSTRING] = L"IngameWindowClass";
HWND hIngameWnd = nullptr;

const int mapSize = 10;  // 맵 크기
const int squareSize = 70; // 각 칸의 크기
const int gapSize = 5;    // 칸 사이 간격

// 구역의 정보
struct Zone {
    int x1, y1, x2, y2; // 구역의 좌표 (x1, y1)부터 (x2, y2)까지
    LPCWSTR name;        // 구역 이름
    COLORREF color;      // 구역 색
    int price;           // 구입 가격
    int owner;           // 소유자 (-1: 없음, 0: 플레이어1, 1: 플레이어2)
};

// 플레이어 정보
struct Player {
    int position;  // 현재 위치
    int money;     // 보유 금액
    COLORREF color; // 플레이어 색상
    int PrisonTurn; // 감옥 
};

// 맵 구조 
std::vector<Zone> zones = {
    // 위쪽 줄 (왼쪽에서 오른쪽으로)
    {0, 0, squareSize, squareSize, L"Start", RGB(255, 0, 0), 0, -1},
    {squareSize, 0, squareSize * 2, squareSize, L"London", RGB(255, 255, 0), 200, -1},
    {squareSize * 2, 0, squareSize * 3, squareSize, L"Paris", RGB(0, 255, 0), 180, -1},
    {squareSize * 3, 0, squareSize * 4, squareSize, L"New York", RGB(0, 255, 255), 180, -1},
    {squareSize * 4, 0, squareSize * 5, squareSize, L"Tokyo", RGB(255, 0, 255), 160, -1},
    {squareSize * 5, 0, squareSize * 6, squareSize, L"Sydney", RGB(128, 0, 128), 350, -1},
    {squareSize * 6, 0, squareSize * 7, squareSize, L"Berlin", RGB(0, 128, 128), 320, -1},
    {squareSize * 7, 0, squareSize * 8, squareSize, L"New York", RGB(128, 128, 0), 400, -1},
    {squareSize * 8, 0, squareSize * 9, squareSize, L"Rome", RGB(0, 0, 128), 200, -1},
    {squareSize * 9, 0, squareSize * 10, squareSize, L"Prison", RGB(192, 192, 192), 0, -1},

    // 오른쪽 줄 (위에서 아래로)
    {squareSize * 9, squareSize, squareSize * 10, squareSize * 2, L"Berlin", RGB(255, 128, 0), 180, -1},
    {squareSize * 9, squareSize * 2, squareSize * 10, squareSize * 3, L"Moscow", RGB(128, 0, 0), 180, -1},
    {squareSize * 9, squareSize * 3, squareSize * 10, squareSize * 4, L"Geneva", RGB(0, 128, 0), 160, -1},
    {squareSize * 9, squareSize * 4, squareSize * 10, squareSize * 5, L"Athens", RGB(0, 0, 255), 140, -1},
    {squareSize * 9, squareSize * 5, squareSize * 10, squareSize * 6, L"Tax", RGB(255, 0, 0), 200, -1},
    {squareSize * 9, squareSize * 6, squareSize * 10, squareSize * 7, L"Madrid", RGB(255, 165, 0), 180, -1},
    {squareSize * 9, squareSize * 7, squareSize * 10, squareSize * 8, L"Toronto", RGB(255, 192, 203), 200, -1},
    {squareSize * 9, squareSize * 8, squareSize * 10, squareSize * 9, L"Singapore", RGB(165, 42, 42), 300, -1},
    {squareSize * 9, squareSize * 9, squareSize * 10, squareSize * 10, L"Random", RGB(255, 215, 0), 0, -1},

    // 아래쪽 줄 (오른쪽에서 왼쪽으로)
    {squareSize * 8, squareSize * 9, squareSize * 9, squareSize * 10, L"Bruges", RGB(219, 112, 147), 140, -1},
    {squareSize * 7, squareSize * 9, squareSize * 8, squareSize * 10, L"Dublin", RGB(255, 20, 147), 150, -1},
    {squareSize * 6, squareSize * 9, squareSize * 7, squareSize * 10, L"Budapest", RGB(199, 21, 133), 160, -1},
    {squareSize * 5, squareSize * 9, squareSize * 6, squareSize * 10, L"Warsaw", RGB(139, 0, 139), 170, -1},
    {squareSize * 4, squareSize * 9, squareSize * 5, squareSize * 10, L"Prague", RGB(75, 0, 130), 180, -1},
    {squareSize * 3, squareSize * 9, squareSize * 4, squareSize * 10, L"Lisbon", RGB(72, 61, 139), 200, -1},
    {squareSize * 2, squareSize * 9, squareSize * 3, squareSize * 10, L"Bell", RGB(123, 104, 238), 210, -1},
    {squareSize, squareSize * 9, squareSize * 2, squareSize * 10, L"Brussels", RGB(138, 43, 226), 220, -1},
    {0, squareSize * 9, squareSize, squareSize * 10, L"Go! Start", RGB(64, 224, 208), 200, -1},

    // 왼쪽 줄 (아래에서 위로)
    {0, squareSize * 8, squareSize, squareSize * 9, L"Rio de Janeiro", RGB(60, 179, 113), 300, -1},
    {0, squareSize * 7, squareSize, squareSize * 8, L"Melbourne", RGB(220, 20, 60), 210, -1},
    {0, squareSize * 6, squareSize, squareSize * 7, L"Havana", RGB(128, 0, 0), 220, -1},
    {0, squareSize * 5, squareSize, squareSize * 6, L"Zurich", RGB(25, 25, 112), 250, -1},
    {0, squareSize * 4, squareSize, squareSize * 5, L"Dubai", RGB(0, 255, 127), 200, -1},
    {0, squareSize * 3, squareSize, squareSize * 4, L"Bangkok", RGB(34, 139, 34), 180, -1},
    {0, squareSize * 2, squareSize, squareSize * 3, L"Osaka", RGB(218, 165, 32), 120, -1},
    {0, squareSize, squareSize, squareSize * 2, L"Shanghai", RGB(255, 140, 0), 150, -1}
};




// 플레이어
std::vector<Player> players = {
    {0, 10000, RGB(255, 0, 0),0},  // 플레이어 1
    {0, 10000, RGB(0, 0, 255),0}   // 플레이어 2
};

int currentPlayer = 0;  // 현재 턴인 플레이어

// 툴팁 핸들
HWND hToolTip;

// 함수 선언쪽 
ATOM MyRegisterIngameClass(HINSTANCE hInstance);
HWND CreateIngameWindow(HWND hParentWnd);
LRESULT CALLBACK IngameWndProc(HWND, UINT, WPARAM, LPARAM);
int rollDice();
void movePlayer(int playerIndex, int steps);
void buyProperty(int playerIndex, int zoneIndex);
void checkGameOver(HWND hWnd);
void resetGame();
void RandomEffect(int playerIndex);


ATOM MyRegisterIngameClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = IngameWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szIngameWindowClass;
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}


HWND CreateIngameWindow(HWND hParentWnd)
{
    RECT parentRect;
    GetClientRect(hParentWnd, &parentRect);
    int width = parentRect.right - parentRect.left;
    int height = parentRect.bottom - parentRect.top;

    hIngameWnd = CreateWindowW(szIngameWindowClass, L"Ingame Window",
        WS_CHILD | WS_VISIBLE,
        0, 0, width, height,
        hParentWnd,
        nullptr,
        hInst,
        nullptr);

    if (!hIngameWnd)
    {
        MessageBox(hParentWnd, L"윈도우 생성에 실패하였습니다.", L"Error", MB_OK | MB_ICONERROR);
    }

    return hIngameWnd;
}

// 주사위 굴리기
int rollDice() {
    int result = (rand() % 6) + 1; // 1부터 6까지의 숫자를 생성
    WCHAR message[100];
    swprintf_s(message, L"%d이 나왔습니다.", result); 
    MessageBox(NULL, message, L"주사위", MB_OK);
    return result;
}

// 랜덤 효과 적용
void RandomEffect(int playerIndex) {
    std::vector<std::pair<LPCWSTR, std::function<void()>>> effects = {
        {L"보너스($500) 지급", []() { players[currentPlayer].money += 1000; }},
        {L"보너스($2000) 지급", []() { players[currentPlayer].money += 2000; }},
        {L"보너스($5000) 지급", []() { players[currentPlayer].money += 5000; }},
        {L"벌금($500) 지불", []() { players[currentPlayer].money -= 500; }},
        {L"벌금($800) 지불", []() { players[currentPlayer].money -= 800; }},
        {L"벌금($1200) 지불", []() { players[currentPlayer].money -= 1200; }},
        {L"앞으로 이동", []() { movePlayer(currentPlayer, 3); }},
        {L"뒤로 이동", []() { movePlayer(currentPlayer, -2); }},
        {L"감옥으로 이동", []() {
            for (size_t i = 0; i < zones.size(); ++i) {
                if (zones[i].name == L"Prison") {
                    players[currentPlayer].position = i;
                    players[currentPlayer].PrisonTurn = 3;
                    break;
                }
            }
        }}
    };

    int randomEffect = rand() % effects.size();
    effects[randomEffect].second();
    MessageBox(NULL, L"랜덤 효과를 부여합니다!", L"이득일까? 손해일까?", MB_OK);
    WCHAR message[100];
    swprintf_s(message, L"랜덤 효과: %s", effects[randomEffect].first);
    MessageBox(NULL, message, L"Random effect", MB_OK);
}
// 플레이어 이동
void movePlayer(int playerIndex, int steps) {
    if (players[playerIndex].PrisonTurn > 0) {
        players[playerIndex].PrisonTurn--;
        return;
    }

    int totalZones = zones.size();
    int currentPosition = players[playerIndex].position;
    bool passedStart = false;

    for (int i = 0; i < steps; i++) {
        currentPosition++;
        if (currentPosition >= totalZones) {
            currentPosition = 0;
            passedStart = true;
        }
    }

    players[playerIndex].position = currentPosition;

    // 출발점을 지나면 월급 지급
    if (passedStart) {
        players[playerIndex].money += 2000;
        MessageBox(NULL, L"출발점을 지났습니다. $2000을 받았습니다!", L"월급", MB_OK);
    }

    // 특별 구역 처리
    if (wcscmp(zones[currentPosition].name, L"Prison") == 0) {
        players[playerIndex].PrisonTurn = 3;
        MessageBox(NULL, L"감옥에 도착했습니다! 3턴 동안 움직일 수 없습니다.", L"감옥", MB_OK);
    }
    else if (wcscmp(zones[currentPosition].name, L"Random") == 0) {
        RandomEffect(playerIndex);
    }
    else if (wcscmp(zones[currentPosition].name, L"Go! Start") == 0) {
        players[playerIndex].position = 0;
        MessageBox(NULL, L"시작 지점으로 돌아갑니다!", L"Go! Start", MB_OK);
    }

    // 현재 위치 정보 출력 (디버깅용)
    WCHAR debugMessage[100];
    swprintf_s(debugMessage, L"플레이어 %d가 %s에 도착했습니다.", playerIndex + 1, zones[currentPosition].name);
    MessageBox(NULL, debugMessage, L"이동 정보", MB_OK);
}


// 도시 구매
void buyProperty(int playerIndex, int zoneIndex) {
    if (zones[zoneIndex].owner == -1 && players[playerIndex].money >= zones[zoneIndex].price) {
        zones[zoneIndex].owner = playerIndex;
        players[playerIndex].money -= zones[zoneIndex].price;
    }
}

// 게임 종료 확인
void checkGameOver(HWND hWnd) {
    for (size_t i = 0; i < players.size(); ++i) {
        if (players[i].money < 0) {
            int winner = (i + 1) % players.size();
            WCHAR message[100];
            swprintf_s(message, L"플레이어 %d 이겼습니다!", winner + 1);
            MessageBox(hWnd, message, L"Game Over", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }
    }
}

// 게임 초기화
void resetGame() {
    // 플레이어 초기화
    for (auto& player : players) {
        player.position = 0;
        player.money = 10000;
        player.PrisonTurn = 0;
    }

    // 소유한 도시 초기화
    for (auto& zone : zones) {
        zone.owner = -1;
    }

    currentPlayer = 0;
}

LRESULT CALLBACK IngameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static TOOLINFO ti;

    switch (message)
    {
    case WM_CREATE:
    {
        InitCommonControls();
        // 툴팁 생성
        hToolTip = CreateWindowEx(0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, NULL, hInst, NULL);

        // 툴팁 스타일 보냄
        SendMessage(hToolTip, TTM_SETMAXTIPWIDTH, 0, 300);

        // 툴팁 기본 설정
        ZeroMemory(&ti, sizeof(ti));
        ti.cbSize = sizeof(TOOLINFO);
        ti.uFlags = TTF_SUBCLASS;
        ti.hwnd = hWnd;
        ti.hinst = hInst;
        ti.uId = 0;
        ti.lpszText = LPSTR_TEXTCALLBACK;
        ti.rect.left = ti.rect.top = ti.rect.right = ti.rect.bottom = 0;

        // 툴팁 추가 보냄 
        SendMessage(hToolTip, TTM_ADDTOOL, 0, (LPARAM)&ti);

        // 랜덤 생성기를 초기화
        srand(static_cast<unsigned int>(time(nullptr)));
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 맵 그리기
        for (size_t i = 0; i < zones.size(); ++i)
        {
            Rectangle(hdc, zones[i].x1, zones[i].y1, zones[i].x2, zones[i].y2);
            SetBkColor(hdc, zones[i].color);
            TextOut(hdc, zones[i].x1 + 5, zones[i].y1 + 5, zones[i].name, lstrlenW(zones[i].name));

            // 소유자 표시
            if (zones[i].owner != -1) {
                HBRUSH hBrush = CreateSolidBrush(players[zones[i].owner].color);
                RECT ownerRect = { zones[i].x1, zones[i].y2 - 10, zones[i].x2, zones[i].y2 };
                FillRect(hdc, &ownerRect, hBrush);
                DeleteObject(hBrush);
            }

            // 가격 표시
            if (zones[i].price > 0) {
                WCHAR priceText[20];
                swprintf_s(priceText, L"$%d", zones[i].price);
                TextOut(hdc, zones[i].x1 + 5, zones[i].y2 - 20, priceText, lstrlenW(priceText));
            }
        }

        // 플레이어 그리기
        for (size_t i = 0; i < players.size(); ++i)
        {
            int x = zones[players[i].position].x1 + (i * 20) + 5;
            int y = zones[players[i].position].y1 + (i * 20) + 5;
            HBRUSH hBrush = CreateSolidBrush(players[i].color);
            SelectObject(hdc, hBrush);
            Ellipse(hdc, x, y, x + 15, y + 15);
            DeleteObject(hBrush);
        }

        // 현재 플레이어의 턴 표시
        WCHAR turnText[50];
        swprintf_s(turnText, L"플레이어 %d'의 차례 입니다. (Money: $%d)", currentPlayer + 1, players[currentPlayer].money);
        TextOut(hdc, 10, mapSize * squareSize + 10, turnText, lstrlenW(turnText));

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_LBUTTONDOWN:
    {
        int dice = rollDice();

        if (players[currentPlayer].PrisonTurn > 0) {
            WCHAR message[100];
            swprintf_s(message, L"탈옥까지 %d턴 남았습니다. ", players[currentPlayer].PrisonTurn);
            MessageBox(hWnd, message, L"Prison", MB_OK);
            players[currentPlayer].PrisonTurn--;
        }
        else {
            movePlayer(currentPlayer, dice);

            // 현재 위치의 구역을 처리
            int currentZone = players[currentPlayer].position;

            if (wcscmp(zones[currentZone].name, L"Go! Start") == 0)
            {
                MessageBox(NULL, L"시작 지점으로 돌아갑니다!", L"Go! Start", MB_OK);
                players[currentPlayer].position = 0;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (zones[currentZone].owner == -1 && zones[currentZone].price > 0) {
                WCHAR message[100];
                swprintf_s(message, L" %s는 $%d 입니다. 구매 하시겠습니까?", zones[currentZone].name, zones[currentZone].price);
                if (MessageBox(hWnd, message, L"정말로?", MB_YESNO) == IDYES) {
                    buyProperty(currentPlayer, currentZone);
                }
            }
            else if (zones[currentZone].owner != -1 && zones[currentZone].owner != currentPlayer) {
                // 다른 플레이어의 구역에 도착한 경우 임대료 지불
                int rent = zones[currentZone].price;
                players[currentPlayer].money -= rent;
                players[zones[currentZone].owner].money += rent;
                WCHAR message[100];
                swprintf_s(message, L"당신은 $%d를 지불해야 합니다.", rent, zones[currentZone].owner + 1);
                MessageBox(hWnd, message, L"상대의 땅을 밟았어요!", MB_OK);
            }
        }
        currentPlayer = (currentPlayer + 1) % players.size();

        checkGameOver(hWnd);

        InvalidateRect(hWnd, NULL, TRUE);

        break;
    }
  
    

    case WM_MOUSEMOVE:
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        for (size_t i = 0; i < zones.size(); ++i)
        {
            if (xPos >= zones[i].x1 && xPos < zones[i].x2 && yPos >= zones[i].y1 && yPos < zones[i].y2)
            {
                // 툴팁 위치 및 텍스트 업데이트
                WCHAR tooltipText[100];
                if (zones[i].price > 0) {
                    swprintf_s(tooltipText, L"%s\n가격: $%d\n소유자: %s", zones[i].name, zones[i].price,
                        (zones[i].owner == -1) ? L"None" : (zones[i].owner == 0 ? L"Player 1" : L"Player 2"));
                }
                else {
                    wcscpy_s(tooltipText, zones[i].name);
                }
                ti.lpszText = tooltipText;
                ti.rect.left = zones[i].x1;
                ti.rect.top = zones[i].y1;
                ti.rect.right = zones[i].x2;
                ti.rect.bottom = zones[i].y2;

                SendMessage(hToolTip, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
                SendMessage(hToolTip, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);
                return 0;
            }
        }

        // 구역 밖에 있을 때 툴팁 숨기기
        ti.rect.left = ti.rect.top = ti.rect.right = ti.rect.bottom = 0;
        SendMessage(hToolTip, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);
    }
    break;

    case WM_NOTIFY:
    {
        LPNMHDR pnmh = (LPNMHDR)lParam;
        if (pnmh->hwndFrom == hToolTip && pnmh->code == TTN_GETDISPINFO)
        {
            LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;
            lpttt->hinst = hInst;
            lpttt->lpszText = ti.lpszText;
        }
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
