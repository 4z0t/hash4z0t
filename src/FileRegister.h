#include "internalhash4z0t.h"
#include <vector>
#include <list>

namespace FS = std::filesystem;

class FileRegister
{

public:

	using Path = FS::path;
	using FileEntry = FS::directory_entry;
	using Files = std::vector<FileEntry>;

	static Files GetFileList(const Path& path);

	static bool Exists(const Path& path);

	FileRegister();
	~FileRegister();

private:
};

FileRegister::Files FileRegister::GetFileList(const Path& path)
{
	if (!std::filesystem::exists(path))
		return Files();
	std::list<FileEntry> filesList;
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

FileRegister::FileRegister()
{
}

FileRegister::~FileRegister()
{
}