#pragma once
#include "normalmapshader.h"
class SpecMapShader : public NormalMapShader
{
	public:
		SpecMapShader(void);
		~SpecMapShader(void);
		bool Initialize(LightClass* lightSource, ID3D11Device* device, HWND hwnd, WCHAR* filename, LPCSTR VSname, LPCSTR PSname);
		void Shutdown();

		virtual void SetTextureArray(ID3D11DeviceContext* deviceContext, vector<ID3D11ShaderResourceView*>& textureArray);
		virtual bool SetCameraPosition(ID3D11DeviceContext* deviceContext, D3DXVECTOR3 cameraPosition, int lightType);
		virtual bool SetLightSource(ID3D11DeviceContext* deviceContext, LightClass* lightSource);

		bool RenderInstanced(ID3D11DeviceContext* deviceContext,
					int vertexCount,
					int instanceCount,
					D3DXMATRIX worldMatrix,
					D3DXMATRIX viewMatrix, 
					D3DXMATRIX projectionMatrix);

		bool RenderOrdinary(ID3D11DeviceContext* deviceContext,
					int indexCount,
					D3DXMATRIX worldMatrix,
					D3DXMATRIX viewMatrix, 
					D3DXMATRIX projectionMatrix);

};
