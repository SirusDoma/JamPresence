#ifndef PRESENCE_MAIN_HPP
#define PRESENCE_MAIN_HPP

#include <string>
#include <windows.h>

#include "JamState.hpp"

namespace Nx
{
    class API;
    class Game;
    class Presence
    {
    private:
        const unsigned long long DEFAULT_APP_ID = 615041815233691659;
        const std::string IMAGE_KEY             = "o2m_guitar_1";

        const DWORD GAME_MUSIC_ID_ADDRESS     = 0x001D84E8;
        const DWORD GAME_INFO_ADDRESS         = 0x001D84F4;
        const DWORD GAME_SLOT_AVAILABLE       = 0x001D8500;
        const DWORD GAME_SCENE_INDEX_ADDRESS  = 0x001D84F8;
        const DWORD GAME_PLAYING_FLAG_ADDRESS = 0x001D8504;

        std::string winClass;
        std::string moduleName;
        JamState lastState;

        Game *process;
        API  *api;

    public:
        Presence();
        Presence(const unsigned long long &appId);
        Presence(const std::string &winClass, const std::string &moduleName);
        Presence(const std::string &winClass, const std::string &moduleName, const unsigned long long &appId);
        ~Presence();

        JamState GetPlayerState();

        void Update(const bool &force = false);
        void Clear();
    };
}

#endif