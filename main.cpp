#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    //ポインタ置き場
    Input* input_ = nullptr;
    WinApp* winApp_ = nullptr;
    DirectXCommon* dxCommon_ = nullptr;

#pragma region WindowsAPI初期化処理
    winApp_ = new WinApp();
    winApp_->Initialize();
#pragma endregion

#pragma region DirectX初期化処理
    // DirectX初期化処理　ここから
    dxCommon_ = new DirectXCommon();
    dxCommon_->Initialize(winApp_);

    // DirectX初期化処理　ここまで
#pragma endregion

    //input生成,初期化
    input_ = new Input();
    input_->Initialize(winApp_);

    // ゲームループ
    while (true) {
        if (winApp_->Update() == true){
            break;
        }

        input_->Update();

        //更新前処理
        dxCommon_->PreDraw();
      
        // ４．描画コマンドここまで

        dxCommon_->PostDraw();

        // DirectX毎フレーム処理　ここまで

    }

    delete input_;
    delete dxCommon_;

    winApp_->Finalize();
    delete winApp_;

  

    return 0;
}
