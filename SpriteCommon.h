#pragma once
#include<Windows.h>
#include<wrl.h>
#include <string>
#include<dxcapi.h>
#include <d3d12.h>

#include"DirectXCommon.h"

#include<DirectXTex.h>

class SpriteCommon
{
public:
	//初期化
	void Initialize(DirectXCommon* dxCommon);

	//Getter
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }



private:
	IDxcBlob* CompilerShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

private:
	DirectXCommon* dxCommon_=nullptr;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
};

