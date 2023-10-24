#pragma once
#include <Windows.h>
//WindowsAPI
class WinApp
{
public: //メンバ関数

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//初期化
	void Initialize();
	//更新
	bool Update();
	//後処理
	void Finalize();
	//Getter
	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }

public:
	// ウィンドウサイズ
	static const int window_width = 1280;  // 横幅
	static const int window_height = 720;  // 縦幅

private:
	HWND hwnd;
	WNDCLASSEX w{};

	MSG msg{};  // メッセージ

};

