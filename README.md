# DX12Study

DirectX 12를 공부하면서 진행한 프로젝트입니다.

해당 프로젝트는 Rookiss님의 인프런 강의 ["[C++과 언리얼로 만드는 MMORPG 게임 개발 시리즈] Part2: 게임 수학과 DirectX12"](https://inf.run/LkdC7)를 기반으로 제작하였습니다.

개발 기간: 2026.08.19.~(진행중)

## 출처

- `Engine/d3dx12.h`: Microsoft DirectX-Graphics-Samples에서 가져온 DX12 helper header

## 빌드 및 실행 (Visual Studio)

Visual Studio 2022 이상 + "C++를 사용한 데스크톱 개발" 워크로드가 필요합니다.
(CMake 도구는 해당 워크로드에 포함되어 있습니다.)

### 1) 폴더 열기 (권장)

Visual Studio에서 **파일 > 열기 > 폴더**로 저장소 루트를 엽니다.
`CMakePresets.json`이 자동으로 인식되므로 구성 드롭다운에서 `Visual Studio (x64)`를
고르고 `Client.exe`를 시작 항목으로 선택한 뒤 F5를 누르면 됩니다.

### 2) .sln 생성

기존처럼 솔루션 파일로 작업하고 싶다면 개발자 명령 프롬프트에서:

```
cmake --preset msvc
```

`build/msvc/DX12Study.sln`이 생성되며, 시작 프로젝트는 `Client`로 설정되어 있습니다.

명령줄로만 빌드하려면:

```
cmake --build --preset debug
```

빌드 결과물은 `Output/`에 생성됩니다. `Game::Init`이 셰이더를
`..\Resources\Shader\default.hlsli` 상대 경로로 읽기 때문에 작업 디렉터리는
반드시 `Output/`이어야 하며, CMake가 이를 자동으로 설정합니다.
