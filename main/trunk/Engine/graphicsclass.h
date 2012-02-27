////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <windows.h>
#include <string>
#include <sstream>

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "RenderTextureClass.h"
#include "bitmapclass.h"
#include "lightclass.h"

#include "Shaders/BasicShader.h"
#include "Shaders/TextureShader.h"
#include "Shaders/LightShader.h"
#include "Shaders/MultitextureShader.h"
#include "Shaders/NormalMapShader.h"
#include "Shaders/SpecMapShader.h"
#include "Shaders/FogShader.h"
#include "Shaders/ReflectionShader.h"
#include "Shaders/TerrainShader.h"
#include "shaders/SkyDomeShader.h"
#include "Terrain.h"
#include "QuadTree.h"
#include "CameraMovement.h"
#include "timerclass.h"
#include "minimap.h"

#include "textclass.h"
#include "inputclass.h"
#include "ModelFactory.h"
#include "MaterialFactory.h"
#include "frustumclass.h"
#include "fpsclass.h"
#include "cpuclass.h"

#include "SkyDome.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#define V_RETURN(statement, messageHeader, messageBody)			\
{																\
	HRESULT hr = (statement);									\
	if( FAILED(hr) )											\
	{															\
		MessageBox(hwnd, messageBody, messageHeader, MB_OK);	\
		return hr;												\
	}															\
}

#define  CREATE_ORDINARY_OBJ_WITH_MAT(obj, objectNmae, objectFilePath, materialToAssign)					\
{																											\
	obj = mObjectFactory->CreateOrdinaryModel(mD3D->GetDevice(), hwnd, objectNmae, objectFilePath);			\
	obj->SetMaterial(mMaterialFactory->GetMaterialByName(materialToAssign));								\
}

#define  CREATE_INSTANCED_OBJ_WITH_MAT(objectNmae, objectFilePath, materialToAssign, numberOfObjects)						\
{																															\
	ModelObject* object = new ModelObject();																				\
	object = mObjectFactory->CreateInstancedModel(mD3D->GetDevice(), hwnd, objectNmae, objectFilePath, numberOfObjects);	\
	object->SetMaterial(mMaterialFactory->GetMaterialByName(materialToAssign));												\
}

#define SHUTDOWN_OBJ(obj)	\
{							\
	if(obj)					\
	{						\
		obj->Shutdown();	\
		delete obj;			\
		obj = 0;			\
	}						\
}

#define  WRITE_SENTENCE(D3D, numericValue, stringValue, posX, posY, colorR, colorG, colorB, sentenceNumber)		\
{																												\
	char tempString[10];																						\
	char dataString[50];																						\
	_itoa_s((int) numericValue, tempString, 10);																\
	strcpy_s(dataString, stringValue);																			\
	strcat_s(dataString, tempString);																			\
	result = m_Text->AddSentence(D3D, dataString, posX, posY, colorR, colorG, colorB, sentenceNumber);			\
	if(!result) { return false; }																				\
}

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
	public:
		GraphicsClass();
		GraphicsClass(const GraphicsClass&);
		~GraphicsClass();

		HRESULT Initialize(int, int, HWND);
		void Shutdown();
		bool Frame();
		bool Render();

	private:
		bool HandleInput(float);
		
		bool InitLights();
		HRESULT InitializeShaders(HWND hwnd);
		bool InitMaterials();
		bool InitObjects(HWND hwnd);

		HRESULT RenderText();
		bool RenderToTextureFromCameraView();
		bool RenderToTextureFromReflectionView();
		HRESULT RenderTerrain(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
		bool RenderScene();
		bool RenderModel(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float fogStart, float fogEnd);
		void ShutdownShaders();
		HRESULT Render2D();
		bool SetFillMode(D3D11_FILL_MODE mode);
		HRESULT RenderObject(ModelObject* modelObj, ID3D11DeviceContext* deviceContext, D3DXMATRIX viewMatrix,
						  D3DXMATRIX projectionMatrix, LightClass* lightSource, LightClass::LightTypes lightType, bool isInstanced);
		void SetPositionAboveTerrain(ModelObject* modelObj, float heightAboveTerrain);

	private:

		enum SkyShape
		{
			SKY_SPHERE = 0,
			SKY_CUBE
		};

		enum SkyPixelShaderType
		{
			SKY_PIXEL_GRADIENT = 0,
			SKY_PIXEL_CUBE_TEXTURE,
			SKY_PIXEL_BOTH
		};

		struct PointLightInfo 
		{
			D3DXVECTOR4 color;
			D3DXVECTOR3 position;
		};

		ModelFactory* mObjectFactory;
		MaterialFactory* mMaterialFactory;
		Terrain* mTerrain;
		QuadTree* mQuadTree;
		MiniMap* m_MiniMap;

		D3DClass* mD3D;
		CameraClass* mCamera;
		FrustumClass* m_Frustum;

		LightClass* mDirSpecLight;
		LightClass* mDirAmbLight;
		BitmapClass* mBitmap, *mCursor;
		RenderTextureClass* m_RenderTexture;

		TextClass* m_Text;

		// Shaders
		BasicShader* mBasicShader;

		TextureShader* mTextureShaderMiniMap;
		TextureShader* mTextureShaderCamDisplay;
		FontShader* mCursorShader;

		LightShader* mDirSpecLightShader;
		LightShader* mPointLightShader;
		LightShader* mDirAmbLightShader;

		TerrainShader* mTerrainShader;
		MultitextureShader* m_MultiTextureShader;
		NormalMapShader* m_BumpMapShader;
		SpecMapShader* m_SpecMapShader;
		SpecMapShader* m_SpecMapShaderNonInstanced;
		FogShader* m_FogShader;
		SkyDomeShader* mSkyDomeShader;
		

		int mNumObjectsRendered;

		ReflectionShader* m_ReflectionShader;
		LightClass* mPointLights[4];
		CameraMovement* mCameraMovement;

		Timer* mTimer;
		DWORD mDrawFuncTime;
		D3DXMATRIX mBaseViewMatrix;

		// For directional light animation
		bool mDirUp;
		bool mDirDown;

		// Input flags
		bool mIsAllowToCameraDisplayRender;
		bool mIsAllowToBBRender;

		int mCursorWidth;
		int mCursorHeight;

		int mScreenWidth;
		int mScreenHeight;

		SkyDome* mSkyDome;

		bool mIsWireFrameModeOn;
		SkyShape mSkyShape;
		SkyPixelShaderType mSkyPixelShaderType;
};

#endif
