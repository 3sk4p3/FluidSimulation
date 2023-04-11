#include "Particle.h"

//const float Particle::getCurrentVelocity()
//{
//	return sqrt(abs((2.0f*m_k*(m_PreviousHeight - m_CurrentHeight)) + static_cast<float>(pow(m_PreviousVelocity,2))));
//}
Particle::Particle(glm::vec2 StartPos, float I_size)
	:m_StartPos(StartPos), m_Size(I_size), m_DirY(false)
	, m_k(0.96f), m_AnimationSpeed(0.1f), m_CurrentVelocity({ 0.0f,0.0f }), m_Acceleration(0.05f)
	
{
	m_CurrentPosition.x = m_StartPos.x;
	m_CurrentPosition.y = m_StartPos.y;
	CreateQuad(this, m_StartPos.x, m_StartPos.y, 1.0f,m_Size);
}

Particle::~Particle()
{
	//czy tu nie powinno czegows byc?
}

void Particle::Update()
{

		if (m_CurrentPosition.x > 15.0f-m_Size || m_CurrentPosition.x < 0.0f)
		{
			m_DirX = !m_DirX;
			m_CurrentVelocity.x *= m_k;


		}
		if ( m_CurrentPosition.y < 0.0f||m_CurrentVelocity.y<0.0f)
		{
			m_DirY = !m_DirY;
			m_CurrentVelocity.y *= m_k;
			m_CurrentPosition.y += m_CurrentVelocity.y*m_AnimationSpeed;

		}
	
		if(m_DirY&&m_CurrentPosition.y>=0.0f){
		m_CurrentPosition.y += m_CurrentVelocity.y * m_AnimationSpeed;
		m_CurrentVelocity.y -= m_Acceleration;
		}
		else
		{
			m_CurrentPosition.y -= m_CurrentVelocity.y * m_AnimationSpeed;
			m_CurrentVelocity.y += m_Acceleration;
		}
		if (m_DirX) {
			m_CurrentPosition.x += m_CurrentVelocity.x * m_AnimationSpeed;

		}
		else
		{
			m_CurrentPosition.x -= m_CurrentVelocity.x * m_AnimationSpeed;

		}

		CreateQuad(this, m_CurrentPosition.x,m_CurrentPosition.y, 1.0f, m_Size);

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

		
				 double Dir = Direction(*result[i][j], *result[i][k]);
				 float Vx  = result[i][j]->m_CurrentVelocity.x *result[i][j]->m_k* cos(Dir) - result[i][j]->m_CurrentVelocity.y*result[i][j]->m_k  *  sin(Dir);
				 float Vy = result[i][j]->m_CurrentVelocity.x  *result[i][j]->m_k* sin(Dir) + result[i][j]->m_CurrentVelocity.y*result[i][j]->m_k *  cos(Dir);
				 float Vy2 = result[i][k]->m_CurrentVelocity.x *result[i][j]->m_k* sin(Dir) + result[i][k]->m_CurrentVelocity.y*result[i][j]->m_k *  cos(Dir);
				 float Vx2 = result[i][k]->m_CurrentVelocity.x *result[i][j]->m_k* cos(Dir) - result[i][k]->m_CurrentVelocity.y*result[i][j]->m_k *  sin(Dir);

				 float buf = Vx;
				 Vx = Vx2;
				 Vx2 = buf;
		/*		 buf = Vy;
				 Vy = Vy2;
				 Vy2 = buf;*/
			///*	 if (Vx * result[i][j]->m_CurrentVelocity.x < 0) {  
			//		 
			//		 result[i][j]->m_DirX = !result[i][j]->m_DirX;
			//		 result[i][k]->m_DirX = !result[i][k]->m_DirX;
			//	 }

			//	 if (Vy * result[i][j]->m_CurrentVelocity.y < 0) {
			//		
			//		 result[i][j]->m_DirY = !result[i][j]->m_DirX;
			//		 result[i][k]->m_DirY = !result[i][k]->m_DirY;
			//	 }*/
				result[i][j]->m_CurrentVelocity.x = Vx * cos(-Dir) - Vy*   sin(-Dir);
				result[i][j]->m_CurrentVelocity.y = Vx * sin(-Dir) + Vy*   cos(-Dir);
			   result[i][k]->m_CurrentVelocity.x = Vx2 * cos(-Dir) - Vy2 * sin(-Dir);
			   result[i][k]->m_CurrentVelocity.y = Vx2 * sin(-Dir) + Vy2 * cos(-Dir);				
				

			}
		}

	}

}

 double Direction(Particle& p1, Particle& p2)
{
		
		glm::vec2 Middle_1 = { p1.m_Vertex[0].Position.x + p1.m_Size / 2,p1.m_Vertex[0].Position.y + p1.m_Size / 2 };
		glm::vec2 Middle_2 = { p2.m_Vertex[0].Position.x + p2.m_Size / 2,p2.m_Vertex[0].Position.y + p2.m_Size / 2 };
		double angle = atan2f(Middle_2.y - Middle_1.y, Middle_2.x - Middle_1.x);
		return angle;

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








