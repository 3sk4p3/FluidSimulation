#include "Particle.h"
#include "SandboxLayer.h"
float FloorLevel = 0.0f;
//const float Particle::getCurrentVelocity()
//{
//	return sqrt(abs((2.0f*m_k*(m_PreviousHeight - m_CurrentHeight)) + static_cast<float>(pow(m_PreviousVelocity,2))));
//}

Particle::Particle(glm::vec2 StartPos, float I_size)
	:m_StartPos(StartPos), m_Size(I_size), m_DirY(false)
	, m_k(1.0f), m_AnimationSpeed(0.1f), m_CurrentVelocity({ 0.0f,0.0f }),m_Gravity{0.0f,1000.0f}
	
{
	m_CurrentPosition.x = m_StartPos.x;
	m_CurrentPosition.y = m_StartPos.y;
	m_PreviousPosition = m_CurrentPosition;
	m_Acceleration = { 0.0f,0.0f };
	CreateQuad(this, m_StartPos.x, m_StartPos.y, 1.0f,m_Size);
}

Particle::~Particle()
{
	//czy tu nie powinno czegows byc?
}

void Particle::Update(float dt)
{
	
	const int sub_steps = 8;
	const float sub_dt = dt / static_cast<float>(sub_steps);
	for (size_t i = 0; i < sub_steps; i++)
	{
		//circular shape equations
	/*	const glm::vec2 position{ 7.5f,7.5f };
		const float radius = 10.0f;
		glm::vec2 to_obj = m_CurrentPosition - position;
		const float dist = glm::length(to_obj);
		if (dist > radius - m_Size / 2)
		{
			const glm::vec2 n = to_obj / dist;
			m_CurrentPosition = position + n * (radius - m_Size / 2);
		}*/
		//rectangular shape equations

		//winda jedzie w gore
		if (ElevatorToggler)
		{

			if (FloorLevel > 15.0f) {
				FloorLevel = 0.0f;
				ElevatorToggler = false;
			}
			FloorLevel += 0.00001f;

		}

		//jak jest rura zamknieta

		if (m_CurrentPosition.y>15.0f&&m_CurrentPosition.x>15.0f )

		{
			glm::vec2 left = { 50.0f, 0.0f };
				m_Acceleration += left;
		}

	
		
		m_Acceleration += m_Gravity;
		m_CurrentVelocity = m_CurrentPosition - m_PreviousPosition;
		m_PreviousPosition = m_CurrentPosition;
		m_CurrentPosition = m_CurrentPosition + m_CurrentVelocity - m_Acceleration * sub_dt * sub_dt;
		//odbitka od rury
		//jak rura zamkmnieta, to ma sie odjbjac od niej.
		if (!LottoToggler&&m_CurrentPosition.x < 15.0f)m_CurrentPosition.x = 15.0f;


		
		else
		{


				if (m_CurrentPosition.x < 5.0f&& m_CurrentPosition.y < 35.0f)m_CurrentPosition.x = 5.0f;
				if (m_CurrentPosition.x < 15.0f)
				{

				//if ((m_CurrentPosition.x >= 15.0f - m_Size)&&(m_CurrentPosition.y<=15.0f))m_CurrentPosition.x = 15.0f - m_Size;
				}
			
				if (m_CurrentPosition.y < FloorLevel)m_CurrentPosition.y = FloorLevel;
		}
		if (m_CurrentPosition.x > 30.0f-m_Size )m_CurrentPosition.x = 30.0f-m_Size ;


		if (m_CurrentPosition.x > 15.0f-m_Size)
		{

		if(m_CurrentPosition.y<15.0f-m_Size)m_CurrentPosition.x = 15.0f - m_Size;
		else if (m_CurrentPosition.y>15.0f)
		{

			if (m_CurrentPosition.y < m_CurrentPosition.x / 2.0f + 7.7f)m_CurrentPosition.y = m_CurrentPosition.x / 2.0f + 7.7f;
			else if (m_CurrentPosition.y > m_CurrentPosition.x / 2.0f + 9.5f)m_CurrentPosition.y = m_CurrentPosition.x / 2.0f + 9.5f;
		}
		}
		m_Acceleration = {};
	}


	CreateQuad(this, m_CurrentPosition.x, m_CurrentPosition.y, 1.0f, m_Size);

}









void SweepAndPrune(std::vector<Particle>& Particles)
{
	
	//for (size_t i = 0; i < Particles.size()-1; ++i)
	//{
	//	for (size_t j = i+1; j < Particles.size(); j++)
	//	{
	//		const glm::vec2 Collision_axis = Particles[i].m_CurrentPosition - Particles[j].m_CurrentPosition ;
	//		const float dist = glm::length(Collision_axis);
	//		float bufSize = Particles[i].m_Size+ Particles[j].m_Size;
	//		//float bufSize = Particles[i].m_Size;
	//		//if (Particles[j].m_Size>bufSize)bufSize=Particles[j].m_Size;
	//		if (dist < bufSize )
	//		{

	//									const glm::vec2 n = Collision_axis / dist;
	//									const float delta = bufSize  - dist;
	//									Particles[j].m_CurrentPosition -= 0.05f * delta * n;
	//									Particles[i].m_CurrentPosition += 0.05f * delta * n;
	//		}
	//	}

	//}

	
	std::vector<std::vector <std::vector<Particle*>>> result;



	result.resize(30);
	for (int i = 0; i <30; ++i)result[i].resize(30);
	for (auto &i : Particles)
	{

		int bufRow = i.m_CurrentPosition.x;
		int bufCol = i.m_CurrentPosition.y;
		if (bufRow >=29)bufRow = 29;
		else if (bufRow < 0)bufRow = 0;
		if (bufCol >= 29)bufCol = 29;
		else if (bufCol < 0)bufCol = 0;
		result[bufRow][bufCol].push_back(&i);
	}

	for (int i = 0; i <= 29; ++i)
	{		
		for (int j = 0; j <= 29; ++j)
		{
			for (int dx = -1; dx <=1 ; ++dx)
			{
				for (int dy = -1; dy <= 1; ++dy)
				{
				
					
					for (auto &Current : result[i][j])
					{


						int where_at_x = i + dx;
						int where_at_y = j + dy;
						
						if (where_at_x > 29 || where_at_y > 29||where_at_x<0||where_at_y<0)continue;
						for (auto& Checked : result[where_at_x][where_at_y])
						{
							//if (dx == 0 && dy == 0)continue;
							/*double Dir = Direction(*Current, *Checked);
							float Vx =  Current->m_CurrentVelocity.x * Current->m_k * cos(Dir) - Current->m_CurrentVelocity.y * Current->m_k * sin(Dir);
							float Vy = Current->m_CurrentVelocity.x * Current->m_k * sin(Dir) + Current->m_CurrentVelocity.y * Current->m_k * cos(Dir);
							float Vy2 = Checked->m_CurrentVelocity.x * Current->m_k * sin(Dir) + Checked->m_CurrentVelocity.y * Current->m_k * cos(Dir);
							float Vx2 = Checked->m_CurrentVelocity.x * Current->m_k * cos(Dir) - Checked->m_CurrentVelocity.y *Current->m_k*  sin(Dir);
							float buf = Vx;
							Vx = Vx2;
							Vx2 = buf;
			

							Current->m_CurrentVelocity.x = Vx * cos(-Dir) - Vy * sin(-Dir);
							Current->m_CurrentVelocity.y = Vx * sin(-Dir) + Vy * cos(-Dir);
							Checked->m_CurrentVelocity.x = Vx2 * cos(-Dir) - Vy2 * sin(-Dir);
							Checked->m_CurrentVelocity.y = Vx2 * sin(-Dir) + Vy2 * cos(-Dir);
							*/
									

	
									//float bufSize = Particles[i].m_Size;
									//if (Particles[j].m_Size>bufSize)bufSize=Particles[j].m_Size;
								
				

								

		
								
								float bufSize = Current->m_Size/2 + Checked->m_Size/2;
								const glm::vec2 Collision_axis =   Current->m_CurrentPosition- Checked->m_CurrentPosition;
								const float dist = glm::length(Collision_axis);
								if (dist < bufSize&&dist>0.0f)
								{
									const glm::vec2 n = Collision_axis / dist;
									const float delta = bufSize - dist;
									Current->m_CurrentPosition +=	0.005f * delta * n;
									Checked->m_CurrentPosition -=	0.005f * delta * n;
									
								}
			


						}
					

					}


				}
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








