#include <windows.h>
#include <time.h>
#include "AudioClip.cpp" 
using namespace std; 
typedef  int(__stdcall* w32mci)(const char*, char*, int,  int);
typedef int(__stdcall *  w32mcierror)(int, char*, int);
HINSTANCE this_windows;
AudioClip muli;

bool playSound=false;
bool playSoundEvent=false;

class Button{
	public:
		bool isEmpty=true;
		int stx;//��ʼx 
		int sty;//��ʼy 
		int _x;//x 
		int _y;//y 
		LPCSTR path;//bmp·�� δѡ�� 
		LPCSTR path_in;//��ѡ��
		LPCSTR path_down;//�Ѱ��� 
		bool isDown; 
		bool *show;
		//���� 
		set(int x,int y,int x_,int y_,LPCSTR path_,LPCSTR path_in_,LPCSTR path_down_,bool *show_){
			stx=x;
			sty=y;
			_x=x_;
			_y=y_;
			path=path_;
			path_in=path_in_;
			path_down=path_down_;
			isEmpty=false;
			show=show_;
		}
		update(int x_,int y_){
			stx=x_;
			sty=y_;
		}
		in(int x,int y){
			return (x>=stx && x<=(stx+_x) && y>=sty && y<=(sty+_y));
		}
	
}; 
Button newButton(int x,int y,int x_,int y_,LPCSTR path_,LPCSTR path_in_,LPCSTR path_down_,bool *show_){
	Button button;
	button.set(x, y, x_,y_,path_,path_in_,path_down_,show_);
	return button;
}
void  paint_button(HWND hwnd,HDC hdc,Button button){//����һ��ͼƬ��ʵ�� 
	//����ͼƬ 
	if(*(button.show)==true){
		HBITMAP bitmap=(HBITMAP)(LoadImageA(this_windows,button.path,IMAGE_BITMAP,0,0,0x0010));
	//����ͼƬ 
	//����DC
	HDC ohdc=CreateCompatibleDC(hdc);
	SelectObject(ohdc,bitmap);
	//��ͼ
	BitBlt(hdc,button.stx,button.sty,
	(button._x),//ͼƬ��� 
	(button._y),//ͼƬ�߶� 
	ohdc,0,0,SRCCOPY);
	}
}
long long time_this; 
bool canshow_button_back=false;
LPCSTR background_str="Data//data.rpkg";
Button button_start=newButton(0,0,60,20,"Data//082914%nk65","","",&canshow_button_back);
Button button_add=newButton(0,0,60,20,"Data//local.dll","","",&canshow_button_back);
Button button_back=newButton(0,0,80,50,"Data//math.dll","","",&canshow_button_back);
//class Buttons{
//	private:
//		Button Empty=Button();
//		Button button[]=new Button();
//		int long_=0;
//	public:
//		void register_(Button botton_){
//			button[long_]=botton_;
//			long_++;
//		}
//		Button get(int index){
//			if(index<=long_){
//				return button[index];
//			}
//			else return Empty;
//		}
//		void Paint_Button(HWND hwnd,HDC hdc,HBITMAP background){
//			//����ͼƬ 
//			//����DC
//			HDC ohdc=CreateCompatibleDC(hdc);
//			SelectObject(ohdc,background);
//			//��ͼ
//			for(int i=0;i<=long_;i++){
//				if(!button[i].isEmpty){
//					paint_button(hwnd,ohdc,button[i]);
//				}
//			}
//			//�ϲ� 
//			BitBlt(hdc,0,0,
//			(640),//ͼƬ��� 
//			(480),//ͼƬ�߶� 
//			ohdc,0,0,SRCCOPY);
//		}
//};
//Buttons main;
void  Paint_background(HWND hwnd,HDC hdc,LPCSTR str){
	
	HBITMAP bitmap=(HBITMAP)(LoadImageA(this_windows,str,IMAGE_BITMAP,0,0,0x0010));
	HDC ohdc=CreateCompatibleDC(hdc);
	SelectObject(ohdc,bitmap);
	
	BitBlt(hdc,0,0,
	640,
	480,
	ohdc,0,0,SRCCOPY);
}
void Windows_paint(HDC hdc,HWND hwnd){
	//���μ���
	//��ȡ�ͻ��˴�С 
	RECT rt;
	GetClientRect(hwnd,&rt);
	int WC_width=rt.right-rt.left; 
	int WC_higth=rt.bottom-rt.top;
	//����λͼ 
	HBITMAP background=CreateCompatibleBitmap(hdc,WC_width,WC_higth);
	//��������DC
	HDC ohdc=CreateCompatibleDC(hdc);
	SelectObject(ohdc,background);
	//������Ʒ
	Paint_background(hwnd,ohdc,background_str);
	//TextOut(ohdc,0,0,"����һ�����֣�",12);
	paint_button(hwnd,ohdc,button_back);
	paint_button(hwnd,ohdc,button_start);
	paint_button(hwnd,ohdc,button_add);
	//�ϲ� 
	BitBlt(hdc,0,0,WC_width,WC_higth,ohdc,0,0,SRCCOPY);
} 
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;//����
	HDC hdc;//��ͼ���
	switch(Message) {
		case WM_PAINT:{
			//��ʼ��ͼ
			hdc=BeginPaint(hwnd,&ps);
			//��ͼ
			Windows_paint(hdc,hwnd);
			//������ͼ 
			EndPaint(hwnd,&ps);
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND:{
			switch(LOWORD(wParam)) {
				case 1002:{
					break;
				} 
				case 1004:
					break;
			}
			break;
		}
		case WM_LBUTTONDOWN:{//���������ʱ 
			//��ȡ������� 
			int cx=LOWORD(lParam);
			int cy=HIWORD(lParam);
			//dialog�ƽ� 
			if(background_str!="Data//www.dll"){
				bool doit=false;
				if(*(button_start.show)){//���¿�ʼʱ 
					if(button_start.in(cx,cy)){
						background_str="Data//www.dll";
						canshow_button_back=false;
						time_this=time(0);
						doit=true;
						playSoundEvent=true;
					}
				}
				if((!doit) && *(button_add.show)){//���¿�ʼʱ 
					if(button_add.in(cx,cy)){
						system("explorer \"https://jq.qq.com/?_wv=1027&k=T1FILR8I\"");
						doit=true;
					}
				}
					
				if(!doit){
					if(*(button_back.show)){
						if(!(button_back.in(cx,cy)))canshow_button_back=false;
						doit=true;
					}
					else{
						button_back.update(cx,cy);
						button_add.update(button_back.stx+10,button_back.sty+5);
						button_start.update(button_back.stx+10,button_back.sty+25);
						canshow_button_back=true;	
						doit=true;
					}
				}
				if(doit)playSound=true;
				
				//�˵��� 
			}
			else{
			}
			
			InvalidateRect(hwnd,NULL,TRUE);//���� 
			break;
		} 
//		case WM_CLOSE:{
//			MessageBox(hwnd,"helloworld","helloworld",NULL);
//			break;
//		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}
void gamemain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow,HWND hwnd){
	if(time(0)-time_this>=30 && time_this!=0){
		if(MessageBox(hwnd,"��������������߼��������ǽ���á�","���ʳ�ʱ��",MB_OK|MB_ICONWARNING)==IDOK){
			time_this=time(0);
			playSoundEvent=true;
		}
	}
	if(playSound){
		muli.play(2500,3200);
		playSound=false;
	}
	if(playSoundEvent){
		muli.play(0,27000);
		playSoundEvent=false;
	}
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Solar Story v1.14.5.14 ���������",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		530, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	this_windows= hInstance;
	muli.load("Data//music.mp3");

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		gamemain(hInstance,  hPrevInstance, lpCmdLine,  nCmdShow,hwnd);
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
