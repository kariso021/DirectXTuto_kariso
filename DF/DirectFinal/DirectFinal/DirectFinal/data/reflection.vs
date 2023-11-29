////////////////////////////////////////////////////////////////////////////////
// Filename: reflection.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer ReflectionBuffer
{
    matrix reflectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 reflectionPosition : TEXCOORD1;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ReflectionVertexShader(VertexInputType input)
{
    PixelInputType output;
    matrix reflectProjectWorld;
    

    // �������� ��� ������ ���� ��ġ ������ ������ ���� �����մϴ�.
    input.position.w = 1.0f;

    // ������ ��ġ�� ����, ��, �������� ����� �̿��Ͽ� ����մϴ�.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // �ȼ� ���̴��� �ѱ� �ؽ��� ��ǥ�� �����մϴ�.
    output.tex = input.tex;

     // �ݻ� ���� ���� ����� �����մϴ�.
    reflectProjectWorld = mul(reflectionMatrix, projectionMatrix);
    reflectProjectWorld = mul(worldMatrix, reflectProjectWorld);

        // reflectProjectWorld ��Ŀ� ���߾� �Է� ��ġ�� ����մϴ�.
    output.reflectionPosition = mul(input.position, reflectProjectWorld);

    return output;
}