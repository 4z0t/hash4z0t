#pragma once
#include "internalhash4z0t.h"
#include "File.h"
#include "FileData.h"

namespace H4z0t {





	class Decoder
	{
	public:



		Decoder();
		Decoder(String);
		Decoder(Path);
		Decoder(DirEntry);
		Decoder(const Path& path, const Arguments& args);


		bool VerifyFile()
		{
			return VerifyFormat() && VerifyVersion();
		}

		void Start(const Path& savePath);



		~Decoder();
	protected:

		u32 FilesCount()
		{
			return _inputFile.Read<u32>();
		}
		bool VerifyFormat();
		bool VerifyVersion();
		void _OpenFile(Path);


		Path _filePath;
		File _inputFile;
		CompressionType compression = CompressionType::None;
		ProtectionType protection = ProtectionType::None;
		EncryptionType encryption = EncryptionType::None;

	};
}