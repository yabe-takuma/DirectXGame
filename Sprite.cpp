#include "Sprite.h"

#include"BufferResource.h"

#include"External/imgui/imgui.h"

#include<DirectXMath.h>
using namespace DirectX;
using namespace Microsoft::WRL;

void Sprite::Initialize(DirectXCommon* dxCommon,SpriteCommon* common)
{
	dxCommon_ = dxCommon;
	common_ = common;

	//Texture
	DirectX::ScratchImage mipImages = common->LoadTexture(L"Resources/mario.jpg");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTextureResource(dxCommon->GetDevice(), metaData);
	common_->UploadTextureData(textureResource, mipImages);
	

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metaData.mipLevels);

	//保存メモリの場所を指定
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = dxCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU = dxCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();

	textureSrvHandleCPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//読み込んだ情報をSrvDesc(枠)とHandle(位置)を使って保存する
	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);

	//頂点情報
	CreateVertex();
	//インデックス情報
	CreateIndex();
	//色
	CreateMaterial();
	//行列
	CreateWVP();
}

void Sprite::Update()
{
	transform.translate = position;

	ImGui::Begin("Texture");
	ImGui::DragFloat3("Pos", &transform.translate.x, 0.1f);

	ImGui::DragFloat3("UV-Pos", &uvTransform.translate.x, 0.01f, -10.f, 10.f);
	ImGui::SliderAngle("UV-Rot", &uvTransform.rotate.z);
	ImGui::DragFloat3("UV-Scale", &uvTransform.scale.x, 0.01f, -10.f, 10.f);

	ImGui::End();
}

void Sprite::Draw()
{
	//transform.rotate.y += 0.03f;
	//ワールド
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotate));
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.translate));

	XMMATRIX rotationAndScaleMatirx = XMMatrixMultiply(rotateMatrix, scaleMatrix);

	XMMATRIX worldMatrix = XMMatrixMultiply(rotationAndScaleMatirx, translationMatrix);
	//行列の代入
	*wvpData = worldMatrix;

	//カメラ
	XMMATRIX cameraScaleMatrix= XMMatrixScalingFromVector(XMLoadFloat3(&cameraTransform.scale));
    XMMATRIX cameraRotateMatrix= XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&cameraTransform.rotate));
    XMMATRIX cameraTranslationMatrix= XMMatrixTranslationFromVector(XMLoadFloat3(&cameraTransform.translate));
    
    XMMATRIX cameraRotationAndScaleMatirx = XMMatrixMultiply(cameraRotateMatrix, cameraScaleMatrix);
    
    XMMATRIX cameraMatrix = XMMatrixMultiply(cameraRotationAndScaleMatirx, cameraTranslationMatrix);
    
    XMMATRIX view = XMMatrixInverse(nullptr,cameraMatrix);
    XMMATRIX proj = XMMatrixPerspectiveFovLH(
	XMConvertToRadians(45.f),
	(float)WinApp::window_width / (float)WinApp::window_height,
	0.1f,
	100.f
);

	//WVP
	XMMATRIX worldViewProjectionMatrix = worldMatrix * (view * proj);

	//行列の代入
	*wvpData = worldViewProjectionMatrix;
	
	//UV
	XMMATRIX uvscaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&uvTransform.scale));
	XMMATRIX uvrotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&uvTransform.rotate));
	XMMATRIX uvtranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&uvTransform.translate));

	XMMATRIX uvrotationAndScaleMatirx = XMMatrixMultiply(uvrotateMatrix, uvscaleMatrix);

	XMMATRIX uvworldMatrix = XMMatrixMultiply(uvrotationAndScaleMatirx, uvtranslationMatrix);
	materialData->uvTransform = uvworldMatrix;

	dxCommon_->GetCommandList()->SetGraphicsRootSignature(common_->GetRootSignature());
	dxCommon_->GetCommandList()->SetPipelineState(common_->GetPipelineState());
	//頂点情報
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//インデックス情報
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//色
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//行列
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	//画像
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
	//頂点情報のみ描画
	//dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);

	//インデックス情報がある場合の描画
	dxCommon_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);

}

void Sprite::CreateVertex()
{
	//VertexResource
	vertexResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 4);

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };

	vertexData[1].position = { -0.5f,+0.5f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.0f,0.0f };

	vertexData[2].position = { +0.5f,-0.5f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };

	vertexData[3].position = { +0.5f,+0.5f,0.0f,1.0f };
	vertexData[3].texcoord = { 1.0f,0.0f };

	

}

void Sprite::CreateIndex()
{
	indexResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(uint32_t) * 6);

	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	//VertexData{0,1,2}の頂点で三角形を一枚作成
	indexData[0] = 0;     indexData[1] = 1;     indexData[2] = 2;

	//VertexData{1,3,2}の頂点で三角形を一枚作成
	indexData[3] = 1;     indexData[4] = 3;     indexData[5] = 2;


}

void Sprite::CreateMaterial()
{
	materialResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(MaterialData));

	
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	materialData->color = color_;
	materialData->uvTransform = XMMatrixIdentity();
}

void Sprite::CreateWVP()
{
	wvpResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMMATRIX));

	
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	*wvpData = XMMatrixIdentity();
}


