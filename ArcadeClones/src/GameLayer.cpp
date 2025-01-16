#include "GameLayer.h"

enum State : uint32_t
{
	Menu = 0,
	Asteroids,
	PacMan,
	BrickBreak
};

enum MenuState : uint32_t
{
	NotActive = 0,
	Main, GameSelect, About
};

static State s_State = State::Menu;
static MenuState s_MenuState = MenuState::Main;

GameLayer::GameLayer() :
	Layer("Test2D")
{
	uint32_t width = Arcane::Application::Get().GetWindow()->GetWidth();
	uint32_t height = Arcane::Application::Get().GetWindow()->GetHeight();
	m_CamController = new Arcane::OrthoCameraController((float)width / (float)height);
}

GameLayer::~GameLayer()
{
	delete m_CamController;
}

void GameLayer::OnAttach()
{
	ARC_PROFILE_FUNCTION();

	m_TextureMap["bb_blocks"] = Arcane::Texture2D::Create("assets/textures/brickbreak-clone/blocks.png");
	m_TextureMap["bb_sprites"] = Arcane::Texture2D::Create("assets/textures/brickbreak-clone/sprites.png");

	m_TextureMap["astr_ship"] = Arcane::Texture2D::Create("assets/textures/asteroids-clone/ship.png");
	m_TextureMap["astr_alien_ship"] = Arcane::Texture2D::Create("assets/textures/asteroids-clone/enemy_ships.png");

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/ARCADEPI.ttf", 36);

}

void GameLayer::OnDetach()
{
	ARC_PROFILE_FUNCTION();

	m_Font->ClearOutputData();
	m_TextureMap.clear();
}

void GameLayer::OnUpdate(Arcane::Timestep ts)
{
	ARC_PROFILE_FUNCTION();
	m_CamController->OnUpdate(ts);

	Arcane::RenderCMD::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	Arcane::RenderCMD::Clear(true);

	Arcane::Renderer2D::ResetStats();
	Arcane::Renderer2D::BeginScene(*m_CamController->GetCamera());

	Arcane::Renderer2D::EndScene();
}

void GameLayer::OnEvent(Arcane::Event& event)
{
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

	switch (s_State)
	{
	case Menu:
	{

		switch (s_MenuState)
		{
		case MenuState::Main: MainMenu(winFlags); break;
		case MenuState::GameSelect: GameSelect(winFlags); break;
		case MenuState::About: About(winFlags); break;
		}

		break;
	}
	case Asteroids:
		break;
	case PacMan:
		break;
	case BrickBreak:
		break;
	default:
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
	ImVec2 fontSize = m_Font->CalcTextSizeA(m_Font->FontSize, FLT_MAX, 0.0f, title.c_str());

	ImGuiViewport* main = ImGui::GetWindowViewport();
	ImVec2 tSize = { fontSize.x + m_Font->FontSize, fontSize.y };
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

	ImGui::PushFont(m_Font);
	ImGui::Text(title.c_str());
	ImGui::PopFont();
	ImGui::End();

	fontSize = m_Font->CalcTextSizeA(m_Font->FontSize, FLT_MAX, 0.0f, "Play");
	tSize = { fontSize.x + m_Font->FontSize, fontSize.y };

	ImGui::SetNextWindowPos({ center.x - tSize.x * 0.5f, winPos.y + tSize.y + 50 });
	ImGui::SetWindowSize({ 200, 200 });

	ImGui::Begin("Buttons", (bool*)0, flags);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.2f, 0.2f, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.72f, 0.2f, 0.3f, 0.2f));

	ImGui::PushFont(m_Font);
	bool playGame = ImGui::Button("Play", { 100, 50 });
	bool exitGame = ImGui::Button("Exit", { 100, 50 });
	ImGui::PopFont();

	ImGui::PopStyleColor(3);
	ImGui::End();

	if (playGame)
	{
		s_MenuState = MenuState::GameSelect;
		return;
	}

	if (exitGame)
	{
		Arcane::Application::Get().Stop();
		return;
	}
}

void GameLayer::GameSelect(ImGuiWindowFlags flags)
{

}

void GameLayer::About(ImGuiWindowFlags flags)
{

}
