#pragma once
#include "GLCore.h"
#include <random>
#include <vector>
#include <cmath>
struct Vec2
{
	float x, y;
};
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
	float m_Energy;
	Vec2 m_CurrentVelocity;
	float m_k;
	float m_AnimationSpeed;
	bool m_DirY;
	bool m_DirX;
	Vec2 m_CurrentPosition;

	

public:
	const float getCurrentVelocity();
	const void getE() {
		std::cout << "x: " << m_CurrentPosition.x << " y:" << m_CurrentPosition.y << std::endl <<
			"velo x:" << m_CurrentVelocity.x << "velo y:" << m_CurrentVelocity.y << std::endl;
	}
	
	Particle(glm::vec2 StartPos);
	~Particle();
	void Update();
	const Vertex* GetBegVertex() { return m_Vertex; }
	const glm::vec2 GetCastedShadowX() { return glm::vec2(m_CurrentPosition.x, m_CurrentPosition.x + m_Size); }
	const glm::vec2 GetCastedShadowY() { return glm::vec2(m_CurrentPosition.y, m_CurrentPosition.y+ m_Size); }
	bool operator < (const Particle& Part) const
	{
		return (m_CurrentPosition.x < Part.m_CurrentPosition.x);
	}
	

private:
	friend  void CreateQuad(Particle* target, float x, float y, float textureID,float size);
	friend void SweepAndPrune(std::vector<Particle>& Particles,size_t size);
	friend  unsigned int  Direction(Particle& p1, Particle& p2) ;
	//friend std::vector<std::pair<int, Particle>>SweepAndPruneColisionHandler(std::vector<std::vector<Particle>>Particles);
	

};
