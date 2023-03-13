
#include "Particle.h"



const float Particle::getCurrentVelocity()
{
	return sqrt(abs(2*(m_PreviousHeight - m_CurrentHeight) + pow(m_PreviousVelocity,2)));
}



Particle::Particle(glm::vec2 StartPos)
	:m_StartPos(StartPos), m_Size(0.5f),m_DirY(false),m_Mass(1.0f),m_CurrentHeight(StartPos.y)
	,m_k(0.99f),m_CurrMax(StartPos.y)
{




	m_CurrentPosition.x = m_StartPos.x;
	m_CurrentPosition.y = m_StartPos.y;
	m_Energy = m_Mass ;
	//przyspieszenie na obu osiach
	m_Acceleration = { 0.0f,1.0f };
	//predkosc na obu osiach
	m_CurrentVelocity = { 0.0f,0.0f };


	

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


	//ustawiamy poprzedni stan czasteczki na ten ktory wlasnie mamy obecnie
	m_PreviousHeight = m_CurrentHeight;
	m_PreviousVelocity = m_CurrentVelocity.y;

	//zmieniamy obecny stan czasteczki os y
	m_CurrentHeight = m_CurrentPosition.y;
	m_CurrentVelocity.y = getCurrentVelocity();
	//zmniejszamy przyspieszenie ???


	m_CurrentVelocity.x -= m_Acceleration.x/50;
	if (m_CurrentVelocity.x > 0.0f)
	{

		//1 na tej zmiennej oznacza ruch w prawo, wiec zwiekszam

		if (m_DirX) m_CurrentPosition.x += m_CurrentVelocity.x* 0.005;
		//ustawiamy predkosc na osi x tej czasteczki= tej co ja uderzyla, po czym zwalniamy o jej przyspieszenie??

		else m_CurrentPosition.x -= m_CurrentVelocity.x* 0.005;
	}







	// ODBICIE Z POJEMNIKIEM -prawo i lewo
	if (m_CurrentPosition.x > 14.0f || m_CurrentPosition.x < 0.0f) {
		m_DirX = !m_DirX;
		m_CurrentPosition.x += m_Acceleration.x *0.005;

}
	
	

	// ODBICIE Z POJEMNIKIEM gora i dol
		if ((m_CurrentPosition.y < 0.0f || m_CurrentPosition.y > 14.0f))
		{
		m_DirY = !m_DirY;
		m_CurrentPosition.y += m_Acceleration.y*0.005;
		}



	//ma sie wykonywac dopoki maksymalna wysokosc mozliwa do osiagniecia przez czasteczke jest wieksza 0.1f;
	if (m_CurrMax > 0.1f)
	{



		if (!m_DirY) {
			//jezeli jest ponad obecnym najwyzszym mozliwym punktem dla czasteczki
			if (m_CurrentPosition.y > m_k * m_CurrMax)
			{
				//przenmnazamy predkosc i maksymalna wysokosc przez wspolczynnik m_k i zmieniamy kierunek ruchu czasteczki;
				m_CurrentVelocity.y *= m_k;
				m_CurrMax *= m_k;

				m_DirY = !m_DirY;


			}
			else {
				//leci do gory

				m_CurrentVelocity.y -= m_Acceleration.y * 0.005;
				m_CurrentPosition.y += m_CurrentVelocity.y *0.005 ;
			
			}
		}
		else if (m_DirY) {//spada
			

			m_CurrentVelocity.y += m_Acceleration.y *0.005;
			m_CurrentPosition.y -= m_CurrentVelocity.y * 0.005;
		}
	
		CreateQuad(this, float(m_CurrentPosition.x), float(m_CurrentPosition.y), 1.0f, m_Size);

		
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

		//std::cout << "NOWE:\n";
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
				//uderzenie prawo lewo
				if (Dir == 3)
				{
					//std::cout << "PRAWO-LEWO\n";
					//zmiana kieruynku 
					
					result[i][k]->m_DirX = !result[i][k]->m_DirX;
					result[i][j]->m_DirX = !result[i][j]->m_DirX;


					//przypisanie predkosci			,x
					float buf = result[i][j]->m_CurrentVelocity.x;
					result[i][j]->m_CurrentVelocity.x = result[i][k]->m_CurrentVelocity.y;
					result[i][k]->m_CurrentVelocity.x = buf;

					//przypisanie przyspieszen
					buf = result[i][j]->m_Acceleration.x;
					result[i][j]->m_Acceleration.x = result[i][k]->m_Acceleration.y;
					result[i][k]->m_Acceleration.x = buf;

				

				}
				else {
					//std::cout << "GORA-DOL\n";
					result[i][j]->m_DirY =!result[i][j]->m_DirY;

					
					//zmiana wspolczynnika spowalnaicjacego
					result[i][j]->m_CurrMax *= result[i][j]->m_k;
					result[i][k]->m_CurrMax *= result[i][k]->m_k;

		
			
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









