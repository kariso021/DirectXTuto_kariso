
#include "skymapclass.h"
#include "DDSTextureLoader.h"


Skymapclass::Skymapclass()
{

}

Skymapclass::Skymapclass(const Skymapclass& other)
{
}


Skymapclass::~Skymapclass()
{
}

bool Skymapclass::CreateSphere(ID3D11Device* device, int LatLines, int LongLines, HWND hwnd)
{
    HRESULT result;

    NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
    NumSphereFaces = ((LatLines - 3) * (LongLines) * 2) + (LongLines * 2);

    float sphereYaw = 0.0f;
    float spherePitch = 0.0f;

    std::vector<Vertex> vertices(NumSphereVertices);

    XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    vertices[0].pos.x = 0.0f;
    vertices[0].pos.y = 0.0f;
    vertices[0].pos.z = 1.0f;

    for (DWORD i = 0; i < DWORD(LatLines - 2); ++i)
    {
        spherePitch = (float)(i + 1) * (3.14f / (float)(LatLines - 1));
        Rotationx = XMMatrixRotationX(spherePitch);
        for (DWORD j = 0; j < DWORD(LongLines); ++j)
        {
            sphereYaw = (float)j * (6.28f / (float)(LongLines));
            Rotationy = XMMatrixRotationZ(sphereYaw);
            currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
            currVertPos = XMVector3Normalize(currVertPos);
            vertices[i * LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
            vertices[i * LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
            vertices[i * LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
        }
    }

    vertices[NumSphereVertices - 1].pos.x = 0.0f;
    vertices[NumSphereVertices - 1].pos.y = 0.0f;
    vertices[NumSphereVertices - 1].pos.z = -1.0f;


    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;

    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = &vertices[0];
    result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


    std::vector<DWORD> indices(NumSphereFaces * 3);

    int k = 0;
    for (DWORD l = 0; l < DWORD(LongLines - 1); ++l)
    {
        indices[k] = 0;
        indices[k + 1] = l + 1;
        indices[k + 2] = l + 2;
        k += 3;
    }

    indices[k] = 0;
    indices[k + 1] = LongLines;
    indices[k + 2] = 1;
    k += 3;

    for (DWORD i = 0; i < DWORD(LatLines - 3); ++i)
    {
        for (DWORD j = 0; j < DWORD(LongLines - 1); ++j)
        {
            indices[k] = i * LongLines + j + 1;
            indices[k + 1] = i * LongLines + j + 2;
            indices[k + 2] = (i + 1) * LongLines + j + 1;

            indices[k + 3] = (i + 1) * LongLines + j + 1;
            indices[k + 4] = i * LongLines + j + 2;
            indices[k + 5] = (i + 1) * LongLines + j + 2;

            k += 6; // next quad
        }

        indices[k] = (i * LongLines) + LongLines;
        indices[k + 1] = (i * LongLines) + 1;
        indices[k + 2] = ((i + 1) * LongLines) + LongLines;

        indices[k + 3] = ((i + 1) * LongLines) + LongLines;
        indices[k + 4] = (i * LongLines) + 1;
        indices[k + 5] = ((i + 1) * LongLines) + 1;

        k += 6;
    }

    for (DWORD l = 0; l < (DWORD)(LongLines - 1); ++l)
    {
        indices[k] = NumSphereVertices - 1;
        indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
        indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
        k += 3;
    }

    indices[k] = NumSphereVertices - 1;
    indices[k + 1] = (NumSphereVertices - 1) - LongLines;
    indices[k + 2] = NumSphereVertices - 2;

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;

    iinitData.pSysMem = &indices[0];
    device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);

    //Create the buffer to send to the cbuffer in effect file
    D3D11_BUFFER_DESC cbbd;
    ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

    cbbd.Usage = D3D11_USAGE_DEFAULT;
    cbbd.ByteWidth = sizeof(cbPerObject);
    cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbbd.CPUAccessFlags = 0;
    cbbd.MiscFlags = 0;

    result = device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
    if (FAILED(result))
    {
        MessageBox(hwnd, L"cbPerObjectBuffer.", L"Error", MB_OK);
        return false;
    }

    result = D3DCompileFromFile(L"./data/Effects.fx", 0, 0, "SKYMAP_VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &SKYMAP_VS_Buffer, 0);
    if (FAILED(result))
    {
        MessageBox(hwnd, L"SKYMAP_VS.", L"Error", MB_OK);
        return false;
    }
    result = D3DCompileFromFile(L"./data/Effects.fx", 0, 0, "SKYMAP_PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &SKYMAP_PS_Buffer, 0);
    if (FAILED(result))
    {
        MessageBox(hwnd, L"SKYMAP_PS.", L"Error", MB_OK);
        return false;
    }
    result = device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
    if (FAILED(result))
    {
        MessageBox(hwnd, L"CreateVertexShader.", L"Error", MB_OK);
        return false;
    }
    result = device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);
    if (FAILED(result))
    {
        MessageBox(hwnd, L"CreatePixelShader.", L"Error", MB_OK);
        return false;
    }

    ID3D11Texture2D* SMTexture = 0;
    result = CreateDDSTextureFromFileEx(device, L"./data/skymap.dds", 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE,
        0, D3D11_RESOURCE_MISC_TEXTURECUBE, false, (ID3D11Resource**)&SMTexture, nullptr);
    if (FAILED(result))
    {
        MessageBox(hwnd, L"CreateDDSTextureFromFileEx.", L"Error", MB_OK);
        return false;
    }

    D3D11_TEXTURE2D_DESC SMTextureDesc;
    SMTexture->GetDesc(&SMTextureDesc);

    D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
    SMViewDesc.Format = SMTextureDesc.Format;
    SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
    SMViewDesc.TextureCube.MostDetailedMip = 0;

    result = device->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);
    if (FAILED(result))
    {
        MessageBox(hwnd, L"CreateShaderResourceView.", L"Error", MB_OK);
        return false;
    }

    D3D11_RASTERIZER_DESC cmdesc;

    ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
    cmdesc.FillMode = D3D11_FILL_SOLID;
    cmdesc.CullMode = D3D11_CULL_BACK;
    cmdesc.FrontCounterClockwise = true;
    result = device->CreateRasterizerState(&cmdesc, &CCWcullMode);

    cmdesc.FrontCounterClockwise = false;

    result = device->CreateRasterizerState(&cmdesc, &CWcullMode);
    cmdesc.CullMode = D3D11_CULL_NONE;
    result = device->CreateRasterizerState(&cmdesc, &RSCullNone);
    if (FAILED(result))
    {
        MessageBox(hwnd, L"CreateRasterizerState.", L"Error", MB_OK);
        return false;
    }

    D3D11_DEPTH_STENCIL_DESC dssDesc;
    ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dssDesc.DepthEnable = true;
    dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    result = device->CreateDepthStencilState(&dssDesc, &DSLessEqual);
    if (FAILED(result))
    {
        MessageBox(hwnd, L"CreateDepthStencilState.", L"Error", MB_OK);
        return false;
    }

    return true;
}

bool Skymapclass::Initialize(ID3D11Device* device, int LatLines, int LongLines, HWND hwnd)
{
    bool result;


    // Initialize the vertex and pixel shaders.
    result = CreateSphere(device, LatLines, LongLines, hwnd);
    if (!result)
    {
        return false;
    }

    return true;
}

bool Skymapclass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
    bool result;
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    // Set the spheres index buffer
    deviceContext->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    // Set the spheres vertex buffer
    deviceContext->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);

    // Set the world view projection matrix and send it to the constant buffer in effect file
    WVP = world * view * projection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    cbPerObj.World = XMMatrixTranspose(world);
    deviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    deviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    // Send our skymap resource view to pixel shader
    deviceContext->PSSetShaderResources(0, 1, &smrv);
    deviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);

    // Set the new VS and PS shaders
    deviceContext->VSSetShader(SKYMAP_VS, 0, 0);
    deviceContext->PSSetShader(SKYMAP_PS, 0, 0);
    // Set the new depth/stencil and RS states
    deviceContext->OMSetDepthStencilState(DSLessEqual, 0);
    deviceContext->RSSetState(RSCullNone);
    deviceContext->DrawIndexed(NumSphereFaces * 3, 0, 0);

    // Set the default VS shader and depth/stencil state
    deviceContext->OMSetDepthStencilState(NULL, 0);

    return true;
}

void Skymapclass::Shutdown()
{
    if (sphereIndexBuffer)
    {
        sphereIndexBuffer->Release();
        sphereIndexBuffer = 0;
    }

    // Release the vertex buffer.
    if (sphereVertBuffer)
    {
        sphereVertBuffer->Release();
        sphereVertBuffer = 0;
    }

    SKYMAP_VS->Release();
    SKYMAP_PS->Release();
    SKYMAP_VS_Buffer->Release();
    SKYMAP_PS_Buffer->Release();

    smrv->Release();

    DSLessEqual->Release();
    RSCullNone->Release();

    return;
}