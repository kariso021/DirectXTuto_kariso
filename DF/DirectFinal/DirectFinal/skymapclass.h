#pragma once

#include<d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>
#include<fstream>
#include<vector>


using namespace std;
using namespace DirectX;


class Skymapclass
{

private:
	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz,
			float tx, float ty, float tz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz),
			tangent(tx, ty, tz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
		///////////////**************new**************////////////////////
		XMFLOAT3 tangent;
		XMFLOAT3 biTangent;
		///////////////**************new**************////////////////////
	};
	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;

		//These will be used for the pixel shader
		XMFLOAT4 difColor;
		BOOL hasTexture;
		///////////////**************new**************////////////////////
		//Because of HLSL structure packing, we will use windows BOOL
		//instead of bool because HLSL packs things into 4 bytes, and
		//bool is only one byte, where BOOL is 4 bytes
		BOOL hasNormMap;
		///////////////**************new**************////////////////////
	};
	struct CameraBufferType
	{

	};

public:
	Skymapclass();
	Skymapclass(const Skymapclass&);
	~Skymapclass();

	bool CreateSphere(ID3D11Device* device, int LatLines, int LongLines, HWND hwnd);
	bool Initialize(ID3D11Device* device, int LatLines, int LongLines, HWND hwnd);
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection);
	void Shutdown();
private:

	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;
	ID3D11SamplerState* CubesTexSamplerState;

	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX WVP;
	XMMATRIX Rotationx;
	XMMATRIX Rotationz;
	XMMATRIX Rotationy;
	cbPerObject cbPerObj;
	ID3D11Buffer* cbPerObjectBuffer;

	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11ShaderResourceView* smrv;
	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;
};

