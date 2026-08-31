#pragma once


// [기안서]
// 외주를 맡길 때 이런 저런 정보들을 같이 넘겨줘야 하는데,
// 아무 형태로나 요청하면 못 알아먹는다
// - 각종 리소스를 어떤 용도로 사용하는지 꼼꼼하게 적어서 넘겨주는 용도
// (이전에는 View라고 불렀으나 이제는 Descriptor, 서술자라고 부르기 시작)
class DescriptorHeap
{
public:
	void Init(ComPtr<ID3D12Device> device, shared_ptr<class SwapChain> swapChain);

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTV(int32 idx) { return _rtvHandle[idx]; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetBackBufferView();


private:
	// rtv: 렌더 타겟 뷰의 약자(실제 리소스!)
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	uint32							_rtvHeapSize = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	shared_ptr<class SwapChain>		_swapChain;

};

