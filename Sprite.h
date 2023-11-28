#pragma once
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include<wrl.h>
#include<d3d12.h>
class Sprite
{
public:
	//初期化
	void Initialize(DirectXCommon* dxCommon,SpriteCommon* common);
	
	void Draw();
private:
	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* common_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
};

