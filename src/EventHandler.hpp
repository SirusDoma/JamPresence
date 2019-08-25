#ifndef PRESENCE_HANDLER_HPP
#define PRESENCE_HANDLER_HPP

#include <iostream>
#include <fstream>
#include <discord.h>

namespace Nx
{
    namespace Handlers
    {
        void Log(discord::LogLevel level,  const char *buffer) 
        {
            std::ofstream stream;
            stream.open("D:\\Discord.log");
            stream << buffer << std::endl;
            stream.close();

            std::printf("PRESENCE: %s\n", buffer);
        }

        void Callback(discord::Result result)
        {
            if (result != discord::Result::Ok)
            {
                std::fprintf(stderr, "PRESENCE: ErrCode: %d\n", result);
            }
        }
    }
}

#endif