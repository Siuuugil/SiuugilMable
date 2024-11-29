#include "IngameMain.h"

#define MAX_LOADSTRING 100

// 전역 변수 
extern HINSTANCE hInst;
WCHAR szIngameWindowClass[MAX_LOADSTRING] = L"IngameWindowClass";
HWND hToolTip;
HWND hIngameWnd = nullptr;
const int mapSize = 15;  // 맵 크기
const int squareSize = 70; // 각 칸의 크기
const int gapSize = 5;    // 칸 사이 간격
int currentPlayer = 0;  // 현재 턴인 플레이어
int currentBank = 0;

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
    int position;
    int money;
    COLORREF color;
    int PrisonTurn;      // 감옥턴
    int loanAmount;      // 대출금
    int collateral;      // 담보
    int loanTerm;        // 대출 만기
    std::vector<int> ownedProperties; // 소유한 부동산 목록을 추가 
};



struct Bank {
    int bank_money; // 은행 잔고 
    int warrant; // 담보 대출 
    COLORREF color; //구역 색 
    int bank_owner; // 은행이 소유한 땅 
};

struct BankZone {
    RECT rect;
    LPCWSTR name;
    COLORREF backgroundColor;
    COLORREF borderColor;
};


BankZone sugilBank = {
    {750, 50, 1100, 500},  
    L"수길 은행",
    RGB(240, 240, 240),   
    RGB(0, 0, 0)           
};

struct UserInformation {
    RECT rect;
    LPCWSTR name;
    COLORREF backgroundColor;
    COLORREF borderColor;
};

UserInformation player1Info = {
    {750, 50, 950, 300},  // P1 영역
    L"플레이어 1 정보",
    RGB(240, 240, 240),
    RGB(0, 0, 0)
};

UserInformation player2Info = {
    {960, 50, 1160, 300},  // P2 영역
    L"플레이어 2 정보",
    RGB(240, 240, 240),
    RGB(0, 0, 0)
};

// 플레이어
std::vector<Player> players = {
    {0, 10000, RGB(255, 0, 0), 0, 0, 0, 0},  // 플레이어 1
    {0, 10000, RGB(0, 0, 255), 0, 0, 0, 0}   // 플레이어 2
};
std::vector<Bank>banks = {
    {1000000,0,RGB(0, 255, 0),0}
};


// 맵 구조 
std::vector<Zone> zones = {
    // 위쪽 줄 (왼쪽에서 오른쪽으로)
    // 위쪽 줄 (아시아 주요 도시들)
    {0, 0, squareSize, squareSize, L"출발!", RGB(255, 0, 0), 0, -1},
    {squareSize, 0, squareSize * 2, squareSize, L"도쿄", RGB(255, 165, 0), 1200, -1},
    {squareSize * 2, 0, squareSize * 3, squareSize, L"베이징", RGB(255, 255, 0), 1100, -1},
    {squareSize * 3, 0, squareSize * 4, squareSize, L"방콕", RGB(0, 255, 0), 900, -1},
    {squareSize * 4, 0, squareSize * 5, squareSize, L"싱가포르", RGB(0, 255, 255), 880, -1},
    {squareSize * 5, 0, squareSize * 6, squareSize, L"자카르타", RGB(128, 0, 128), 700, -1},
    {squareSize * 6, 0, squareSize * 7, squareSize, L"홍콩", RGB(0, 128, 128), 950, -1},
    {squareSize * 7, 0, squareSize * 8, squareSize, L"마닐라", RGB(128, 128, 0), 720, -1},
    {squareSize * 8, 0, squareSize * 9, squareSize, L"서울", RGB(64, 224, 208), 1000, -1},
    {squareSize * 9, 0, squareSize * 10, squareSize, L"북한", RGB(192, 192, 192), 0, -1},

    // 오른쪽 줄 (유럽 주요 도시들)
    {squareSize * 9, squareSize, squareSize * 10, squareSize * 2, L"런던", RGB(255, 128, 0), 1500, -1},
    {squareSize * 9, squareSize * 2, squareSize * 10, squareSize * 3, L"파리", RGB(128, 0, 0), 1600, -1},
    {squareSize * 9, squareSize * 3, squareSize * 10, squareSize * 4, L"로마", RGB(0, 128, 0), 1400, -1},
    {squareSize * 9, squareSize * 4, squareSize * 10, squareSize * 5, L"마드리드", RGB(0, 0, 255), 1350, -1},
    {squareSize * 9, squareSize * 5, squareSize * 10, squareSize * 6, L"빈", RGB(255, 0, 255), 1300, -1},
    {squareSize * 9, squareSize * 6, squareSize * 10, squareSize * 7, L"프라하", RGB(255, 192, 203), 1250, -1},
    {squareSize * 9, squareSize * 7, squareSize * 10, squareSize * 8, L"베를린", RGB(165, 42, 42), 1450, -1},
    {squareSize * 9, squareSize * 8, squareSize * 10, squareSize * 9, L"암스테르담", RGB(255, 215, 0), 1200, -1},
    {squareSize * 9, squareSize * 9, squareSize * 10, squareSize * 10, L"랜덤", RGB(255, 255, 255), 0, -1},

    // 아래쪽 줄 (중동 주요 도시들)
    {squareSize * 8, squareSize * 9, squareSize * 9, squareSize * 10, L"이스탄불", RGB(219, 112, 147), 1100, -1},
    {squareSize * 7, squareSize * 9, squareSize * 8, squareSize * 10, L"두바이", RGB(255, 20, 147), 1400, -1},
    {squareSize * 6, squareSize * 9, squareSize * 7, squareSize * 10, L"도하", RGB(199, 21, 133), 1000, -1},
    {squareSize * 5, squareSize * 9, squareSize * 6, squareSize * 10, L"리야드", RGB(139, 0, 139), 1050, -1},
    {squareSize * 4, squareSize * 9, squareSize * 5, squareSize * 10, L"암만", RGB(75, 0, 130), 800, -1},
    {squareSize * 3, squareSize * 9, squareSize * 4, squareSize * 10, L"테헤란", RGB(72, 61, 139), 900, -1},
    {squareSize * 2, squareSize * 9, squareSize * 3, squareSize * 10, L"바그다드", RGB(123, 104, 238), 750, -1},
    {squareSize, squareSize * 9, squareSize * 2, squareSize * 10, L"카이로", RGB(138, 43, 226), 700, -1},
    {0, squareSize * 9, squareSize, squareSize * 10, L"비행기", RGB(64, 224, 208), 0, -1},

    // 왼쪽 줄 (아래에서 위로)
    {0, squareSize * 8, squareSize, squareSize * 9, L"리우데자네이루", RGB(60, 179, 113), 850, -1},
    {0, squareSize * 7, squareSize, squareSize * 8, L"부에노스아이레스", RGB(220, 20, 60), 800, -1},
    {0, squareSize * 6, squareSize, squareSize * 7, L"하바나", RGB(128, 0, 0), 750, -1},
    {0, squareSize * 5, squareSize, squareSize * 6, L"멕시코시티", RGB(25, 25, 112), 950, -1},
    {0, squareSize * 4, squareSize, squareSize * 5, L"리마", RGB(0, 255, 127), 880, -1},
    {0, squareSize * 3, squareSize, squareSize * 4, L"산티아고", RGB(34, 139, 34), 830, -1},
    {0, squareSize * 2, squareSize, squareSize * 3, L"보고타", RGB(218, 165, 32), 700, -1},
    {0, squareSize, squareSize, squareSize * 2, L"킹스턴", RGB(255, 140, 0), 720, -1},
};







// 함수 선언쪽 
ATOM MyRegisterIngameClass(HINSTANCE hInstance);
HWND CreateIngameWindow(HWND hParentWnd);
LRESULT CALLBACK IngameWndProc(HWND, UINT, WPARAM, LPARAM);
int rollDice(); //주사위 
void movePlayer(int playerIndex, int steps);  //플레이어 움직임 
void buyProperty(int playerIndex, int zoneIndex); 
void checkGameOver(HWND hWnd);
void resetGame();
void RandomEffect(int playerIndex);
void executeLoan(int playerIndex);
void repayLoan(int playerIndex);



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
        //MessageBox(hParentWnd, L"인게임 윈도우 생성에 실패했습니다.", L"오류", MB_OK | MB_ICONERROR);
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
        {L"북한으로 이동", []() {
            for (size_t i = 0; i < zones.size(); ++i) {
                if (zones[i].name == L"북한") {
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
    if (wcscmp(zones[currentPosition].name, L"북한") == 0) {
        players[playerIndex].PrisonTurn = 3;
        MessageBox(NULL, L"북한에 도착했습니다! 3턴 동안 움직일 수 없습니다.", L"아오지탄광", MB_OK);
    }
    else if (wcscmp(zones[currentPosition].name, L"랜덤") == 0) {
        RandomEffect(playerIndex);
    }
    else if (wcscmp(zones[currentPosition].name, L"비행기") == 0) {
        players[playerIndex].position = 0;
        MessageBox(NULL, L"원하는 위치로 이동하세요", L"비행기", MB_OK);
    }

    // 현재 위치 정보 출력
    WCHAR debug[100];
    swprintf_s(debug, L"플레이어 %d가 %s에 도착했습니다.", playerIndex + 1, zones[currentPosition].name);
    MessageBox(NULL, debug, L"이동 정보", MB_OK);
}


// 도시 구매
void buyProperty(int playerIndex, int zoneIndex) {
    if (zones[zoneIndex].owner == -1 && players[playerIndex].money >= zones[zoneIndex].price) {
        zones[zoneIndex].owner = playerIndex;
        players[playerIndex].money -= zones[zoneIndex].price;
        players[playerIndex].ownedProperties.push_back(zoneIndex); // 이 줄을 추가합니다
    }
}

// 부동산 가치를 계산하는 새로운 함수를 추가합니다
int calculatePropertyValue(int playerIndex) {
    int totalValue = 0;
    for (int propertyIndex : players[playerIndex].ownedProperties) {
        totalValue += zones[propertyIndex].price;
    }
    return totalValue;
}


// 게임 종료 확인
void checkGameOver(HWND hWnd) {
    for (size_t i = 0; i < players.size(); ++i) {
        int totalAssets = players[i].money + calculatePropertyValue(i);
        if (totalAssets >= 30000) {
            WCHAR message[100];
            swprintf_s(message, L"플레이어 %d가 총 자산 $%d로 승리했습니다!", i + 1, totalAssets);
            MessageBox(hWnd, message, L"게임 종료", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }

        if (players[i].money < 0) {
            int winner = (i + 1) % players.size();
            WCHAR message[100];
            swprintf_s(message, L"플레이어 %d가 파산했습니다. 플레이어 %d 승리!", i + 1, winner + 1);
            MessageBox(hWnd, message, L"게임 종료", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }
    }
}

// 게임 초기화
void resetGame() {
  
    for (auto& player : players) {
        player.position = 0;
        player.money = 10000;
        player.PrisonTurn = 0;
        player.loanAmount = 0;
        player.collateral = 0;
        player.loanTerm = 0;
        player.ownedProperties.clear();
    }

   
    for (auto& zone : zones) {
        zone.owner = -1;
    }

    currentPlayer = 0;
}

LRESULT CALLBACK IngameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static TOOLINFO ti;
    RECT WindowRect;
    

    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1: // 대출 실행 (Player 1)
            executeLoan(0);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 2: // 대출 상환 (Player 1)
            repayLoan(0);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 3: // 대출 실행 (Player 2)
            executeLoan(1);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 4: // 대출 상환 (Player 2)
            repayLoan(1);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;

    case WM_CREATE:
    {   
       
        GetClientRect(hWnd, &WindowRect);
        HWND D_Button1 = CreateWindow(L"BUTTON", L"대출 실행", WS_CHILD | WS_VISIBLE, 770, 300, 100, 50, hWnd, (HMENU)1, hInst, NULL);
        HWND H_Button1 = CreateWindow(L"BUTTON", L"대출 상환", WS_CHILD | WS_VISIBLE, 900, 300, 100, 50, hWnd, (HMENU)2, hInst, NULL);
        HWND D_Button2 = CreateWindow(L"BUTTON", L"대출 실행", WS_CHILD | WS_VISIBLE, 1130, 300, 100, 50, hWnd, (HMENU)3, hInst, NULL);
        HWND H_Button2 = CreateWindow(L"BUTTON", L"대출 상환", WS_CHILD | WS_VISIBLE, 1260, 300, 100, 50, hWnd, (HMENU)4, hInst, NULL);

        
            
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
        HBRUSH hBankBrush = CreateSolidBrush(sugilBank.backgroundColor);
        HPEN hBankPen = CreatePen(PS_SOLID, 2, sugilBank.borderColor);


        GetClientRect(hWnd, &WindowRect);

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
            Ellipse(hdc, x, y, x+20, y+20); //왼 위 오른 아래 
            DeleteObject(hBrush);
        }

        // 현재 플레이어의 턴 표시
        WCHAR turnText[50];
        swprintf_s(turnText, L"플레이어 %d'의 차례 입니다. (Money: $%d)", currentPlayer + 1, players[currentPlayer].money);
        TextOut(hdc, WindowRect.left+100, WindowRect.top+400, turnText, lstrlenW(turnText));


        SelectObject(hdc, hBankBrush);
        SelectObject(hdc, hBankPen);
        Rectangle(hdc, sugilBank.rect.left, sugilBank.rect.top-45, sugilBank.rect.right + 300, sugilBank.rect.bottom-120);
        //4k에서는 right + 400; 


       
        // 은행 이름
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));
        HFONT hBankFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"맑은 고딕");
        SelectObject(hdc, hBankFont);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top - 30,
            sugilBank.name, lstrlenW(sugilBank.name));

        // 은행 정보
        WCHAR bankText[100];
        swprintf_s(bankText, L"안녕하세요? 수길 은행입니다. (은행 잔고: $%d)", banks[currentBank].bank_money);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top + 10, bankText, lstrlenW(bankText));

        WCHAR loanText1[100];
        WCHAR loanText2[100];
        WCHAR loanText3[100];
        swprintf_s(loanText1, L"플레이어 1의\n대출금: $%d",players[0].loanAmount);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top + 80, loanText1, lstrlenW(loanText1));
        swprintf_s(loanText2, L"플레이어 1의\n담보: %d", players[0].collateral);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top + 110, loanText2, lstrlenW(loanText2));
        swprintf_s(loanText3, L"플레이어 1의\n대출 만기: %d턴", players[0].loanTerm);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top + 140, loanText3, lstrlenW(loanText3));

        WCHAR loanText4[100];
        WCHAR loanText5[100];
        WCHAR loanText6[100];
        swprintf_s(loanText4, L"플레이어 2의\n대출금: $%d", players[0].loanAmount);
        TextOut(hdc, sugilBank.rect.left + 380, sugilBank.rect.top + 80, loanText4, lstrlenW(loanText4));
        swprintf_s(loanText5, L"플레이어 2의\n담보: %d", players[0].collateral);
        TextOut(hdc, sugilBank.rect.left + 380, sugilBank.rect.top + 110, loanText5, lstrlenW(loanText5));
        swprintf_s(loanText6, L"플레이어 2의\n대출 만기: %d턴", players[0].loanTerm);
        TextOut(hdc, sugilBank.rect.left + 380, sugilBank.rect.top + 140, loanText6, lstrlenW(loanText6));
        //4k에서는 left + 400

        //플레이어 1 정보 
        Rectangle(hdc, player1Info.rect.left, player1Info.rect.top+370,
            player1Info.rect.right, player1Info.rect.bottom+370);
        TextOut(hdc, player1Info.rect.left + 10, player1Info.rect.top + 380,
            player1Info.name, lstrlenW(player1Info.name));
        //4k에서는 top,bottom 400, 410 

        // P1 상세 정보
        WCHAR p1Info[100];
        swprintf_s(p1Info, L"보유 금액: $%d", players[0].money);
        TextOut(hdc, player1Info.rect.left + 10, player1Info.rect.top + 440, p1Info, lstrlenW(p1Info));

        swprintf_s(p1Info, L"총 자산: $%d", players[0].money + calculatePropertyValue(0));
        TextOut(hdc, player1Info.rect.left + 10, player1Info.rect.top + 460, p1Info, lstrlenW(p1Info));

        // P2 정보창 그리기
        Rectangle(hdc, player2Info.rect.left, player2Info.rect.top+370,
            player2Info.rect.right, player2Info.rect.bottom+370);
        TextOut(hdc, player2Info.rect.left + 10, player2Info.rect.top + 380,
            player2Info.name, lstrlenW(player2Info.name));

        // P2 상세 정보
        WCHAR p2Info[100];
        swprintf_s(p2Info, L"보유 금액: $%d", players[1].money);
        TextOut(hdc, player2Info.rect.left + 10, player2Info.rect.top + 440, p2Info, lstrlenW(p2Info));

        swprintf_s(p2Info, L"총 자산: $%d", players[1].money + calculatePropertyValue(1));
        TextOut(hdc, player2Info.rect.left + 10, player2Info.rect.top + 460, p2Info, lstrlenW(p2Info));

        
        DeleteObject(hBankFont);
        DeleteObject(hBankBrush);
        DeleteObject(hBankPen);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_LBUTTONDOWN:
    {
        int dice = rollDice();

        if (players[currentPlayer].PrisonTurn > 0) {
            WCHAR message[100];
            swprintf_s(message, L"강제노역까지 %d턴 남았습니다. ", players[currentPlayer].PrisonTurn);
            MessageBox(hWnd, message, L"아오지탄광", MB_OK);
            players[currentPlayer].PrisonTurn--;
        }
        else {
            movePlayer(currentPlayer, dice);

            // 현재 위치의 구역을 처리
            int currentZone = players[currentPlayer].position;

            if (wcscmp(zones[currentZone].name, L"비행기") == 0)
            {
                MessageBox(NULL, L"원하는 위치로 이동하세요", L"비행기", MB_OK);
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
void executeLoan(int playerIndex) {
    const int LOAN_AMOUNT = 3000;
    const int LOAN_TERM = 5;
    HDC hdc = GetDC(hIngameWnd);
    if (players[playerIndex].loanAmount == 0) {
        players[playerIndex].money += LOAN_AMOUNT;
        players[playerIndex].loanAmount = LOAN_AMOUNT;
        players[playerIndex].loanTerm = LOAN_TERM;

        wchar_t message[100];
        SetBkMode(hdc, TRANSPARENT); // 뒷배경 투명
        SetTextColor(hdc, RGB(0, 0, 0));
        wsprintf(message, L"Player %d님은 $%d을 빌리셨습니다.", playerIndex + 1, LOAN_AMOUNT);
        TextOut(hdc, 80, 80, message, lstrlenW(message));
        

    }
    else {
        MessageBox(NULL, L"당신은 이미 빚을 가지고 있어요", L"또 빌리게?", MB_OK);
    }
}

void repayLoan(int playerIndex) {
    if (players[playerIndex].loanAmount > 0) {
        if (players[playerIndex].money >= players[playerIndex].loanAmount) {
            players[playerIndex].money -= players[playerIndex].loanAmount;

            WCHAR message[100];
            swprintf_s(message, L"Player %d님은 $%d을 상환하셨습니다.", playerIndex + 1, players[playerIndex].loanAmount);
            MessageBox(NULL, message, L"넌 자유야", MB_OK);

            players[playerIndex].loanAmount = 0;
            players[playerIndex].loanTerm = 0;
        }
        else {
            MessageBox(NULL, L"오류", L"치명적인 오류", MB_OK);
        }
    }
    else {
        MessageBox(NULL, L"당신은 빚이 없어요", L"갚을 대출이 없는데", MB_OK);
    }
}