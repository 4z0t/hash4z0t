#include "internalhash4z0t.h"
namespace H4z0t
{
	
	Files GetFileList(const Path& path)
	{
		if (!std::filesystem::exists(path))
			return Files();
		std::list<DirEntry> filesList;
		if (FS::is_regular_file(path))
		{
			filesList.push_back(FS::directory_entry(path));
		}
		else if (FS::is_directory(path))
		{
			for (const auto& entry : FS::recursive_directory_iterator(path))
			{
				if (entry.is_regular_file())
					filesList.push_back(entry);
			}
		}
		return Files(filesList.begin(), filesList.end());
	}

}