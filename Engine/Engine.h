#pragma once



class Engine
{
public:

	void Init(const WindowInfo& info);
	void Render();

public:
	void RenderBegin();	// 커맨드 큐에 렌더링에 필요한 요소들을 전달하는 과정
	void RenderEnd();	// 커맨드 큐의 요소를 실제 GPU에 전달하여 렌더링 수행

	void ResizeWindow(int32 width, int32 height);

private:
	// 그려질 화면 크기 관련
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};


	shared_ptr<class Device> _device;
	shared_ptr<class CommandQueue> _cmdQueue;
	shared_ptr<class SwapChain> _swapChain;
	shared_ptr<class DescriptorHeap> _descHeap;
};

