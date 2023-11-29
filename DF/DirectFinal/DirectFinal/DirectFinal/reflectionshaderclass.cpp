////////////////////////////////////////////////////////////////////////////////
// Filename: reflectionshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "reflectionshaderclass.h"


ReflectionShaderClass::ReflectionShaderClass()
{
    m_vertexShader = 0;
    m_pixelShader = 0;
    m_layout = 0;
    m_matrixBuffer = 0;
    m_sampleState = 0;
    m_reflectionBuffer = 0;
}


ReflectionShaderClass::ReflectionShaderClass(const ReflectionShaderClass& other)
{
}


ReflectionShaderClass::~ReflectionShaderClass()
{
}


bool ReflectionShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    bool result;
    // ���� �� �ȼ� ���̴��� �ʱ�ȭ�մϴ�.
    result = InitializeShader(device, hwnd, L"./data/reflection.vs", L"./data/reflection.ps");
    if (!result)
    {
        return false;
    }

    return true;
}


void ReflectionShaderClass::Shutdown()
{
    // ���� �� �ȼ� ���̴�, �׸��� ���õ� ��ü���� �����մϴ�.
    ShutdownShader();

    return;
}

bool ReflectionShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix,
    XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture,
    ID3D11ShaderResourceView* reflectionTexture, XMMATRIX reflectionMatrix)
{
    bool result;


    // �������� ����� ���̴� ���ڵ��� �����մϴ�.
    result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, reflectionTexture,
        reflectionMatrix);
    if (!result)
    {
        return false;
    }

    // ���̴��� �̿��Ͽ� ���ۿ� �������մϴ�.
    RenderShader(deviceContext, indexCount);

    return true;
}


bool ReflectionShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename)
{
    HRESULT result;
    ID3D10Blob* errorMessage;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc;
    D3D11_SAMPLER_DESC samplerDesc;
    D3D11_BUFFER_DESC reflectionBufferDesc;


    // �� �Լ����� ����� �����͵��� null�� �ʱ�ȭ�մϴ�.
    errorMessage = 0;
    vertexShaderBuffer = 0;
    pixelShaderBuffer = 0;
    // ���� ���̴��� �������մϴ�.
    result = D3DCompileFromFile(vsFilename, NULL, NULL, "ReflectionVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
NULL, &vertexShaderBuffer,&errorMessage);
    if (FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
        }

        return false;
    }
    // �ȼ� ���̴��� �������մϴ�.
    result = D3DCompileFromFile(psFilename, NULL, NULL, "ReflectionPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
        0, & pixelShaderBuffer, & errorMessage);


    if (FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
        }
        // If there was  nothing in the error message then it simply could not find the file itself.
        else
        {
            MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
        }

        return false;
    }

    // ���ۿ� ���� ���̴��� �����մϴ�.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL,
        & m_vertexShader);
    if (FAILED(result))
    {
        return false;
    }

    // ���ۿ� �ȼ� ���̴��� �����մϴ�.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL,
        & m_pixelShader);
    if (FAILED(result))
    {
        return false;
    }

    // ���� �Է� ���̾ƿ��� description�� �ۼ��մϴ�.
    // �� ���̾ƿ��� ModelClass�� ���̴��� �ִ� VertexType�� �� ������ ��ġ�ؾ� �մϴ�.
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    // ���̾ƿ��� ������ ���� ���մϴ�.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    // ���� �Է� ���̾ƿ��� �����մϴ�.
    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), & m_layout);
    if (FAILED(result))
    {
        return false;
    }

    // �� �̻� ������� �ʴ� ���� �� �ȼ� ���̴��� ���۸� �����մϴ�.
    vertexShaderBuffer->Release();
    vertexShaderBuffer = 0;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = 0;

    // ���� ���̴��� �ִ� ��� ���� ��� ���۸� �����մϴ�.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // �� Ŭ�������� ���� ���̴��� ��� ���ۿ� �����ϱ� ���� ���۸� �����մϴ�.
    result = device->CreateBuffer(& matrixBufferDesc, NULL, & m_matrixBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // �ؽ��� ���÷��� description�� �ۼ��մϴ�.
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // �ؽ��� ���÷��� �����մϴ�.
    result = device->CreateSamplerState(& samplerDesc, & m_sampleState);
    if (FAILED(result))
    {
        return false;
    }

    // ���� ���̴��� ���� �ݻ� ����� ���� ��� ���ۿ� ���� description�� �ۼ��մϴ�.
    reflectionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    reflectionBufferDesc.ByteWidth = sizeof(ReflectionBufferType);
    reflectionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    reflectionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    reflectionBufferDesc.MiscFlags = 0;
    reflectionBufferDesc.StructureByteStride = 0;

    // ���۸� �����Ͽ� ���� ���̴��� ��� ���ۿ� ������ �� �ְ� �մϴ�.
    result = device->CreateBuffer(& reflectionBufferDesc, NULL, & m_reflectionBuffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}


void ReflectionShaderClass::ShutdownShader()
{
    // �ݻ� ��� ���۸� �����մϴ�.
    if (m_reflectionBuffer)
    {
        m_reflectionBuffer->Release();
        m_reflectionBuffer = 0;
    }

    // ���÷��� �����մϴ�.
    if (m_sampleState)
    {
        m_sampleState->Release();
        m_sampleState = 0;
    }

    // ��� ��� ���۸� �����մϴ�.
    if (m_matrixBuffer)
    {
        m_matrixBuffer->Release();
        m_matrixBuffer = 0;
    }

    // ���̾ƿ��� �����մϴ�.
    if (m_layout)
    {
        m_layout->Release();
        m_layout = 0;
    }

    // �ȼ� ���̴��� �����մϴ�.
    if (m_pixelShader)
    {
        m_pixelShader->Release();
        m_pixelShader = 0;
    }

    // ���� ���̴��� �����մϴ�.
    if (m_vertexShader)
    {
        m_vertexShader->Release();
        m_vertexShader = 0;
    }

    return;
}


void ReflectionShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
    char* compileErrors;
    unsigned long bufferSize, i;
    ofstream fout;


    // Get a pointer to the error message text buffer.
    compileErrors = (char*)(errorMessage->GetBufferPointer());

    // Get the length of the message.
    bufferSize = errorMessage->GetBufferSize();

    // Open a file to write the error message to.
    fout.open("shader-error.txt");

    // Write out the error message.
    for (i = 0; i < bufferSize; i++)
    {
        fout << compileErrors[i];
    }

    // Close the file.
    fout.close();

    // Release the error message.
    errorMessage->Release();
    errorMessage = 0;

    // Pop a message up on the screen to notify the user to check the text file for compile errors.
    MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

    return;
}


bool ReflectionShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
    XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture,
    ID3D11ShaderResourceView* reflectionTexture, XMMATRIX reflectionMatrix)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    unsigned int bufferNumber;
    ReflectionBufferType* dataPtr2;


    // ���̴��� ����� ����� �غ��մϴ�.
    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projectionMatrix = XMMatrixTranspose(projectionMatrix);
    // ���̴��� ����� �ݻ� ����� ��ȯ�� �Ӵϴ�.
    reflectionMatrix = XMMatrixTranspose(reflectionMatrix);

    // ����� �� �ֵ��� ��� ��� ���۸� ��޴ϴ�.
    result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, & mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // ��� ��� ������ ������ �����͸� ���ɴϴ�.
    dataPtr = (MatrixBufferType*)mappedResource.pData;

    // ��ĵ��� ��� ���ۿ� �����մϴ�.
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    // ��� ��� ������ ����� �����մϴ�.
    deviceContext->Unmap(m_matrixBuffer, 0);

    // ���� ���̴����� ��� ��� ������ ��ġ�Դϴ�.
    bufferNumber = 0;

    // ���ŵ� ������ ���� ���̴��� ��� ��� ���۸� �����մϴ�.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, & m_matrixBuffer);
    // ����� �� �ֵ��� �ݻ� ��� ���۸� ��޴ϴ�.
    result = deviceContext->Map(m_reflectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, & mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // ��� ��� ������ ������ �����͸� ���ɴϴ�.
    dataPtr2 = (ReflectionBufferType*)mappedResource.pData;

    // ����� ��� ���ۿ� �����մϴ�.
    dataPtr2->reflectionMatrix = reflectionMatrix;

    // �ݻ� ��� ������ ����� �����մϴ�.
    deviceContext->Unmap(m_reflectionBuffer, 0);

    // ���� ���̴����� �ݻ� ��� ������ ��ġ�Դϴ�.
    bufferNumber = 1;

    // ���ŵ� ������ ���� ���̴��� �ݻ� ��� ��� ���۸� �����մϴ�.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, & m_reflectionBuffer);

    // �ȼ� ���̴��� �ؽ��ĸ� �����մϴ�.
    deviceContext->PSSetShaderResources(0, 1, & texture);
    // �ȼ� ���̴��� �ݻ� �ؽ��ĸ� �����մϴ�.
    deviceContext->PSSetShaderResources(1, 1, & reflectionTexture);

    return true;
}


void ReflectionShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
    // ���� �Է� ���̾ƿ��� �����մϴ�.
    deviceContext->IASetInputLayout(m_layout);

    // ���� �� �ȼ� ���̴��� �����մϴ�.
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);

    // �ȼ� ���̴��� ���÷��� �����մϴ�.
    deviceContext->PSSetSamplers(0, 1, & m_sampleState);

    // ���� �׸��ϴ�.
    deviceContext->DrawIndexed(indexCount, 0, 0);

    return;
}