#pragma once
class ConstantBuffer
{


public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	void PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);

private:
	void CreateBuffer();

private:
	ComPtr<ID3D12Resource>	_cbvBuffer;					// 이것 하나가 Constant Buffer 모두를 가리킴
	BYTE*					_mappedBuffer = nullptr;	// Memcpy용
	uint32					_elementSize = 0;			
	uint32					_elementCount = 0;
	
	uint32					_currentIndex = 0;
};

