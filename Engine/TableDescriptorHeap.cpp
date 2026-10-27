#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init(uint32 count)
{
	_groupCount = count;

	// 뷰 생성: 
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = count * REGISTER_COUNT; // 각 그룹별 레지스터 개수(b0~b4라면 5개)
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// Shader Visible: GPU에 상주하는 형태
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;		// CBV 형태(셰이더리소스/순서 지정X)

	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_groupSize = _handleSize * REGISTER_COUNT;

}