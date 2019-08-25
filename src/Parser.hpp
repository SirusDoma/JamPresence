#ifndef PRESENCE_META_PARSER_HPP
#define PRESENCE_META_PARSER_HPP

#include <string>
#include "ChartMeta.hpp"

namespace Nx
{
	class Parser
	{
	public:
		static ChartMeta Parse(std::string path, unsigned int difficulty);
	};
}

#endif