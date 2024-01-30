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
	void Initialize(DirectXCommon* dxCommon,SpriteCommon* common,std::wstring textureFilePath);
	
	void Update();

	void Draw();

	//Getter/Setter
	DirectX::XMFLOAT2 GetPosition() { return position; }
	float GetRotation() { return rotation; }
	DirectX::XMFLOAT4 GetColor() { return color_; }
	DirectX::XMFLOAT2 GetSize() { return size; }

	void SetPosition(DirectX::XMFLOAT2 pos) { position = pos; }
	void SetRotation(float rot) { rotation = rot; }
	void SetColor(DirectX::XMFLOAT4 color) { color_ = color; }
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }

	void SetTexture(std::wstring textureFilePath);

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
	VertexData* vertexData = nullptr;

	//インデックス
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	//マテリアル情報
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	MaterialData* materialData = nullptr;

	//行列情報
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;
	DirectX::XMMATRIX* wvpData = nullptr;

	

	//パラメータ
	DirectX::XMFLOAT4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//UV
	Transform uvTransform = { {1,1,1},{0,0,0},{0,0,0} };


	Transform transform = { {1,1,1},{0,0,0},{0,0,0} };
	DirectX::XMFLOAT2 position = { 0,0 };
	float rotation = 0;
	DirectX::XMFLOAT2 size = { 512,512 };

	//画像の保存されている場所
	uint32_t textureIndex_ = 0;

	//カメラ
	Transform cameraTransform = { {1,1,1},{0,0,0},{0,0,-5} };

};

