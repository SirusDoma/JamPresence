# Jam Presence #

<p align="center">
  <img src="https://i.imgur.com/v0IUsFd.png">
</p>

Let the others know you're jamming right now!  

JamPresence is a SDK to broadcast your O2Jamming activities into [Discord](https://discordapp.com/).  
This can be useful for your custom game launcher of your private server and can be combined with offline server / emulator as long they use unpacked OTwo v1.8 client.

This project is my debut in Reverse Engineering, some basic RE techniques was involved during collecting necessary addresses to make this work done.

## Usage ##

1. Build the project
2. include the `include` folder and add `Presence.hpp` into your code and link necessary file into your project.
3. Instantiate `Nx::Presence` class and keep calling `Update()` while the game is running

Example Code:
```
#include <Presence.hpp>

int main()
{
    // Initialize presence client
    auto presence = Nx::Presence();

    // Best to do this in separate thread instead
    while (true)
    {
        // Update the presence to update game state from client
        presence.Update();

        // Additionally you can query game state from presence object
        auto state = presence.GetPlayerState();
	
	// Do something with state..
	// std::printf(state.Chart.Title.c_str()); // e.g Bach Alive
	
	// Let's not take the entire cpu
        Sleep(100);
    }

    // Clear the presence of jamming
    // Note that the presence will be cleared as well if the instance is destroyed
    presence.Clear();
    
    return 0;
}
```

## Notes ##
By default, engine will make an attempt to look for window handle (`hwnd`) with `O2-JAM` winclass, which is default value of O2Jam title bar. Once the window handle acquired, it will look for `OTwo.exe` module in the corresponding process of window handle.  

Therefore, the engine will failed to start if these parameters was altered in original game client. You can specify this `winClass` and `moduleName` upon initializing `Nx::Presence` class. 

Game client that packed and / or compressed by compressor such as upx will not work against this presence engine.

## License ##

This is an open-sourced library licensed under the [MIT License](http://github.com/SirusDoma/JamPresence/blob/master/LICENSE)
