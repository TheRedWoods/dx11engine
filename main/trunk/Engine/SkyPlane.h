#ifndef _SKYPLANE_H_
#define _SKYPLANE_H_

#include <d3d11.h>
#include <d3dx10math.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include "BufferManager.h"

class SkyPlane
{
	public:
		SkyPlane();
		~SkyPlane();

		bool Initialize(ID3D11Device* device, wchar_t* cloudTextureFilename, wchar_t* perturbTextureFilename);
		void Shutdown();
		void Render(ID3D11DeviceContext*);
		void Frame();

		int GetIndexCount();
		ID3D11ShaderResourceView* GetCloudTexture();
		ID3D11ShaderResourceView* GetPerturbTexture();

		float GetScale();
		float GetBrightness();
		float GetTranslation(int);

	private:
		struct SkyPlaneType
		{
			float x, y, z;
			float tu, tv;
		};

		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 texture;
		};

		bool InitializeSkyPlane(int, float, float, float, int);
		void ShutdownSkyPlane();

		bool InitializeBuffers(ID3D11Device*, int);
		void RenderBuffers(ID3D11DeviceContext*);

	private:
		SkyPlaneType* m_skyPlane;
		int m_vertexCount, m_indexCount;
		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
		Texture *m_CloudTexture, *m_PerturbTexture;
		float m_scale, m_brightness, m_translation;
};

#endif