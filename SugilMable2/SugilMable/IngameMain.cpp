#include "IngameMain.h"

#define MAX_LOADSTRING 100

// ���� ���� 
extern HINSTANCE hInst;
WCHAR szIngameWindowClass[MAX_LOADSTRING] = L"IngameWindowClass";
HWND hToolTip;
HWND hIngameWnd = nullptr;
const int mapSize = 15;  // �� ũ��
const int squareSize = 70; // �� ĭ�� ũ��
const int gapSize = 5;    // ĭ ���� ����
int currentPlayer = 0;  // ���� ���� �÷��̾�
int currentBank = 0;

// ������ ����
struct Zone {
    int x1, y1, x2, y2; // ������ ��ǥ (x1, y1)���� (x2, y2)����
    LPCWSTR name;        // ���� �̸�
    COLORREF color;      // ���� ��
    int price;           // ���� ����
    int owner;           // ������ (-1: ����, 0: �÷��̾�1, 1: �÷��̾�2)
};

// �÷��̾� ����
struct Player {
    int position;
    int money;
    COLORREF color;
    int PrisonTurn;      // ������
    int loanAmount;      // �����
    int collateral;      // �㺸
    int loanTerm;        // ���� ����
    std::vector<int> ownedProperties; // ������ �ε��� ����� �߰� 
};



struct Bank {
    int bank_money; // ���� �ܰ� 
    int warrant; // �㺸 ���� 
    COLORREF color; //���� �� 
    int bank_owner; // ������ ������ �� 
};

struct BankZone {
    RECT rect;
    LPCWSTR name;
    COLORREF backgroundColor;
    COLORREF borderColor;
};


BankZone sugilBank = {
    {750, 50, 1100, 500},  
    L"���� ����",
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
    {750, 50, 950, 300},  // P1 ����
    L"�÷��̾� 1 ����",
    RGB(240, 240, 240),
    RGB(0, 0, 0)
};

UserInformation player2Info = {
    {960, 50, 1160, 300},  // P2 ����
    L"�÷��̾� 2 ����",
    RGB(240, 240, 240),
    RGB(0, 0, 0)
};

// �÷��̾�
std::vector<Player> players = {
    {0, 10000, RGB(255, 0, 0), 0, 0, 0, 0},  // �÷��̾� 1
    {0, 10000, RGB(0, 0, 255), 0, 0, 0, 0}   // �÷��̾� 2
};
std::vector<Bank>banks = {
    {1000000,0,RGB(0, 255, 0),0}
};


// �� ���� 
std::vector<Zone> zones = {
    // ���� �� (���ʿ��� ����������)
    // ���� �� (�ƽþ� �ֿ� ���õ�)
    {0, 0, squareSize, squareSize, L"���!", RGB(255, 0, 0), 0, -1},
    {squareSize, 0, squareSize * 2, squareSize, L"����", RGB(255, 165, 0), 1200, -1},
    {squareSize * 2, 0, squareSize * 3, squareSize, L"����¡", RGB(255, 255, 0), 1100, -1},
    {squareSize * 3, 0, squareSize * 4, squareSize, L"����", RGB(0, 255, 0), 900, -1},
    {squareSize * 4, 0, squareSize * 5, squareSize, L"�̰�����", RGB(0, 255, 255), 880, -1},
    {squareSize * 5, 0, squareSize * 6, squareSize, L"��ī��Ÿ", RGB(128, 0, 128), 700, -1},
    {squareSize * 6, 0, squareSize * 7, squareSize, L"ȫ��", RGB(0, 128, 128), 950, -1},
    {squareSize * 7, 0, squareSize * 8, squareSize, L"���Ҷ�", RGB(128, 128, 0), 720, -1},
    {squareSize * 8, 0, squareSize * 9, squareSize, L"����", RGB(64, 224, 208), 1000, -1},
    {squareSize * 9, 0, squareSize * 10, squareSize, L"����", RGB(192, 192, 192), 0, -1},

    // ������ �� (���� �ֿ� ���õ�)
    {squareSize * 9, squareSize, squareSize * 10, squareSize * 2, L"����", RGB(255, 128, 0), 1500, -1},
    {squareSize * 9, squareSize * 2, squareSize * 10, squareSize * 3, L"�ĸ�", RGB(128, 0, 0), 1600, -1},
    {squareSize * 9, squareSize * 3, squareSize * 10, squareSize * 4, L"�θ�", RGB(0, 128, 0), 1400, -1},
    {squareSize * 9, squareSize * 4, squareSize * 10, squareSize * 5, L"���帮��", RGB(0, 0, 255), 1350, -1},
    {squareSize * 9, squareSize * 5, squareSize * 10, squareSize * 6, L"��", RGB(255, 0, 255), 1300, -1},
    {squareSize * 9, squareSize * 6, squareSize * 10, squareSize * 7, L"������", RGB(255, 192, 203), 1250, -1},
    {squareSize * 9, squareSize * 7, squareSize * 10, squareSize * 8, L"������", RGB(165, 42, 42), 1450, -1},
    {squareSize * 9, squareSize * 8, squareSize * 10, squareSize * 9, L"�Ͻ��׸���", RGB(255, 215, 0), 1200, -1},
    {squareSize * 9, squareSize * 9, squareSize * 10, squareSize * 10, L"����", RGB(255, 255, 255), 0, -1},

    // �Ʒ��� �� (�ߵ� �ֿ� ���õ�)
    {squareSize * 8, squareSize * 9, squareSize * 9, squareSize * 10, L"�̽�ź��", RGB(219, 112, 147), 1100, -1},
    {squareSize * 7, squareSize * 9, squareSize * 8, squareSize * 10, L"�ι���", RGB(255, 20, 147), 1400, -1},
    {squareSize * 6, squareSize * 9, squareSize * 7, squareSize * 10, L"����", RGB(199, 21, 133), 1000, -1},
    {squareSize * 5, squareSize * 9, squareSize * 6, squareSize * 10, L"���ߵ�", RGB(139, 0, 139), 1050, -1},
    {squareSize * 4, squareSize * 9, squareSize * 5, squareSize * 10, L"�ϸ�", RGB(75, 0, 130), 800, -1},
    {squareSize * 3, squareSize * 9, squareSize * 4, squareSize * 10, L"�����", RGB(72, 61, 139), 900, -1},
    {squareSize * 2, squareSize * 9, squareSize * 3, squareSize * 10, L"�ٱ״ٵ�", RGB(123, 104, 238), 750, -1},
    {squareSize, squareSize * 9, squareSize * 2, squareSize * 10, L"ī�̷�", RGB(138, 43, 226), 700, -1},
    {0, squareSize * 9, squareSize, squareSize * 10, L"�����", RGB(64, 224, 208), 0, -1},

    // ���� �� (�Ʒ����� ����)
    {0, squareSize * 8, squareSize, squareSize * 9, L"���쵥�ڳ��̷�", RGB(60, 179, 113), 850, -1},
    {0, squareSize * 7, squareSize, squareSize * 8, L"�ο��뽺���̷���", RGB(220, 20, 60), 800, -1},
    {0, squareSize * 6, squareSize, squareSize * 7, L"�Ϲٳ�", RGB(128, 0, 0), 750, -1},
    {0, squareSize * 5, squareSize, squareSize * 6, L"�߽��ڽ�Ƽ", RGB(25, 25, 112), 950, -1},
    {0, squareSize * 4, squareSize, squareSize * 5, L"����", RGB(0, 255, 127), 880, -1},
    {0, squareSize * 3, squareSize, squareSize * 4, L"��Ƽ�ư�", RGB(34, 139, 34), 830, -1},
    {0, squareSize * 2, squareSize, squareSize * 3, L"����Ÿ", RGB(218, 165, 32), 700, -1},
    {0, squareSize, squareSize, squareSize * 2, L"ŷ����", RGB(255, 140, 0), 720, -1},
};







// �Լ� ������ 
ATOM MyRegisterIngameClass(HINSTANCE hInstance);
HWND CreateIngameWindow(HWND hParentWnd);
LRESULT CALLBACK IngameWndProc(HWND, UINT, WPARAM, LPARAM);
int rollDice(); //�ֻ��� 
void movePlayer(int playerIndex, int steps);  //�÷��̾� ������ 
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
        //MessageBox(hParentWnd, L"�ΰ��� ������ ������ �����߽��ϴ�.", L"����", MB_OK | MB_ICONERROR);
    }
    return hIngameWnd;
}

// �ֻ��� ������
int rollDice() {
    int result = (rand() % 6) + 1; // 1���� 6������ ���ڸ� ����
    WCHAR message[100];
    swprintf_s(message, L"%d�� ���Խ��ϴ�.", result); 
    MessageBox(NULL, message, L"�ֻ���", MB_OK);
    return result;
}

// ���� ȿ�� ����
void RandomEffect(int playerIndex) {
    std::vector<std::pair<LPCWSTR, std::function<void()>>> effects = {
        {L"���ʽ�($500) ����", []() { players[currentPlayer].money += 1000; }},
        {L"���ʽ�($2000) ����", []() { players[currentPlayer].money += 2000; }},
        {L"���ʽ�($5000) ����", []() { players[currentPlayer].money += 5000; }},
        {L"����($500) ����", []() { players[currentPlayer].money -= 500; }},
        {L"����($800) ����", []() { players[currentPlayer].money -= 800; }},
        {L"����($1200) ����", []() { players[currentPlayer].money -= 1200; }},
        {L"������ �̵�", []() { movePlayer(currentPlayer, 3); }},
        {L"�ڷ� �̵�", []() { movePlayer(currentPlayer, -2); }},
        {L"�������� �̵�", []() {
            for (size_t i = 0; i < zones.size(); ++i) {
                if (zones[i].name == L"����") {
                    players[currentPlayer].position = i;
                    players[currentPlayer].PrisonTurn = 3;
                    break;
                }
            }
        }}
    };

    int randomEffect = rand() % effects.size();
    effects[randomEffect].second();
    MessageBox(NULL, L"���� ȿ���� �ο��մϴ�!", L"�̵��ϱ�? �����ϱ�?", MB_OK);
    WCHAR message[100];
    swprintf_s(message, L"���� ȿ��: %s", effects[randomEffect].first);
    MessageBox(NULL, message, L"Random effect", MB_OK);
}
// �÷��̾� �̵�
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


    // ������� ������ ���� ����
    if (passedStart) {
        players[playerIndex].money += 2000;
        MessageBox(NULL, L"������� �������ϴ�. $2000�� �޾ҽ��ϴ�!", L"����", MB_OK);
    }

    // Ư�� ���� ó��
    if (wcscmp(zones[currentPosition].name, L"����") == 0) {
        players[playerIndex].PrisonTurn = 3;
        MessageBox(NULL, L"���ѿ� �����߽��ϴ�! 3�� ���� ������ �� �����ϴ�.", L"�ƿ���ź��", MB_OK);
    }
    else if (wcscmp(zones[currentPosition].name, L"����") == 0) {
        RandomEffect(playerIndex);
    }
    else if (wcscmp(zones[currentPosition].name, L"�����") == 0) {
        players[playerIndex].position = 0;
        MessageBox(NULL, L"���ϴ� ��ġ�� �̵��ϼ���", L"�����", MB_OK);
    }

    // ���� ��ġ ���� ���
    WCHAR debug[100];
    swprintf_s(debug, L"�÷��̾� %d�� %s�� �����߽��ϴ�.", playerIndex + 1, zones[currentPosition].name);
    MessageBox(NULL, debug, L"�̵� ����", MB_OK);
}


// ���� ����
void buyProperty(int playerIndex, int zoneIndex) {
    if (zones[zoneIndex].owner == -1 && players[playerIndex].money >= zones[zoneIndex].price) {
        zones[zoneIndex].owner = playerIndex;
        players[playerIndex].money -= zones[zoneIndex].price;
        players[playerIndex].ownedProperties.push_back(zoneIndex); // �� ���� �߰��մϴ�
    }
}

// �ε��� ��ġ�� ����ϴ� ���ο� �Լ��� �߰��մϴ�
int calculatePropertyValue(int playerIndex) {
    int totalValue = 0;
    for (int propertyIndex : players[playerIndex].ownedProperties) {
        totalValue += zones[propertyIndex].price;
    }
    return totalValue;
}


// ���� ���� Ȯ��
void checkGameOver(HWND hWnd) {
    for (size_t i = 0; i < players.size(); ++i) {
        int totalAssets = players[i].money + calculatePropertyValue(i);
        if (totalAssets >= 30000) {
            WCHAR message[100];
            swprintf_s(message, L"�÷��̾� %d�� �� �ڻ� $%d�� �¸��߽��ϴ�!", i + 1, totalAssets);
            MessageBox(hWnd, message, L"���� ����", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }

        if (players[i].money < 0) {
            int winner = (i + 1) % players.size();
            WCHAR message[100];
            swprintf_s(message, L"�÷��̾� %d�� �Ļ��߽��ϴ�. �÷��̾� %d �¸�!", i + 1, winner + 1);
            MessageBox(hWnd, message, L"���� ����", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }
    }
}

// ���� �ʱ�ȭ
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
        case 1: // ���� ���� (Player 1)
            executeLoan(0);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 2: // ���� ��ȯ (Player 1)
            repayLoan(0);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 3: // ���� ���� (Player 2)
            executeLoan(1);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 4: // ���� ��ȯ (Player 2)
            repayLoan(1);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;

    case WM_CREATE:
    {   
       
        GetClientRect(hWnd, &WindowRect);
        HWND D_Button1 = CreateWindow(L"BUTTON", L"���� ����", WS_CHILD | WS_VISIBLE, 770, 300, 100, 50, hWnd, (HMENU)1, hInst, NULL);
        HWND H_Button1 = CreateWindow(L"BUTTON", L"���� ��ȯ", WS_CHILD | WS_VISIBLE, 900, 300, 100, 50, hWnd, (HMENU)2, hInst, NULL);
        HWND D_Button2 = CreateWindow(L"BUTTON", L"���� ����", WS_CHILD | WS_VISIBLE, 1130, 300, 100, 50, hWnd, (HMENU)3, hInst, NULL);
        HWND H_Button2 = CreateWindow(L"BUTTON", L"���� ��ȯ", WS_CHILD | WS_VISIBLE, 1260, 300, 100, 50, hWnd, (HMENU)4, hInst, NULL);

        
            
        InitCommonControls();
        // ���� ����
        hToolTip = CreateWindowEx(0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, NULL, hInst, NULL);

        // ���� ��Ÿ�� ����
        SendMessage(hToolTip, TTM_SETMAXTIPWIDTH, 0, 300);

        // ���� �⺻ ����
        ZeroMemory(&ti, sizeof(ti));
        ti.cbSize = sizeof(TOOLINFO);
        ti.uFlags = TTF_SUBCLASS;
        ti.hwnd = hWnd;
        ti.hinst = hInst;
        ti.uId = 0;
        ti.lpszText = LPSTR_TEXTCALLBACK;
        ti.rect.left = ti.rect.top = ti.rect.right = ti.rect.bottom = 0;

        // ���� �߰� ���� 
        SendMessage(hToolTip, TTM_ADDTOOL, 0, (LPARAM)&ti);

        // ���� �����⸦ �ʱ�ȭ
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

        // �� �׸���
        for (size_t i = 0; i < zones.size(); ++i)
        {
            Rectangle(hdc, zones[i].x1, zones[i].y1, zones[i].x2, zones[i].y2);
            SetBkColor(hdc, zones[i].color);
            TextOut(hdc, zones[i].x1 + 5, zones[i].y1 + 5, zones[i].name, lstrlenW(zones[i].name));

            // ������ ǥ��

            if (zones[i].owner != -1) {
                HBRUSH hBrush = CreateSolidBrush(players[zones[i].owner].color);
                RECT ownerRect = { zones[i].x1, zones[i].y2 - 10, zones[i].x2, zones[i].y2 };
                FillRect(hdc, &ownerRect, hBrush);
                DeleteObject(hBrush);
            }

            // ���� ǥ��
            if (zones[i].price > 0) {
                WCHAR priceText[20];
                swprintf_s(priceText, L"$%d", zones[i].price);
                TextOut(hdc, zones[i].x1 + 5, zones[i].y2 - 20, priceText, lstrlenW(priceText));
            }
        }

        // �÷��̾� �׸���
        for (size_t i = 0; i < players.size(); ++i)
        {
            int x = zones[players[i].position].x1 + (i * 20) + 5;
            int y = zones[players[i].position].y1 + (i * 20) + 5;
            HBRUSH hBrush = CreateSolidBrush(players[i].color);
            SelectObject(hdc, hBrush);
            Ellipse(hdc, x, y, x+20, y+20); //�� �� ���� �Ʒ� 
            DeleteObject(hBrush);
        }

        // ���� �÷��̾��� �� ǥ��
        WCHAR turnText[50];
        swprintf_s(turnText, L"�÷��̾� %d'�� ���� �Դϴ�. (Money: $%d)", currentPlayer + 1, players[currentPlayer].money);
        TextOut(hdc, WindowRect.left+100, WindowRect.top+400, turnText, lstrlenW(turnText));


        SelectObject(hdc, hBankBrush);
        SelectObject(hdc, hBankPen);
        Rectangle(hdc, sugilBank.rect.left, sugilBank.rect.top-45, sugilBank.rect.right + 300, sugilBank.rect.bottom-120);
        //4k������ right + 400; 


       
        // ���� �̸�
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));
        HFONT hBankFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"���� ���");
        SelectObject(hdc, hBankFont);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top - 30,
            sugilBank.name, lstrlenW(sugilBank.name));

        // ���� ����
        WCHAR bankText[100];
        swprintf_s(bankText, L"�ȳ��ϼ���? ���� �����Դϴ�. (���� �ܰ�: $%d)", banks[currentBank].bank_money);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top + 10, bankText, lstrlenW(bankText));

        WCHAR loanText1[100];
        WCHAR loanText2[100];
        WCHAR loanText3[100];
        swprintf_s(loanText1, L"�÷��̾� 1��\n�����: $%d",players[0].loanAmount);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top + 80, loanText1, lstrlenW(loanText1));
        swprintf_s(loanText2, L"�÷��̾� 1��\n�㺸: %d", players[0].collateral);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top + 110, loanText2, lstrlenW(loanText2));
        swprintf_s(loanText3, L"�÷��̾� 1��\n���� ����: %d��", players[0].loanTerm);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top + 140, loanText3, lstrlenW(loanText3));

        WCHAR loanText4[100];
        WCHAR loanText5[100];
        WCHAR loanText6[100];
        swprintf_s(loanText4, L"�÷��̾� 2��\n�����: $%d", players[0].loanAmount);
        TextOut(hdc, sugilBank.rect.left + 380, sugilBank.rect.top + 80, loanText4, lstrlenW(loanText4));
        swprintf_s(loanText5, L"�÷��̾� 2��\n�㺸: %d", players[0].collateral);
        TextOut(hdc, sugilBank.rect.left + 380, sugilBank.rect.top + 110, loanText5, lstrlenW(loanText5));
        swprintf_s(loanText6, L"�÷��̾� 2��\n���� ����: %d��", players[0].loanTerm);
        TextOut(hdc, sugilBank.rect.left + 380, sugilBank.rect.top + 140, loanText6, lstrlenW(loanText6));
        //4k������ left + 400

        //�÷��̾� 1 ���� 
        Rectangle(hdc, player1Info.rect.left, player1Info.rect.top+370,
            player1Info.rect.right, player1Info.rect.bottom+370);
        TextOut(hdc, player1Info.rect.left + 10, player1Info.rect.top + 380,
            player1Info.name, lstrlenW(player1Info.name));
        //4k������ top,bottom 400, 410 

        // P1 �� ����
        WCHAR p1Info[100];
        swprintf_s(p1Info, L"���� �ݾ�: $%d", players[0].money);
        TextOut(hdc, player1Info.rect.left + 10, player1Info.rect.top + 440, p1Info, lstrlenW(p1Info));

        swprintf_s(p1Info, L"�� �ڻ�: $%d", players[0].money + calculatePropertyValue(0));
        TextOut(hdc, player1Info.rect.left + 10, player1Info.rect.top + 460, p1Info, lstrlenW(p1Info));

        // P2 ����â �׸���
        Rectangle(hdc, player2Info.rect.left, player2Info.rect.top+370,
            player2Info.rect.right, player2Info.rect.bottom+370);
        TextOut(hdc, player2Info.rect.left + 10, player2Info.rect.top + 380,
            player2Info.name, lstrlenW(player2Info.name));

        // P2 �� ����
        WCHAR p2Info[100];
        swprintf_s(p2Info, L"���� �ݾ�: $%d", players[1].money);
        TextOut(hdc, player2Info.rect.left + 10, player2Info.rect.top + 440, p2Info, lstrlenW(p2Info));

        swprintf_s(p2Info, L"�� �ڻ�: $%d", players[1].money + calculatePropertyValue(1));
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
            swprintf_s(message, L"�����뿪���� %d�� ���ҽ��ϴ�. ", players[currentPlayer].PrisonTurn);
            MessageBox(hWnd, message, L"�ƿ���ź��", MB_OK);
            players[currentPlayer].PrisonTurn--;
        }
        else {
            movePlayer(currentPlayer, dice);

            // ���� ��ġ�� ������ ó��
            int currentZone = players[currentPlayer].position;

            if (wcscmp(zones[currentZone].name, L"�����") == 0)
            {
                MessageBox(NULL, L"���ϴ� ��ġ�� �̵��ϼ���", L"�����", MB_OK);
                players[currentPlayer].position = 0;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (zones[currentZone].owner == -1 && zones[currentZone].price > 0) {
                WCHAR message[100];
                swprintf_s(message, L" %s�� $%d �Դϴ�. ���� �Ͻðڽ��ϱ�?", zones[currentZone].name, zones[currentZone].price);
                if (MessageBox(hWnd, message, L"������?", MB_YESNO) == IDYES) {
                    buyProperty(currentPlayer, currentZone);
                }
            }
            else if (zones[currentZone].owner != -1 && zones[currentZone].owner != currentPlayer) {
                // �ٸ� �÷��̾��� ������ ������ ��� �Ӵ�� ����
                int rent = zones[currentZone].price;
                players[currentPlayer].money -= rent;
                players[zones[currentZone].owner].money += rent;
                WCHAR message[100];
                swprintf_s(message, L"����� $%d�� �����ؾ� �մϴ�.", rent, zones[currentZone].owner + 1);
                MessageBox(hWnd, message, L"����� ���� ��Ҿ��!", MB_OK);
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
                    swprintf_s(tooltipText, L"%s\n����: $%d\n������: %s", zones[i].name, zones[i].price,
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

        // ���� �ۿ� ���� �� ���� �����
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
        SetBkMode(hdc, TRANSPARENT); // �޹�� ����
        SetTextColor(hdc, RGB(0, 0, 0));
        wsprintf(message, L"Player %d���� $%d�� �����̽��ϴ�.", playerIndex + 1, LOAN_AMOUNT);
        TextOut(hdc, 80, 80, message, lstrlenW(message));
        

    }
    else {
        MessageBox(NULL, L"����� �̹� ���� ������ �־��", L"�� ������?", MB_OK);
    }
}

void repayLoan(int playerIndex) {
    if (players[playerIndex].loanAmount > 0) {
        if (players[playerIndex].money >= players[playerIndex].loanAmount) {
            players[playerIndex].money -= players[playerIndex].loanAmount;

            WCHAR message[100];
            swprintf_s(message, L"Player %d���� $%d�� ��ȯ�ϼ̽��ϴ�.", playerIndex + 1, players[playerIndex].loanAmount);
            MessageBox(NULL, message, L"�� ������", MB_OK);

            players[playerIndex].loanAmount = 0;
            players[playerIndex].loanTerm = 0;
        }
        else {
            MessageBox(NULL, L"����", L"ġ������ ����", MB_OK);
        }
    }
    else {
        MessageBox(NULL, L"����� ���� �����", L"���� ������ ���µ�", MB_OK);
    }
}