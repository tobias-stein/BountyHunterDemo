///-------------------------------------------------------------------------------------------------
/// File:	Game.cpp.
///
/// Summary:	Implements the game application class.
///-------------------------------------------------------------------------------------------------

#include "Game.h"

Game::Game(const char* name) :
	m_GameState(GameState::NOT_INITIALIZED),
	m_GameTitle(name),
	m_Window(nullptr),
	m_WindowPosX(-1), m_WindowPosY(-1),
	m_WindowWidth(-1), m_WindowHeight(-1)
{}

Game::~Game()
{}


void Game::InitializeECS()
{
	// start the engine
	ECS::Initialize();
}

void Game::InitializeSDL()
{
	// Initialize SDL 2.0
	SDL_Init(SDL_INIT_VIDEO);

	// Create a new window for OpenGL rendering prupose
	this->m_Window = SDL_CreateWindow(this->m_GameTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->m_WindowWidth, this->m_WindowHeight, SDL_WINDOW_OPENGL);
	if (m_Window == 0) {

		SDL_Log("Unable to create game application window! %s", SDL_GetError());
		exit(-1);
	}

	// Place and resize application window
	SDL_GetWindowPosition(this->m_Window, &this->m_WindowPosX, &this->m_WindowPosY);
	SDL_GetWindowSize(this->m_Window, &this->m_WindowWidth, &this->m_WindowHeight);
}


void Game::Initialize(int width, int height) 
{
	this->m_WindowWidth		= width;
	this->m_WindowHeight	= height;

	// Initialize SDL
	InitializeSDL();

	// Initialize ECS
	InitializeECS();

	// broadcast initial window state
	ECS::ECS_Engine->SendEvent<WindowResizedEvent>(this->m_WindowWidth, this->m_WindowHeight);

	// Create and initialize systems
	{
		if (ALLOW_CHEATS == true)
		{
			// CheatSystem
			CheatSystem*		CHEATS = ECS::ECS_Engine->GetSystemManager()->AddSystem<CheatSystem>();
		}

		// InputSystem
		InputSystem*		InS = ECS::ECS_Engine->GetSystemManager()->AddSystem<InputSystem>();

		// RenderSystem
		RenderSystem*		RdS = ECS::ECS_Engine->GetSystemManager()->AddSystem<RenderSystem>(this->m_Window);

		// ATTENTION: The order how the Physics and World System are added matters!
		// PhysicsSystem
		PhysicsSystem*		PyS = ECS::ECS_Engine->GetSystemManager()->AddSystem<PhysicsSystem>();

		// WorldSystem
		WorldSystem*		WoS = ECS::ECS_Engine->GetSystemManager()->AddSystem<WorldSystem>();

		// RespawnSystem
		RespawnSystem*		ReS = ECS::ECS_Engine->GetSystemManager()->AddSystem<RespawnSystem>();

		// LifetimeSystem
		LifetimeSystem*		LS = ECS::ECS_Engine->GetSystemManager()->AddSystem<LifetimeSystem>();

		// ControllerSystem
		ControllerSystem*	CoS = ECS::ECS_Engine->GetSystemManager()->AddSystem<ControllerSystem>();

		// PlayerSystem
		PlayerSystem*		PlS = ECS::ECS_Engine->GetSystemManager()->AddSystem<PlayerSystem>();

		// Change InputSystem's priority to high
		ECS::ECS_Engine->GetSystemManager()->SetSystemPriority<InputSystem>(ECS::HIGH_SYSTEM_PRIORITY);

		// Add system dependencies
		CoS->AddDependencies(InS);
		WoS->AddDependencies(InS);
		PyS->AddDependencies(InS, WoS);
		RdS->AddDependencies(PyS);
		LS->AddDependencies(ReS);

		ECS::ECS_Engine->GetSystemManager()->UpdateSystemWorkOrder();

		// create two system work state masks: one when runnign, one when game is paused

		// ingame system work state
		this->m_Ingame_SystemWSM = ECS::ECS_Engine->GetSystemManager()->GetSystemWorkState();

		// not ingame system work state (gameover, paused, ...)
		this->m_NotIngame_SystemWSM = ECS::ECS_Engine->GetSystemManager()->GenerateActiveSystemWorkState(PlS, RdS, InS);
	}

	this->Restart();
}

void Game::Restart()
{
	//------------------------------------------
	// Reset systems
	//------------------------------------------
	
	// change system manager active work state
	ECS::ECS_Engine->GetSystemManager()->SetSystemWorkState(this->m_Ingame_SystemWSM);

	// Clear player
	ECS::ECS_Engine->GetSystemManager()->GetSystem<PlayerSystem>()->RemoveAllPlayers();

	// Clear world
	ECS::ECS_Engine->GetSystemManager()->GetSystem<WorldSystem>()->Clear();

	ECS::ECS_Engine->GetSystemManager()->GetSystem<RespawnSystem>()->Reset();
	ECS::ECS_Engine->GetSystemManager()->GetSystem<LifetimeSystem>()->Reset();

	// reset game context
	this->m_GameContext = GameContext();




	WorldSystem* worldSystem = ECS::ECS_Engine->GetSystemManager()->GetSystem<WorldSystem>();
	RespawnSystem* respawnSystem = ECS::ECS_Engine->GetSystemManager()->GetSystem<RespawnSystem>();
	PlayerSystem* playerSystem = ECS::ECS_Engine->GetSystemManager()->GetSystem<PlayerSystem>();

	//------------------------------------------
	// Create Camera
	//------------------------------------------

	GameObjectId cameraId = ECS::ECS_Engine->GetEntityManager()->CreateEntity<TabletopCamera>(glm::vec2(0.0f, 0.0f), -10.0f, 5.0f);
	TabletopCamera* camera = (TabletopCamera*)ECS::ECS_Engine->GetEntityManager()->GetEntity(cameraId);
	camera->SetViewport(0, 0, this->m_WindowWidth, this->m_WindowHeight);


	//------------------------------------------
	// Create Walls
	//------------------------------------------

	glm::vec3 wallSize = glm::vec3(1.0f, WORLD_BOUND_MAX[1] * 1.5f, 1.0f);

	// left
	worldSystem->AddGameObject<Wall>(Transform(Position(WORLD_BOUND_MIN[0] - 3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(0.0f), wallSize), wallSize);

	// right
	worldSystem->AddGameObject<Wall>(Transform(Position(WORLD_BOUND_MAX[0] + 3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(180.0f), wallSize), wallSize);

	// top
	worldSystem->AddGameObject<Wall>(Transform(Position(0.0f, WORLD_BOUND_MAX[0] + 3.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(90.0f), wallSize), wallSize);

	// bottom
	worldSystem->AddGameObject<Wall>(Transform(Position(0.0f, WORLD_BOUND_MIN[0] - 3.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(270.0f), wallSize), wallSize);


	//------------------------------------------
	// Create Player
	//------------------------------------------

	const float STEP = glm::two_pi<float>() / max(1.0f, (float)MAX_PLAYER);
	const float R = (WORLD_BOUND_MAX[0] - WORLD_BOUND_MIN[0]) * 0.5f;

	for (size_t i = 0; i < MAX_PLAYER; ++i)
	{
		const float angle = i * STEP;
		const float xR = glm::cos(angle) * R;
		const float yR = glm::sin(angle) * R;


		Position spawnPosition(xR, yR, 0.0f);

		Transform initialTransform = glm::translate(glm::mat4(1.0f), spawnPosition) * glm::rotate(angle + glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::vec3(2.0f));

		// create collector spawn
		GameObjectId collectorSpawn = worldSystem->AddGameObject<PlayerSpawn>(Transform(Position(xR, yR, 1.0f)), spawnPosition, angle + glm::radians(90.0f));


		PlayerId playerId = INVALID_PLAYER_ID;
		Player* player = nullptr;
		if ((i == 0) && (HAS_HUMAN_PLAYER == true))
		{
			// create human player
			playerId = playerSystem->AddNewPlayer(DEFAULT_PLAYER_NAME);

			// create stash and collector
			GameObjectId playerStashId = worldSystem->AddGameObject<Stash>(glm::translate(glm::mat4(1.0f), Position(xR, yR, 1.0f)) * glm::scale(glm::vec3(2.5f)), playerId);
			GameObjectId collectorId = worldSystem->AddGameObject<Collector>(initialTransform, collectorSpawn);

			player = playerSystem->GetPlayer(playerId);
			player->SetStash(playerStashId);
			player->SetController(new PlayerCollectorController(collectorId, playerId));
		}
		else
		{
			// create ai player
			playerId = playerSystem->AddNewPlayer(("Player #" + std::to_string(i)).c_str());

			// create stash and collector
			GameObjectId playerStashId = worldSystem->AddGameObject<Stash>(glm::translate(glm::mat4(1.0f), Position(xR, yR, 1.0f)) * glm::scale(glm::vec3(2.5f)), playerId);
			GameObjectId collectorId = worldSystem->AddGameObject<Collector>(initialTransform, collectorSpawn);

			player = playerSystem->GetPlayer(playerId);
			player->SetStash(playerStashId);
			player->SetController(new AICollectorController(collectorId, playerId, AICollectorControllerDesc()));
		}
	}


	//------------------------------------------
	// Create Bounty
	//------------------------------------------

	// create bounty spawn
	const float bountyHalfSpawnSize = R * 0.75f;
	GameObjectId bountySpawnId = worldSystem->AddGameObject<BountySpawn>(Transform(Position(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, glm::vec3(bountyHalfSpawnSize, bountyHalfSpawnSize, 0.0f)), Position(0.0f, 0.0f, 0.0f), glm::vec2(bountyHalfSpawnSize, bountyHalfSpawnSize), 0.0f);
	BountySpawn* bountySpawn = (BountySpawn*)ECS::ECS_Engine->GetEntityManager()->GetEntity(bountySpawnId);

	// spawn bounty
	for (size_t i = 0; i < MAX_BOUNTY; ++i)
	{
		SpawnInfo spawnInfo = bountySpawn->GetSpawnInfo();
		GameObjectId bountyId = worldSystem->AddGameObject<Bounty>(Transform(spawnInfo.m_SpawnPosition, glm::vec3(0.0f, 0.0f, 1.0f), spawnInfo.m_SpawnOrientation.z), bountySpawnId);
		((Bounty*)ECS::ECS_Engine->GetEntityManager()->GetEntity(bountyId))->OnEnable(); // little hack to get the initial bounty size, color and value
	}

	RegisterEventCallback(&Game::OnCollisionBegin);
	RegisterEventCallback(&Game::OnStashFull);

	// put game into game state 'RESTARTED'
	ChangeState(GameState::RESTARTED);
}

void Game::GameOver()
{
	// change system manager active work state
	ECS::ECS_Engine->GetSystemManager()->SetSystemWorkState(this->m_NotIngame_SystemWSM);

	UnregisterEventCallback(&Game::OnCollisionBegin);
	UnregisterEventCallback(&Game::OnStashFull);
}

void Game::Terminate()
{
	// Unregister
	UnregisterAllEventCallbacks();

	// Terminate ECS
	ECS::Terminate();

	// Destroy window
	if (this->m_Window)
		SDL_DestroyWindow(this->m_Window);

	// Terminate SDL
	SDL_Quit();

	// this will break the main game loop.
	this->m_Window = nullptr;
}

void Game::ProcessWindowEvent()
{
	// Pump all SDL events to queue
	SDL_PumpEvents();
	
	SDL_Event event;
	while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_SYSWMEVENT)) {

		switch (event.window.event) 
		{
			case SDL_WINDOWEVENT_SHOWN:
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				break;
			case SDL_WINDOWEVENT_MOVED:
				break;
			case SDL_WINDOWEVENT_RESIZED:
				ECS::ECS_Engine->SendEvent<WindowResizedEvent>(event.window.data1, event.window.data2);
				break;

			case SDL_WINDOWEVENT_SIZE_CHANGED:
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				ECS::ECS_Engine->SendEvent<WindowMinimizedEvent>();
				break;

			case SDL_WINDOWEVENT_MAXIMIZED:
				break;
			case SDL_WINDOWEVENT_RESTORED:
				ECS::ECS_Engine->SendEvent<WindowRestoredEvent>();
				break;

			case SDL_WINDOWEVENT_ENTER:
				break;
			case SDL_WINDOWEVENT_LEAVE:
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				break;
			case SDL_WINDOWEVENT_CLOSE:
				this->Terminate();
				return;

#if SDL_VERSION_ATLEAST(2, 0, 5)
			case SDL_WINDOWEVENT_TAKE_FOCUS:
				break;
			case SDL_WINDOWEVENT_HIT_TEST:
				break;
#endif
		}
	}
}

void Game::Step()
{
	switch (this->m_GameState)
	{
		case GameState::NOT_INITIALIZED:
			// GAME MUST BE INITIALIZED!
			return;

		case GameState::GAMEOVER:
			// GAME NEEDS RESTART!
			return;

		case GameState::RESTARTED:
			this->m_GameState = GameState::RUNNING;
			break;

		case GameState::TERMINATED:
			// ALREADY TERMINATED!
			return;
	}
		

	// Process game application window events
	ProcessWindowEvent();
	if (this->m_Window == nullptr)
		return;

	// Update game logic
	{
		// if there is freeze time ...
		if (this->m_GameContext.FreezeTime > 0.0f)
		{
			this->m_GameContext.FreezeTime -= DELTA_TIME_STEP;
		}
		// else update game time ...
		else if (this->m_GameContext.PlayTime > 0.0f)
		{
			this->m_GameContext.PlayTime -= DELTA_TIME_STEP;
		}
		// else game time elapsed ...
		else
		{
			Player* winner = nullptr;
			for (PlayerId p = 0; p < MAX_PLAYER; ++p)
			{
				Player* player = ECS::ECS_Engine->GetSystemManager()->GetSystem<PlayerSystem>()->GetPlayer(p);
				if (player != nullptr)
				{
					if (winner == nullptr)
					{
						winner = player;
						continue;
					}

					Stash* playerStash = (Stash*)ECS::ECS_Engine->GetEntityManager()->GetEntity(player->GetStash());
					Stash* winnerStash = (Stash*)ECS::ECS_Engine->GetEntityManager()->GetEntity(winner->GetStash());

					if (playerStash->GetStashValue() > winnerStash->GetStashValue())
					{
						winner = player;
					}
				}

			}
			this->GameOver();
		}
	}

	// Update the ECS
	ECS::ECS_Engine->Update(DELTA_TIME_STEP);
}