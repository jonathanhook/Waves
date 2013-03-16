/**
 * Class:	FileAccess
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef FileAccess_H
#define FileAccess_H

#include <vector>

namespace JDHUtility
{
	class FileAccess
	{
	public:
		static std::vector<std::string> getFiles(std::string path);

	private:
		static std::vector<std::string> WIN32_getFiles(std::string path);
	};
}
#endif

