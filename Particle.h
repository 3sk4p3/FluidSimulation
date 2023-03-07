#pragma once
#include "GLCore.h"
#include <random>
#include <vector>
#include <cmath>

struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoords;
	float TexID;
};

class Particle
{

private:
	glm::vec2 m_StartPos;
	Vertex  m_Vertex[4];
	glm::vec2 m_VelocityXY;//predkosc
	float m_Size;
	float m_Mass;
	float m_Acceleration;
	float m_Energy;
	float m_CurrentHeight;
	float m_PreviousHeight;
	float m_CurrentVelocity;
	float m_PreviousVelocity;
	float m_k;
	float m_CurrMax;
	glm::vec2 m_CurrentPosition;

	bool m_Dir;
	

public:
	const float getCurrentVelocity();
	const void getE() {
		std::cout << "x:" << m_CurrentPosition.x << " y:" << m_CurrentPosition.y << " Acceleration:" << m_Acceleration
			<< " Current Velocity :" <<m_CurrentVelocity<<" Curr Max:"<<m_CurrMax<< std::endl;
	}
	Particle(glm::vec2 StartPos);
	~Particle();
	void Update();
	const Vertex* GetBegVertex() { return m_Vertex; }
	const glm::vec2 GetCastedShadowX() { return glm::vec2(m_StartPos.x + m_VelocityXY.x, m_StartPos.x + m_VelocityXY.x + m_Size); }
	const glm::vec2 GetCastedShadowY() { return glm::vec2(m_StartPos.y + m_VelocityXY.y, m_StartPos.y + m_VelocityXY.y + m_Size); }
	bool operator < (const Particle& Part) const
	{
		return ((m_StartPos.x+m_VelocityXY.x) < (Part.m_StartPos.x+Part.m_VelocityXY.x));
	}
	

private:
	friend  void CreateQuad(Particle* target, float x, float y, float textureID,float size);
	friend void SweepAndPrune(std::vector<Particle>& Particles,size_t size);
	friend  unsigned int  Direction(Particle& p1, Particle& p2) ;
	//friend std::vector<std::pair<int, Particle>>SweepAndPruneColisionHandler(std::vector<std::vector<Particle>>Particles);
	

};
