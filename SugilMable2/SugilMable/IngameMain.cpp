#include "IngameMain.h"


// 전역 변수 정의
WCHAR szIngameWindowClass[MAX_LOADSTRING] = L"IngameWindowClass";
HWND hToolTip;
HWND hIngameWnd = nullptr;
WCHAR messageBuffer[256] = L"";
WCHAR messageBuffer2[256] = L"";
const int squareSize = 70; // 맵 사이즈 
int currentPlayer = 0; //현재 플레이어
int currentBank = 0; //현재 선택된 은행 <- 플레이어1, 2 각각의 은행 업무 구분을 위해 
HBITMAP hBackgroundBitmap = NULL;


//플레이어1 정보 
UserInformation player1Info = {
    {750, 50, 950, 300},
    L"플레이어 1 정보",
    RGB(255, 107, 107),
    RGB(0, 0, 0)
};

//플레이어2 정보 
UserInformation player2Info = {
    {960, 50, 1160, 300},
    L"플레이어 2 정보",
    RGB(100, 149, 237),
    RGB(0, 0, 0)
};
//플레이어1,2의 말판 
std::vector<Player> players = {
    {0, 10000, RGB(255, 107, 107), 0, 0, 0, false},
    {0, 10000, RGB(100, 149, 237), 0, 0, 0, false}
};

//은행 구역 
BankZone sugilBank = {
    {750, 50, 1100, 500},
    L"수길 은행",
    RGB(240, 240, 240),
    RGB(0, 0, 0)
};

// 은행 초기값
std::vector<Bank> banks = {
    {1000000, 0, RGB(0, 255, 0), 0}
};

// 맵 
std::vector<Zone> zones = {
    // 위쪽 줄 (왼쪽에서 오른쪽으로) - 연한 파란색
    {0, 0, squareSize, squareSize, L"출발!", RGB(255, 255, 255), 0, -1},
    {squareSize, 0, squareSize * 2, squareSize, L"도쿄", RGB(173, 206, 240), 1200, -1},
    {squareSize * 2, 0, squareSize * 3, squareSize, L"베이징", RGB(173, 206, 240), 1100, -1},
    {squareSize * 3, 0, squareSize * 4, squareSize, L"방콕", RGB(173, 206, 240), 900, -1},
    {squareSize * 4, 0, squareSize * 5, squareSize, L"싱가포르", RGB(173, 206, 240), 880, -1},
    {squareSize * 5, 0, squareSize * 6, squareSize, L"자카르타", RGB(173, 206, 240), 700, -1},
    {squareSize * 6, 0, squareSize * 7, squareSize, L"홍콩", RGB(173, 206, 240), 950, -1},
    {squareSize * 7, 0, squareSize * 8, squareSize, L"마닐라", RGB(173, 206, 240), 720, -1},
    {squareSize * 8, 0, squareSize * 9, squareSize, L"서울", RGB(173, 206, 240), 1000, -1},
    {squareSize * 9, 0, squareSize * 10, squareSize, L"북한", RGB(255, 255, 255), 0, -1},

    // 오른쪽 줄 (위에서 아래로) - 연한 녹색
    {squareSize * 9, squareSize, squareSize * 10, squareSize * 2, L"런던", RGB(160, 235, 144), 1500, -1},
    {squareSize * 9, squareSize * 2, squareSize * 10, squareSize * 3, L"파리", RGB(160, 235, 144), 1600, -1},
    {squareSize * 9, squareSize * 3, squareSize * 10, squareSize * 4, L"로마", RGB(160, 235, 144), 1400, -1},
    {squareSize * 9, squareSize * 4, squareSize * 10, squareSize * 5, L"마드리드", RGB(160, 235, 144), 1350, -1},
    {squareSize * 9, squareSize * 5, squareSize * 10, squareSize * 6, L"빈", RGB(160, 235, 144), 1300, -1},
    {squareSize * 9, squareSize * 6, squareSize * 10, squareSize * 7, L"프라하", RGB(160, 235, 144), 1250, -1},
    {squareSize * 9, squareSize * 7, squareSize * 10, squareSize * 8, L"베를린", RGB(160, 235, 144), 1450, -1},
    {squareSize * 9, squareSize * 8, squareSize * 10, squareSize * 9, L"암스테르담", RGB(160, 235, 144), 1200, -1},
    {squareSize * 9, squareSize * 9, squareSize * 10, squareSize * 10, L"랜덤", RGB(255, 255, 255), 0, -1},

    // 아래쪽 줄 (오른쪽에서 왼쪽으로) - 연한 주황색
    {squareSize * 8, squareSize * 9, squareSize * 9, squareSize * 10, L"이스탄불", RGB(255, 218, 185), 1100, -1},
    {squareSize * 7, squareSize * 9, squareSize * 8, squareSize * 10, L"두바이", RGB(255, 218, 185), 1400, -1},
    {squareSize * 6, squareSize * 9, squareSize * 7, squareSize * 10, L"도하", RGB(255, 218, 185), 1000, -1},
    {squareSize * 5, squareSize * 9, squareSize * 6, squareSize * 10, L"리야드", RGB(255, 218, 185), 1050, -1},
    {squareSize * 4, squareSize * 9, squareSize * 5, squareSize * 10, L"암만", RGB(255, 218, 185), 800, -1},
    {squareSize * 3, squareSize * 9, squareSize * 4, squareSize * 10, L"테헤란", RGB(255, 218, 185), 900, -1},
    {squareSize * 2, squareSize * 9, squareSize * 3, squareSize * 10, L"바그다드", RGB(255, 218, 185), 750, -1},
    {squareSize, squareSize * 9, squareSize * 2, squareSize * 10, L"카이로", RGB(255, 218, 185), 700, -1},
    {0, squareSize * 9, squareSize, squareSize * 10, L"비행기", RGB(255, 255, 255), 0, -1},

    // 왼쪽 줄 (아래에서 위로) - 연한 보라색
    {0, squareSize * 8, squareSize, squareSize * 9, L"리우데자네이루", RGB(216, 191, 216), 850, -1},
    {0, squareSize * 7, squareSize, squareSize * 8, L"부에노스아이레스", RGB(216, 191, 216), 800, -1},
    {0, squareSize * 6, squareSize, squareSize * 7, L"하바나", RGB(216, 191, 216), 750, -1},
    {0, squareSize * 5, squareSize, squareSize * 6, L"멕시코시티", RGB(216, 191, 216), 950, -1},
    {0, squareSize * 4, squareSize, squareSize * 5, L"리마", RGB(216, 191, 216), 880, -1},
    {0, squareSize * 3, squareSize, squareSize * 4, L"산티아고", RGB(216, 191, 216), 830, -1},
    {0, squareSize * 2, squareSize, squareSize * 3, L"보고타", RGB(216, 191, 216), 700, -1},
    {0, squareSize, squareSize, squareSize * 2, L"킹스턴", RGB(216, 191, 216), 720, -1},
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
        MessageBox(NULL, L"윈도우 생성 실패요", L"", MB_OK);
    }
    return hIngameWnd;
}

//주사위
int rollDice() {
    int result = (rand() % 6) + 1;
    swprintf_s(messageBuffer2, L"주사위 결과: %d", result);
    InvalidateRect(hIngameWnd, NULL, TRUE);
    return result;
}

//플레이어 이동 관련 
void movePlayer(int playerIndex, int steps) {
    if (players[playerIndex].PrisonTurn > 0) {
        players[playerIndex].PrisonTurn--;
        updateMessage(L"플레이어 %d는 아직 북한에 있습니다. 남은 턴: %d", playerIndex + 1, players[playerIndex].PrisonTurn);
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
        updateMessage(L"출발점을 지났습니다. $2000을 받았습니다!");
    }

    if (wcscmp(zones[currentPosition].name, L"북한") == 0) {
        players[playerIndex].PrisonTurn = 3;
        updateMessage(L"북한에 도착했습니다! 3턴 동안 움직일 수 없습니다.");
    }
    else if (wcscmp(zones[currentPosition].name, L"랜덤") == 0) {
        RandomEffect(playerIndex);
    }
    else if (wcscmp(zones[currentPosition].name, L"비행기") == 0) {
        int destination = RandomDestination(playerIndex);
        players[playerIndex].position = destination;
        updateMessage(L"플레이어 %d가 비행기를 타고 %s로 이동했습니다.", playerIndex + 1, zones[destination].name);
    }

    updateMessage(L"플레이어 %d가 %s에 도착했습니다.", playerIndex + 1, zones[currentPosition].name);
}

//랜덤 구역 효과  
//랜덤 효과들을 vector(동적 배열 컨테이너)에 저장. 
//pair : 두 개의 값을 하나의 단위로 묶음. 
//LPCWSTR : 유니코드 문자열을 가리키는 포인터(효과의 설명 L"...")
//function<void()> : void() 함수를 저장(효과를 실행하는 함수)
//즉, std::vector<std::pair<LPCWSTR, std::function<void()>>>는  효과의 설명과 그 효과를 실행하는 함수를 묶어 동적 배열 컨테이너에 저장. 
//players[currentPlayer].money : 현재 플레이어가 가지고 있는 돈
//movePlayer(currentPlayer) : movePlayer() <- 말판 이동을 위한 함수. 즉 현재 플레이어의 위치를 n칸 만큼 옮김 
//for(size_t ....) size_t : 부호 없는 정수 타입. i 가 zones의 크기보다 작은 동안 계속 반복 


void RandomEffect(int playerIndex) {
    std::vector<std::pair<LPCWSTR, std::function<void()>>> effects = {
        {L"보너스($500) 지급", []() { players[currentPlayer].money += 500; }},
        {L"보너스($2000) 지급", []() { players[currentPlayer].money += 2000; }},
        {L"보너스($5000) 지급", []() { players[currentPlayer].money += 5000; }},
        {L"벌금($500) 지불", []() { players[currentPlayer].money -= 500; }},
        {L"벌금($800) 지불", []() { players[currentPlayer].money -= 800; }},
        {L"벌금($1200) 지불", []() { players[currentPlayer].money -= 1200; }},
        {L"앞으로 이동", []() { movePlayer(currentPlayer, 3); }},
        {L"뒤로 이동", []() { movePlayer(currentPlayer, -2); }},
        {L"북한으로 이동", []() {
            for (size_t i = 0; i < zones.size(); ++i) {
                if (wcscmp(zones[i].name, L"북한") == 0) {
                    players[currentPlayer].position = i;
                    players[currentPlayer].PrisonTurn = 3;
                    break;
                }
            }
        }},
        {L"모든 플레이어에게 $500 지불", []() {
            int otherPlayersCount = players.size() - 1;
            players[currentPlayer].money -= 500 * otherPlayersCount;
            for (size_t i = 0; i < players.size(); ++i) {
                if (i != currentPlayer) {
                 players[i].money += 500;
                }
            }
        }},
     {L"가장 비싼 소유 부동산 매각", []() {
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
                    updateMessage(L"%s를 매각하고 $%d를 받았습니다.", zones[maxPriceIndex].name, zones[maxPriceIndex].price);
                }
            }
        }},

        {L"무작위 플레이어와 위치 교환", []() {
            int otherPlayer = (currentPlayer + 1) % players.size();
            std::swap(players[currentPlayer].position, players[otherPlayer].position);
            updateMessage(L"플레이어 %d와 위치를 교환했습니다.", otherPlayer + 1);
        }},
    };

    int randomEffect = rand() % effects.size();
    effects[randomEffect].second();
    updateMessage(L"랜덤 효과: %s", effects[randomEffect].first);
}



//비행기 구역 효과 
int RandomDestination(int playerIndex) {
    std::vector<int> emptyDestinations;
    for (size_t i = 0; i < zones.size(); ++i) {
        // 비행기와 북한을 제외하고, 소유자가 없는(-1) 땅만 선택
        if (wcscmp(zones[i].name, L"비행기") != 0 &&
            wcscmp(zones[i].name, L"북한") != 0 &&
            zones[i].owner == -1 &&
            zones[i].price > 0) {  // 가격이 있는 땅만 선택 (특수 구역 제외)
            emptyDestinations.push_back(i);
        }
    }

    // 비어있는 땅이 없는 경우
    if (emptyDestinations.empty()) {
        // 모든 일반 땅을 대상으로 다시 선택
        for (size_t i = 0; i < zones.size(); ++i) {
            if (wcscmp(zones[i].name, L"비행기") != 0 &&
                wcscmp(zones[i].name, L"북한") != 0 &&
                zones[i].price > 0) {
                emptyDestinations.push_back(i);
            }
        }
    }

    // 여전히 목적지가 없다면 (모든 땅이 특수 구역인 경우), 시작 지점으로 이동
    if (emptyDestinations.empty()) {
        return 0;  // 시작 지점의 인덱스 (보통 0)
    }

    return emptyDestinations[rand() % emptyDestinations.size()];
}

// 구매 관련 이벤트 
void buyProperty(int playerIndex, int zoneIndex) {
    if (zones[zoneIndex].owner == -1 && players[playerIndex].money >= zones[zoneIndex].price) {
        WCHAR message[100];
        swprintf_s(message, L"%s를 $%d에 구매하시겠습니까?", zones[zoneIndex].name, zones[zoneIndex].price);

        if (MessageBox(hIngameWnd, message, L"구매 확인", MB_YESNO | MB_ICONQUESTION) == IDYES) {
            zones[zoneIndex].owner = playerIndex;
            players[playerIndex].money -= zones[zoneIndex].price;
            players[playerIndex].ownedProperties.push_back(zoneIndex);
            updateMessage(L"플레이어 %d가 %s를 구매했습니다.", playerIndex + 1, zones[zoneIndex].name);
        }
        else {
            updateMessage(L"플레이어 %d가 %s 구매를 거절했습니다.", playerIndex + 1, zones[zoneIndex].name);
        }
    }
    else {
        updateMessage(L"플레이어 %d가 %s를 구매할 수 없습니다.", playerIndex + 1, zones[zoneIndex].name);
    }
}


// 플레이어의 총 부동산 가치
int calculatePropertyValue(int playerIndex) {
    int totalValue = 0;
    for (int i = 0; i < players[playerIndex].ownedProperties.size(); i++) {
        int propertyIndex = players[playerIndex].ownedProperties[i];
        totalValue = totalValue + zones[propertyIndex].price;
    }
    return totalValue;
}

// 게임 오버
void checkGameOver(HWND hWnd) {
    WCHAR winnerbuffer[256];
    for (size_t i = 0; i < players.size(); ++i) {
        int totalAssets = players[i].money + calculatePropertyValue(i);
        if (totalAssets >= 30000) {
            swprintf_s(winnerbuffer, L"플레이어 %d가 총 자산 $%d로 승리했습니다!", i + 1, totalAssets);
            MessageBox(hIngameWnd, winnerbuffer, L"승리!", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }

        if (players[i].money < 0) {
            int winner = (i + 1) % players.size();
            swprintf_s(winnerbuffer, L"플레이어 %d가 파산했습니다. 플레이어 %d 승리!", i + 1, winner + 1);
            MessageBox(hIngameWnd, winnerbuffer, L"승리!", MB_OK);
            resetGame();
            InvalidateRect(hWnd, NULL, TRUE);
            return;
        }
    }
} 
// 게임 리셋
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

// 대출 실행
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
            updateMessage(L"플레이어 %d가 $%d을 대출받았습니다. 만기: %d턴", playerIndex + 1, LOAN_AMOUNT, LOAN_TERM);
        }
        else {
            updateMessage(L"플레이어 %d는 이미 대출이 있습니다.", playerIndex + 1);
        }
    }
    else {
        updateMessage(L"플레이어 %d의 보유 금액이 $%d를 초과했습니다.", playerIndex + 1, LOAN_THRESHOLD);
    }
}
//대출 상환
void repayLoan(int playerIndex) {
    if (players[playerIndex].loanAmount > 0) {
        if (players[playerIndex].money >= players[playerIndex].loanAmount) {
            players[playerIndex].money -= players[playerIndex].loanAmount;
            updateMessage(L"플레이어 %d가 $%d의 대출금을 상환했습니다.", playerIndex + 1, players[playerIndex].loanAmount);
            players[playerIndex].loanAmount = 0;
            players[playerIndex].loanTerm = 0;
            players[playerIndex].isInDebt = false;
        }
        else {
            updateMessage(L"플레이어 %d의 돈이 부족하여 대출금을 상환할 수 없습니다.", playerIndex + 1);
        }
    }
    else {
        updateMessage(L"플레이어 %d는 상환할 대출이 없습니다.", playerIndex + 1);
    }
}

//대출 상황
void checkLoanStatus(int playerIndex) {
    if (players[playerIndex].loanAmount > 0) {
        players[playerIndex].loanTerm--;

        if (players[playerIndex].loanTerm <= 0) {
            players[playerIndex].isInDebt = true;
            updateMessage(L"플레이어 %d가 대출금을 상환해야 합니다!", playerIndex + 1);

            if (players[playerIndex].money >= players[playerIndex].loanAmount) {
                players[playerIndex].money -= players[playerIndex].loanAmount;
                updateMessage(L"플레이어 %d가 $%d의 대출금을 상환했습니다.", playerIndex + 1, players[playerIndex].loanAmount);
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
                        updateMessage(L"플레이어 %d의 %s(이)가 압류되었습니다. 남은 빚: $%d",
                            playerIndex + 1, zones[mostExpensiveProperty].name, remainingDebt);
                    }
                    else {
                        players[playerIndex].loanAmount = 0;
                        players[playerIndex].isInDebt = false;
                        updateMessage(L"플레이어 %d의 %s가 압류되어 모든 빚이 상환되었습니다.",
                            playerIndex + 1, zones[mostExpensiveProperty].name);
                    }
                }
                else {
                    updateMessage(L"플레이어 %d는 압류할 부동산이 없습니다. 게임 오버!", playerIndex + 1);
                    resetGame();
                }
            }

            //대출 초기화
            if (players[playerIndex].loanAmount <= 0) {
                players[playerIndex].loanTerm = 0;
                players[playerIndex].isInDebt = false;
            }
        }
        else {
            updateMessage(L"플레이어 %d의 대출 만기까지 %d턴 남았습니다.", playerIndex + 1, players[playerIndex].loanTerm);
        }
    }
}

//const WCHAR* format : 메시지의 형식을 지정하는 문자열, ... : 가변 인자(여러 개의 추가 인자를 받을 수 있음)
//updateMessage의 내용들을 한 줄에 모두 담기 위해 사용. 
void updateMessage(const WCHAR* format, ...) {
    va_list args; // 가면 인자들을 처리하기 위한 특별한 변수 
    va_start(args, format); //가변 인자 처리를 시작함. format의 다음에 오는 인자들을 처리할 준비
    vswprintf_s(messageBuffer, 256, format, args); //문자열을 버퍼에 담고 출력 
    va_end(args); //마무리 
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
        L"게임 설명:",
        L"클릭 : 주사위 굴리기",
        L"자유여행 : 비어 있는 땅으로 랜덤 이동",
        L"랜덤 : 모험을 즐기면서 알아가세요.",
        L"북한 : 3턴 간 움직임 제한",
        L"출발! : 월급 $2000 지급",
        L"승리조건 :",
        L"- 총 자산 $30000 이상",
        L"- 상대 플레이어 총 자산 $0",
        L"- 상대 플레이어 보유 금액 $0"
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
        swprintf_s(turnText, L"플레이어 %d의 차례입니다. (Money: $%d)", currentPlayer + 1, players[currentPlayer].money);
        TextOut(hdc, WindowRect.left + 100, WindowRect.top + 150, turnText, lstrlenW(turnText));

        SelectObject(hdc, hBankBrush);
        SelectObject(hdc, hBankPen);
        Rectangle(hdc, sugilBank.rect.left, sugilBank.rect.top - 45, sugilBank.rect.right + 300, sugilBank.rect.bottom - 120);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));
        HFONT hInformationFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"맑은 고딕");
        SelectObject(hdc, hInformationFont);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top - 30,
            sugilBank.name, lstrlenW(sugilBank.name));

        WCHAR bankText[100];
        swprintf_s(bankText, L"안녕하세요? 수길 은행입니다. (은행 잔고: $%d)", banks[currentBank].bank_money);
        TextOut(hdc, sugilBank.rect.left + 20, sugilBank.rect.top + 10, bankText, lstrlenW(bankText));

        for (int i = 0; i < 2; i++) {
            WCHAR loanText[3][100];
            swprintf_s(loanText[0], L"플레이어 %d의 대출금: $%d", i + 1, players[i].loanAmount);
            swprintf_s(loanText[1], L"플레이어 %d의 담보: %d", i + 1, players[i].collateral);
            swprintf_s(loanText[2], L"플레이어 %d의 대출 만기: %d턴", i + 1, players[i].loanTerm);

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
        swprintf_s(p1Info[0], L"보유 금액: $%d", players[0].money);
        swprintf_s(p1Info[1], L"총 자산: $%d", players[0].money + calculatePropertyValue(0));
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
        swprintf_s(p2Info[0], L"보유 금액: $%d", players[1].money);
        swprintf_s(p2Info[1], L"총 자산: $%d", players[1].money + calculatePropertyValue(1));
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
            updateMessage(L"강제노역까지 %d턴 남았습니다.", players[currentPlayer].PrisonTurn);
            players[currentPlayer].PrisonTurn--;
        }
        else {
            movePlayer(currentPlayer, dice);

            int currentZone = players[currentPlayer].position;

            if (wcscmp(zones[currentZone].name, L"비행기") == 0)
            {
                int destination = RandomDestination(currentPlayer);
                players[currentPlayer].position = destination;
                updateMessage(L"플레이어 %d가 비행기를 타고 %s로 이동했습니다.", currentPlayer + 1, zones[destination].name);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (zones[currentZone].owner == -1 && zones[currentZone].price > 0) {
                buyProperty(currentPlayer, currentZone);
            }
            else if (zones[currentZone].owner != -1 && zones[currentZone].owner != currentPlayer) {
                int rent = zones[currentZone].price / 2;
                players[currentPlayer].money -= rent;
                players[zones[currentZone].owner].money += rent;
                updateMessage(L"플레이어 %d가 $%d의 임대료를 지불했습니다.",
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




