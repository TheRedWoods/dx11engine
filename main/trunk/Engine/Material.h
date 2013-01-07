#pragma once

#include <vector>
#include "textureclass.h"
#include "Shaders/BasicShader.h"
#include "BasicResource.h"

using namespace std;

// For .MLT material
/*
struct SurfaceMaterial
{
	std::wstring matName;
	D3DXVECTOR4 difColor;
	int texArrayIndex;
	bool hasTexture;
	bool transparent;
};
*/

class Material
{
	public:
		Material();
		Material(char* materialName);
		~Material();
		bool AppentTextureToMaterial(ID3D11Device* device, wchar_t* textureName);
		vector<ID3D11ShaderResourceView*>& GetTextureVector();
		char* GetMaterialName();
		void Shutdown();
		void SetMaterialShader(BasicShader* shader); // BasicShader
		BasicShader* GetMaterialShader();

	private:
		bool LoadTexture(ID3D11Device* device, WCHAR* filename);
		char mMaterialName[100];
		BasicShader* mShader;

		vector<ID3D11ShaderResourceView*> mTextureVector;
		
		// For .MLT material
		/*
		vector<int> meshSubsetTexture;
		ID3D11BlendState* Transparency;
		vector<ID3D11ShaderResourceView*> meshSRV; // The same as mTextureVector

		vector<SurfaceMaterial> material;
		vector<std::wstring> textureNameArray;
		*/
};

