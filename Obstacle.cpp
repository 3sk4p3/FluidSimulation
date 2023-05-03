#include "Obstacle.h"



Obstacle::Obstacle(glm::vec2 i_BottomLeft, glm::vec2 i_BottomRight, glm::vec2 i_TopRight, glm::vec2 i_TopLeft, glm::vec4 i_Color):
	m_Color(i_Color)
{
	m_Vertex[0].Position = { i_BottomLeft.x,i_BottomLeft.y,0.0f };
	m_Vertex[0].Color = i_Color;
	m_Vertex[0].TexCoords = { 0.0f, 0.0f };
	m_Vertex[0].TexID =2.0f ;


	m_Vertex[1].Position = { i_BottomRight.x,i_BottomRight.y,0.0f };
	m_Vertex[1].Color= i_Color;
	m_Vertex[1].TexCoords = { 1.0f, 0.0f };
	m_Vertex[1].TexID = 2.0f;

	m_Vertex[2].Position = { i_TopRight.x,i_TopRight.y,0.0f };
	m_Vertex[2].Color = i_Color;
	m_Vertex[2].TexCoords = { 1.0f, 1.0f };
	m_Vertex[2].TexID = 2.0f;


	m_Vertex[3].Position = { i_TopLeft.x,i_TopLeft.y,0.0f };
	m_Vertex[3].Color = i_Color;
	m_Vertex[3].TexCoords = { 0.0f, 1.0f };
	m_Vertex[3].TexID = 2.0f;
	m_Width = i_BottomRight.x - i_BottomLeft.x;
	m_Height = i_TopLeft.y - i_BottomLeft.y;
}
Obstacle::~Obstacle()
{
	
}
