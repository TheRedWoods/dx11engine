#include "MiniMap.h"


MiniMap::MiniMap(void)
{
	m_MiniMapBitmap = 0;
	m_Border = 0;
	m_Point = 0;
}


MiniMap::~MiniMap(void)
{
}

HRESULT MiniMap::Initialize(ID3D11Device* device, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX viewMatrix, float terrainWidth, float terrainHeight)
{
	HRESULT result = S_FALSE;

	// Initialize the location of the mini-map on the screen.
	m_mapLocationX = 302;
	m_mapLocationY = 73;

	// Set the size of the mini-map.
	m_mapSizeX = 150.0f;
	m_mapSizeY = 150.0f;

	// Store the base view matrix.
	m_viewMatrix = viewMatrix;

	// Store the terrain size.
	m_terrainWidth = terrainWidth;
	m_terrainHeight = terrainHeight;

	// Create the mini-map bitmap object.
	m_MiniMapBitmap = new BitmapClass;
	if(!m_MiniMapBitmap)
	{
		return result;
	}

	// Initialize the mini-map bitmap object.
	result = m_MiniMapBitmap->Initialize(device, screenWidth, screenHeight, L"Engine/data/textures/colorm01.dds", 150, 150);
	if(FAILED(result))
	{
		MessageBox(hwnd, L"Could not initialize the mini-map object.", L"Error", MB_OK);
		return result;
	}

	// Create the border bitmap object.
	m_Border = new BitmapClass;
	if(!m_Border)
	{
		return result;
	}

	// Initialize the border bitmap object.
	result = m_Border->Initialize(device, screenWidth, screenHeight, L"Engine/data/textures/border01.dds", 154, 154);
	if(FAILED(result))
	{
		MessageBox(hwnd, L"Could not initialize the border object.", L"Error", MB_OK);
		return result;
	}

	// Create the point bitmap object.
	m_Point = new BitmapClass;
	if(!m_Point)
	{
		return result;
	}

	// Initialize the point bitmap object.
	result = m_Point->Initialize(device, screenWidth, screenHeight, L"Engine/data/textures/point01.dds", 3, 3);
	if(FAILED(result))
	{
		MessageBox(hwnd, L"Could not initialize the point object.", L"Error", MB_OK);
		return result;
	}

	return result;
}

void MiniMap::Shutdown()
{
	// Release the point bitmap object.
	if(m_Point)
	{
		m_Point->Shutdown();
		delete m_Point;
		m_Point = 0;
	}

	// Release the border bitmap object.
	if(m_Border)
	{
		m_Border->Shutdown();
		delete m_Border;
		m_Border = 0;
	}

	// Release the mini-map bitmap object.
	if(m_MiniMapBitmap)
	{
		m_MiniMapBitmap->Shutdown();
		delete m_MiniMapBitmap;
		m_MiniMapBitmap = 0;
	}

	return;
}


bool MiniMap::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix, TextureShader* textureShader)
{
	bool result;
	// Put the border bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Border->Render(deviceContext, (m_mapLocationX - 2), (m_mapLocationY - 2));
	if(!result)
	{
		return false;
	}

	// Render the border bitmap using the texture shader.
	vector<ID3D11ShaderResourceView*> borderTextureArray;
	borderTextureArray.push_back(m_Border->GetTexture());
	textureShader->SetTextureArray(deviceContext, borderTextureArray);
	textureShader->RenderOrdinary(deviceContext, m_Border->GetIndexCount(), worldMatrix, m_viewMatrix, orthoMatrix);

	// Put the mini-map bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_MiniMapBitmap->Render(deviceContext, m_mapLocationX, m_mapLocationY);
	if(!result)
	{
		return false;
	}
	// Render the mini-map bitmap using the texture shader.
	vector<ID3D11ShaderResourceView*> minimapTextureArray;
	minimapTextureArray.push_back(m_MiniMapBitmap->GetTexture());
	textureShader->SetTextureArray(deviceContext, minimapTextureArray);
	textureShader->RenderOrdinary(deviceContext, m_MiniMapBitmap->GetIndexCount(), worldMatrix, m_viewMatrix, orthoMatrix);

	// Put the point bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Point->Render(deviceContext, m_pointLocationX, m_pointLocationY);
	if(!result)
	{
		return false;
	}
	// Render the point bitmap using the texture shader.
	vector<ID3D11ShaderResourceView*> pointTextureArray;
	pointTextureArray.push_back(m_Point->GetTexture());
	textureShader->SetTextureArray(deviceContext, pointTextureArray);
	textureShader->RenderOrdinary(deviceContext, m_Point->GetIndexCount(), worldMatrix, m_viewMatrix, orthoMatrix);
	
	return true;
}

void MiniMap::PositionUpdate(float positionX, float positionZ)
{
	float percentX, percentY;

	// Ensure the point does not leave the minimap borders even if the camera goes past the terrain borders.
	if(positionX < 0)
	{
		positionX = 0;
	}

	if(positionZ < 0)
	{
		positionZ = 0;
	}

	if(positionX > m_terrainWidth)
	{
		positionX = m_terrainWidth;
	}

	if(positionZ > m_terrainHeight)
	{
		positionZ = m_terrainHeight;
	}

	// Calculate the position of the camera on the minimap in terms of percentage.
	percentX = positionX / m_terrainWidth;
	percentY = 1.0f - (positionZ / m_terrainHeight);

	// Determine the pixel location of the point on the mini-map.
	m_pointLocationX = m_mapLocationX + (int)(percentX * m_mapSizeX);
	m_pointLocationY = m_mapLocationY + (int)(percentY * m_mapSizeY);

	// Subtract one from the location to center the point on the mini-map according to the 3x3 point pixel image size.
	m_pointLocationX = m_pointLocationX - 1;
	m_pointLocationY = m_pointLocationY - 1;

	return;
}