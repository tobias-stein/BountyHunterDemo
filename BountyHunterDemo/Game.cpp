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

		// PlayerStateSystem
		PlayerStateSystem*	PSS = ECS::ECS_Engine->GetSystemManager()->AddSystem<PlayerStateSystem>();

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

	ECS::ECS_Engine->GetSystemManager()->GetSystem<PlayerStateSystem>()->Reset();

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
	// Create Bounty
	//------------------------------------------

	// create bounty spawn
	const float R = (WORLD_BOUND_MAX[0] - WORLD_BOUND_MIN[0]) * 0.5f;
	const float bountyHalfSpawnSize = R * 0.75f;
	GameObjectId bountySpawnId = worldSystem->AddGameObject<BountySpawn>(Transform(Position(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, glm::vec3(bountyHalfSpawnSize, bountyHalfSpawnSize, 0.0f)), Position(0.0f, 0.0f, 0.0f), glm::vec2(bountyHalfSpawnSize, bountyHalfSpawnSize), 0.0f);
	BountySpawn* bountySpawn = (BountySpawn*)ECS::ECS_Engine->GetEntityManager()->GetEntity(bountySpawnId);

	// spawn bounty
	for (size_t i = 0; i < INT_SETTING(MAX_BOUNTY); ++i)
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

PlayerId Game::AddPlayer()
{
	assert(this->m_GameContext.NumPlayer < INT_SETTING(MAX_PLAYER));

	WorldSystem* worldSystem = ECS::ECS_Engine->GetSystemManager()->AddSystem<WorldSystem>();
	PlayerSystem* playerSystem = ECS::ECS_Engine->GetSystemManager()->AddSystem<PlayerSystem>();

	const float STEP = glm::two_pi<float>() / max(1.0f, (float)INT_SETTING(MAX_PLAYER));
	const float R = (WORLD_BOUND_MAX[0] - WORLD_BOUND_MIN[0]) * 0.5f;


	const float angle = this->m_GameContext.NumPlayer * STEP;
	const float xR = glm::cos(angle) * R;
	const float yR = glm::sin(angle) * R;


	Position spawnPosition(xR, yR, 0.0f);

	Transform initialTransform = glm::translate(glm::mat4(1.0f), spawnPosition) * glm::rotate(angle + glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::vec3(2.0f));

	// create collector spawn
	GameObjectId collectorSpawn = worldSystem->AddGameObject<PlayerSpawn>(Transform(Position(xR, yR, 1.0f)), spawnPosition, angle + glm::radians(90.0f));


	PlayerId playerId = INVALID_PLAYER_ID;
	Player* player = nullptr;

	// create human player
	playerId = playerSystem->AddNewPlayer(DEFAULT_PLAYER_NAME);

	// create stash and collector
	GameObjectId playerStashId = worldSystem->AddGameObject<Stash>(glm::translate(glm::mat4(1.0f), Position(xR, yR, 1.0f)) * glm::scale(glm::vec3(2.5f)), playerId);
	GameObjectId collectorId = worldSystem->AddGameObject<Collector>(initialTransform, collectorSpawn);

	player = playerSystem->GetPlayer(playerId);
	player->SetStash(playerStashId);
	player->SetController(new PlayerCollectorController(collectorId, playerId));


	// increment player count
	this->m_GameContext.NumPlayer++;

	return playerId;
}

void Game::GameOver(PlayerId winnerId)
{
	// change system manager active work state
	ECS::ECS_Engine->GetSystemManager()->SetSystemWorkState(this->m_NotIngame_SystemWSM);

	UnregisterEventCallback(&Game::OnCollisionBegin);
	UnregisterEventCallback(&Game::OnStashFull);

	this->m_GameContext.WinnerId = winnerId;
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

int Game::Step(PlayerAction** const in_actions, PlayerState* out_states, void* out_framebuffer)
{
	if (this->m_GameContext.WinnerId > 0)
		return this->m_GameContext.WinnerId;

	switch (this->m_GameState)
	{
		case GameState::NOT_INITIALIZED:
			// GAME MUST BE INITIALIZED!
			return -1;

		case GameState::GAMEOVER:
			// GAME NEEDS RESTART!
			return -1;

		case GameState::RESTARTED:
			this->m_GameState = GameState::RUNNING;
			break;

		case GameState::TERMINATED:
			// ALREADY TERMINATED!
			return -1;
	}	

	// Process game application window events
	ProcessWindowEvent();
	if (this->m_Window == nullptr)
		return -1;

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
			// set player actions, iff any
			if (in_actions != nullptr)
			{
				PlayerSystem* playerSystem = ECS::ECS_Engine->GetSystemManager()->GetSystem<PlayerSystem>();
				for (PlayerId p = 0; p < INT_SETTING(MAX_PLAYER); ++p)
				{
					Player* player = playerSystem->GetPlayer(p);
					if (player != nullptr)
					{
						player->GetController().SetFrameAction(in_actions[p]);
					}
				}
			}

			this->m_GameContext.PlayTime -= DELTA_TIME_STEP;
		}
		// else game time elapsed ...
		else
		{
			Player* winner = nullptr;
			Stash* winnerStash = nullptr;
			for (PlayerId p = 0; p < INT_SETTING(MAX_PLAYER); ++p)
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
					winnerStash = (Stash*)ECS::ECS_Engine->GetEntityManager()->GetEntity(winner->GetStash());

					if (playerStash->GetStashValue() > winnerStash->GetStashValue())
					{
						winner = player;
					}
				}

			}
			
			// if winner has empty stash, all player have. Choose random winner.
			this->GameOver((glm::abs(winnerStash->GetStashValue()) < glm::epsilon<float>()) ? glm::linearRand<PlayerId>(0, ECS::ECS_Engine->GetSystemManager()->GetSystem<PlayerSystem>()->GetActivePlayerCount()) : winner->GetPlayerId());
		}
	}

	// Update the ECS
	ECS::ECS_Engine->Update(DELTA_TIME_STEP);

	// copy states
	if (out_states != nullptr)
	{
		const PlayerState* states = ECS::ECS_Engine->GetSystemManager()->GetSystem<PlayerStateSystem>()->GetPlayerStates();

		for (PlayerId pID = 0; pID < INT_SETTING(MAX_PLAYER); ++pID)
		{
			out_states[pID] = states[pID];
		}
	}

	// copy framebuffer
	if (out_framebuffer != nullptr)
	{
		std::memcpy(out_framebuffer, (const void*)ECS::ECS_Engine->GetSystemManager()->GetSystem<RenderSystem>()->GetCurrentFrameBuffer(), size_t(GAME_WINDOW_WIDTH * GAME_WINDOW_HEIGHT * 3));
	}

	return -1;
}

