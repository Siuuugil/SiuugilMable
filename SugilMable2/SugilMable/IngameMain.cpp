#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <functional>


#define MAX_LOADSTRING 100

// ���� ����
extern HINSTANCE hInst;
WCHAR szIngameWindowClass[MAX_LOADSTRING] = L"IngameWindowClass";
HWND hIngameWnd = nullptr;

const int mapSize = 10;  // �� ũ��
const int squareSize = 70; // �� ĭ�� ũ��
const int gapSize = 5;    // ĭ ���� ����

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
    int position;  // ���� ��ġ
    int money;     // ���� �ݾ�
    COLORREF color; // �÷��̾� ����
    int PrisonTurn; // ���� 
};

// �� ���� 
std::vector<Zone> zones = {
    // ���� �� (���ʿ��� ����������)
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

    // ������ �� (������ �Ʒ���)
    {squareSize * 9, squareSize, squareSize * 10, squareSize * 2, L"Berlin", RGB(255, 128, 0), 180, -1},
    {squareSize * 9, squareSize * 2, squareSize * 10, squareSize * 3, L"Moscow", RGB(128, 0, 0), 180, -1},
    {squareSize * 9, squareSize * 3, squareSize * 10, squareSize * 4, L"Geneva", RGB(0, 128, 0), 160, -1},
    {squareSize * 9, squareSize * 4, squareSize * 10, squareSize * 5, L"Athens", RGB(0, 0, 255), 140, -1},
    {squareSize * 9, squareSize * 5, squareSize * 10, squareSize * 6, L"Tax", RGB(255, 0, 0), 200, -1},
    {squareSize * 9, squareSize * 6, squareSize * 10, squareSize * 7, L"Madrid", RGB(255, 165, 0), 180, -1},
    {squareSize * 9, squareSize * 7, squareSize * 10, squareSize * 8, L"Toronto", RGB(255, 192, 203), 200, -1},
    {squareSize * 9, squareSize * 8, squareSize * 10, squareSize * 9, L"Singapore", RGB(165, 42, 42), 300, -1},
    {squareSize * 9, squareSize * 9, squareSize * 10, squareSize * 10, L"Random", RGB(255, 215, 0), 0, -1},

    // �Ʒ��� �� (�����ʿ��� ��������)
    {squareSize * 8, squareSize * 9, squareSize * 9, squareSize * 10, L"Bruges", RGB(219, 112, 147), 140, -1},
    {squareSize * 7, squareSize * 9, squareSize * 8, squareSize * 10, L"Dublin", RGB(255, 20, 147), 150, -1},
    {squareSize * 6, squareSize * 9, squareSize * 7, squareSize * 10, L"Budapest", RGB(199, 21, 133), 160, -1},
    {squareSize * 5, squareSize * 9, squareSize * 6, squareSize * 10, L"Warsaw", RGB(139, 0, 139), 170, -1},
    {squareSize * 4, squareSize * 9, squareSize * 5, squareSize * 10, L"Prague", RGB(75, 0, 130), 180, -1},
    {squareSize * 3, squareSize * 9, squareSize * 4, squareSize * 10, L"Lisbon", RGB(72, 61, 139), 200, -1},
    {squareSize * 2, squareSize * 9, squareSize * 3, squareSize * 10, L"Bell", RGB(123, 104, 238), 210, -1},
    {squareSize, squareSize * 9, squareSize * 2, squareSize * 10, L"Brussels", RGB(138, 43, 226), 220, -1},
    {0, squareSize * 9, squareSize, squareSize * 10, L"Go! Start", RGB(64, 224, 208), 200, -1},

    // ���� �� (�Ʒ����� ����)
    {0, squareSize * 8, squareSize, squareSize * 9, L"Rio de Janeiro", RGB(60, 179, 113), 300, -1},
    {0, squareSize * 7, squareSize, squareSize * 8, L"Melbourne", RGB(220, 20, 60), 210, -1},
    {0, squareSize * 6, squareSize, squareSize * 7, L"Havana", RGB(128, 0, 0), 220, -1},
    {0, squareSize * 5, squareSize, squareSize * 6, L"Zurich", RGB(25, 25, 112), 250, -1},
    {0, squareSize * 4, squareSize, squareSize * 5, L"Dubai", RGB(0, 255, 127), 200, -1},
    {0, squareSize * 3, squareSize, squareSize * 4, L"Bangkok", RGB(34, 139, 34), 180, -1},
    {0, squareSize * 2, squareSize, squareSize * 3, L"Osaka", RGB(218, 165, 32), 120, -1},
    {0, squareSize, squareSize, squareSize * 2, L"Shanghai", RGB(255, 140, 0), 150, -1}
};




// �÷��̾�
std::vector<Player> players = {
    {0, 10000, RGB(255, 0, 0),0},  // �÷��̾� 1
    {0, 10000, RGB(0, 0, 255),0}   // �÷��̾� 2
};

int currentPlayer = 0;  // ���� ���� �÷��̾�

// ���� �ڵ�
HWND hToolTip;

// �Լ� ������ 
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
        MessageBox(hParentWnd, L"������ ������ �����Ͽ����ϴ�.", L"Error", MB_OK | MB_ICONERROR);
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
    if (wcscmp(zones[currentPosition].name, L"Prison") == 0) {
        players[playerIndex].PrisonTurn = 3;
        MessageBox(NULL, L"������ �����߽��ϴ�! 3�� ���� ������ �� �����ϴ�.", L"����", MB_OK);
    }
    else if (wcscmp(zones[currentPosition].name, L"Random") == 0) {
        RandomEffect(playerIndex);
    }
    else if (wcscmp(zones[currentPosition].name, L"Go! Start") == 0) {
        players[playerIndex].position = 0;
        MessageBox(NULL, L"���� �������� ���ư��ϴ�!", L"Go! Start", MB_OK);
    }

    // ���� ��ġ ���� ��� (������)
    WCHAR debugMessage[100];
    swprintf_s(debugMessage, L"�÷��̾� %d�� %s�� �����߽��ϴ�.", playerIndex + 1, zones[currentPosition].name);
    MessageBox(NULL, debugMessage, L"�̵� ����", MB_OK);
}


// ���� ����
void buyProperty(int playerIndex, int zoneIndex) {
    if (zones[zoneIndex].owner == -1 && players[playerIndex].money >= zones[zoneIndex].price) {
        zones[zoneIndex].owner = playerIndex;
        players[playerIndex].money -= zones[zoneIndex].price;
    }
}

// ���� ���� Ȯ��
void checkGameOver(HWND hWnd) {
    for (size_t i = 0; i < players.size(); ++i) {
        if (players[i].money < 0) {
            int winner = (i + 1) % players.size();
            WCHAR message[100];
            swprintf_s(message, L"�÷��̾� %d �̰���ϴ�!", winner + 1);
            MessageBox(hWnd, message, L"Game Over", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }
    }
}

// ���� �ʱ�ȭ
void resetGame() {
    // �÷��̾� �ʱ�ȭ
    for (auto& player : players) {
        player.position = 0;
        player.money = 10000;
        player.PrisonTurn = 0;
    }

    // ������ ���� �ʱ�ȭ
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
            Ellipse(hdc, x, y, x + 15, y + 15);
            DeleteObject(hBrush);
        }

        // ���� �÷��̾��� �� ǥ��
        WCHAR turnText[50];
        swprintf_s(turnText, L"�÷��̾� %d'�� ���� �Դϴ�. (Money: $%d)", currentPlayer + 1, players[currentPlayer].money);
        TextOut(hdc, 10, mapSize * squareSize + 10, turnText, lstrlenW(turnText));

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_LBUTTONDOWN:
    {
        int dice = rollDice();

        if (players[currentPlayer].PrisonTurn > 0) {
            WCHAR message[100];
            swprintf_s(message, L"Ż������ %d�� ���ҽ��ϴ�. ", players[currentPlayer].PrisonTurn);
            MessageBox(hWnd, message, L"Prison", MB_OK);
            players[currentPlayer].PrisonTurn--;
        }
        else {
            movePlayer(currentPlayer, dice);

            // ���� ��ġ�� ������ ó��
            int currentZone = players[currentPlayer].position;

            if (wcscmp(zones[currentZone].name, L"Go! Start") == 0)
            {
                MessageBox(NULL, L"���� �������� ���ư��ϴ�!", L"Go! Start", MB_OK);
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
                // ���� ��ġ �� �ؽ�Ʈ ������Ʈ
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
