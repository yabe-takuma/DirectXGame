#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"ImGuiManager.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    //ポインタ置き場
    Input* input_ = nullptr;
    WinApp* winApp_ = nullptr;
    DirectXCommon* dxCommon_ = nullptr;
    SpriteCommon* spriteCommon = nullptr;
    Sprite* sprite = nullptr;

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

    ImGuiManager* imgui = ImGuiManager::Create();
    ImGuiManager::Initialize(winApp_->GetHwnd(), dxCommon_);

    //スプライトコモン
    spriteCommon = new SpriteCommon();
    spriteCommon->Initialize(dxCommon_);

    //スプライト
    sprite = new Sprite();
    sprite->Initialize(dxCommon_,spriteCommon);

    // ゲームループ
    while (true) {
        if (winApp_->Update() == true){
            break;
        }
        ImGuiManager::NewFrame();
        imgui->ShowDemo();


        input_->Update();

        sprite->Update();

        //更新前処理
        ImGuiManager::CreateCommand();
        dxCommon_->PreDraw();
      
        sprite->Draw();

        // ４．描画コマンドここまで
        ImGuiManager::CommandsExcute(dxCommon_->GetCommandList());
        dxCommon_->PostDraw();

        // DirectX毎フレーム処理　ここまで

    }

    delete sprite;
    delete spriteCommon;
    delete input_;
    delete dxCommon_;
    

    winApp_->Finalize();
    delete winApp_;
  
  

    return 0;
}
