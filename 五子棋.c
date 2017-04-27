#include <Windows.h>
#include<time.h>
#include "resource.h"

#define CHESS_BLACK (1)
#define CHESS_WHITE (-1)
#define CHESS_ROW 15
#define CHESS_COL 15
#define CHESS_WIDTH 515
#define CHESS_HIGHT 614

LRESULT CALLBACK MyWinProc(HWND, UINT, WPARAM, LPARAM);

HBITMAP hBitmap_back;
HBITMAP hBitmap_bb;
HBITMAP hBitmap_bw;
HBITMAP hBitmap_ww;
HBITMAP hBitmap_wb;

int chess_flag = CHESS_BLACK;
int chess_arr[CHESS_ROW][CHESS_COL] = {0};

int IsWin();

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	HWND hwnd;
	MSG msg;

	static TCHAR szGameName[] = TEXT( "������");

	//�õ���Ļ�ֱ���
	int CX = GetSystemMetrics(SM_CXSCREEN );
	int CY = GetSystemMetrics(SM_CYSCREEN );

	//1.��ƴ���
	WNDCLASSEX wndClassEx;
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	wndClassEx.cbSize = sizeof(wndClassEx);
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = hInstance;
	wndClassEx.hIcon = LoadIcon(hInstance,NULL);
	wndClassEx.hIconSm = NULL;
	wndClassEx.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndClassEx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.lpszClassName = szGameName;
	wndClassEx.lpfnWndProc = MyWinProc;

	//ע�ᴰ��
	if( !RegisterClassEx(&wndClassEx))
	{
		MessageBox( NULL, TEXT("������ע��ʧ��!"), TEXT("Ӧ�ó������"), MB_OK | MB_ICONERROR );
		return 0;
	}

	//��������
	hwnd = CreateWindow(szGameName,"������",WS_OVERLAPPEDWINDOW,(CX-CHESS_WIDTH)/2,(CY-CHESS_HIGHT)/2,CHESS_WIDTH,CHESS_HIGHT,NULL,NULL,hInstance,NULL);

	hBitmap_back = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_BACK));
	hBitmap_bb = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP_BB));
	hBitmap_bw = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP_BW));
	hBitmap_ww = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP_WW));
	hBitmap_wb = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP_WB));

	srand((unsigned int)time(NULL));

	//��ʾ����
	ShowWindow(hwnd,nShowCmd);

	//��Ϣ
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;

}

LRESULT CALLBACK MyWinProc(HWND hwnd, UINT Msg, WPARAM wparam, LPARAM lparam)
{
	HDC  hdc;
	PAINTSTRUCT ps;
	HDC hdcMem;
	int mouse_x = 0;
	int mouse_y = 0;
	int xIndex = 0;
	int yIndex = 0;

	int isWin_flag = 0;

	switch (Msg)
	{
	case WM_PAINT:

		hdc = BeginPaint(hwnd,&ps);
		hdcMem = CreateCompatibleDC(hdc);

		SelectObject(hdcMem,hBitmap_back);
		BitBlt(hdc,0,0,CHESS_WIDTH,CHESS_HIGHT,hdcMem,0,0,SRCCOPY);
		DeleteDC(hdcMem);

		EndPaint(hwnd,&ps);
		break;
	case WM_KEYDOWN:
		switch(wparam)
		{
		case VK_ESCAPE:
			if ( MessageBox( NULL, TEXT("��Ϸ��������, ȷ���˳���?"), \
				TEXT("ȷ��"), MB_OKCANCEL | MB_ICONQUESTION ) == IDOK )
				PostQuitMessage(0);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		// ����������
		mouse_x = LOWORD(lparam);
		mouse_y = HIWORD(lparam);

		// �ж������Ƿ���������
		if (mouse_x < 0 || mouse_x > 500 || mouse_y < 75 || mouse_y > 575)
		{
			break;
		}

		// ���������ɶ�ά����
		xIndex = mouse_x / 33;
		yIndex = (mouse_y - 75) / 33;

		hdc = GetDC(hwnd);
		hdcMem = CreateCompatibleDC(hdc);

		// ����Ϊ����
		if (chess_flag == CHESS_BLACK && chess_arr[yIndex][xIndex] == 0)
		{
			SelectObject(hdcMem,hBitmap_bw);
			BitBlt(hdc,xIndex * 33,75 + yIndex * 33,30,30,hdcMem,0,0,SRCAND);
			SelectObject(hdcMem,hBitmap_bb);
			BitBlt(hdc,xIndex * 33,75 + yIndex * 33,30,30,hdcMem,0,0,SRCPAINT);
			chess_arr[yIndex][xIndex] = CHESS_BLACK;
			chess_flag = -chess_flag;
		}
		// ����Ϊ����
		else if (chess_flag == CHESS_WHITE && chess_arr[yIndex][xIndex] == 0)
		{
			SelectObject(hdcMem,hBitmap_ww);
			BitBlt(hdc,xIndex * 33,75 + yIndex * 33,30,30,hdcMem,0,0,SRCAND);
			SelectObject(hdcMem,hBitmap_wb);
			BitBlt(hdc,xIndex * 33,75 + yIndex * 33,30,30,hdcMem,0,0,SRCPAINT);
			chess_arr[yIndex][xIndex] = CHESS_WHITE;
			chess_flag = -chess_flag;
		}

		DeleteDC(hdcMem);
		ReleaseDC(hwnd,hdc);

		// �ж���Ӯ
		isWin_flag = IsWin();
		if(isWin_flag == CHESS_WHITE)
		{
			if ( MessageBox( hwnd, TEXT("�����ʤ��"), \
				TEXT("ʤ��"), MB_OK) == IDOK )
				PostQuitMessage(0);
		}
		else if(isWin_flag == CHESS_BLACK)
		{
			if ( MessageBox( hwnd, TEXT("�����ʤ��"), \
				TEXT("ʤ��"), MB_OK) == IDOK )
				PostQuitMessage(0);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd,Msg,wparam,lparam);
}

int IsWin()
{
	int count = 1;
	int row,col;
	int i,j;
	// ��������
	for(row = 0; row < CHESS_ROW; row++)
	{
		for (col = 0; col < CHESS_COL - 4; col++)
		{
			i = row;
			j = col;
			while((chess_arr[i][j] != 0) && (chess_arr[i][j] == chess_arr[i][j+1]))
			{
				count++;
				j++;
				if (count == 5)
				{
					if (chess_arr[row][col] == CHESS_BLACK)
					{
						return CHESS_BLACK;
					}
					if (chess_arr[row][col] == CHESS_WHITE)
					{
						return CHESS_WHITE;
					}
				}
			}
			count = 1;
		}
	}

	// ��������
	for(col = 0; col < CHESS_COL; col++)
	{
		for (row = 0; row < CHESS_ROW - 4; row++)
		{
			i = row;
			j = col;
			while((chess_arr[i][j] != 0) && (chess_arr[i][j] == chess_arr[i+1][j]))
			{
				count++;
				i++;
				if (count == 5)
				{
					if (chess_arr[row][col] == CHESS_BLACK)
					{
						return CHESS_BLACK;
					}
					if (chess_arr[row][col] == CHESS_WHITE)
					{
						return CHESS_WHITE;
					}
				}
			}
			count = 1;
		}
	}

	// ��������
	for(row = 0; row < CHESS_ROW - 4; row++)
	{
		for (col = 0; col < CHESS_COL - 4; col++)
		{
			i = row;
			j = col;
			while((chess_arr[i][j] != 0) && (chess_arr[i][j] == chess_arr[i+1][j+1]))
			{
				count++;
				i++;
				j++;
				if (count == 5)
				{
					if (chess_arr[row][col] == CHESS_BLACK)
					{
						return CHESS_BLACK;
					}
					if (chess_arr[row][col] == CHESS_WHITE)
					{
						return CHESS_WHITE;
					}
				}
			}
			count = 1;
		}
	}

	// ��������
	for(row = 0; row < CHESS_ROW - 4; row++)
	{
		for (col = CHESS_COL - 1; col > 3; col--)
		{
			i = row;
			j = col;
			while((chess_arr[i][j] != 0) && (chess_arr[i][j] == chess_arr[i+1][j-1]))
			{
				count++;
				i++;
				j--;
				if (count == 5)
				{
					if (chess_arr[row][col] == CHESS_BLACK)
					{
						return CHESS_BLACK;
					}
					if (chess_arr[row][col] == CHESS_WHITE)
					{
						return CHESS_WHITE;
					}
				}
			}
			count = 1;
		}
	}

	return 0;
}



