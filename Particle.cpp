
#include "Particle.h"



Particle::Particle(glm::vec2 StartPos)
	:m_StartPos(StartPos), m_Size(1.0f),m_K(0.01),m_SpringEnergy(0.0f)
{
	m_VelocityXY = { 1.0f,1.0f };
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> distinit(0.0001, 0.08);
	std::uniform_real_distribution<float> distinit2(2.0, 12.0);

	float height = distinit2(mt);
	m_Energy = height * 10;//CHYBA GIT
	height = -distinit(mt);
	m_AccelerationXY = { 0.0f,height};	



	

	CreateQuad(this, m_StartPos.x, m_StartPos.y, 1.0f,m_Size);
}

Particle::~Particle()
{
}

void Particle::Update()
{
	bool flipped = false;


	//prawo i lewo
	if (m_VelocityXY.x + m_StartPos.x> 14.0f || m_VelocityXY.x + m_StartPos.x < 0.0f)
	{
		
		m_AccelerationXY.x= -m_AccelerationXY.x;	
	}
	//gora i dol
	if (flipped==false&&(m_VelocityXY.y + m_StartPos.y > 14.0f || m_VelocityXY.y + m_StartPos.y< 0.0f))
	{
		flipped = true;
		m_AccelerationXY.y = -m_AccelerationXY.y;//zmiana kierunkiu
	}
	if (flipped == false && m_SpringEnergy < 0)
	{
		flipped = true;
		m_AccelerationXY = -m_AccelerationXY;
	}
	m_Energy -= 0.1;
	if (flipped)
	{
		m_K = -0.02;
	}
	else m_K = 0.01;
	m_SpringEnergy += m_K;
	//predkosc = predkosc+ przyspieszenie
	m_VelocityXY.x += m_AccelerationXY.x;
	m_VelocityXY.y += m_AccelerationXY.y;
	if (m_Energy > 0)CreateQuad(this, float(m_StartPos.x + m_VelocityXY.x), float(m_StartPos.y + m_VelocityXY.y), 1.0f,m_Size);




}						   


void CreateQuad(Particle* target, float x, float y, float textureID,float size)
{
	{
	

		target->m_Vertex[0].Position = { x ,y, 0.0f };
		target->m_Vertex[0].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[0].TexCoords = { 0.0f, 0.0f };
		target->m_Vertex[0].TexID = textureID;
		
		

		target->m_Vertex[1].Position = { x+size,y, 0.0f };
		target->m_Vertex[1].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[1].TexCoords = { 1.0f, 0.0f };
		target->m_Vertex[1].TexID = textureID;


		target->m_Vertex[2].Position = { x + size,y+size, 0.0f };
		target->m_Vertex[2].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[2].TexCoords = { 1.0f, 1.0f };
		target->m_Vertex[2].TexID = textureID;


		target->m_Vertex[3].Position = { x ,y + size, 0.0f };
		target->m_Vertex[3].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[3].TexCoords = { 0.0f, 1.0f };
		target->m_Vertex[3].TexID = textureID;





	}
}





//TO SPRAWDZONE I DZIALA TAK JAK POWINNO.
void SweepAndPrune(std::vector<Particle> &Particles, size_t size)
{
	std::vector <std::vector<Particle*>> result;
	std::vector<Particle*> Active;
	unsigned int CurrentLast = 0;

	

	while (CurrentLast < size)
	{
		Active.push_back(&Particles[CurrentLast]);
		for (size_t i = CurrentLast+1; i < size; ++i)
		{

			if (Particles[CurrentLast].GetCastedShadowX().y >= Particles[i].GetCastedShadowX().x)
			{
				Active.push_back(&Particles[i]);
				CurrentLast = i;
			}
		}
		result.push_back(Active);
		Active.clear();
		++CurrentLast;

	}

	for (int i=0;i<result.size();++i)
	{

		for (size_t j = 0; j < result[i].size() - 1; ++j)
		{
			for (size_t k = j + 1; k < result[i].size(); ++k)
			{

				if (result[i][j]->GetCastedShadowX().y <= result[i][k]->GetCastedShadowX().x ||
					result[i][j]->GetCastedShadowY().y <= result[i][k]->GetCastedShadowY().x ||
					result[i][j]->GetCastedShadowX().x >= result[i][k]->GetCastedShadowX().y ||
					result[i][j]->GetCastedShadowY().x >= result[i][k]->GetCastedShadowY().y
					)continue;
				
				unsigned int Dir = Direction(*result[i][k], *result[i][j]);
				if (Dir == 3)
				{
					result[i][j]->m_AccelerationXY.x = -result[i][j]->m_AccelerationXY.x;
					result[i][k]->m_AccelerationXY.x = -result[i][k]->m_AccelerationXY.x;

				}
				else {

					result[i][j]->m_AccelerationXY.y = -result[i][j]->m_AccelerationXY.y;
					result[i][k]->m_AccelerationXY.y = -result[i][k]->m_AccelerationXY.y;
				    }

	
			
			}
		}

	}

}

unsigned int Direction(Particle& p1, Particle& p2)
{

	glm::vec2 Middle = { p1.m_Vertex[0].Position.x + p1.m_Size / 2,p1.m_Vertex[0].Position.y + p1.m_Size / 2 };
	float currentMin=p1.m_Size+p2.m_Size;
	int result = 0;
	for (size_t i=0;i<3;++i)
	{
		float sum=0.0f;
		glm::vec2 currentPos = { p2.m_Vertex[i].Position.x,p2.m_Vertex[i].Position.y};
		auto distance = glm::length(Middle - currentPos);
		sum += distance;
		 currentPos = { p2.m_Vertex[i+1].Position.x,p2.m_Vertex[i+1].Position.y };
		 distance = glm::length(Middle - currentPos);
		 sum += distance;
		 if (currentMin>sum)
		 {
			 currentMin = sum;
			 result = i + i + 1;
		 }

	}
	return result;
}









