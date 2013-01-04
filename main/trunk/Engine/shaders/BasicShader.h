////////////////////////////////////////////////////////////////////////////////
// Filename: BasicShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BASICSHADER_H_
#define _BASICSHADER_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <vector>
#include "BaseShaderClass.h"
#include "../BasicResource.h"

using namespace std;

#define LAYOUT_POSITION					"POSITION"
#define LAYOUT_COLOR					"COLOR"
#define LAYOUT_TEXCOORD					"TEXCOORD"
#define LAYOUT_TEXCOORD_NORMAL_DETAIL	"TEXCOORD_NORMAL_DETAIL"
#define LAYOUT_TEXCOORD_INST			"TEXCOORD_INST"
#define LAYOUT_NORMAL					"NORMAL"
#define LAYOUT_TANGENT					"TANGENT"
#define LAYOUT_BINORMAL					"BINORMAL"  

////////////////////////////////////////////////////////////////////////////////
// Class name: BasicShader
////////////////////////////////////////////////////////////////////////////////
class BasicShader: public BasicResource// BaseShaderClass
{
	public:
		BasicShader(void);
		BasicShader(const BasicShader&);
		~BasicShader();
		virtual HRESULT Initialize(ID3D11Device*, HWND, WCHAR*, LPCSTR, LPCSTR);
		HRESULT InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* FXfilename,
							  LPCSTR VSname, LPCSTR PSname, vector<char *>& layouts);
		// void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
		virtual void Shutdown();

		virtual HRESULT RenderInstanced(ID3D11DeviceContext* deviceContext,
									 int vertexCount,
									 int instanceCount,
									 D3DXMATRIX worldMatrix, 
									 D3DXMATRIX viewMatrix,
									 D3DXMATRIX projectionMatrix) const;

		virtual HRESULT RenderOrdinary(ID3D11DeviceContext* deviceContext, 
									 int indexCount,
									 D3DXMATRIX worldMatrix, 
									 D3DXMATRIX viewMatrix,
									 D3DXMATRIX projectionMatrix) const;

		// Not implemented
		virtual void SetTextureArray(ID3D11DeviceContext* deviceContext, vector<ID3D11ShaderResourceView*>& textureArray);
		virtual HRESULT SetCameraPosition(ID3D11DeviceContext* deviceContext, D3DXVECTOR3 cameraPosition, int lightType);
		virtual bool SetLightSource(ID3D11DeviceContext* deviceContext, LightClass* lightSource);
		
		virtual HRESULT SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, bool isInstanced) const;

		virtual void RenderShaderInstanced(ID3D11DeviceContext* deviceContext, int vertexCount, int instanceCount) const;
		virtual void RenderShaderOrdinary(ID3D11DeviceContext* deviceContext, int indexCount) const;

		// TODO: Temprory
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;

	private:
		vector<D3D11_INPUT_ELEMENT_DESC> CreateInputLayout(vector<char *>& layouts);
		void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

		struct MatrixBufferType
		{
			D3DXMATRIX world;
			D3DXMATRIX view;
			D3DXMATRIX projection;
			bool isInstanced;
		};
		/*ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;*/
		ID3D11Buffer* m_matrixBuffer;
};

#endif
