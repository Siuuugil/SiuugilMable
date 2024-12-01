#include "IngameMain.h"


// ���� ���� ����
WCHAR szIngameWindowClass[MAX_LOADSTRING] = L"IngameWindowClass";
HWND hToolTip;
HWND hIngameWnd = nullptr;
WCHAR messageBuffer[256] = L"";
WCHAR messageBuffer2[256] = L"";
const int squareSize = 70; // �� ������ 
int currentPlayer = 0; //���� �÷��̾�
int currentBank = 0; //���� ���õ� ���� <- �÷��̾�1, 2 ������ ���� ���� ������ ���� 
HBITMAP hBackgroundBitmap = NULL;


//�÷��̾�1 ���� 
UserInformation player1Info = {
    {750, 50, 950, 300},
    L"�÷��̾� 1 ����",
    RGB(255, 107, 107),
    RGB(0, 0, 0)
};

//�÷��̾�2 ���� 
UserInformation player2Info = {
    {960, 50, 1160, 300},
    L"�÷��̾� 2 ����",
    RGB(100, 149, 237),
    RGB(0, 0, 0)
};
//�÷��̾�1,2�� ���� 
std::vector<Player> players = {
    {0, 10000, RGB(255, 107, 107), 0, 0, 0, false},
    {0, 10000, RGB(100, 149, 237), 0, 0, 0, false}
};

//���� ���� 
BankZone sugilBank = {
    {750, 50, 1100, 500},
    L"���� ����",
    RGB(240, 240, 240),
    RGB(0, 0, 0)
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
        int destination = RandomDestination(playerIndex);
        players[playerIndex].position = destination;
        updateMessage(L"�÷��̾� %d�� ����⸦ Ÿ�� %s�� �̵��߽��ϴ�.", playerIndex + 1, zones[destination].name);
    }

    updateMessage(L"�÷��̾� %d�� %s�� �����߽��ϴ�.", playerIndex + 1, zones[currentPosition].name);
}

//���� ���� ȿ��  
//���� ȿ������ vector(���� �迭 �����̳�)�� ����. 
//pair : �� ���� ���� �ϳ��� ������ ����. 
//LPCWSTR : �����ڵ� ���ڿ��� ����Ű�� ������(ȿ���� ���� L"...")
//function<void()> : void() �Լ��� ����(ȿ���� �����ϴ� �Լ�)
//��, std::vector<std::pair<LPCWSTR, std::function<void()>>>��  ȿ���� ����� �� ȿ���� �����ϴ� �Լ��� ���� ���� �迭 �����̳ʿ� ����. 
//players[currentPlayer].money : ���� �÷��̾ ������ �ִ� ��
//movePlayer(currentPlayer) : movePlayer() <- ���� �̵��� ���� �Լ�. �� ���� �÷��̾��� ��ġ�� nĭ ��ŭ �ű� 
//for(size_t ....) size_t : ��ȣ ���� ���� Ÿ��. i �� zones�� ũ�⺸�� ���� ���� ��� �ݺ� 


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
            int otherPlayersCount = players.size() - 1;
            players[currentPlayer].money -= 500 * otherPlayersCount;
            for (size_t i = 0; i < players.size(); ++i) {
                if (i != currentPlayer) {
                 players[i].money += 500;
                }
            }
        }},
     {L"���� ��� ���� �ε��� �Ű�", []() {
            if (!players[currentPlayer].ownedProperties.empty()) {
                int maxPrice = 0;
                int maxPriceIndex = -1;

                for (int i = 0; i < players[currentPlayer].ownedProperties.size(); i++) {
                    int propertyIndex = players[currentPlayer].ownedProperties[i];
                    if (zones[propertyIndex].price > maxPrice) {
                        maxPrice = zones[propertyIndex].price;
                        maxPriceIndex = propertyIndex;
                    }
                }
                if (maxPriceIndex != -1) {
                    players[currentPlayer].money += zones[maxPriceIndex].price;
                    zones[maxPriceIndex].owner = -1;
                    for (int i = 0; i < players[currentPlayer].ownedProperties.size(); i++) {
                        if (players[currentPlayer].ownedProperties[i] == maxPriceIndex) {
                            players[currentPlayer].ownedProperties.erase(players[currentPlayer].ownedProperties.begin() + i);
                            break;
                        }
                    }
                    updateMessage(L"%s�� �Ű��ϰ� $%d�� �޾ҽ��ϴ�.", zones[maxPriceIndex].name, zones[maxPriceIndex].price);
                }
            }
        }},

        {L"������ �÷��̾�� ��ġ ��ȯ", []() {
            int otherPlayer = (currentPlayer + 1) % players.size();
            std::swap(players[currentPlayer].position, players[otherPlayer].position);
            updateMessage(L"�÷��̾� %d�� ��ġ�� ��ȯ�߽��ϴ�.", otherPlayer + 1);
        }},
    };

    int randomEffect = rand() % effects.size();
    effects[randomEffect].second();
    updateMessage(L"���� ȿ��: %s", effects[randomEffect].first);
}



//����� ���� ȿ�� 
int RandomDestination(int playerIndex) {
    std::vector<int> emptyDestinations;
    for (size_t i = 0; i < zones.size(); ++i) {
        // ������ ������ �����ϰ�, �����ڰ� ����(-1) ���� ����
        if (wcscmp(zones[i].name, L"�����") != 0 &&
            wcscmp(zones[i].name, L"����") != 0 &&
            zones[i].owner == -1 &&
            zones[i].price > 0) {  // ������ �ִ� ���� ���� (Ư�� ���� ����)
            emptyDestinations.push_back(i);
        }
    }

    // ����ִ� ���� ���� ���
    if (emptyDestinations.empty()) {
        // ��� �Ϲ� ���� ������� �ٽ� ����
        for (size_t i = 0; i < zones.size(); ++i) {
            if (wcscmp(zones[i].name, L"�����") != 0 &&
                wcscmp(zones[i].name, L"����") != 0 &&
                zones[i].price > 0) {
                emptyDestinations.push_back(i);
            }
        }
    }

    // ������ �������� ���ٸ� (��� ���� Ư�� ������ ���), ���� �������� �̵�
    if (emptyDestinations.empty()) {
        return 0;  // ���� ������ �ε��� (���� 0)
    }

    return emptyDestinations[rand() % emptyDestinations.size()];
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


// �÷��̾��� �� �ε��� ��ġ
int calculatePropertyValue(int playerIndex) {
    int totalValue = 0;
    for (int i = 0; i < players[playerIndex].ownedProperties.size(); i++) {
        int propertyIndex = players[playerIndex].ownedProperties[i];
        totalValue = totalValue + zones[propertyIndex].price;
    }
    return totalValue;
}

// ���� ����
void checkGameOver(HWND hWnd) {
    WCHAR winnerbuffer[256];
    for (size_t i = 0; i < players.size(); ++i) {
        int totalAssets = players[i].money + calculatePropertyValue(i);
        if (totalAssets >= 30000) {
            swprintf_s(winnerbuffer, L"�÷��̾� %d�� �� �ڻ� $%d�� �¸��߽��ϴ�!", i + 1, totalAssets);
            MessageBox(hIngameWnd, winnerbuffer, L"�¸�!", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }

        if (players[i].money < 0) {
            int winner = (i + 1) % players.size();
            swprintf_s(winnerbuffer, L"�÷��̾� %d�� �Ļ��߽��ϴ�. �÷��̾� %d �¸�!", i + 1, winner + 1);
            MessageBox(hIngameWnd, winnerbuffer, L"�¸�!", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }
    }
} 
// ���� ����
void resetGame() {
    for (int i = 0; i < players.size(); i++) {
        players[i].position = 0;
        players[i].money = 10000;
        players[i].PrisonTurn = 0;
        players[i].loanAmount = 0;
        players[i].loanTerm = 0;
        players[i].isInDebt = false;
        players[i].collateral = 0;
        players[i].ownedProperties.clear();
    }
    for (int i = 0; i < zones.size(); i++) {
        zones[i].owner = -1;
    }
    currentPlayer = 0;
}

// ���� ����
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
//���� ��ȯ
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

//���� ��Ȳ
void checkLoanStatus(int playerIndex) {
    if (players[playerIndex].loanAmount > 0) {
        players[playerIndex].loanTerm--;

        if (players[playerIndex].loanTerm <= 0) {
            players[playerIndex].isInDebt = true;
            updateMessage(L"�÷��̾� %d�� ������� ��ȯ�ؾ� �մϴ�!", playerIndex + 1);

            if (players[playerIndex].money >= players[playerIndex].loanAmount) {
                players[playerIndex].money -= players[playerIndex].loanAmount;
                updateMessage(L"�÷��̾� %d�� $%d�� ������� ��ȯ�߽��ϴ�.", playerIndex + 1, players[playerIndex].loanAmount);
                players[playerIndex].loanAmount = 0;
                players[playerIndex].loanTerm = 0;
                players[playerIndex].isInDebt = false;
            }
            else {
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

            //���� �ʱ�ȭ
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

//const WCHAR* format : �޽����� ������ �����ϴ� ���ڿ�, ... : ���� ����(���� ���� �߰� ���ڸ� ���� �� ����)
//updateMessage�� ������� �� �ٿ� ��� ��� ���� ���. 
void updateMessage(const WCHAR* format, ...) {
    va_list args; // ���� ���ڵ��� ó���ϱ� ���� Ư���� ���� 
    va_start(args, format); //���� ���� ó���� ������. format�� ������ ���� ���ڵ��� ó���� �غ�
    vswprintf_s(messageBuffer, 256, format, args); //���ڿ��� ���ۿ� ��� ��� 
    va_end(args); //������ 
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

        
    
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hBankBrush = CreateSolidBrush(sugilBank.backgroundColor);
        HPEN hBankPen = CreatePen(PS_SOLID, 2, sugilBank.borderColor);

        HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(245, 245, 220));
        RECT backclientRect;
        GetClientRect(hWnd, &backclientRect);
        FillRect(hdc, &backclientRect, hBackgroundBrush);
        DeleteObject(hBackgroundBrush);

        const int messageX = 90;
        const int messageY1 = 200;
        const int messageY2 = 230;
        int guideX = 1200;
        int guideY = 450;
        int lineHeight = 25;

        RECT clientRect2;
        GetClientRect(hWnd, &clientRect2);

        SetTextColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);

            hBackgroundBitmap = (HBITMAP)LoadImage(NULL, L"earth.jpg", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBackgroundBitmap);

            BITMAP bm;
            GetObject(hBackgroundBitmap, sizeof(bm), &bm);

            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            StretchBlt(hdc, 0, 0, clientRect.right, clientRect.bottom,
                hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

            SelectObject(hdcMem, hOldBitmap);
            DeleteDC(hdcMem);
        
        WCHAR gameInstructions[][100] = {
        L"���� ����:",
        L"Ŭ�� : �ֻ��� ������",
        L"�������� : ��� �ִ� ������ ���� �̵�",
        L"���� : ������ ���鼭 �˾ư�����.",
        L"���� : 3�� �� ������ ����",
        L"���! : ���� $2000 ����",
        L"�¸����� :",
        L"- �� �ڻ� $30000 �̻�",
        L"- ��� �÷��̾� �� �ڻ� $0",
        L"- ��� �÷��̾� ���� �ݾ� $0"
        };

        int instructionsCount = sizeof(gameInstructions) / sizeof(gameInstructions[0]);
        for (int i = 0; i < instructionsCount; i++) {
            TextOut(hdc,
                guideX,
                guideY + i * lineHeight,
                gameInstructions[i],
                lstrlenW(gameInstructions[i]));
        }

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

        RECT WindowRect;
        GetClientRect(hWnd, &WindowRect);
        WCHAR turnText[50];
        swprintf_s(turnText, L"�÷��̾� %d�� �����Դϴ�. (Money: $%d)", currentPlayer + 1, players[currentPlayer].money);
        TextOut(hdc, WindowRect.left + 100, WindowRect.top + 150, turnText, lstrlenW(turnText));

        SelectObject(hdc, hBankBrush);
        SelectObject(hdc, hBankPen);
        Rectangle(hdc, sugilBank.rect.left, sugilBank.rect.top - 45, sugilBank.rect.right + 300, sugilBank.rect.bottom - 120);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));
        HFONT hInformationFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"���� ���");
        SelectObject(hdc, hInformationFont);
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


        HBRUSH hPlayer1Brush = CreateSolidBrush(player1Info.backgroundColor);
        HPEN hPlayer1Pen = CreatePen(PS_SOLID, 2, player1Info.borderColor);
        SelectObject(hdc, hPlayer1Brush);
        SelectObject(hdc, hPlayer1Pen);
        Rectangle(hdc, player1Info.rect.left, player1Info.rect.top + 370,
            player1Info.rect.right, player1Info.rect.bottom + 370);
        SetTextColor(hdc, player1Info.borderColor);
        TextOut(hdc, player1Info.rect.left + 10, player1Info.rect.top + 380,
            player1Info.name, lstrlenW(player1Info.name));

        WCHAR p1Info[2][100];
        swprintf_s(p1Info[0], L"���� �ݾ�: $%d", players[0].money);
        swprintf_s(p1Info[1], L"�� �ڻ�: $%d", players[0].money + calculatePropertyValue(0));
        for (int i = 0; i < 2; i++) {
            TextOut(hdc, player1Info.rect.left + 10, player1Info.rect.top + 440 + (i * 20), p1Info[i], lstrlenW(p1Info[i]));
        }


        HBRUSH hPlayer2Brush = CreateSolidBrush(player2Info.backgroundColor);
        HPEN hPlayer2Pen = CreatePen(PS_SOLID, 2, player2Info.borderColor);
        SelectObject(hdc, hPlayer2Brush);
        SelectObject(hdc, hPlayer2Pen);
        Rectangle(hdc, player2Info.rect.left, player2Info.rect.top + 370,
            player2Info.rect.right, player2Info.rect.bottom + 370);
        SetTextColor(hdc, player2Info.borderColor);
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

        DeleteObject(hPlayer1Brush);
        DeleteObject(hPlayer1Pen);
        DeleteObject(hPlayer2Brush);
        DeleteObject(hPlayer2Pen);
        DeleteObject(hInformationFont);
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
                int destination = RandomDestination(currentPlayer);
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
    break;

    case WM_DESTROY:
        if (hBackgroundBitmap)
        {
            DeleteObject(hBackgroundBitmap);
        }
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}




