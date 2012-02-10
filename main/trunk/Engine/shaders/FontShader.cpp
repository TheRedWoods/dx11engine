#include "FontShader.h"


FontShader::FontShader(void) : TextureShader()
{
	m_pixelBuffer = 0;
}


FontShader::~FontShader(void)
{

}

void FontShader::Shutdown()
{
	TextureShader::Shutdown();
	if(m_pixelBuffer)
	{
		m_pixelBuffer->Release();
		m_pixelBuffer = 0;
	}
}

bool FontShader::Initialize(ID3D11Device* device, HWND hwnd, WCHAR* filename, LPCSTR VSname, LPCSTR PSname)
{
	HRESULT result;

	vector<char *> layouts;
	layouts.push_back("POSITION");
	layouts.push_back("TEXCOORD");
	layouts.push_back("TEXCOORD_INST");

	result = InitializeShader(device, hwnd, filename, VSname, PSname, layouts);

	if (!result)
	{
		return false;
	}
}

bool FontShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* FXfilename,
	LPCSTR VSname, LPCSTR PSname, vector<char *>& layouts)
{
	HRESULT result;
	D3D11_BUFFER_DESC pixelBufferDesc;

	result = TextureShader::InitializeShader(device, hwnd, FXfilename, VSname, PSname, layouts);
	if (!result)
	{
		return false;
	}

	// Setup the description of the dynamic pixel constant buffer that is in the pixel shader.
	pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
	pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pixelBufferDesc.MiscFlags = 0;
	pixelBufferDesc.StructureByteStride = 0;

	// Create the pixel constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&pixelBufferDesc, NULL, &m_pixelBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool FontShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
	D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixelColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PixelBufferType* dataPtr2;
	unsigned int bufferNumber;

	vector<ID3D11ShaderResourceView*> textureArray;
	textureArray.push_back(texture);
	TextureShader::SetTextureArray(deviceContext, textureArray);
	TextureShader::SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, false);
	// Lock the pixel constant buffer so it can be written to.
	result = deviceContext->Map(m_pixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the pixel constant buffer.
	dataPtr2 = (PixelBufferType*)mappedResource.pData;

	// Copy the pixel color into the pixel constant buffer.
	dataPtr2->pixelColor = pixelColor;

	// Unlock the pixel constant buffer.
	deviceContext->Unmap(m_pixelBuffer, 0);

	// Set the position of the pixel constant buffer in the pixel shader.
	bufferNumber = 0;

	// Now set the pixel constant buffer in the pixel shader with the updated value.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pixelBuffer);

	return true;
}

bool FontShader::Render(ID3D11DeviceContext* deviceContext,
						int indexCount,
						D3DXMATRIX worldMatrix,
						D3DXMATRIX viewMatrix, 
						D3DXMATRIX projectionMatrix,
						ID3D11ShaderResourceView* texture,
						D3DXVECTOR4 pixelColor)
{
	bool result;

	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, pixelColor);
	if(!result)
	{
		return false;
	}

	vector<ID3D11ShaderResourceView*> textureArray;
	textureArray.push_back(texture);
	TextureShader::SetTextureArray(deviceContext, textureArray);
	result = TextureShader::RenderOrdinary(deviceContext, 
						  indexCount,
						  worldMatrix,
						  viewMatrix,
						  projectionMatrix);
	if(!result)
	{
		return false;
	}
	
	return true;
}