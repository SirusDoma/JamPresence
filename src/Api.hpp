#ifndef PRESENCE_API_HPP
#define PRESENCE_API_HPP

#include "discord.h"

namespace Nx
{
    class API
    {
    public:
        discord::Core *client;

        API()  { client = {}; }
        ~API() { delete client; }
    };
}

#endif