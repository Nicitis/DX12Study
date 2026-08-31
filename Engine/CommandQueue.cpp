#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain, shared_ptr<DescriptorHeap> descHeap)
{
	_swapChain = swapChain;
	_descHeap = descHeap;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));

	// - D3D12_COMMAND_LIST_TYPE_DIRECT : GPU가 직접 실행하는 명령 목록
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	// GPU가 하나인 시스템에서는 0으로
	// DIRECT or BUNDLE
	// Allocator
	// 초기 상태 (그리기 명령은 nullptr 지정)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));

	// CommandList는 Close / Open 상태가 있는데
	// Open 상태에서 Command를 넣다가 Close한 다음 제출하는 개념
	_cmdList->Close();

	// CreateFence
	// - CPU와 GPU의 동기화 수단으로 쓰인다
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

//WaitSync: 기본적인 wait 이벤트의 형태
void CommandQueue::WaitSync()
{
	// Advance the fence value to mark commands up to this fence point.
	_fenceValue++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		// Fire event when GPU hits current fence.  
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}

void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	// 현재 백버퍼 리소스를 얻어 이를 GPU 작업 용도로 활용하겠다, 를 선언
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetCurrentBackBufferResource().Get(),	// 1. 현재 백버퍼 리소스 획득
		D3D12_RESOURCE_STATE_PRESENT,						// 2. 화면 출력
		D3D12_RESOURCE_STATE_RENDER_TARGET);				// 3. 외주 결과물

	_cmdList->ResourceBarrier(1, &barrier);

	// Set the viewport scissor rect. This needs to be reset whenever the command list is reset.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// Specify the buffers we are going to render to.
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _descHeap->GetBackBufferView();		// 백버퍼 설정
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr); // 기본 색 설정
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);
}

void CommandQueue::RenderEnd()
{
	// RenderBegin의 역순: RT에서 Present 순
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetCurrentBackBufferResource().Get(),	// 1. 현재 백버퍼 리소스 획득
		D3D12_RESOURCE_STATE_RENDER_TARGET,					// 2. 외주 결과물
		D3D12_RESOURCE_STATE_PRESENT);						// 3. 화면 출력

	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close(); // 배리어닫기

	// 커맨드 리스트 수행 
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	_swapChain->Present();

	// 프레임 커맨드가 완료될 때까지 대기한다. 이 방법 자체는 사실 단순화를 위해
	// 비효율적으로 굴리는건데, 나중에 좀 더 효율적인 렌더링 방법을 알아볼 예정.
	WaitSync();

	_swapChain->SwapIndex(); // 스왑
}