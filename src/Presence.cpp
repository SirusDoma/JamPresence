#include "Presence.hpp"

#include <ctime>

#include "Api.hpp"
#include "Game.hpp"
#include "Parser.hpp";
#include "EventHandler.hpp"

namespace Nx
{
	Presence::Presence()
		: Presence::Presence("O2-JAM", "OTwo.exe", 0)
	{
	}

	Presence::Presence(const unsigned long long &appId)
		: Presence::Presence("O2-JAM", "OTwo.exe", appId)
	{
	}

	Presence::Presence(const std::string& winClass, const std::string& moduleName)
		: Presence::Presence(winClass, moduleName, 0)
	{
	}

	Presence::Presence(const std::string &winClass, const std::string &moduleName, const unsigned long long &appId) : 
		winClass(winClass),
		moduleName(moduleName),
		process(0),
		api(0)
	{
		// Initialize wrapper API
		api = new API();

		// Determine which Client ID to use
		unsigned long long id = appId;
		if (!id)
		{
			id = Presence::DEFAULT_APP_ID;
		}

		// Initialize Discord Client
		auto result = discord::Core::Create(id, DiscordCreateFlags_Default, &api->client);
		if (result != discord::Result::Ok)
		{
			std::fprintf(stderr, "PRESENCE: Failed to initialize Discord API Client (Code: %d)\n", result);
		}

		// Initialize Hooks
		api->client->SetLogHook(discord::LogLevel::Debug, Nx::Handlers::Log);
	}

	Presence::~Presence()
	{
		Clear();
		delete api->client;
	}

	JamState Presence::GetPlayerState()
	{
		// Define jamming state
		JamState state = {};

		// Initialize hook process
		if (!process || !process->GetProcessHandle())
		{
			process = new Game(winClass, moduleName);
		}

		// Get play info
		int musicId       = process->ReadMemory(Presence::GAME_MUSIC_ID_ADDRESS);
		int gameinfo      = process->ReadMemory(Presence::GAME_INFO_ADDRESS);
		int occupied      = process->ReadMemory(Presence::GAME_SLOT_AVAILABLE);
		state.SceneIndex  = process->ReadMemory(Presence::GAME_SCENE_INDEX_ADDRESS);
		state.Playing     = process->ReadMemory(Presence::GAME_PLAYING_FLAG_ADDRESS);

		// Get the number of player
		state.PlayerCount = gameinfo & 0xFF;

		// Calculate slot count
		occupied = (occupied - state.PlayerCount);
		state.RoomSize = occupied < 0 ? 8 + occupied : 8;

		// Parse chart metadata for more play detail
		if (state.Playing)
		{
			int difficulty = (gameinfo >> 8) & 0xFF;
			std::string path = "Music/o2ma" + std::to_string(musicId) + ".ojn";
			state.Chart = Parser::Parse(path, difficulty);
		}
		else
		{
			state.Chart = ChartMeta{};
		}

		return state;
	}

	void Presence::Update(const bool &force)
	{
		// Parse activity details
		auto state = GetPlayerState();
		static bool first = true;

		if (!force && !first && lastState.Playing == state.Playing && 
			lastState.RoomSize == state.RoomSize && 
			lastState.SceneIndex == state.SceneIndex)
		{
			api->client->RunCallbacks();
			return;
		}

		first = false;
		std::string details = "Idle";
		std::string status = "Let's play together~";

		if (state.Playing)
		{
			std::string xmod = "[HX] ";
			switch (state.Chart.Difficulty)
			{
				case 0: xmod = "[EX] "; break;
				case 1: xmod = "[NX] "; break;
				default: break;
			}

			status = state.PlayerCount > 1 ? "Playing with others " + std::to_string(state.PlayerCount - 1) + " Jammers" : "Solo Jamming";
			details = (xmod + state.Chart.Title).c_str();
		}
		else
		{
			if (state.SceneIndex == 1)
			{
				details = "Waiting Room";
				if (state.RoomSize == 1)
				{
					status = "Solo Jamming";
				}
				else
				{
					details += " (" + std::to_string(state.PlayerCount) + " of " + std::to_string(state.RoomSize) + ")";
				}
			}
			else
			{
				details = "Room Selection";
			}
		}

		// Construct player activity
		discord::Activity activity = {};
		activity.SetState(status.c_str());
		activity.SetDetails(details.c_str());
		
		// Set activity timestamp
		if (state.Playing)
		{
			auto timestamp = &activity.GetTimestamps();
			timestamp->SetStart(std::time(0));
		}

		// Set activity assets
		auto asset = &activity.GetAssets();
		asset->SetLargeImage(Presence::IMAGE_KEY.c_str());
		asset->SetLargeText("Let's play together!");

		// Feel my presence!
		api->client->ActivityManager().UpdateActivity(activity, Nx::Handlers::Callback);

		// Run pending callback
		api->client->RunCallbacks();

		// Update state
		lastState = state;
	}

	void Presence::Clear()
	{
		// Clear presence
		api->client->ActivityManager().ClearActivity(Nx::Handlers::Callback);
	}
}