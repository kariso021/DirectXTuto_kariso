////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
using namespace DirectX;


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	/*m_Tree = 0;*/
	m_Model_bilboard = 0;
	m_Model_plane = 0;
	//m_Fox = 0;
	m_Bizon = 0;
	/*m_Wolf = 0;*/
	m_DeerA = 0;
	m_Giraf = 0;
	m_Skymap = 0;
	

	m_LightShader = 0;
	m_Light = 0;
	m_Light1 = 0;

	m_TextureShader = 0;
	//m_Bitmap = 0;
	m_Title = 0;

	m_Text = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 3.0f, -30.0f);	// for cube
//	m_Camera->SetPosition(0.0f, 0.5f, -3.0f);	// for chair

	// Create the model object.
	m_Model_bilboard = new ModelClass;
	m_Model_plane = new ModelClass;
	//m_Tree = new ModelClass;
	//m_Fox = new ModelClass;
	m_Bizon = new ModelClass;
	/*m_Wolf = new ModelClass;*/
	m_DeerA = new ModelClass;
	/*m_Giraf = new ModelClass;*/

	if (!m_Model_bilboard)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model_bilboard->Initialize(m_D3D->GetDevice(), L"./data/clock.obj", L"./data/Clock.dds");
	m_Model_plane->Initialize(m_D3D->GetDevice(), L"./data/plane.obj", L"./data/floor.dds");
	/*m_Tree->Initialize(m_D3D->GetDevice(), L"./data/tree.obj", L"./data/Spooky1Twig_Diffuse4.dds");*/
	//m_Fox->Initialize(m_D3D->GetDevice(), L"./data/fox.obj", L"./data/01foxtexture.dds");
	//m_Wolf->Initialize(m_D3D->GetDevice(), L"./data/Wolf.obj", L"./data/25dogtexture.dds");
	m_Bizon->Initialize(m_D3D->GetDevice(), L"./data/chess.obj", L"./data/04bizontexture.dds");
	m_DeerA->Initialize(m_D3D->GetDevice(), L"./data/dolphin.obj", L"./data/dolphin.dds");
	//m_Giraf->Initialize(m_D3D->GetDevice(), L"./data/giraf.obj", L"./data/18giraftexture.dds");


	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	//	m_Light->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	//	m_Light->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
	//	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	//	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light->SetDirection(1.0f, 0.0f, -1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	m_Light1 = new LightClass;
	if (!m_Light1)
	{
		return false;
	}
	// Initialize the first light object.
	/*m_Light1->SetDirection(0.0f, -1.0f, 0.0f);*/
	m_Light1->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);


	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the bitmap object.
	/*m_Bitmap = new BitmapClass;*/
	m_Title = new BitmapClass;
	if (!m_Title)
	{
		return false;
	}

	// Initialize the bitmap object.
	/*result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/sky.dds", screenWidth, screenHeight);*/
	result = m_Title->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title.dds", screenWidth, screenHeight);

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
//	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(m_baseViewMatrix);


	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}


	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, m_baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}


	//Create the SkyboxObject
	m_Skymap = new Skymapclass;
	if (!m_Skymap)
	{
		return false;
	}
	result=m_Skymap->Initialize(m_D3D->GetDevice(),10,10, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}


	return true;
}

CameraClass* GraphicsClass::Getcamera()
{
	return m_Camera;
}


void GraphicsClass::Shutdown()
{
	// Release the model object.
	if(m_Model_bilboard)
	{
		m_Model_bilboard->Shutdown();
		delete m_Model_bilboard;
		m_Model_bilboard = 0;
	}

	if (m_Model_plane)
	{
		m_Model_plane->Shutdown();
		delete m_Model_plane;
		m_Model_plane = 0;
	}

	if (m_Skymap)
	{
		m_Skymap->Shutdown();
		delete m_Skymap;
		m_Skymap = 0;
	}

	//if (m_Tree)
	//{
	//	m_Tree->Shutdown();
	//	delete m_Tree;
	//	m_Tree = 0;
	//}

	//if (m_Fox)
	//{
	//	m_Fox->Shutdown();
	//	delete m_Fox;
	//	m_Fox = 0;
	//}


	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	
	//// Release the bitmap object.
	//if (m_Bitmap)
	//{
	//	m_Bitmap->Shutdown();
	//	delete m_Bitmap;
	//	m_Bitmap = 0;
	//}

	if (m_Title)
	{
		m_Title->Shutdown();
		delete m_Title;
		m_Title = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	return;
}


bool GraphicsClass::Frame(int fps, int cpu, int mouseX, int mouseY, bool Main)
{
	bool result;
	static float rotation = 0.0f;
	int faceCount;
	static float rotation_f = -90.0f;
	static float xpos_f = 0.0f;
	static float ypos_d = 1.5f;
	static int up = 1;
	static float rotation_g = 0.0f;
	static float increase = 0.0f;
	
	ypos_d += up * 0.1f;

	if (ypos_d < 1.5f) {
		up = 1;
	}

	if (ypos_d > 5.5f) {
		up = -1;
	}

	

	if (rotation_f < -80.0f) {
		xpos_f += 0.08f;
		if (xpos_f > 10.0f) {
			rotation_f = 90.0f;
		}
	}

	if (rotation_f > 80.0f) {
		xpos_f -= 0.08f;
		if (xpos_f < -1.0f) {
			rotation_f = -90.0f;
		}
	}

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	increase += (float)XM_PI * 0.1f;
	if (increase > 0.0f && increase < 90.0f) {
		rotation_g = 0.0f;
	}

	if (increase > 90.0f && increase < 180.0f) {
		rotation_g = 90.0f;
	}

	if (increase > 180.0f && increase < 270.0f) {
		rotation_g = 180.0f;
	}

	if (increase > 270.0f && increase < 360.0f) {
		rotation_g = 270.0f;
		increase -= 360.0f;
	}

	// Set the frames per second.
	result = m_Text->SetFPS(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCPU(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	faceCount = m_Model_bilboard->GetFaceCount() + m_Model_plane->GetFaceCount() + m_Bizon->GetFaceCount() + m_DeerA->GetFaceCount();//m_Tree disable fox+ /m_Giraf->GetFaceCount()
	
	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, faceCount, m_D3D->GetDeviceContext());

	if (!result)
	{
		return false;
	}

	
	

	if (Main == TRUE) {
		// Render the graphics scene.
		result = MainRender();
		if (!result)
		{
			return false;
		}
	}
	else {
		result = Render(rotation, rotation_f, rotation_g, xpos_f, ypos_d);
		if (!result)
		{
			return false;
		}
	}

	/*result = SkymapRender();

	if(!result)
	{
		return false;
	}*/
	
	

	// Set the position of the camera.
//	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	return true;
}

bool GraphicsClass::Render(float rotation,float rotation_f,float rotation_g,float xpos_f,float ypos_d)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	XMFLOAT4 diffuseColor_G[2], diffuseColor_A[2];
	XMFLOAT4 lightPosition;
	XMFLOAT3 cameraPosition, modelPosition;
	double angle;
	float rotationX;

	diffuseColor_G[0] = m_Light->GetDiffuseColor();
	diffuseColor_G[1] = m_Light1->GetDiffuseColor();

	diffuseColor_A[0] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	diffuseColor_A[1] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	lightPosition = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0);

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//// Turn off the Z buffer to begin all 2D rendering.
	//m_D3D->TurnZBufferOff();

	//// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	//if (!result)
	//{
	//	return false;
	//}
	//viewMatrix = m_baseViewMatrix;
	//// Render the bitmap with the texture shader.
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	//if (!result)
	//{
	//	return false;
	//}

	m_Camera->GetViewMatrix(viewMatrix);


	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	m_D3D->TurnZBufferOn();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	
	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();
	
	worldMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model_plane->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model_plane->GetIndexCount(), 1, 
		worldMatrix, viewMatrix, projectionMatrix,
		m_Model_plane->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_G,lightPosition,
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	//m_D3D->GetWorldMatrix(worldMatrix);
	//
	//worldMatrix = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	//// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_Tree->Render(m_D3D->GetDeviceContext());

	//// Render the model using the light shader.
	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Tree->GetIndexCount(), m_Tree->GetInstanceCount(), 
	//	worldMatrix, viewMatrix, projectionMatrix,
	//	m_Tree->GetTexture(), 
	//	m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_G,lightPosition,
	//	m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	//
	//if(!result)
	//{
	//	return false;
	//}


	cameraPosition = m_Camera->GetPosition();

	modelPosition.x = 0.0f;
	modelPosition.y = 1.5f;
	modelPosition.z = 0.0f;

	// atan2 함수를 이용하여 빌보드 모델에 적용될 회전값을 계산합니다. 이렇게 하여 빌보드 모델이 현재 카메라 위치를 바라보게 합니다.
	angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / XM_PI);

	// 회전각도를 라디안으로 변환합니다.
	rotationX = (float)angle * 0.0174532925f;

	worldMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(rotationX) * XMMatrixTranslation(modelPosition.x, modelPosition.y, modelPosition.z) * XMMatrixTranslation(7.0f, 8.0f + xpos_f / 2.0f, 0.0f);

	m_Model_bilboard->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model_bilboard->GetIndexCount(), 1,
		worldMatrix, viewMatrix, projectionMatrix,
		m_Model_bilboard->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}


	//worldMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixRotationY(rotation_f * ((float)XM_PI / 180.0f)) * XMMatrixTranslation(xpos_f * 2.0f, 1.5f, -8.0f);
	//
	//m_Fox->Render(m_D3D->GetDeviceContext());

	//// Render the model using the light shader.
	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Fox->GetIndexCount(), 1,
	//	worldMatrix, viewMatrix, projectionMatrix,
	//	m_Fox->GetTexture(),
	//	m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
	//	m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	//if (!result)
	//{
	//	return false;
	//}

	/*worldMatrix = XMMatrixTranslation(xpos_f, 0.5f, -5.0f);*/

	worldMatrix = XMMatrixScaling(2.2f, 2.2f, 2.2f) * XMMatrixRotationY(-90.0f * ((float)XM_PI / 180.0f)) * XMMatrixTranslation(0.0f, ypos_d + -3.0f, -45.0f) * XMMatrixRotationY(-rotation);

	m_DeerA->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_DeerA->GetIndexCount(), 1,
		worldMatrix, viewMatrix, projectionMatrix,
		m_DeerA->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	//m_D3D->GetWorldMatrix(worldMatrix);

	//worldMatrix = XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixRotationY((rotation_f - 90.f) * ((float)XM_PI / 180.0f)) * XMMatrixTranslation(-10.0f, ypos_d + 0.5f, -10.0f + xpos_f * 4.0f);

	//m_Wolf->Render(m_D3D->GetDeviceContext());

	//// Render the model using the light shader.
	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Wolf->GetIndexCount(), 1,
	//	worldMatrix, viewMatrix, projectionMatrix,
	//	m_Wolf->GetTexture(),
	//	m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
	//	m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	//if (!result)
	//{
	//	return false;
	//}

	worldMatrix = XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixRotationY(-130.0f * ((float)XM_PI / 180.0f)) * XMMatrixTranslation(7.0f, 2.0f, 0.0f);

	m_Bizon->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Bizon->GetIndexCount(), 1,
		worldMatrix, viewMatrix, projectionMatrix,
		m_Bizon->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	//skybox 

	result = SkymapRender();


	//worldMatrix = XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixRotationY((90.0f + rotation_g) * ((float)XM_PI / 180.0f)) * XMMatrixTranslation(-18.0f, 2.0f, -3.0f);

	//m_Giraf->Render(m_D3D->GetDeviceContext());

	//// Render the model using the light shader.
	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Giraf->GetIndexCount(), 1,
	//	worldMatrix, viewMatrix, projectionMatrix,
	//	m_Giraf->GetTexture(),
	//	m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
	//	m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	//if (!result)
	//{
	//	return false;
	//}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::MainRender()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Title->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result)
	{
		return false;
	}
	viewMatrix = m_baseViewMatrix;
	//수정된 부분
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Title->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Title->GetTexture());
	if (!result)
	{
		return false;
	}
	// Render the bitmap with the texture shader.
	 
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::SkymapRender() 
{

	// Matrices for transformations
	XMMATRIX skymap_groundWorld, skymap_sphereWorld;
	XMMATRIX skymap_Scale, skymap_Translation;
	XMMATRIX viewMatrix, projectionMatrix,worldMatrix;
	bool result;

	//초기화
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	//Reset cube1World
		  //Reset cube1World
	skymap_groundWorld = XMMatrixIdentity();

	//Define cube1's world space matrix
	skymap_Scale = XMMatrixScaling(500.0f, 10.0f, 500.0f);
	skymap_Translation = XMMatrixTranslation(0.0f, 10.0f, 0.0f);

	//Set cube1's world space using the transformations
	skymap_groundWorld = skymap_Scale * skymap_Translation;

	///////////////**************new**************////////////////////
	//Reset sphereWorld
	skymap_sphereWorld = XMMatrixIdentity();

	//Define sphereWorld's world space matrix
	skymap_Scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	//Make sure the sphere is always centered around camera
	skymap_Translation = XMMatrixTranslation(XMVectorGetX(m_Camera->GetPostionByVector()), XMVectorGetY(m_Camera-> GetPostionByVector()), XMVectorGetZ(m_Camera-> GetPostionByVector()));

	//Set sphereWorld's world space using the transformations
	skymap_sphereWorld = skymap_Scale * skymap_Translation;

	///////////////**************new**************////////////////////
	result=m_Skymap->Render(m_D3D->GetDeviceContext(), skymap_sphereWorld, viewMatrix, projectionMatrix);

	return true;

}

