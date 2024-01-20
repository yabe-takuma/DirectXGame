#pragma once
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include<wrl.h>

#include<DirectXMath.h>

#include<d3d12.h>
class Sprite
{
private:
	struct Transform {
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 rotate;
		DirectX::XMFLOAT3 translate;
	};

	struct VertexData {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texcoord;
	};
	//マテリアル
	struct MaterialData {
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX uvTransform;
	};

public:
	//初期化
	void Initialize(DirectXCommon* dxCommon,SpriteCommon* common);
	
	void Update();

	void Draw();

private:
	//頂点情報作成
	void CreateVertex();
	//インデックス情報作成
	void CreateIndex();
	//マテリアル情報作成
	void CreateMaterial();
	//行列情報作成
	void CreateWVP();

private:
	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* common_ = nullptr;
	//頂点情報
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	//インデックス
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	//マテリアル情報
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	MaterialData* materialData = nullptr;

	//行列情報
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;
	DirectX::XMMATRIX* wvpData = nullptr;

	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

	//パラメータ
	DirectX::XMFLOAT4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//UV
	Transform uvTransform = { {1,1,1},{0,0,0},{0,0,0} };


	Transform transform = { {1,1,1},{0,0,0},{0,0,0} };

	//カメラ
	Transform cameraTransform = { {1,1,1},{0,0,0},{0,0,-5} };

};

