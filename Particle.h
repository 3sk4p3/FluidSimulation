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
	glm::vec2 m_AccelerationXY;//przyspeiszenie
	float m_Energy;
	float m_SpringEnergy;
	float m_K;
	float m_Size;

public:
	const float getE() { return m_SpringEnergy; }
	Particle(glm::vec2 StartPos);
	~Particle();
	void Update();
	const Vertex* GetBegVertex() { return m_Vertex; }
	const Vertex* GetEndVertex() { return &m_Vertex[3]; }
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
