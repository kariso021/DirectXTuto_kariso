////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
using namespace DirectX;


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	
	m_Model_bilboard = 0;
	m_Model_plane = 0;
	m_Chess = 0;
	m_dolphin = 0;
	m_Giraf = 0;
	m_Skymap = 0;
	m_WaterModel = 0;
	m_WaterBath = 0;
	

	m_LightShader = 0;
	m_Light = 0;
	m_Light1 = 0;
	m_WaterShader = 0;

	m_TextureShader = 0;
	//m_Bitmap = 0;
	m_Title = 0;

	m_RefractionShader = 0;
	m_ReflectionTexture = 0;
	m_RefractionTexture = 0;

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
	m_WaterModel = new ModelClass;
	m_Model_bilboard = new ModelClass;
	m_Model_plane = new ModelClass;
	//m_Tree = new ModelClass;
	//m_Fox = new ModelClass;
	m_Chess = new ModelClass;
	/*m_Wolf = new ModelClass;*/
	m_dolphin = new ModelClass;
	m_WaterBath = new ModelClass;
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
	m_Chess->Initialize(m_D3D->GetDevice(), L"./data/chess.obj", L"./data/04bizontexture.dds");
	m_dolphin->Initialize(m_D3D->GetDevice(), L"./data/dolphin.obj", L"./data/dolphin.dds");
	//m_Giraf->Initialize(m_D3D->GetDevice(), L"./data/giraf.obj", L"./data/18giraftexture.dds");
	m_WaterModel->Initialize(m_D3D->GetDevice(), L"./data/Watercube2.obj", L"./data/Water.dds");
	m_WaterBath->Initialize(m_D3D->GetDevice(), L"./data/Watercube2.obj", L"./data/WaterB.dds");


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

	//Create Water Obj Referation

	 // Create the refraction render to texture object.
	m_RefractionTexture = new RenderTextureClass;
	if (!m_RefractionTexture)
	{
		return false;
	}

	// Initialize the refraction render to texture object.
	result = m_RefractionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection render to texture object.
	m_ReflectionTexture = new RenderTextureClass;
	if (!m_ReflectionTexture)
	{
		return false;
	}

	// Initialize the reflection render to texture object.
	result = m_ReflectionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}
	
	m_RefractionShader = new RefractionShaderClass;
	if (!m_RefractionShader)
	{
		return false;
	}

	// Initialize the refraction shader object.
	result = m_RefractionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_WaterShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}


	// Set the height of the water.
	m_waterHeight = 2.75f;

	// Initialize the position of the water.
	m_waterTranslation = 0.0f;




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

	if (m_WaterBath)
	{
		m_WaterBath->Shutdown();
		delete m_WaterBath;
		m_WaterBath = 0;
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
	//For Water
	if (m_WaterModel)
	{
		m_WaterModel->Shutdown();
		delete m_WaterModel;
		m_WaterModel = 0;
	}

	if (m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	if (m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	if (m_ReflectionTexture)
	{
		m_ReflectionTexture->Shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	if (m_RefractionShader)
	{
		m_RefractionShader->Shutdown();
		delete m_RefractionShader;
		m_RefractionShader = 0;
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
	static float ypos_d = 2.5f;
	static int up = 1;
	static float rotation_g = 0.0f;
	static float increase = 0.0f;
	
	ypos_d += up * 0.1f;

	if (ypos_d < -5.5f) {
		up = 1;
	}

	if (ypos_d > 3.5f) {
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

	//Water 의 움직임
	m_waterTranslation += 0.001f;
	if (m_waterTranslation > 3.0f)
	{
		m_waterTranslation -= 3.0f;
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

	faceCount = m_Model_bilboard->GetFaceCount() + m_Model_plane->GetFaceCount() + m_Chess->GetFaceCount() + m_dolphin->GetFaceCount();//m_Tree disable fox+ /m_Giraf->GetFaceCount()
	
	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, faceCount, m_D3D->GetDeviceContext());

	if (!result)
	{
		return false;
	}

	
	

	if (Main == TRUE) {
		// Render the graphics scene.
		result = TitleRender();
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
	bool result;

	result = RenderRefractionToTexture();
	if (!result)
	{
		return false;
	}

	// Render the reflection of the scene to a texture.
	result = RenderReflectionToTexture();
	if (!result)
	{
		return false;
	}

	//Scene

	result = RenderScene(rotation, rotation_f, rotation_g, xpos_f, ypos_d);

	//skybox 

	result = SkymapRender();

	result = RenderText();


	
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::TitleRender()
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

bool GraphicsClass::RenderRefractionToTexture()
{
	XMFLOAT4 clipPlane;
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	// Setup a clipping plane based on the height of the water to clip everything above it.
	clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Translate to where the bath model will be rendered.
	worldMatrix = XMMatrixTranslation(0.0f, -3.0f, 0.0f);

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_WaterBath->Render(m_D3D->GetDeviceContext());

	// Render the bath model using the light shader.
	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_WaterModel->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_WaterBath->GetTexture(), m_Light->GetDirection(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderReflectionToTexture()
{
	XMFLOAT4 diffuseColor_G[2], diffuseColor_A[2];
	XMFLOAT4 lightPosition;
	XMMATRIX reflectionViewMatrix, projectionMatrix, worldMatrix;
	bool result;

	diffuseColor_G[0] = m_Light->GetDiffuseColor();
	diffuseColor_G[1] = m_Light1->GetDiffuseColor();

	diffuseColor_A[0] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	diffuseColor_A[1] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	lightPosition = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0);


	// Set the render target to be the reflection render to texture.
	m_ReflectionTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the reflection render to texture.
	m_ReflectionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Use the camera to render the reflection and create a reflection view matrix.
	m_Camera->RenderReflection(m_waterHeight);

	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	// Get the world and projection matrices from the d3d object.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Translate to where the wall model will be rendered.

	//No RenderReflection

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene(float rotation, float rotation_f, float rotation_g, float xpos_f, float ypos_d)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, reflectionMatrix;
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

	

	worldMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model_plane->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model_plane->GetIndexCount(), 1,
		worldMatrix, viewMatrix, projectionMatrix,
		m_Model_plane->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_G, lightPosition,
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

	m_dolphin->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_dolphin->GetIndexCount(), 1,
		worldMatrix, viewMatrix, projectionMatrix,
		m_dolphin->GetTexture(),
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

	m_Chess->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Chess->GetIndexCount(), 1,
		worldMatrix, viewMatrix, projectionMatrix,
		m_Chess->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}
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
		// WaterBathRender




	//Water
	  // Get the camera reflection view matrix.
	reflectionMatrix = m_Camera->GetReflectionViewMatrix();

	// Translate to where the water model will be rendered.
	worldMatrix = XMMatrixTranslation(0.0f, m_waterHeight-6.0f, 0.0f);

	// Put the water model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_WaterModel->Render(m_D3D->GetDeviceContext());

	// Render the water model using the water shader.
	result = m_WaterShader->Render(m_D3D->GetDeviceContext(), m_WaterModel->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, reflectionMatrix, m_ReflectionTexture->GetShaderResourceView(),
		m_RefractionTexture->GetShaderResourceView(), m_WaterModel->GetTexture(),
		m_waterTranslation, 0.03f);
	if (!result)
	{
		return false;
	}









	// Present the rendered scene to the screen.



	return true;
}

bool GraphicsClass::RenderText()
{
	bool result;
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);


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


	return false;
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

