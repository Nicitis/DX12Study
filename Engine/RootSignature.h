#pragma once

// [계약서 / 결재]
// 
// CPU [           ]    GPU [         ]
// CPU에서 GPU로 외주를 맡겨야 하지만, 둘의 저장 공간은 다름
// 한국 [           ]   GPU [         ]

class RootSignature
{
public:
	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	ComPtr<ID3D12RootSignature> _signature;
};

