///-------------------------------------------------------------------------------------------------
/// File:	GameConfiguration.cpp.
///
/// Summary:	Implements the game configuration class.
///-------------------------------------------------------------------------------------------------

#include "GameConfiguration.h"

#include <fstream>

Settings Settings::Get;

Settings::Settings()
{
	// read setting from file, if available;
	std::ifstream fs;
	fs.open(Settings::SETTINGS_FILE, std::ios::in);
	if (fs.good())
	{	
		std::string line;
		while (std::getline(fs, line))
		{
			size_t delimiterPos = line.find("=");
			
			// if this line does not hold a key-value pair, skip line
			if (delimiterPos == std::string::npos)
				continue;

			std::string key = line.substr(0, delimiterPos);
			std::string value = line.substr(delimiterPos + 1);
			
			// check if there is a decimal point -> parse it as double value
			if (value.find(".") != std::string::npos)
			{
				this->m_Settings[key].asFloat = std::atof(value.c_str());
			}
			else if ((key.compare("true") == 0) || (key.compare("True") == 0) || (key.compare("TRUE") == 0))
			{
				this->m_Settings[key].asBool = true;
			}
			else if ((key.compare("false") == 0) || (key.compare("False") == 0) || (key.compare("FALSE") == 0))
			{
				this->m_Settings[key].asBool = false;
			}
			else
			{
				this->m_Settings[key].asInt64 = std::atol(value.c_str());
			}
		}

		// close file stream
		fs.close();
	}	

	// fill missing settings with defaults
	{
		//CHECK_DEFAULT_INT(GAME_WINDOW_WIDTH);
		//CHECK_DEFAULT_INT(GAME_WINDOW_HEIGHT);
		CHECK_DEFAULT_FLOAT(GLOBAL_SCALE);
		CHECK_DEFAULT_INT(MAX_PLAYER);
		CHECK_DEFAULT_INT(MAX_BOUNTY);
		CHECK_DEFAULT_FLOAT(DEFAULT_FREEZE_TIME);
		CHECK_DEFAULT_FLOAT(DEFAULT_PLAY_TIME);
		CHECK_DEFAULT_FLOAT(COLLECTOR_RESPAWNTIME);
		CHECK_DEFAULT_FLOAT(COLLECTOR_MAX_MOVE_SPEED);
		CHECK_DEFAULT_FLOAT(BOUNTY_RESPAWNTIME);
		CHECK_DEFAULT_FLOAT(BOUNTY_MIN_LIFETIME);
		CHECK_DEFAULT_FLOAT(BOUNTY_MAX_LIFETIME);
		CHECK_DEFAULT_FLOAT(BOUNTY_MIN_VALUE);
		CHECK_DEFAULT_FLOAT(BOUNTY_MAX_VALUE);
		CHECK_DEFAULT_FLOAT(BOUNTY_MIN_SCALE);
		CHECK_DEFAULT_FLOAT(BOUNTY_MAX_SCALE);
		CHECK_DEFAULT_FLOAT(BOUNTY_COLOR_R);
		CHECK_DEFAULT_FLOAT(BOUNTY_COLOR_G);
		CHECK_DEFAULT_FLOAT(BOUNTY_COLOR_B);
		CHECK_DEFAULT_FLOAT(BOUNTY_COLOR_A);
		CHECK_DEFAULT_FLOAT(PLAYER_POCKET_SIZE);
		CHECK_DEFAULT_FLOAT(PLAYER_STASH_SIZE);
		CHECK_DEFAULT_FLOAT(REWARD_SCALE_PLAYER_DIED);
		CHECK_DEFAULT_FLOAT(REWARD_SCALE_BOUNTY_COLLECTED);
		CHECK_DEFAULT_FLOAT(REWARD_SCALE_BOUNTY_STASHED);
	}
}