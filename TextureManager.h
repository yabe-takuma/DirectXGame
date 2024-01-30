#pragma once
#include<string>
#include"DirectXCommon.h"
#include <DirectXTex.h>
class TextureManager
{
private:
	struct TextureData {
		std::wstring filePath;
		DirectX::TexMetadata metaData;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};


public:
	//シングルトン
	static TextureManager* Getinstance();
	void Finalize();

	//初期化
	void Initialize(DirectXCommon* dxCommon);

	//画像読み込み
	void LoadTexture(const std::wstring& filePath);

	//指定した画像の何番目にあるのか確認
	uint32_t GetTextureIndexFilePath(const std::wstring& filePath);
	//指定した要素番号のGPUハンドルを受け取る
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);


private:
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

private:
	static TextureManager* instance;
	static uint32_t kSRVIndexTop;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

private:
	DirectXCommon* dxCommon_ = nullptr;


	std::vector<TextureData> textureDatas;

};

