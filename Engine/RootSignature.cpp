#include "pch.h"
#include "RootSignature.h"

void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	// Constant Buffer View 선언!
	CD3DX12_ROOT_PARAMETER param[2];
	param[0].InitAsConstantBufferView(0);	// 0번 -> b0 -> CBV
	param[1].InitAsConstantBufferView(1);	// 1번 -> b1 -> CBV
	// shader register: N번으로 넣으면 B(N) 레지스터를 보는 식. 보통, b0, b0처럼 같은 레지스터를 참조하면 문제가 생김.
	// b0-0, b0-1과 같이 구분할 수도 있는데 굳이?
	// Visibility: 다른 쉐이더에서도 볼 수 있는 변수인가? 기본은 D3D12_SHADER_VISIBILITY_ALL(모두 볼 수 있음)

	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(2, param);
	 sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조합기 단계(IA)

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}