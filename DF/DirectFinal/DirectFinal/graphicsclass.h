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
	bool MainRender();

	bool SkymapRender();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass * m_Model_bilboard, * m_Model_plane, *m_Bizon, *m_Giraf, *m_DeerA;//m_tree ,*m_fox »­*m_Wolf
	
	
	LightShaderClass* m_LightShader;
	LightClass* m_Light, * m_Light1;

	TextureShaderClass* m_TextureShader;
	BitmapClass *m_Title;
	Skymapclass *m_Skymap;



	TextClass* m_Text;

	XMMATRIX m_baseViewMatrix;
};

#endif