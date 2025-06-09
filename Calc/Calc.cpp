// Calc.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Calc.h"
#include <string>
#include <vector>
#include <locale>
#include <cwctype>
using namespace std;
#define MAX_LOADSTRING 100

// 全局变量:
int zuo, you;
HINSTANCE hInst;                                // 当前实例
HINSTANCE g_hInstance = NULL;
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
bool isshu(wstring s) {
    if (s[0] <= L'9' && s[0] >= L'0'||s[0]==L'.') return true;
    if (s[0] == L'-' && s.size() > 1) return true;
    return false;
}

double jisuan(vector<wstring> x) {
    vector<double> a; wstring w = L""; double b, c;
    for (int i = 0; i < x.size(); ++i) {
        if (x[i] == L" ") continue;
        if (isshu(x[i])) a.push_back(stod(x[i]));
        if (x[i] == L"*") {
            while (!isshu(x[i])) {
                if (x[i] == L"-") {
                    c = a.back(); a.pop_back();
                    a.push_back(INT_MIN);
                    a.push_back(c);
                }
                ++i;
            }
            b = stod(x[i]);
            c = a.back() * b;
            a.pop_back();
            a.push_back(c);
        }
        if (x[i] == L"/") {
            while (!isshu(x[i])) {
                if (x[i] == L"-") {
                    c = a.back(); a.pop_back();
                    a.push_back(INT_MIN);
                    a.push_back(c);
                }
                ++i;
            }
            b = stod(x[i]);
            if (b == 0) {
                MessageBox(NULL, L"The divisor cannot be 0", L"Warning", MB_OK);
                return 0;
            }
            c = a.back() / b;
            a.pop_back();
            a.push_back(c);
        }
        if (x[i] == L"-") a.push_back(-a.back());
    }
    b = 0;
    for (double val : a) {
        b += val;
    }
    return b;
}

bool kh(vector<wstring> x) {
    for (int i = 0; i < x.size(); ++i) {
        if (x[i] == L"{" || x[i] == L"(" || x[i] == L"[") zuo = i;
        else if (x[i] == L"}" || x[i] == L")" || x[i] == L"]") {
            you = i;
            return true;
        }
    }
    return false;
}

double youkuohao(vector<wstring> x) {
    vector<wstring> b; double aw;
    while (kh(x)) {
        b.assign(x.begin() + zuo + 1, x.begin() + you);
        aw = jisuan(b);
        x[zuo] = to_wstring(aw);
        for (int i = zuo + 1; i <= you; ++i) x[i] = L" ";
    }
    aw = jisuan(x);
    return aw;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    g_hInstance = hInstance;
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CALC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CALC));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CALC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CALC);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_PROGRESS:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_PROGRESS), hWnd, About);
                break;
            case IDC_BUTTON1: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加数字“1”
                    wcscat_s(szText, 256, L"1");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTON2: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加数字“2”
                    wcscat_s(szText, 256, L"2");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTON3: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加数字“3”
                    wcscat_s(szText, 256, L"3");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTON4: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加数字“4”
                    wcscat_s(szText, 256, L"4");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTON5: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加数字“5”
                    wcscat_s(szText, 256, L"5");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTON6: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加数字“6”
                    wcscat_s(szText, 256, L"6");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTON7: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加数字“7”
                    wcscat_s(szText, 256, L"7");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTON8: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加数字“8”
                    wcscat_s(szText, 256, L"8");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTON9: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加数字“9”
                    wcscat_s(szText, 256, L"9");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTON0: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加数字“0”
                    wcscat_s(szText, 256, L"0");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTONjia: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加“+”
                    wcscat_s(szText, 256, L"+");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTONjian: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加“-”
                    wcscat_s(szText, 256, L"-");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTONcheng: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加“*”
                    wcscat_s(szText, 256, L"*");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTONchu: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加“/”
                    wcscat_s(szText, 256, L"/");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTONdengyu: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);
                    std::wstring s(szText),w=L"",c=L"";
                    std::vector<wstring> ss;
                    for (int i = 0; i < s.size(); ++i) {
                        if (s[i] <= L'9' && s[i] >= L'0'||s[i]==L'.') {
                            w += s[i];
                        }
                        else {
                            if (w != L"") ss.push_back(w);
                            w = L""; c += s[i];
                            ss.push_back(c);
                            c = L"";
                        }
                    }
                    ss.push_back(w);
                    double a = youkuohao(ss);
                    const size_t arraySize = 100;
                    WCHAR wcharArray[arraySize];

                    // 将 double 转换为 std::wstring
                    std::wstring wstr = std::to_wstring(a);
                    size_t i = wstr.length()-1;
                    while(i>0&&(wstr[i]==L'0'||wstr[i]==L'\0')) wstr[i]=L'\0',i--;
                    if(wstr[i]==L'.') wstr[i]=L'\0';
                    // 将 std::wstring 复制到 WCHAR 数组
                    wcscpy_s(wcharArray, arraySize, wstr.c_str());
                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, wcharArray);
                }
                break;
            }case IDC_BUTTONAC: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    for (size_t i = 0; i < 256; i++) szText[i] = L'\0';
                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTONCE: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);
                    size_t i;
                    for (i = 0; i < wcslen(szText); ++i) {
                        if (szText[i]==L'+'|| szText[i] == L'-'|| szText[i] == L'*'|| szText[i] == L'/') {
                            break;
                        }
                    }
                    for(i++;i<wcslen(szText);i++) {
                        szText[i] = L'\0';
                    }
                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTONDel: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    // 获取当前文本
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);
                    size_t i=wcslen(szText)-1;
                    szText[i] = L'\0';
                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTONdian: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加“.”
                    wcscat_s(szText, 256, L".");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }case IDC_BUTTONpai: {
                HWND hEditBox = GetDlgItem(hWnd, IDC_EDITBOX);
                if (hEditBox)
                {
                    WCHAR szText[256];
                    GetWindowText(hEditBox, szText, 256);

                    // 在当前文本末尾追加“.”
                    wcscat_s(szText, 256, L"3.141592");

                    // 更新EDIT控件的文本
                    SetWindowText(hEditBox, szText);
                }
                break;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE: {
        // 创建数字“1”的按钮
        HWND hButton1 = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"1",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            10, 60,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTON1, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButton2 = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"2",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            10, 110,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTON2, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButton3 = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"3",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            10, 160,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTON3, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButton4 = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"4",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            80, 60,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTON4, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButton5 = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"5",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            80, 110,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTON5, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButton6 = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"6",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            80, 160,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTON6, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButton7 = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"7",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            150, 60,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTON7, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButton8 = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"8",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            150, 110,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTON8, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButton9 = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"9",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            150, 160,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTON9, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButton0 = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"0",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            220, 60,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTON0, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButtonjia = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"+",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            220, 110,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTONjia, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButtonjian = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"-",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            220, 160,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTONjian, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButtoncheng = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"*",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            220, 210,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTONcheng, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButtonchu = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"/",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            150, 210,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTONchu, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButtondengyu = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"=",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            80, 210,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTONdengyu, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButtonAC = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"AC",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            10, 210,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTONAC, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButtonCE = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"CE",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            10, 260,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTONCE, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButtonDel = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"Del",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            80, 260,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTONDel, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButtondian = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L".",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            150, 260,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTONdian, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hButtonpai = CreateWindowW(
            L"BUTTON",   // 按钮控件类
            L"pi",        // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 样式：可见、子窗口、推按钮
            220, 260,      // 位置
            50, 30,      // 大小
            hWnd,        // 父窗口句柄
            (HMENU)IDC_BUTTONpai, // 控件ID
            g_hInstance,
            NULL
        );
        HWND hEditBox = CreateWindowW(
            L"EDIT",           // 预定义编辑控件类
            L"",              // 初始文本（可以为空）
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | WS_DISABLED, // 添加WS_DISABLED样式
            10, 10,            // 位置（x, y）
            500, 45,           // 大小（宽度, 高度）
            hWnd,             // 父窗口句柄
            (HMENU)IDC_EDITBOX, // 控件ID
            g_hInstance,      // 实例句柄
            NULL              // 额外的创建参数（通常为NULL）
        );
        if (hEditBox) {
            // 获取当前字体（如果有的话）
            HFONT hOldFont = (HFONT)SendMessage(hEditBox, WM_GETFONT, 0, 0);

            // 创建新字体（例如，将字号设置为16）
            HFONT hNewFont = CreateFontW(
                32,                 // nHeight
                0,                  // nWidth
                0,                  // nEscapement
                0,                  // nOrientation
                FW_NORMAL,          // nWeight
                FALSE,              // bItalic
                FALSE,              // bUnderline
                FALSE,              // bStrikeOut
                DEFAULT_CHARSET,    // nCharSet
                OUT_DEFAULT_PRECIS, // nOutPrecision
                CLIP_DEFAULT_PRECIS,// nClipPrecision
                DEFAULT_QUALITY,    // nQuality
                DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
                L"Arial"           // lpszFacename
            );

            // 应用新字体
            SendMessage(hEditBox, WM_SETFONT, (WPARAM)hNewFont, MAKELPARAM(TRUE, 0));

            // 如果获取了旧字体，并且新字体创建成功，则删除旧字体
            if (hOldFont && hNewFont) {
                DeleteObject(hOldFont);
          }
        // 您可以在此处继续添加其他数字按钮
       }
    }  
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
