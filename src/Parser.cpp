#include "Parser.hpp"

#include <iostream>
#include <fstream>

namespace Nx
{
	ChartMeta Parser::Parse(std::string path, unsigned int difficulty)
	{
		ChartMeta meta = {};
		std::ifstream stream(path.c_str(), std::ifstream::binary);

		if (!stream)
		{
			return meta;
		}

		// Set difficulty
		meta.Difficulty = difficulty;

		// Read music id
		stream.read((char*)& meta.MusicId, sizeof(int));
		
		// Read music title
		char buffer[64];
		stream.seekg(108, stream.beg);
		stream.read(buffer, 64);
		meta.Title = std::string(buffer);

		// Determine level and duration offset
		unsigned int levelOffset = 0;
		unsigned int durationOffset = 0;
		switch (difficulty)
		{
			case 0:
				levelOffset    = 20;
				durationOffset = 284;

				break;
			case 1:
				levelOffset    = 22;
				durationOffset = 288;

				break;
			default:
				levelOffset    = 24;
				durationOffset = 302;

				break;
		}

		// Read level
		stream.seekg(levelOffset);
		stream.read((char*)&meta.Level, sizeof(short));

		// Read duration
		stream.seekg(durationOffset);
		stream.read((char*)&meta.Duration, sizeof(int));

		return meta;
	}
}