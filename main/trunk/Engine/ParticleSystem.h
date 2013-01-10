#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include "ResourceMgr.h"
#include "BufferManager.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ParticleSystem
////////////////////////////////////////////////////////////////////////////////
class ParticleSystem
{
	private:
		struct ParticleType
		{
			float positionX, positionY, positionZ;
			float red, green, blue;
			float velocity;
			float lifeTime;
			bool active;
		};

		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 texture;
			D3DXVECTOR4 color;
		};

	public:
		ParticleSystem();
		ParticleSystem(const ParticleSystem&);
		~ParticleSystem();

		bool Initialize(ID3D11Device*, WCHAR*);
		void SetPosition(D3DXVECTOR3 position);
		D3DXVECTOR3 GetPosition();
		void Shutdown();
		bool Frame(float, ID3D11DeviceContext*);
		void Render(ID3D11DeviceContext*);

		ID3D11ShaderResourceView* GetTexture();
		int GetIndexCount();

	private:
		bool InitializeParticleSystem();
		void ShutdownParticleSystem();

		bool InitializeBuffers(ID3D11Device*);

		void EmitParticles(float);
		void UpdateParticles(float);
		void KillParticles(float frameTime);

		bool UpdateBuffers(ID3D11DeviceContext*);

		void RenderBuffers(ID3D11DeviceContext*);

	private:
		float m_PositionX, m_PositionY, m_PositionZ, m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
		float m_particleVelocity, m_particleVelocityVariation;
		float m_particleSize, m_particlesPerSecond;
		int m_maxParticles;
		float m_ParticleLifeTime;

		int m_currentParticleCount;
		float m_accumulatedTime;

		Texture* m_Texture;

		ParticleType* m_particleList;

		int m_vertexCount, m_indexCount;
		VertexType* m_vertices;
		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
};

#endif
