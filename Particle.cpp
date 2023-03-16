#include "Particle.h"

//const float Particle::getCurrentVelocity()
//{
//	return sqrt(abs((2.0f*m_k*(m_PreviousHeight - m_CurrentHeight)) + static_cast<float>(pow(m_PreviousVelocity,2))));
//}



Particle::Particle(glm::vec2 StartPos)
	:m_StartPos(StartPos), m_Size(0.5f),m_DirY(false),m_Mass(5.0f)
	,m_k(0.8f),m_AnimationSpeed(0.0005f),m_CurrentVelocity({0.0f,0.0f})
	
{
	m_CurrentPosition.x = m_StartPos.x;
	m_CurrentPosition.y = m_StartPos.y;
	m_Energy = m_k * m_CurrentPosition.y;
	CreateQuad(this, m_StartPos.x, m_StartPos.y, 1.0f,m_Size);
}

Particle::~Particle()
{
	//czy tu nie powinno czegows byc?
}

void Particle::Update()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TODO~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//box ma byc na zmiennych, moze niech sie rozszerza im wiecej jest kulek???
		// ODBICIE Z POJEMNIKIEM -prawo i lewo
		if (m_CurrentPosition.x > 14.0f || m_CurrentPosition.x < 0.0f) 	m_DirX = !m_DirX;
		// ODBICIE Z POJEMNIKIEM gora i dol
		if ((m_CurrentPosition.y <= 0.0f && m_DirY == 0) ||( m_CurrentVelocity.y<=0 && m_DirY ==1&& m_CurrentPosition.y>=0.0f))
	
		{
			m_CurrentVelocity.y*= 0.9;
			m_DirY = !m_DirY;
		}
		// RUCH NA OSI X:
		if (m_CurrentVelocity.x > 0.0f)
		{
			//1 na tej zmiennej oznacza ruch w prawo, wiec zwiekszam
			if (m_DirX) m_CurrentPosition.x += m_CurrentVelocity.x * m_AnimationSpeed;
			//ustawiamy predkosc na osi x tej czasteczki= tej co ja uderzyla, po czym zwalniamy o jej przyspieszenie??
			else m_CurrentPosition.x -= m_CurrentVelocity.x * m_AnimationSpeed;
		}
		// RUCH NA OSI Y:
		if (m_DirY)
		{			
					m_CurrentVelocity.y -= m_k;
					m_CurrentPosition.y += m_CurrentVelocity.y * m_AnimationSpeed;					
		}
		else if(!m_DirY)
		{	
				m_CurrentVelocity.y += m_k;	
				m_CurrentPosition.y -= m_CurrentVelocity.y * m_AnimationSpeed;		
		}
			
		if(m_CurrentPosition.y>0.0f&&m_CurrentVelocity.y>=0.0f)	CreateQuad(this, float(m_CurrentPosition.x), float(m_CurrentPosition.y), 1.0f, m_Size);
}









void SweepAndPrune(std::vector<Particle> &Particles, size_t size)
{
	std::vector <std::vector<Particle*>> result;
	std::vector<Particle*> Active;
	unsigned int CurrentLast = 0;
	result.clear();
	Active.clear();
	
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
				//KOLIZJA PRAWO-LEWO
				if (Dir == 3)
				{
					//zmiana kieruynku 	obu czasteczek na osi x
					result[i][j]->m_DirX = !result[i][j]->m_DirX;
					result[i][k]->m_DirX = !result[i][k]->m_DirX;


					//>>
					
					result[i][j]->m_CurrentVelocity.x = result[i][k]->m_CurrentVelocity.y;
					result[i][k]->m_CurrentVelocity.x = result[i][k]->m_CurrentVelocity.y;

				}
				//KOLIZJA GORA-DOL
				else {
					bool direction = result[i][j]->m_DirY;
					result[i][j]->m_DirY = result[i][k]->m_DirY;
					result[i][k]->m_DirY = direction;

					float buf = result[i][j]->m_CurrentVelocity.y*0.99f;
					result[i][j]->m_CurrentVelocity.y = result[i][k]->m_CurrentVelocity.y*0.99f;
					result[i][k]->m_CurrentVelocity.y = buf;
					
					
				    }

	
			
			}
		}

	}

}

unsigned int Direction(Particle& p1, Particle& p2)
{

	glm::vec2 Middle = { p1.m_Vertex[0].Position.x + p1.m_Size / 2,p1.m_Vertex[0].Position.y + p1.m_Size / 2 };
	float currentMin=p1.m_Size*sqrt(2) + p2.m_Size * sqrt(2);
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

void CreateQuad(Particle* target, float x, float y, float textureID, float size)
{
	{


		target->m_Vertex[0].Position = { x ,y, 0.0f };
		target->m_Vertex[0].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[0].TexCoords = { 0.0f, 0.0f };
		target->m_Vertex[0].TexID = textureID;



		target->m_Vertex[1].Position = { x + size,y, 0.0f };
		target->m_Vertex[1].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[1].TexCoords = { 1.0f, 0.0f };
		target->m_Vertex[1].TexID = textureID;


		target->m_Vertex[2].Position = { x + size,y + size, 0.0f };
		target->m_Vertex[2].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[2].TexCoords = { 1.0f, 1.0f };
		target->m_Vertex[2].TexID = textureID;


		target->m_Vertex[3].Position = { x ,y + size, 0.0f };
		target->m_Vertex[3].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[3].TexCoords = { 0.0f, 1.0f };
		target->m_Vertex[3].TexID = textureID;





	}
}








