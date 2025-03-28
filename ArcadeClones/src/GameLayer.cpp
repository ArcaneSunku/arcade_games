#include <Arcane.h>

#include "GameLayer.h"

static GameState s_State = GameState::Menu;
static MenuState s_MenuState = MenuState::Main;

GameLayer::GameLayer() :
	Layer("Test2D")
{
	uint32_t width = Arcane::Application::Get().GetWindow()->GetWidth();
	uint32_t height = Arcane::Application::Get().GetWindow()->GetHeight();
	m_CamController = Arcane::CreateShared<Arcane::OrthoCameraController>((float)width / (float)height);
}

GameLayer::~GameLayer()
{

}

void GameLayer::OnAttach()
{
	ARC_PROFILE_FUNCTION();
	m_SndDevice = Arcane::SoundDevice::Create();
	m_SndLib = Arcane::SoundLibrary::Create();

	m_CamController->GetPosition().x += m_CamController->GetAspectRatio() * m_CamController->GetZoom();
	m_CamController->GetPosition().y += m_CamController->GetZoom();

	m_Game = new Game();
	m_Game->Init(m_SndLib);

	m_SndDevice->SetAttunation(AL_INVERSE_DISTANCE_CLAMPED);

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	m_Fonts["arcade36"] = io.Fonts->AddFontFromFileTTF("assets/fonts/ARCADEPI.ttf", 36);
	m_Fonts["arcade24"] = io.Fonts->AddFontFromFileTTF("assets/fonts/ARCADEPI.ttf", 24);
	m_Fonts["arcade16"] = io.Fonts->AddFontFromFileTTF("assets/fonts/ARCADEPI.ttf", 16);
}

void GameLayer::OnDetach()
{
	ARC_PROFILE_FUNCTION();

	m_Game->Dispose();
	m_Fonts.clear();
}

void GameLayer::OnUpdate(Arcane::Timestep ts)
{
	ARC_PROFILE_FUNCTION();

	m_Game->OnUpdate(ts, m_CamController, s_State, s_MenuState);

	Arcane::RenderCMD::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	Arcane::RenderCMD::Clear(true);

	Arcane::Renderer2D::ResetStats();
	Arcane::Renderer2D::BeginScene(*m_CamController->GetCamera());

	m_Game->OnRender(ts, m_CamController, s_State);

	Arcane::Renderer2D::EndScene();
}

void GameLayer::OnEvent(Arcane::Event& event)
{
	m_Game->OnEvent(event);
	m_CamController->OnEvent(event);
}

void GameLayer::OnImGuiRender()
{
	ARC_PROFILE_FUNCTION();

	ImGuiWindowFlags winFlags = ImGuiWindowFlags_None;
	winFlags |= ImGuiWindowFlags_NoDecoration;
	winFlags |= ImGuiWindowFlags_NoResize;
	winFlags |= ImGuiWindowFlags_NoCollapse;
	winFlags |= ImGuiWindowFlags_NoBackground;

	//
	switch (s_State)
	{
	case GameState::Menu:
	{
		switch (s_MenuState)
		{
			case MenuState::Main: MainMenu(winFlags); break;
			case MenuState::GameSelect: GameSelect(winFlags); break;
			case MenuState::About: About(winFlags); break;
		}
		break;
	}
	default:
		m_Game->OnImGuiRender(winFlags, s_State);
		break;
	}

#if 0
	ImGui::Begin("Settings");
	Arcane::Renderer2D::Statistics stats = Arcane::Renderer2D::GetStats();
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);

	ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
	ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());
	ImGui::End();
#endif
}

void GameLayer::MainMenu(ImGuiWindowFlags flags)
{
	std::string title = "Arcane Classics";
	float fntSize = m_Fonts["arcade36"]->FontSize;
	ImVec2 fontSize = m_Fonts["arcade36"]->CalcTextSizeA(fntSize, FLT_MAX, 0.0f, title.c_str());

	ImGuiViewport* main = ImGui::GetWindowViewport();
	ImVec2 tSize = { fontSize.x + fntSize, fontSize.y };
	ImVec2 center = main->GetCenter();

	ImVec2 winPos = { center.x - tSize.x * 0.5f, center.y - (main->Size.y - tSize.y) * 0.25f };

	if (winPos.x <= main->Pos.x) winPos.x = main->Pos.x;
	if (winPos.y <= main->Pos.y) winPos.y = main->Pos.y;

	ImGui::SetNextWindowPos(winPos);
	ImGui::SetNextWindowSize({ tSize.x, tSize.y });
	ImGui::Begin("TitleScreen", (bool*)0, flags);

	auto winSize = ImGui::GetWindowSize();
	ImGui::SetCursorPosX((winSize.x - fontSize.x) * 0.5f);
	ImGui::SetCursorPosY((winSize.y - fontSize.y) * 0.5f);

	ImGui::PushFont(m_Fonts["arcade36"]);
	ImGui::Text(title.c_str());
	ImGui::PopFont();
	ImGui::End();

	fontSize = m_Fonts["arcade24"]->CalcTextSizeA(fntSize, FLT_MAX, 0.0f, "About");
	tSize = { fontSize.x + m_Fonts["arcade24"]->FontSize, fontSize.y };

	ImGui::SetNextWindowPos({ center.x - tSize.x * 0.5f, winPos.y + tSize.y + 50 });
	ImGui::SetWindowSize({ 200, 200 });

	ImGui::Begin("Buttons", (bool*)0, flags);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.2f, 0.2f, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.72f, 0.2f, 0.3f, 0.2f));

	ImGui::PushFont(m_Fonts["arcade24"]);
	bool playGame = ImGui::Button("Play", { 100, 30 });
	bool about = ImGui::Button("About", { 100, 30 });
	bool exitGame = ImGui::Button("Exit", { 100, 30 });
	ImGui::PopFont();

	ImGui::PopStyleColor(3);
	ImGui::End();

	if (playGame)
	{
		s_MenuState = MenuState::GameSelect;
		return;
	}

	if (about)
	{
		s_MenuState = MenuState::About;
		return;
	}

	if (exitGame)
	{
		Arcane::Application::Get().Stop();
		s_MenuState = MenuState::NotActive;
		return;
	}
}

void GameLayer::GameSelect(ImGuiWindowFlags flags)
{
	float fntSize = m_Fonts["arcade24"]->FontSize;
	ImVec2 fontSize = m_Fonts["arcade24"]->CalcTextSizeA(fntSize, FLT_MAX, 0.0f, "Brick Breaker");

	ImGuiViewport* main = ImGui::GetWindowViewport();
	ImVec2 tSize = { fontSize.x + fntSize, fontSize.y };
	ImVec2 center = main->GetCenter();

	ImGui::SetNextWindowPos({ center.x - (200.0f / 2.0f), center.y - 100.0f });
	ImGui::SetNextWindowSize({ 200, 200 });

	ImGui::Begin("Buttons", (bool*)0, flags);
	auto winSize = ImGui::GetWindowSize();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.2f, 0.2f, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.72f, 0.2f, 0.3f, 0.2f));

	ImGui::PushFont(m_Fonts["arcade24"]);
	bool asteroids = ImGui::Button("Asteroids", { 200, 25 });
	ImGui::PopFont();

	ImGui::PushFont(m_Fonts["arcade16"]);
	bool back = ImGui::Button("Back", { 200, 25 });
	ImGui::PopFont();

	ImGui::PopStyleColor(3);
	ImGui::End();

	if (asteroids)
	{
		s_MenuState = MenuState::NotActive;
		s_State = GameState::Asteroids;
		m_Game->StartGame(s_State, m_CamController);
		return;
	}

	if (back)
	{
		s_MenuState = MenuState::Main;
		return;
	}
}

void GameLayer::About(ImGuiWindowFlags flags)
{
	std::string about[] = { "This game was made with the Arcane Framework.",
						    "It was developed by ArcaneSunku(Tahnner)." };

	float fntSize = m_Fonts["arcade24"]->FontSize;
	ImVec2 fontSize = m_Fonts["arcade24"]->CalcTextSizeA(fntSize, FLT_MAX, 0.0f, about[0].c_str());
	ImVec2 fontSize1 = m_Fonts["arcade24"]->CalcTextSizeA(fntSize, FLT_MAX, 0.0f, about[1].c_str());

	ImGuiViewport* main = ImGui::GetWindowViewport();
	ImVec2 tSize = { fontSize.x + fntSize, fontSize.y + fontSize1.y + (fntSize * 2) };
	ImVec2 center = main->GetCenter();

	ImVec2 winPos = { center.x - tSize.x * 0.5f, center.y - (main->Size.y - tSize.y) * 0.25f };

	if (winPos.x <= main->Pos.x) winPos.x = main->Pos.x;
	if (winPos.y <= main->Pos.y) winPos.y = main->Pos.y;

	ImGui::SetNextWindowPos(winPos);
	ImGui::SetNextWindowSize({ tSize.x, tSize.y });

	ImGui::Begin("AboutScreen", (bool*)0, flags);

	auto winSize = ImGui::GetWindowSize();
	ImGui::SetCursorPosX((winSize.x - fontSize.x) * 0.5f);
	ImGui::SetCursorPosY((winSize.y - fontSize.y) * 0.5f);

	ImGui::PushFont(m_Fonts["arcade24"]);
	ImGui::Text(about[0].c_str());

	ImGui::SetCursorPosX((winSize.x - fontSize1.x) * 0.5f);
	ImGui::Text(about[1].c_str());
	ImGui::PopFont();
	ImGui::End();

	fontSize = m_Fonts["arcade16"]->CalcTextSizeA(fntSize, FLT_MAX, 0.0f, "Back");
	tSize = { fontSize.x + fntSize, fontSize.y };

	ImGui::SetNextWindowPos({ center.x - (150.0f / 2.0f), winPos.y + 100.0f });
	ImGui::SetNextWindowSize({ 150, 200 });

	ImGui::Begin("Buttons", (bool*)0, flags);
	winSize = ImGui::GetWindowSize();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.2f, 0.2f, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.72f, 0.2f, 0.3f, 0.2f));

	ImGui::PushFont(m_Fonts["arcade16"]);
	bool back = ImGui::Button("Back", { 100, 25 });
	ImGui::PopFont();

	ImGui::PopStyleColor(3);
	ImGui::End();

	if (back)
	{
		s_MenuState = MenuState::Main;
		return;
	}
}
