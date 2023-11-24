////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"
#include "skymapclass.h"
#include "Refractionshaderclass.h"
#include "Watershaderclass.h"
#include "Rendertextureclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool Frame(int, int, int, int, bool);

	CameraClass* Getcamera();

private:
	bool Render(float,float,float,float,float);
	bool TitleRender();

	bool RenderRefractionToTexture();
	bool RenderReflectionToTexture();
	bool RenderScene(float rotation, float rotation_f, float rotation_g, float xpos_f, float ypos_d);

	bool SkymapRender();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass * m_Model_bilboard, * m_Model_plane, *m_Chess, *m_Giraf, *m_dolphin, *m_WaterModel;//m_tree ,*m_fox 뺌*m_Wolf

	
	
	LightShaderClass* m_LightShader;
	LightClass* m_Light, * m_Light1;

	TextureShaderClass* m_TextureShader;
	BitmapClass *m_Title;
	Skymapclass *m_Skymap;

	//물 반사 및 물 쉐이더
	RefractionShaderClass* m_RefractionShader;
	WaterShaderClass* m_WaterShader;
	RenderTextureClass* m_RefractionTexture, * m_ReflectionTexture;


	//물의 높이와 위치
	float m_waterHeight, m_waterTranslation;




	TextClass* m_Text;

	XMMATRIX m_baseViewMatrix;
};

#endif