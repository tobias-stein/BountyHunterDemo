///-------------------------------------------------------------------------------------------------
/// File:	GameConfiguration.h.
///
/// Summary:	Declares the game configuration class.
///-------------------------------------------------------------------------------------------------

#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__

#include <string>
//#include <variant> // since C++17
#include <unordered_map>


#define NAME_AS_STRING(name) #name

#define CHECK_DEFAULT_BOOL(setting) \
	if (this->m_Settings.find(NAME_AS_STRING(setting)) == this->m_Settings.end()) this->m_Settings[NAME_AS_STRING(setting)].asBool = setting;

#define CHECK_DEFAULT_INT(setting) \
	if (this->m_Settings.find(NAME_AS_STRING(setting)) == this->m_Settings.end()) this->m_Settings[NAME_AS_STRING(setting)].asUint64 = setting;

#define CHECK_DEFAULT_FLOAT(setting) \
	if (this->m_Settings.find(NAME_AS_STRING(setting)) == this->m_Settings.end()) this->m_Settings[NAME_AS_STRING(setting)].asFloat = setting;


#define BOOL_SETTING(setting) \
	Settings::Get.asBool(NAME_AS_STRING(setting))

#define INT_SETTING(setting) \
	Settings::Get.asInt64(NAME_AS_STRING(setting))

#define UINT_SETTING(setting) \
	Settings::Get.asUint64(NAME_AS_STRING(setting))

#define FLOAT_SETTING(setting) \
	Settings::Get.asFloat(NAME_AS_STRING(setting))


/// Summary:	The game title.
static constexpr const char*			GAME_TITLE							{ "BountyHunter Demo" };

/// Summary:	The default player name.
static constexpr const char*			DEFAULT_PLAYER_NAME					{ "BountyHunter" };


// <<<< GAME WINDOW SETTINGS >>>>

/// Summary:	Width of the game window.
static constexpr unsigned int			GAME_WINDOW_WIDTH					{ 768 };

/// Summary:	Height of the game window.
static constexpr unsigned int			GAME_WINDOW_HEIGHT					{ 768 };


// <<<< GAME WORLD SETTINGS >>>>
 
/// Summary:	Defines the delta time step the game simulation will be advanced per update.
static constexpr float					DELTA_TIME_STEP						{ 1.0f / 60.0f };

/// Summary:	The world up vector.
static constexpr float					WORLD_UP_VECTOR[2]					{ 0.0f, 1.0f };

/// Summary:	The world min and max bounds.
static constexpr float					WORLD_BOUND_MIN[2]					{ -75.0f, -75.0f };
static constexpr float					WORLD_BOUND_MAX[2]					{  75.0f,  75.0f };

// <<<< GAME PHYSICS >>>>
 
/// Summary:	The world gravity.
static constexpr float					WORLD_GRAVITY[2]					{ 0.0f, 0.0f };

static constexpr size_t					PHYSICS_VELOCITY_ITERATIONS			{ 4 };

static constexpr size_t					PHYSICS_POSITION_ITERATIONS			{ 3 };


// <<<< GAME META SETTINGS >>>>

/// Summary:	The global for all game entities scale.
static constexpr float					GLOBAL_SCALE						{ 0.75f };

/// Summary:	The max. player amount.
static constexpr size_t					MAX_PLAYER							{ 8 };

/// Summary:	The max. amount of spawned bounty.
static constexpr size_t					MAX_BOUNTY							{ 12 };

/// Summary:	The default freeze time before game starts.
static constexpr float					DEFAULT_FREEZE_TIME					{ 3.0f }; // seconds

/// Summary:	The default play time.
static constexpr float					DEFAULT_PLAY_TIME					{ 90.0f }; // seconds

/// Summary:	The default collector respawntime.
static constexpr float					COLLECTOR_RESPAWNTIME				{ 3.0f }; // seconds

/// Summary:	The default bounty respawntime.
static constexpr float					BOUNTY_RESPAWNTIME					{ 2.0f }; // seconds

/// Summary:	The bounty life span.
static constexpr float					BOUNTY_MIN_LIFETIME					{ 4.0f }; // seconds
static constexpr float					BOUNTY_MAX_LIFETIME					{ 7.0f }; // seconds

/// Summary:	Collector max move speed.
static constexpr float					COLLECTOR_MAX_MOVE_SPEED			{ 25.0f }; // m/s

/// Summary:	Collector max turn speed.
static constexpr float					RADIANS								{ 0.0174533f };
static constexpr float					TURN								{ 360.0f * RADIANS }; // 360 = 1 turn
static constexpr float					COLLECTOR_MAX_TURN_SPEED			{ TURN };

/// Summary:	The minimum and maximum bounty value.
static constexpr float					BOUNTY_MIN_VALUE					{  5.0f };
static constexpr float					BOUNTY_MAX_VALUE					{ 30.0f };

/// Summary:	The minimum and maximum bounty scale.
static constexpr float					BOUNTY_MIN_SCALE					{ 1.0f };
static constexpr float					BOUNTY_MAX_SCALE					{ 3.0f };

/// Summary:	Bounty color RGBA
static constexpr float					BOUNTY_COLOR_R						{ 1.0f };
static constexpr float					BOUNTY_COLOR_G						{ 1.0f };
static constexpr float					BOUNTY_COLOR_B						{ 1.0f };
static constexpr float					BOUNTY_COLOR_A						{ 1.0f };

/// Summary:	The default player pocket size, that is, how many bounty he/she can carry at a time.
static constexpr float					PLAYER_POCKET_SIZE					{ 100.0f }; 

/// Summary:	Size of the player stash.
static constexpr float					PLAYER_STASH_SIZE					{ 250.0f }; 

/// Summary:	The line of sight of the ai collector bounty radar.
static constexpr float					AI_BOUNTY_RADAR_LOS					{ 120.0f * RADIANS };

/// Summary:	Distance the ai can see bounty.
static constexpr float					AI_VIEW_DISTANCE_BOUNTY				{ 25.0f }; // meter

/// Summary:	Distance the ai can see obstacles/collector.
static constexpr float					AI_VIEW_DISTANCE_OBSTACLE			{ AI_VIEW_DISTANCE_BOUNTY * 0.75f };

static constexpr float					PLAYER_COLOR[MAX_PLAYER][4]			{
																				{ 1.0f, 0.0f, 0.0f, 1.0f }, // RED
																				{ 0.0f, 1.0f, 0.0f, 1.0f }, // GREEN
																				{ 0.0f, 0.0f, 1.0f, 1.0f }, // BLUE
																				{ 1.0f, 1.0f, 0.0f, 1.0f }, // YELLOW
																				{ 1.0f, 0.0f, 1.0f, 1.0f }, // PINK
																				{ 0.0f, 1.0f, 1.0f, 1.0f }, // CYAN
																				{ 1.0f, 0.5f, 0.0f, 1.0f }, // ORANGE
																				{ 0.2f, 0.0f, 0.5f, 1.0f }  // PURPLE
																			};

// <<<< DEBUG SETTINGS >>>>

/// Summary:	Enable/Disable CheatSystem
static constexpr bool					ALLOW_CHEATS						{ true };

/// Summary:	True to enable the debug drawing.
static constexpr bool					DEBUG_DRAWING_ENABLED				{ true };


class Settings
{
	static constexpr const char* SETTINGS_FILE { "D:\\Projects\\2018\\Cpp\\BountyHunter-AI\\x64\\Debug\\Game.config" };

	union Variant
	{
		bool asBool;
		int8_t asInt8;
		int16_t asInt16;
		int32_t asInt32;
		int64_t asInt64;
		uint8_t asUint8;
		uint16_t asUint16;
		uint32_t asUint32;
		uint64_t asUint64;
		float asFloat;
	};

	using TSettingKey = std::string;
	using TSettingValue = Variant; //std::variant<bool, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float, double>; // since C++17
	using TSettings = std::unordered_map<TSettingKey, TSettingValue>;

private:

	TSettings m_Settings;

	Settings();

public:

	static Settings Get;

	const bool		asBool(const TSettingKey& name) { return m_Settings[name].asBool; }
	const int8_t	asInt8(const TSettingKey& name) { return m_Settings[name].asInt8; }
	const int16_t	asInt16(const TSettingKey& name) { return m_Settings[name].asInt16; }
	const int32_t	asInt32(const TSettingKey& name) { return m_Settings[name].asInt32; }
	const int64_t	asInt64(const TSettingKey& name) { return m_Settings[name].asInt64; }
	const uint8_t	asUint8(const TSettingKey& name) { return m_Settings[name].asUint8; }
	const uint16_t	asUint16(const TSettingKey& name) { return m_Settings[name].asUint16; }
	const uint32_t	asUint32(const TSettingKey& name) { return m_Settings[name].asUint32; }
	const uint64_t	asUint64(const TSettingKey& name) { return m_Settings[name].asUint64; }
	const float		asFloat(const TSettingKey& name) { return m_Settings[name].asFloat; }
};


#endif // __GAME_CONFIG_H__