#include "SandboxLayer.h"
#include <algorithm>
#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
using namespace GLCore;
using namespace GLCore::Utils;
const size_t NumsofParticles =1000;
const size_t MaxParticleVertexCount = NumsofParticles * 4;
const size_t MaxParticleIndexCount = NumsofParticles* 6;
const float BaseSize = 0.5f;
bool	LottoToggler = false;



SandboxLayer::SandboxLayer()
	: m_CameraController(16.0f / 9.0f,true)

{

}



SandboxLayer::~SandboxLayer()
{
}
static GLuint LoadTexture(const std::string& path)
{
	int w, h, bits;
	stbi_set_flip_vertically_on_load(1);
	auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);
	GLuint textureID;
	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	stbi_image_free(pixels);
	return textureID;
}


void SandboxLayer::OnAttach()
{
	
	EnableGLDebugging();

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/test.vert.glsl",
		"assets/shaders/test.frag.glsl"		
	);
	glUseProgram(m_Shader->GetRendererID());
	auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
	int samplers[3] = { 0,1,2 };
	glUniform1iv(loc, 3, samplers);
	
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	
	
	



	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);
	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*(MaxParticleVertexCount+4), nullptr, GL_DYNAMIC_DRAW);

	uint32_t indices[(MaxParticleIndexCount+6)];
	uint32_t offset = 0;
	for (size_t i = 0; i < (MaxParticleIndexCount+6); i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;


		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;
		offset += 4;
	}


	glCreateBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);




	

	float _x = 0.45f;
	float _y = 14.0f;
	bool direction = false;
	m_Particles.reserve(NumsofParticles);
	float bufSize=BaseSize;
	int count=0;

	while (true)
	{
		if (count > NumsofParticles)
		{
			_y = 14.0f;
			break;

		}
		else bufSize /= 1.5f;
		count = 0;
		_y = 14.0f;
		while (true)
		{

			if (_y < 1.5f)break;


			_x += bufSize  *3;
			if (_x >= 15.0f-BaseSize)
			{
				_y -= bufSize * 1.2;

				if (direction)
				{
					direction = !direction;
					_x = bufSize*9/10 ;
				}
				else
				{
					direction = !direction;
					_x = 0.0f;
				}
			}
			++count;


		}
		
	
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(2.1*bufSize, 3*bufSize);


	for (size_t i = 0; i < NumsofParticles; ++i)
	{




		m_Particles.push_back(Particle({ _x+16.0f ,_y+16.0f }, bufSize));
		_x += dist(mt);
		if (_x >=15.0f -bufSize)
		{
			_y -= bufSize * 1.2;

			if (direction)
			{
				direction = !direction;
				_x = dist(mt);
			}
			else
			{
				direction = !direction;
				_x = dist(mt);
			}
		}

	}
	
	




	
	

	//koordynaty
	glEnableVertexAttribArray( 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) , (const void*)offsetof(Vertex, Position));

	//kolor
	glEnableVertexAttribArray( 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,Color));


	//mapowanie textury
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));


	//index textury
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));
	//glDisableVertexAttribArray(2);
	//glDisableVertexAttribArray(3);
;
	


	// Init here

	m_txt1 = LoadTexture("assets/textures/wb.png");
	m_txt2 = LoadTexture("assets/textures/wb.png");
	m_txt3 = LoadTexture("assets/textures/wb.png");
	m_CameraController.SetZoomLevel(0.0f);










	
}



static void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
{
	int loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}
void SandboxLayer::OnDetach()
{
	// Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);

	
	// Events here
}

static Vertex* CreateLine(Vertex* target, glm::vec2 topL, glm::vec2 topR,glm::vec2 botR, glm::vec2 botL)
{

	if (!LottoToggler)
	{

	target->Position = { 15.0f,15.0f,0.0f };//15 15
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 2.0f };
	++target;
	}

	target->Position = { 15.0f,17.5f,0.0f };//15 15
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 2.0f };
	++target;

	target->Position = { 30.0f,25.0f,0.0f };//15 15
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 2.0f };
	++target;

	target->Position = { 30.0f,22.5f,0.0f };//15 15
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 2.0f };
	++target;



	target->Position = { topR.x,topR.y,0.0f };//15 15
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 2.0f };
	++target;

	target->Position = { botR.x,botR.y,0.0f };//15 0
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 2.0f };
	++target;


	target->Position = { 5.0f,0.0f,0.0f };// 0 0
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 2.0f };
	++target;

	target->Position = { 5.0f,15.0f,0.0f }; //0 15
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 2.0f };
	++target;

	return target;
}



void SandboxLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	
	




	//`````````````````````QUADDDDDDDDS``````````````

	glBindVertexArray(m_QuadVA);

	 std::vector<Vertex>Vertices;
	Vertices.reserve(MaxParticleVertexCount+4);
	std::pair<double, double>CurrentMousePosition = GLCore::Input::GetMousePosition();
	m_Particles.push_back(Particle ({ (CurrentMousePosition.first-550.0f	)/20.0f ,(-(CurrentMousePosition.second-550.0f)		)/20.0f }, 1.0f));
	//CreateQuad(&MouseParticle, (CurrentMousePosition.first -645.0f)/100.0f, (CurrentMousePosition.second - 470.0f)/100.0f, 2.0f, 1.0f);
	
	//for (size_t i = 0; i < NumsofParticles+1; ++i)
	//{

	//	std::copy(m_Particles[i].GetBegVertex(), m_Particles[i].GetBegVertex()+4 , std::back_inserter(Vertices));

	//}
	for (size_t i = 0; i < NumsofParticles+1 ; ++i)
	{

			std::copy(m_Particles[i].GetBegVertex(), m_Particles[i].GetBegVertex()+4 , std::back_inserter(Vertices));

	}
	//Set dynamic vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (MaxParticleVertexCount+4)*sizeof(Vertex), Vertices.data());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_Shader->GetRendererID());
	glBindTextureUnit(0,m_txt1);
	glBindTextureUnit(1,m_txt2);
	glBindTextureUnit(2,m_txt3);

	auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();
	SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProjection", vp);
	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));



	glBindVertexArray(m_QuadVA);

	for (auto& i : m_Particles)
	{
		
		i.Update(0.02f);

	}
	SweepAndPrune(m_Particles);
	glDrawElements(GL_TRIANGLES, (MaxParticleIndexCount+6), GL_UNSIGNED_INT, nullptr);
	m_Particles.pop_back();


	

	// Render here




	// my shot at lines

	glCreateVertexArrays(1, &m_LineVA);
	glBindVertexArray(m_LineVA);
	glCreateBuffers(1, &m_LineVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_LineVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, nullptr, GL_DYNAMIC_DRAW);
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);



	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

	//kolor
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));


	//mapowanie textury
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));


	//index textury
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));


	std::array<Vertex, 8> my_vertices;
	Vertex* my_buffer = my_vertices.data();
	my_buffer = CreateLine(my_buffer, { 0,15 }, { 15,15 }, { 15,0 }, { 0,0 });
	glBufferSubData(GL_ARRAY_BUFFER,0, my_vertices.size() * sizeof(Vertex), my_vertices.data());
	

	
	float my_indices[] =
	{
 		0.0f,1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f
	};
	glCreateBuffers(1, &m_LineIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_LineIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*7, my_indices, GL_STATIC_DRAW);
	
	glDrawArrays(GL_LINE_STRIP, 0, 8);


	






}

void SandboxLayer::OnImGuiRender()
{

	ImGui::Begin("Options");

	if (ImGui::Button("Zwolnienie blokady")) {

		LottoToggler = !LottoToggler;
	}

	ImGui::End();
	/*ImGui::Begin("Controls");
	ImGui::DragFloat2("Quad Position", m_QuadPosition, 0.1f);
	ImGui::End();*/

	// ImGui here
}
