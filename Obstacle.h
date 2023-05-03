#pragma once
#include "GLCore.h"
struct oVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoords;
	float TexID;
};
class Obstacle
{
public:

	Obstacle(glm::vec2 i_BottomLeft, glm::vec2 i_BottomRight, glm::vec2 i_TopRight, glm::vec2 i_TopLeft, glm::vec4 i_Color);
	~Obstacle();
	 oVertex* GetBegVertex() { return m_Vertex; }
	 inline float GetHeight() { return m_Height; }
	 inline float GetWidth() { return m_Width; }
	 inline oVertex* GetVertices() { return m_Vertex; }
	oVertex  m_Vertex[4];
private:
	float m_Width, m_Height;
	glm::vec4 m_Color;

};
