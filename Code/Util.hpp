#ifndef __Util_HPP__
#define __Util_HPP__

#include <filesystem>
#include <string>

inline auto CheckFileExists(const std::string& szFileName) -> bool
{
	if (szFileName.empty())
	{
		return false;
	}
	return std::filesystem::exists(szFileName.c_str());
}


#endif
