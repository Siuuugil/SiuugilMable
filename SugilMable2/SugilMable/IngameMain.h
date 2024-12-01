#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <windowsx.h>
#include <commctrl.h>
#include <ctime>

#define MAX_LOADSTRING 100

// 구조체 정의
struct Zone {
    int x1, y1, x2, y2;
    LPCWSTR name;
    COLORREF color;
    int price;
    int owner;
};

struct Player {
    int position;
    int money;
    COLORREF color;
    int PrisonTurn;
    int loanAmount;
    int loanTerm;
    bool isInDebt;
    int collateral;
    std::vector<int> ownedProperties;
};

struct Bank {
    int bank_money;
    int warrant;
    COLORREF color;
    int bank_owner;
};

struct BankZone {
    RECT rect;
    LPCWSTR name;
    COLORREF backgroundColor;
    COLORREF borderColor;
};

struct UserInformation {
    RECT rect;
    LPCWSTR name;
    COLORREF backgroundColor;
    COLORREF borderColor;
};

// 전역 변수 선언
extern HINSTANCE hInst;
extern WCHAR szIngameWindowClass[MAX_LOADSTRING];
extern HWND hToolTip;
extern HWND hIngameWnd;
extern WCHAR messageBuffer[256];
extern WCHAR messageBuffer2[256];
extern const int mapSize;
extern const int squareSize;
extern const int gapSize;
extern int currentPlayer;
extern int currentBank;
extern BankZone sugilBank;
extern UserInformation player1Info;
extern UserInformation player2Info;
extern std::vector<Player> players;
extern std::vector<Bank> banks;
extern std::vector<Zone> zones;

// 함수 선언
ATOM MyRegisterIngameClass(HINSTANCE hInstance);
HWND CreateIngameWindow(HWND hParentWnd);
LRESULT CALLBACK IngameWndProc(HWND, UINT, WPARAM, LPARAM);
int rollDice();
void movePlayer(int playerIndex, int steps);
void buyProperty(int playerIndex, int zoneIndex);
void checkGameOver(HWND hWnd);
void resetGame();
void RandomEffect(int playerIndex);
void executeLoan(int playerIndex);
void repayLoan(int playerIndex);
void updateMessage(const WCHAR* format, ...);
void checkLoanStatus(int playerIndex);
int chooseDestination(int playerIndex);
int calculatePropertyValue(int playerIndex);