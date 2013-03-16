/**
 * Class:	FileAccess
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/FileAccess.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace JDHUtility
{
	/* PUBLIC STATIC FUNCTIONS */
	std::vector<std::string> FileAccess::getFiles(std::string path)
	{
#ifdef _WIN32
		return WIN32_getFiles(path);
#endif
	}

	/* PRIVATE STATIC FUNCTIONS */
	std::vector<std::string> FileAccess::WIN32_getFiles(std::string path)
	{
		std::vector<std::string> s;

		HANDLE h;
		WIN32_FIND_DATA data;
		h = FindFirstFile(path.c_str(), &data);
		
		do
		{
			if(h !=INVALID_HANDLE_VALUE)
			{ 
				std::string filename = data.cFileName;
				s.push_back(filename);
			}
			else 
			{
				break;
			}
		}
		while(FindNextFile(h, &data));
		
		FindClose(h);
		return s;
	}

}