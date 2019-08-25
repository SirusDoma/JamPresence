#ifndef PRESENCE_STATE_HPP
#define PRESENCE_STATE_HPP

#include "ChartMeta.hpp"

namespace Nx
{
	struct JamState
	{
		bool Playing;
		ChartMeta Chart;
		unsigned int PlayerCount;	
		unsigned int SceneIndex;
		unsigned int RoomSize;
	};
}

#endif