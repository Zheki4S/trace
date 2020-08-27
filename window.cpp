#include<windows.h>
#include<tchar.h>
#include<iostream>
#include"objects.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 
TCHAR WinName[] = _T("MainFrame");
int APIENTRY _tWinMain(HINSTANCE This, HINSTANCE Prev, LPTSTR cmd, int mode){  
    HWND hWnd; 
    MSG msg;
    WNDCLASS wc;
    wc.hInstance = This;    
    wc.lpszClassName = WinName;   // Имя класса окна  
    wc.lpfnWndProc = WndProc;    // Функция окна 
    wc.style = CS_HREDRAW | CS_VREDRAW;  // Стиль окна  
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Стандартная иконка  
    wc.hCursor = LoadCursor(NULL,IDC_ARROW); // Стандартный курсор  
    wc.lpszMenuName = NULL; // Нет меню  
    wc.cbClsExtra = 0;  // Нет дополнительных данных класса  
    wc.cbWndExtra = 0;  // Нет дополнительных данных окна  
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // Заполнение окна белым цветом   
    if(!RegisterClass(&wc)) 
        return 0; // Регистрация класса окна 
    hWnd = CreateWindow(WinName, _T("3Dmin"), WS_OVERLAPPEDWINDOW, 300, 300,  CW_USEDEFAULT,  CW_USEDEFAULT, HWND_DESKTOP, NULL, This,  NULL);
    ShowWindow(hWnd, mode);
    //UpdateWindow(hWnd); 
    while(GetMessage(&msg, NULL, 0, 0)) {  
        TranslateMessage(&msg);// Функция трансляции кодов нажатой клавиши  
        DispatchMessage(&msg); // Посылает сообщение функции WndProc()  
    }  
    return 0; 
} 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc, hCmpDC;
    HBITMAP hBmp;
    PAINTSTRUCT ps;
    COLORREF current_color;
    static int t;
    static char key;
    static int x, y, z, width, height;
    static double teta, phi;
    static Vector dir(sin(M_PI / 2 + teta) * cos(M_PI / 2 + phi), sin(M_PI / 2 + teta) * sin(M_PI / 2 + phi), cos(M_PI / 2 + teta)); // направление обзора
    static Vector eye(500 + x, -150 + y, 500 + z); // точка обзора
    static Vector kdir;
    static Vector ndir;
    trio(dir, kdir, ndir);
    switch(message){
        case WM_CREATE:
            break;
        case WM_SIZE:
            width = LOWORD(lParam);
            height = HIWORD(lParam);
            break;
        case WM_KEYDOWN:
            switch(wParam){
                case (TCHAR)'W':
                    eye = eye + 100 * dir;
                    break;
                case (TCHAR)'A':
                    trio(dir, kdir, ndir);
                    eye = eye - 100 * kdir;
                    break;
                case (TCHAR)'S':
                    eye = eye - 100 * dir;
                    break;
                case (TCHAR)'D':
                    trio(dir, kdir, ndir);
                    eye = eye + 100 * kdir;
                    break;
                case VK_UP:
                    trio(dir, kdir, ndir);
                    eye = eye - 100 * ndir;
                    break;
                case VK_DOWN:
                    trio(dir, kdir, ndir);
                    eye = eye + 100 * ndir;
                    break; 
                case VK_NUMPAD2:
                    teta += 0.25;
                    dir = Vector(sin(M_PI / 2 + teta) * cos(M_PI / 2 + phi), sin(M_PI / 2 + teta) * sin(M_PI / 2 + phi), cos(M_PI / 2 + teta));
                    break;
                case VK_NUMPAD8:
                    teta -= 0.25;
                    dir = Vector(sin(M_PI / 2 + teta) * cos(M_PI / 2 + phi), sin(M_PI / 2 + teta) * sin(M_PI / 2 + phi), cos(M_PI / 2 + teta));
                    break;
                case VK_NUMPAD4:
                    phi += 0.25;
                    dir = Vector(sin(M_PI / 2 + teta) * cos(M_PI / 2 + phi), sin(M_PI / 2 + teta) * sin(M_PI / 2 + phi), cos(M_PI / 2 + teta));
                    break;
                case VK_NUMPAD6:
                    phi -= 0.25;
                    dir = Vector(sin(M_PI / 2 + teta) * cos(M_PI / 2 + phi), sin(M_PI / 2 + teta) * sin(M_PI / 2 + phi), cos(M_PI / 2 + teta));
                    break;         
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;    
        case WM_PAINT:
            {  
                //объекты
                std::vector<Sphere> objects;
                Vector c1(500, 500, -200); // центр сферы 1
                Sphere sp1(c1, 400); // сфера 1
                Vector c2(-1000, 1500, 700); // центр сферы 2
                Sphere sp2(c2, 200); // сфера 2
                sp1.setColor(Vector(101, 103, 78));
                sp2.setColor(Vector(176, 78, 223));
                Vector light(100000, 1000, 100000); //точка освещения
                objects.push_back(sp1);
                objects.push_back(sp2);
                Picture p1(eye, dir, light, objects, width, height, 150);
                hdc = BeginPaint(hWnd, &ps);
                //контекст для двойной буфферизации
                hCmpDC = CreateCompatibleDC(hdc);
                hBmp = CreateCompatibleBitmap(hdc, width, height);
                SelectObject(hCmpDC, hBmp);
                //отрисовка сцены
                Vector *px = new Vector[width * height];
                px = p1.draw();
                for(int i = 0; i < height; i++){
                    for(int j = 0; j < width; j++){
                        current_color = RGB((int)px[i * width + j].getX(), (int)px[i * width + j].getY(), (int)px[i * width + j].getZ());
                        SetPixel(hCmpDC, j, i, current_color);
                    }
                }
                //вывод
                SetStretchBltMode(hdc, COLORONCOLOR);
                BitBlt(hdc, 0, 0, width, height, hCmpDC, 0, 0, SRCCOPY);
                DeleteDC(hCmpDC);
                DeleteObject(hBmp);
                delete [] px;
                hCmpDC = NULL; 
                EndPaint(hWnd, &ps);
            } 
            break;
        case WM_DESTROY:
            PostQuitMessage(0);     
            break; 
        default : 
            return DefWindowProc(hWnd, message, wParam, lParam);  
    } 
    return 0; 
} 