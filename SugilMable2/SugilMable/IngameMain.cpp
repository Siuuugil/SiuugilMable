#include "IngameMain.h"


// ���� ���� ����
WCHAR szIngameWindowClass[MAX_LOADSTRING] = L"IngameWindowClass";
HWND hToolTip;
HWND hIngameWnd = nullptr;
WCHAR messageBuffer[256] = L"";
WCHAR messageBuffer2[256] = L"";
const int mapSize = 20;
const int squareSize = 70;
const int gapSize = 5;
int currentPlayer = 0;
int currentBank = 0;

//���� ���� 
BankZone sugilBank = {
    {750, 50, 1100, 500},
    L"���� ����",
    RGB(240, 240, 240),
    RGB(0, 0, 0)
};
//����1 ���� 
UserInformation player1Info = {
    {750, 50, 950, 300},
    L"�÷��̾� 1 ����",
    RGB(240, 240, 240),
    RGB(0, 0, 0)
};

//����2 ���� 
UserInformation player2Info = {
    {960, 50, 1160, 300},
    L"�÷��̾� 2 ����",
    RGB(240, 240, 240),
    RGB(0, 0, 0)
};
//���� 1, 2 �ʱⰪ
std::vector<Player> players = {
    {0, 10000, RGB(255, 0, 0), 0, 0, 0, false},
    {0, 10000, RGB(0, 0, 255), 0, 0, 0, false}
};
// ���� �ʱⰪ
std::vector<Bank> banks = {
    {1000000, 0, RGB(0, 255, 0), 0}
};
// �� 
std::vector<Zone> zones = {
    // ���� �� (���ʿ��� ����������) - ���� �Ķ���
    {0, 0, squareSize, squareSize, L"���!", RGB(255, 255, 255), 0, -1},
    {squareSize, 0, squareSize * 2, squareSize, L"����", RGB(173, 206, 240), 1200, -1},
    {squareSize * 2, 0, squareSize * 3, squareSize, L"����¡", RGB(173, 206, 240), 1100, -1},
    {squareSize * 3, 0, squareSize * 4, squareSize, L"����", RGB(173, 206, 240), 900, -1},
    {squareSize * 4, 0, squareSize * 5, squareSize, L"�̰�����", RGB(173, 206, 240), 880, -1},
    {squareSize * 5, 0, squareSize * 6, squareSize, L"��ī��Ÿ", RGB(173, 206, 240), 700, -1},
    {squareSize * 6, 0, squareSize * 7, squareSize, L"ȫ��", RGB(173, 206, 240), 950, -1},
    {squareSize * 7, 0, squareSize * 8, squareSize, L"���Ҷ�", RGB(173, 206, 240), 720, -1},
    {squareSize * 8, 0, squareSize * 9, squareSize, L"����", RGB(173, 206, 240), 1000, -1},
    {squareSize * 9, 0, squareSize * 10, squareSize, L"����", RGB(255, 255, 255), 0, -1},

    // ������ �� (������ �Ʒ���) - ���� ���
    {squareSize * 9, squareSize, squareSize * 10, squareSize * 2, L"����", RGB(160, 235, 144), 1500, -1},
    {squareSize * 9, squareSize * 2, squareSize * 10, squareSize * 3, L"�ĸ�", RGB(160, 235, 144), 1600, -1},
    {squareSize * 9, squareSize * 3, squareSize * 10, squareSize * 4, L"�θ�", RGB(160, 235, 144), 1400, -1},
    {squareSize * 9, squareSize * 4, squareSize * 10, squareSize * 5, L"���帮��", RGB(160, 235, 144), 1350, -1},
    {squareSize * 9, squareSize * 5, squareSize * 10, squareSize * 6, L"��", RGB(160, 235, 144), 1300, -1},
    {squareSize * 9, squareSize * 6, squareSize * 10, squareSize * 7, L"������", RGB(160, 235, 144), 1250, -1},
    {squareSize * 9, squareSize * 7, squareSize * 10, squareSize * 8, L"������", RGB(160, 235, 144), 1450, -1},
    {squareSize * 9, squareSize * 8, squareSize * 10, squareSize * 9, L"�Ͻ��׸���", RGB(160, 235, 144), 1200, -1},
    {squareSize * 9, squareSize * 9, squareSize * 10, squareSize * 10, L"����", RGB(255, 255, 255), 0, -1},

    // �Ʒ��� �� (�����ʿ��� ��������) - ���� ��Ȳ��
    {squareSize * 8, squareSize * 9, squareSize * 9, squareSize * 10, L"�̽�ź��", RGB(255, 218, 185), 1100, -1},
    {squareSize * 7, squareSize * 9, squareSize * 8, squareSize * 10, L"�ι���", RGB(255, 218, 185), 1400, -1},
    {squareSize * 6, squareSize * 9, squareSize * 7, squareSize * 10, L"����", RGB(255, 218, 185), 1000, -1},
    {squareSize * 5, squareSize * 9, squareSize * 6, squareSize * 10, L"���ߵ�", RGB(255, 218, 185), 1050, -1},
    {squareSize * 4, squareSize * 9, squareSize * 5, squareSize * 10, L"�ϸ�", RGB(255, 218, 185), 800, -1},
    {squareSize * 3, squareSize * 9, squareSize * 4, squareSize * 10, L"�����", RGB(255, 218, 185), 900, -1},
    {squareSize * 2, squareSize * 9, squareSize * 3, squareSize * 10, L"�ٱ״ٵ�", RGB(255, 218, 185), 750, -1},
    {squareSize, squareSize * 9, squareSize * 2, squareSize * 10, L"ī�̷�", RGB(255, 218, 185), 700, -1},
    {0, squareSize * 9, squareSize, squareSize * 10, L"�����", RGB(255, 255, 255), 0, -1},

    // ���� �� (�Ʒ����� ����) - ���� �����
    {0, squareSize * 8, squareSize, squareSize * 9, L"���쵥�ڳ��̷�", RGB(216, 191, 216), 850, -1},
    {0, squareSize * 7, squareSize, squareSize * 8, L"�ο��뽺���̷���", RGB(216, 191, 216), 800, -1},
    {0, squareSize * 6, squareSize, squareSize * 7, L"�Ϲٳ�", RGB(216, 191, 216), 750, -1},
    {0, squareSize * 5, squareSize, squareSize * 6, L"�߽��ڽ�Ƽ", RGB(216, 191, 216), 950, -1},
    {0, squareSize * 4, squareSize, squareSize * 5, L"����", RGB(216, 191, 216), 880, -1},
    {0, squareSize * 3, squareSize, squareSize * 4, L"��Ƽ�ư�", RGB(216, 191, 216), 830, -1},
    {0, squareSize * 2, squareSize, squareSize * 3, L"����Ÿ", RGB(216, 191, 216), 700, -1},
    {0, squareSize, squareSize, squareSize * 2, L"ŷ����", RGB(216, 191, 216), 720, -1},
};

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
        MessageBox(NULL, L"������ ���� ���п�", L"", MB_OK);
    }
    return hIngameWnd;
}

//�ֻ���
int rollDice() {
    int result = (rand() % 6) + 1;
    swprintf_s(messageBuffer2, L"�ֻ��� ���: %d", result);
    InvalidateRect(hIngameWnd, NULL, TRUE);
    return result;
}

//���� ���� ȿ��
void RandomEffect(int playerIndex) {
    std::vector<std::pair<LPCWSTR, std::function<void()>>> effects = {
        {L"���ʽ�($500) ����", []() { players[currentPlayer].money += 500; }},
        {L"���ʽ�($2000) ����", []() { players[currentPlayer].money += 2000; }},
        {L"���ʽ�($5000) ����", []() { players[currentPlayer].money += 5000; }},
        {L"����($500) ����", []() { players[currentPlayer].money -= 500; }},
        {L"����($800) ����", []() { players[currentPlayer].money -= 800; }},
        {L"����($1200) ����", []() { players[currentPlayer].money -= 1200; }},
        {L"������ �̵�", []() { movePlayer(currentPlayer, 3); }},
        {L"�ڷ� �̵�", []() { movePlayer(currentPlayer, -2); }},
        {L"�������� �̵�", []() {
            for (size_t i = 0; i < zones.size(); ++i) {
                if (wcscmp(zones[i].name, L"����") == 0) {
                    players[currentPlayer].position = i;
                    players[currentPlayer].PrisonTurn = 3;
                    break;
                }
            }
        }},
        {L"��� �÷��̾�� $500 ����", []() {
            for (size_t i = 0; i < players.size(); ++i) {
                if (i != currentPlayer) {
                    players[currentPlayer].money -= 500;
                    players[i].money += 500;
                }
            }
        }},
        {L"���� ��� ���� �ε��� �Ű�", []() {
            if (!players[currentPlayer].ownedProperties.empty()) {
                auto maxProperty = std::max_element(players[currentPlayer].ownedProperties.begin(),
                    players[currentPlayer].ownedProperties.end(),
                    [](int a, int b) { return zones[a].price < zones[b].price; });
                int propertyIndex = *maxProperty;
                players[currentPlayer].money += zones[propertyIndex].price;
                zones[propertyIndex].owner = -1;
                players[currentPlayer].ownedProperties.erase(maxProperty);
                updateMessage(L"%s�� �Ű��ϰ� $%d�� �޾ҽ��ϴ�.", zones[propertyIndex].name, zones[propertyIndex].price);
            }
        }},
        {L"������ �÷��̾�� ��ġ ��ȯ", []() {
            int otherPlayer = (currentPlayer + 1) % players.size();
            std::swap(players[currentPlayer].position, players[otherPlayer].position);
            updateMessage(L"�÷��̾� %d�� ��ġ�� ��ȯ�߽��ϴ�.", otherPlayer + 1);
        }},
        {L"���� 3�� ���� �Ӵ�� ����", []() {
            updateMessage(L"���� 3�� ���� �Ӵ�ᰡ �����˴ϴ�.");
        }}
    };

    int randomEffect = rand() % effects.size();
    effects[randomEffect].second();
    updateMessage(L"���� ȿ��: %s", effects[randomEffect].first);
}

//�÷��̾� �̵� ���� 
void movePlayer(int playerIndex, int steps) {
    if (players[playerIndex].PrisonTurn > 0) {
        players[playerIndex].PrisonTurn--;
        updateMessage(L"�÷��̾� %d�� ���� ���ѿ� �ֽ��ϴ�. ���� ��: %d", playerIndex + 1, players[playerIndex].PrisonTurn);
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

    if (passedStart) {
        players[playerIndex].money += 2000;
        updateMessage(L"������� �������ϴ�. $2000�� �޾ҽ��ϴ�!");
    }

    if (wcscmp(zones[currentPosition].name, L"����") == 0) {
        players[playerIndex].PrisonTurn = 3;
        updateMessage(L"���ѿ� �����߽��ϴ�! 3�� ���� ������ �� �����ϴ�.");
    }
    else if (wcscmp(zones[currentPosition].name, L"����") == 0) {
        RandomEffect(playerIndex);
    }
    else if (wcscmp(zones[currentPosition].name, L"�����") == 0) {
        int destination = chooseDestination(playerIndex);
        players[playerIndex].position = destination;
        updateMessage(L"�÷��̾� %d�� ����⸦ Ÿ�� %s�� �̵��߽��ϴ�.", playerIndex + 1, zones[destination].name);
    }

    updateMessage(L"�÷��̾� %d�� %s�� �����߽��ϴ�.", playerIndex + 1, zones[currentPosition].name);
}

//����� ���� �̺�Ʈ 
int chooseDestination(int playerIndex) {
    std::vector<int> destinations;
    for (size_t i = 0; i < zones.size(); ++i) {
        if (wcscmp(zones[i].name, L"�����") != 0 && wcscmp(zones[i].name, L"����") != 0) {
            destinations.push_back(i);
        }
    }

    WCHAR message[256];
    swprintf_s(message, L"�÷��̾� %d, �������� �����ϼ���:", playerIndex + 1);
    int choice = MessageBox(hIngameWnd, message, L"������ ����", MB_OK);

    // ���� ���ӿ����� ���⿡ ����� �Է��� �޴� ������ �����ؾ� �մϴ�.
    // �� ���������� ������ �������� �������� �����մϴ�.
    return destinations[rand() % destinations.size()];
}

// ���� ���� �̺�Ʈ 
void buyProperty(int playerIndex, int zoneIndex) {
    if (zones[zoneIndex].owner == -1 && players[playerIndex].money >= zones[zoneIndex].price) {
        WCHAR message[100];
        swprintf_s(message, L"%s�� $%d�� �����Ͻðڽ��ϱ�?", zones[zoneIndex].name, zones[zoneIndex].price);

        if (MessageBox(hIngameWnd, message, L"���� Ȯ��", MB_YESNO | MB_ICONQUESTION) == IDYES) {
            zones[zoneIndex].owner = playerIndex;
            players[playerIndex].money -= zones[zoneIndex].price;
            players[playerIndex].ownedProperties.push_back(zoneIndex);
            updateMessage(L"�÷��̾� %d�� %s�� �����߽��ϴ�.", playerIndex + 1, zones[zoneIndex].name);
        }
        else {
            updateMessage(L"�÷��̾� %d�� %s ���Ÿ� �����߽��ϴ�.", playerIndex + 1, zones[zoneIndex].name);
        }
    }
    else {
        updateMessage(L"�÷��̾� %d�� %s�� ������ �� �����ϴ�.", playerIndex + 1, zones[zoneIndex].name);
    }
}

//  
int calculatePropertyValue(int playerIndex) {
    int totalValue = 0;
    for (int propertyIndex : players[playerIndex].ownedProperties) {
        totalValue += zones[propertyIndex].price;
    }
    return totalValue;
}

// ���� ���� �̺�Ʈ  
void checkGameOver(HWND hWnd) {
    WCHAR winbuffer[256];
    for (size_t i = 0; i < players.size(); ++i) {
        int totalAssets = players[i].money + calculatePropertyValue(i);
        if (totalAssets >= 30000) {
            swprintf_s(winbuffer, L"�÷��̾� %d�� �� �ڻ� $%d�� �¸��߽��ϴ�!", i + 1, totalAssets);
            MessageBox(hIngameWnd, winbuffer, L"�¸�!", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }

        if (players[i].money < 0) {
            int winner = (i + 1) % players.size();
            swprintf_s(winbuffer, L"�÷��̾� %d�� �Ļ��߽��ϴ�. �÷��̾� %d �¸�!", i + 1, winner + 1);
            MessageBox(hIngameWnd, winbuffer, L"�¸�!", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }
    }
}

// ���� ���� �Լ� 
void resetGame() {
    for (auto& player : players) {
        player.position = 0;
        player.money = 10000;
        player.PrisonTurn = 0;
        player.loanAmount = 0;
        player.loanTerm = 0;
        player.isInDebt = false;
        player.collateral = 0;
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
        hToolTip = CreateWindowEx(0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, NULL, hInst, NULL);

        SendMessage(hToolTip, TTM_SETMAXTIPWIDTH, 0, 300);

        ZeroMemory(&ti, sizeof(ti));
        ti.cbSize = sizeof(TOOLINFO);
        ti.uFlags = TTF_SUBCLASS;
        ti.hwnd = hWnd;
        ti.hinst = hInst;
        ti.uId = 0;
        ti.lpszText = LPSTR_TEXTCALLBACK;
        ti.rect.left = ti.rect.top = ti.rect.right = ti.rect.bottom = 0;

        SendMessage(hToolTip, TTM_ADDTOOL, 0, (LPARAM)&ti);

        srand(static_cast<unsigned int>(time(nullptr)));
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hBankBrush = CreateSolidBrush(sugilBank.backgroundColor);
        HPEN hBankPen = CreatePen(PS_SOLID, 2, sugilBank.borderColor);
        const int messageX = 90;  
        const int messageY1 = 200; 
        const int messageY2 = 230; 


        GetClientRect(hWnd, &WindowRect);

        for (size_t i = 0; i < zones.size(); ++i)
        {
            HBRUSH hZoneBrush = CreateSolidBrush(zones[i].color);
            SelectObject(hdc, hZoneBrush);
            Rectangle(hdc, zones[i].x1, zones[i].y1, zones[i].x2, zones[i].y2);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(0, 0, 0));
            TextOut(hdc, zones[i].x1 + 5, zones[i].y1 + 5, zones[i].name, lstrlenW(zones[i].name));

            if (zones[i].owner != -1) {
                HBRUSH hOwnerBrush = CreateSolidBrush(players[zones[i].owner].color);
                RECT ownerRect = { zones[i].x1, zones[i].y2 - 10, zones[i].x2, zones[i].y2 };
                FillRect(hdc, &ownerRect, hOwnerBrush);
                DeleteObject(hOwnerBrush);
            }

            if (zones[i].price > 0) {
                WCHAR priceText[20];
                swprintf_s(priceText, L"$%d", zones[i].price);
                TextOut(hdc, zones[i].x1 + 5, zones[i].y2 - 20, priceText, lstrlenW(priceText));
            }

            DeleteObject(hZoneBrush);
        }

        for (size_t i = 0; i < players.size(); ++i)
        {
            int x = zones[players[i].position].x1 + 5 + (i * 20);
            int y = zones[players[i].position].y2 - 35;
            HBRUSH hBrush = CreateSolidBrush(players[i].color);
            SelectObject(hdc, hBrush);
            Ellipse(hdc, x, y, x + 20, y + 20);
            DeleteObject(hBrush);
        }

        WCHAR turnText[50];
        swprintf_s(turnText, L"�÷��̾� %d�� �����Դϴ�. (Money: $%d)", currentPlayer + 1, players[currentPlayer].money);
        TextOut(hdc, WindowRect.left + 100, WindowRect.top + 150, turnText, lstrlenW(turnText));

        SelectObject(hdc, hBankBrush);
        SelectObject(hdc, hBankPen);
        Rectangle(hdc, sugilBank.rect.left, sugilBank.rect.top - 45, sugilBank.rect.right + 300, sugilBank.rect.bottom - 120);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));
        HFONT hBankFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"���� ���");
        SelectObject(hdc, hBankFont);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top - 30,
            sugilBank.name, lstrlenW(sugilBank.name));

        WCHAR bankText[100];
        swprintf_s(bankText, L"�ȳ��ϼ���? ���� �����Դϴ�. (���� �ܰ�: $%d)", banks[currentBank].bank_money);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top + 10, bankText, lstrlenW(bankText));

        for (int i = 0; i < 2; i++) {
            WCHAR loanText[3][100];
            swprintf_s(loanText[0], L"�÷��̾� %d�� �����: $%d", i + 1, players[i].loanAmount);
            swprintf_s(loanText[1], L"�÷��̾� %d�� �㺸: %d", i + 1, players[i].collateral);
            swprintf_s(loanText[2], L"�÷��̾� %d�� ���� ����: %d��", i + 1, players[i].loanTerm);

            for (int j = 0; j < 3; j++) {
                TextOut(hdc, sugilBank.rect.left + 20 + (i * 360), sugilBank.rect.top + 80 + (j * 30), loanText[j], lstrlenW(loanText[j]));
            }
        }

        Rectangle(hdc, player1Info.rect.left, player1Info.rect.top + 370,
            player1Info.rect.right, player1Info.rect.bottom + 370);
        TextOut(hdc, player1Info.rect.left + 10, player1Info.rect.top + 380,
            player1Info.name, lstrlenW(player1Info.name));

        WCHAR p1Info[2][100];
        swprintf_s(p1Info[0], L"���� �ݾ�: $%d", players[0].money);
        swprintf_s(p1Info[1], L"�� �ڻ�: $%d", players[0].money + calculatePropertyValue(0));
        for (int i = 0; i < 2; i++) {
            TextOut(hdc, player1Info.rect.left + 10, player1Info.rect.top + 440 + (i * 20), p1Info[i], lstrlenW(p1Info[i]));
        }

        Rectangle(hdc, player2Info.rect.left, player2Info.rect.top + 370,
            player2Info.rect.right, player2Info.rect.bottom + 370);
        TextOut(hdc, player2Info.rect.left + 10, player2Info.rect.top + 380,
            player2Info.name, lstrlenW(player2Info.name));

        WCHAR p2Info[2][100];
        swprintf_s(p2Info[0], L"���� �ݾ�: $%d", players[1].money);
        swprintf_s(p2Info[1], L"�� �ڻ�: $%d", players[1].money + calculatePropertyValue(1));
        for (int i = 0; i < 2; i++) {
            TextOut(hdc, player2Info.rect.left + 10, player2Info.rect.top + 440 + (i * 20), p2Info[i], lstrlenW(p2Info[i]));
        }

        TextOut(hdc, messageX, messageY1, messageBuffer, lstrlenW(messageBuffer));
        TextOut(hdc, messageX, messageY2, messageBuffer2, lstrlenW(messageBuffer2));

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
            updateMessage(L"�����뿪���� %d�� ���ҽ��ϴ�.", players[currentPlayer].PrisonTurn);
            players[currentPlayer].PrisonTurn--;
        }
        else {
            movePlayer(currentPlayer, dice);

            int currentZone = players[currentPlayer].position;

            if (wcscmp(zones[currentZone].name, L"�����") == 0)
            {
                int destination = chooseDestination(currentPlayer);
                players[currentPlayer].position = destination;
                updateMessage(L"�÷��̾� %d�� ����⸦ Ÿ�� %s�� �̵��߽��ϴ�.", currentPlayer + 1, zones[destination].name);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (zones[currentZone].owner == -1 && zones[currentZone].price > 0) {
                buyProperty(currentPlayer, currentZone);
            }
            else if (zones[currentZone].owner != -1 && zones[currentZone].owner != currentPlayer) {
                int rent = zones[currentZone].price / 2;
                players[currentPlayer].money -= rent;
                players[zones[currentZone].owner].money += rent;
                updateMessage(L"�÷��̾� %d�� $%d�� �Ӵ�Ḧ �����߽��ϴ�.",
                    currentPlayer + 1, rent);
            }
        }

        checkLoanStatus(currentPlayer);
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
                        (zones[i].owner == -1) ? L"����" : (zones[i].owner == 0 ? L"�÷��̾� 1" : L"�÷��̾� 2"));
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
    const int LOAN_THRESHOLD = 2000;
    if (players[playerIndex].money <= LOAN_THRESHOLD) {
        if (players[playerIndex].loanAmount == 0) {
            players[playerIndex].money += LOAN_AMOUNT;
            players[playerIndex].loanAmount = LOAN_AMOUNT;
            players[playerIndex].loanTerm = LOAN_TERM;
            players[playerIndex].isInDebt = false;
            updateMessage(L"�÷��̾� %d�� $%d�� ����޾ҽ��ϴ�. ����: %d��", playerIndex + 1, LOAN_AMOUNT, LOAN_TERM);
        }
        else {
            updateMessage(L"�÷��̾� %d�� �̹� ������ �ֽ��ϴ�.", playerIndex + 1);
        }
    }
    else {
        updateMessage(L"�÷��̾� %d�� ���� �ݾ��� $%d�� �ʰ��߽��ϴ�.", playerIndex + 1, LOAN_THRESHOLD);
    }
}

void repayLoan(int playerIndex) {
    if (players[playerIndex].loanAmount > 0) {
        if (players[playerIndex].money >= players[playerIndex].loanAmount) {
            players[playerIndex].money -= players[playerIndex].loanAmount;
            updateMessage(L"�÷��̾� %d�� $%d�� ������� ��ȯ�߽��ϴ�.", playerIndex + 1, players[playerIndex].loanAmount);
            players[playerIndex].loanAmount = 0;
            players[playerIndex].loanTerm = 0;
            players[playerIndex].isInDebt = false;
        }
        else {
            updateMessage(L"�÷��̾� %d�� ���� �����Ͽ� ������� ��ȯ�� �� �����ϴ�.", playerIndex + 1);
        }
    }
    else {
        updateMessage(L"�÷��̾� %d�� ��ȯ�� ������ �����ϴ�.", playerIndex + 1);
    }
}

void updateMessage(const WCHAR* format, ...) {
    va_list args;
    va_start(args, format);
    vswprintf_s(messageBuffer, 256, format, args);
    va_end(args);
    wprintf(L"\n");
}

void checkLoanStatus(int playerIndex) {
    if (players[playerIndex].loanAmount > 0) {
        players[playerIndex].loanTerm--;

        if (players[playerIndex].loanTerm <= 0) {
            players[playerIndex].isInDebt = true;
            updateMessage(L"�÷��̾� %d�� ������� ��ȯ�ؾ� �մϴ�!", playerIndex + 1);

            if (players[playerIndex].money >= players[playerIndex].loanAmount) {
                // ������� ��ȯ�� �� �ִ� ���
                players[playerIndex].money -= players[playerIndex].loanAmount;
                updateMessage(L"�÷��̾� %d�� $%d�� ������� ��ȯ�߽��ϴ�.", playerIndex + 1, players[playerIndex].loanAmount);
                players[playerIndex].loanAmount = 0;
                players[playerIndex].loanTerm = 0;
                players[playerIndex].isInDebt = false;
            }
            else {
                // ������� ��ȯ�� �� ���� ���, �ڻ� �з� ���� ����
                int mostExpensiveProperty = -1;
                int highestPrice = 0;
                for (int propertyIndex : players[playerIndex].ownedProperties) {
                    if (zones[propertyIndex].price > highestPrice) {
                        highestPrice = zones[propertyIndex].price;
                        mostExpensiveProperty = propertyIndex;
                    }
                }

                if (mostExpensiveProperty != -1) {
                    zones[mostExpensiveProperty].owner = -1;
                    players[playerIndex].ownedProperties.erase(
                        std::remove(players[playerIndex].ownedProperties.begin(),
                            players[playerIndex].ownedProperties.end(),
                            mostExpensiveProperty),
                        players[playerIndex].ownedProperties.end()
                    );

                    int remainingDebt = players[playerIndex].loanAmount - zones[mostExpensiveProperty].price;
                    if (remainingDebt > 0) {
                        players[playerIndex].loanAmount = remainingDebt;
                        updateMessage(L"�÷��̾� %d�� %s(��)�� �з��Ǿ����ϴ�. ���� ��: $%d",
                            playerIndex + 1, zones[mostExpensiveProperty].name, remainingDebt);
                    }
                    else {
                        players[playerIndex].loanAmount = 0;
                        players[playerIndex].isInDebt = false;
                        updateMessage(L"�÷��̾� %d�� %s�� �з��Ǿ� ��� ���� ��ȯ�Ǿ����ϴ�.",
                            playerIndex + 1, zones[mostExpensiveProperty].name);
                    }
                }
                else {
                    updateMessage(L"�÷��̾� %d�� �з��� �ε����� �����ϴ�. ���� ����!", playerIndex + 1);
                    resetGame();
                }
            }

            // ���� ���� ���� �ʱ�ȭ
            if (players[playerIndex].loanAmount <= 0) {
                players[playerIndex].loanTerm = 0;
                players[playerIndex].isInDebt = false;
            }
        }
        else {
            updateMessage(L"�÷��̾� %d�� ���� ������� %d�� ���ҽ��ϴ�.", playerIndex + 1, players[playerIndex].loanTerm);
        }
    }
}

