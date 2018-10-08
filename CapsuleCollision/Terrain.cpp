#include "Terrain.h"
#include "ShaderLoader.h"
#include "Utilities.h"
#include "Camera.h"

Terrain::Terrain()
{
	m_iNumCols = 513;
	m_iNumRows = 513;
	m_fHeightScale = 0.35f;
	m_fHeightOffset = -20.0f;
	m_fWidth = m_iNumCols;
	m_fDepth = m_iNumRows;
	m_strFilePath = "Resources\\Images\\coastMountain513.raw";
}

Terrain::~Terrain()
{
}

void Terrain::Initialize()
{	
	// Create program
	m_program = ShaderLoader::GetInstance().CreateProgram("Resources\\Shaders\\VertexShader.vs",
		"Resources\\Shaders\\FragmentShader.fs");	

	LoadHeightMap();
	Smooth();
	BuildVertexBuffer();
	BuildIndexBuffer();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(offsetof(TerrainVertex, v3Color)));
	glEnableVertexAttribArray(1);
}

void Terrain::BuildVertexBuffer()
{		
	float halfWidth = (m_iNumCols - 1) * 0.5f;
	float halfDepth = (m_iNumRows - 1) * 0.5f;
	float du = 1.0f / (m_iNumCols - 1);
	float dv = 1.0f / (m_iNumRows - 1);
	
	std::vector<TerrainVertex> vertices((m_iNumCols * m_iNumRows));
	
	int CurrentIndex = 0;
	for (int i = 0; i < m_iNumRows; ++i)
	{
		float z = halfDepth - i;
	
		for (int j = 0; j < m_iNumCols; ++j)
		{
			float x = -halfWidth + j;
			float y = m_vecHeightMap[i * m_iNumCols + j];				
			
			vertices[i * m_iNumCols + j].v3Pos = glm::vec3(x, y, z);
			vertices[i * m_iNumCols + j].v3Color = glm::vec3(1.0f, 0.0f, 0.0f);
		}
	}	
		
	m_vecVertices = vertices;
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TerrainVertex), &vertices[0], GL_STATIC_DRAW);
}

void Terrain::BuildIndexBuffer()
{		
	//std::vector<int> vecIndices(6);
	//
	//vecIndices[0] = 0;
	//vecIndices[1] = 1;
	//vecIndices[2] = 2;
	//vecIndices[3] = 0;
	//vecIndices[4] = 2;
	//vecIndices[5] = 3;
	//
	//m_vecIndices = vecIndices;
	//
	//GLuint uiEBO;
	//glGenBuffers(1, &uiEBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiEBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndices.size() * sizeof(int), &m_vecIndices[0], GL_STATIC_DRAW);
		
	std::vector<int> vecIndices(((m_iNumRows - 1) * (m_iNumCols - 1) * 2) * 3);
	//std::vector<int> vecIndices;
	int CurrentIndex = 0;
	
	for (int i = 0; i < m_iNumRows - 1; ++i)
	{
		for (int j = 0; j < m_iNumCols - 1; ++j)
		{
			vecIndices[CurrentIndex + 0] = i * m_iNumCols + j;
			vecIndices[CurrentIndex + 1] = i * m_iNumCols + j + 1;
			vecIndices[CurrentIndex + 2] = (i + 1) * m_iNumCols + j;
			
			vecIndices[CurrentIndex + 3] = (i + 1) * m_iNumCols + j;
			vecIndices[CurrentIndex + 4] = i * m_iNumCols + j + 1;
			vecIndices[CurrentIndex + 5] = (i + 1) * m_iNumCols + j + 1;
			
			CurrentIndex += 6; // next quad
		}
	}	
	GLuint uiEBO;
	glGenBuffers(1, &uiEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vecIndices.size() * sizeof(int), &vecIndices[0], GL_STATIC_DRAW);

	m_vecIndices = vecIndices;
}

void Terrain::LoadHeightMap()
{
	// A height for each vertex
	std::vector<unsigned char> in(m_iNumRows * m_iNumCols);

	// Open the file.
	std::ifstream inFile;
	inFile.open(m_strFilePath.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array, and scale and offset the heights.
	m_vecHeightMap.resize(m_iNumRows * m_iNumCols, 0);
	for (UINT i = 0; i < m_iNumRows * m_iNumCols; ++i)
	{
		m_vecHeightMap[i] = static_cast<float>(in[i]) * m_fHeightScale + m_fHeightOffset;
	}
}

float Terrain::GetHeight(float x, float z) const
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*m_fWidth) / 1.0f;
	float d = (z - 0.5f*m_fDepth) / -1.0f;

	// Get the row and column we are in.
	int row = static_cast<int>(floorf(d));
	int col = static_cast<int>(floorf(c));

	// Check that we're on the terrain
	if (row > m_iNumRows - 2 || col > m_iNumCols - 2 || row < 0 || col < 0)
	{
		return -11111.0f;
	}

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_vecHeightMap[row*m_iNumCols + col];
	float B = m_vecHeightMap[row*m_iNumCols + col + 1];
	float C = m_vecHeightMap[(row + 1)*m_iNumCols + col];
	float D = m_vecHeightMap[(row + 1)*m_iNumCols + col + 1];

	// Where we are relative to the cell.
	float s = c - static_cast<float>(col);
	float t = d - static_cast<float>(row);

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s * uy + t * vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s)*uy + (1.0f - t)*vy;
	}
}

void Terrain::Smooth()
{
	std::vector<float> vecSmoothedHeightmap(m_vecHeightMap.size());

	for (UINT i = 0; i < m_iNumRows; ++i)
	{		
		for (UINT j = 0; j < m_iNumCols; ++j)
		{
			vecSmoothedHeightmap[i * m_iNumCols + j] = Average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_vecHeightMap = vecSmoothedHeightmap;
}

bool Terrain::InBounds(UINT _a, UINT _b)
{
	return 
	(
		(_a >= 0 && _a < m_iNumRows) &&
		(_b >= 0 && _b < m_iNumCols)
	);
}

float Terrain::Average(UINT _a, UINT _b)
{
	// Function computes the average height of the AB element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |AB| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float fAvg = 0.0f;
	float fNum = 0.0f;

	for (UINT i = _a - 1; i <= _a + 1; ++i)
	{
		for (UINT j = _b - 1; j <= _b + 1; ++j)
		{
			if (InBounds(i, j))
			{
				fAvg += m_vecHeightMap[i * m_iNumCols + j];
				fNum += 1.0f;
			}
		}
	}

	return (fAvg * (1.0f / fNum));
}

void Terrain::Render()
{
	// Set Culling and Use program	
	glUseProgram(m_program);

	// Pass mvp to shader
	glm::mat4 MVP = Camera::GetInstance()->GetProj() * Camera::GetInstance()->GetView() * glm::mat4();
	GLint MVPloc = glGetUniformLocation(m_program, "MVP");
	glUniformMatrix4fv(MVPloc, 1, GL_FALSE, value_ptr(MVP));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Bind vao and draw object, unbind vao
	glBindVertexArray(m_vao);
	//glDrawArrays(GL_TRIANGLES, 0, m_vecVertices.size());
	glDrawElements(GL_TRIANGLES, m_vecIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}