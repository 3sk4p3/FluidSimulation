
#include "Particle.h"



const float Particle::getCurrentVelocity()
{
	return (float)sqrt(2 * 10.0f*(m_PreviousHeight - m_CurrentHeight) + pow(m_PreviousVelocity, 2));
}

Particle::Particle(glm::vec2 StartPos)
	:m_StartPos(StartPos), m_Size(1.0f),m_Dir(false),m_Mass(1.0f),m_CurrentVelocity(0.0f),m_CurrentHeight(StartPos.y)
	,m_k(0.8f),m_CurrMax(StartPos.y)
{

	m_VelocityXY = { 0.0f,0.0f };



	m_CurrentPosition.x = m_StartPos.x;
	m_CurrentPosition.y = m_StartPos.y;
	m_Energy = m_Mass *0.1f;
	m_Acceleration = 0.000009f*m_Energy / m_Mass;
	

	

	CreateQuad(this, m_StartPos.x, m_StartPos.y, 1.0f,m_Size);
}

Particle::~Particle()
{
}

void Particle::Update(float ts)
{

	

	m_PreviousHeight = m_CurrentHeight;
	m_PreviousVelocity = m_CurrentVelocity;
	m_CurrentHeight = m_CurrentPosition.y;
	m_CurrentVelocity = getCurrentVelocity();

	//prawo i lewo
	if (m_CurrentPosition.x> 14.0f || m_CurrentPosition.x < 0.0f)
	{	
	}
	//gora i dol
	if (( m_CurrentPosition.y < 0.0f|| m_CurrentPosition.y > 14.0f ))
	{
		
		if (m_Dir == true&&m_CurrentPosition.y>0.0f)m_Dir = false;
		else if(m_Dir == false && m_CurrentPosition.y < 0.0f) m_Dir = true;
		
		std::cout << "ODBICIE:"<<m_Dir << std::endl;
	}
	


	if (m_CurrMax > 0.1f)
	{



		if (m_Dir) {//do gory leci
			if (m_CurrentPosition.y > m_k * m_CurrMax)
			{
				m_CurrentVelocity *= m_k;
				m_CurrMax *= m_k;
				if (m_Dir == true)m_Dir = false;
				else m_Dir = true;

				std::cout << "SPADEK:" << m_Dir << std::endl;

			}
			else {

				m_CurrentVelocity -= m_Acceleration * 0.15;
				m_CurrentPosition.y += m_CurrentVelocity *0.05 ;
				CreateQuad(this, float(m_CurrentPosition.x), float(m_CurrentPosition.y), 1.0f, m_Size);
				std::cout << "do GORY" << std::endl;
			}
		}
		else {//spada
			

			m_CurrentVelocity += m_Acceleration *0.15;
			m_CurrentPosition.y -= m_CurrentVelocity * 0.05;
			CreateQuad(this, float(m_CurrentPosition.x), float(m_CurrentPosition.y), 1.0f, m_Size);
			std::cout << "SPADA" << std::endl;
		}
		
	}
	


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
		

				/*	result[i][j]->m_AccelerationXY.x = -result[i][j]->m_AccelerationXY.x;
					result[i][k]->m_AccelerationXY.x = -result[i][k]->m_AccelerationXY.x;*/

				}
				else {
					if (result[i][j]->m_Dir == true)result[i][j]->m_Dir = false;
					else result[i][j]->m_Dir = true;
					//result[i][j]->m_CurrentPosition.y += result[i][j]->m_CurrentVelocity * 0.2;


					if (result[i][k]->m_Dir == true)result[i][k]->m_Dir = false;
					else result[i][k]->m_Dir = true;
					//result[i][k]->m_CurrentPosition.y += result[i][k]->m_CurrentVelocity * 0.2;
					std::cout << " S&P ODBICIE:" << result[i][j]->m_Dir << std::endl;
					std::cout << "x:" << result[i][j]->m_CurrentPosition.x << " y:" << result[i][j]->m_CurrentPosition.y << " Acceleration:" << result[i][j]->m_Acceleration
						<< " Current Velocity :" << result[i][j]->m_CurrentVelocity << " Curr Max:" << result[i][j]->m_CurrMax << std::endl;
					float buf = result[i][j]->m_CurrentVelocity;
					result[i][j]->m_CurrentVelocity= result[i][k]->m_CurrentVelocity;
					result[i][k]->m_CurrentVelocity=buf;
					buf = result[i][j]->m_CurrMax;
					result[i][j]->m_CurrMax = result[i][k]->m_CurrMax;
					result[i][k]->m_CurrMax = buf;
					//result[i][j]->m_AccelerationXY.y = -result[i][j]->m_AccelerationXY.y;
					//result[i][k]->m_AccelerationXY.y = -result[i][k]->m_AccelerationXY.y;
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









