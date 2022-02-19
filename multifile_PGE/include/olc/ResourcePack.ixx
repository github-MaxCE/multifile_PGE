export module olc.ResourcePack;

import std.core;
import std.filesystem;

export namespace olc
{
	// O------------------------------------------------------------------------------O
	// | olc::ResourcePack - A virtual scrambled filesystem to pack your assets into  |
	// O------------------------------------------------------------------------------O
	struct ResourceBuffer : public std::streambuf
	{
		ResourceBuffer(std::ifstream& ifs, uint32_t offset, uint32_t size)
		{
			vMemory.resize(size);
			ifs.seekg(offset); ifs.read(vMemory.data(), vMemory.size());
			setg(vMemory.data(), vMemory.data(), vMemory.data() + size);
		}

		std::vector<char> vMemory;
	};

	class ResourcePack : public std::streambuf
	{
	public:
		ResourcePack() { }

		~ResourcePack()
		{
			baseFile.close();
		}

		bool AddFile(const std::string& sFile)
		{
			const std::string file = makeposix(sFile);

			if (std::filesystem::exists(file))
			{
				sResourceFile e;
				e.nSize = (uint32_t)std::filesystem::file_size(file);
				e.nOffset = 0; // Unknown at this stage			
				mapFiles[file] = e;
				return true;
			}
			return false;
		}

		bool LoadPack(const std::string& sFile, const std::string& sKey)
		{
			// Open the resource file
			baseFile.open(sFile, std::ifstream::binary);
			if (!baseFile.is_open()) return false;

			// 1) Read Scrambled index
			uint32_t nIndexSize = 0;
			baseFile.read((char*)&nIndexSize, sizeof(uint32_t));

			std::vector<char> buffer(nIndexSize);
			for (uint32_t j = 0; j < nIndexSize; j++)
				buffer[j] = baseFile.get();

			std::vector<char> decoded = scramble(buffer, sKey);
			size_t pos = 0;
			auto read = [&decoded, &pos](char* dst, size_t size) {
				memcpy((void*)dst, (const void*)(decoded.data() + pos), size);
				pos += size;
			};

			auto get = [&read]() -> int { char c; read(&c, 1); return c; };

			// 2) Read Map
			uint32_t nMapEntries = 0;
			read((char*)&nMapEntries, sizeof(uint32_t));
			for (uint32_t i = 0; i < nMapEntries; i++)
			{
				uint32_t nFilePathSize = 0;
				read((char*)&nFilePathSize, sizeof(uint32_t));

				std::string sFileName(nFilePathSize, ' ');
				for (uint32_t j = 0; j < nFilePathSize; j++)
					sFileName[j] = get();

				sResourceFile e;
				read((char*)&e.nSize, sizeof(uint32_t));
				read((char*)&e.nOffset, sizeof(uint32_t));
				mapFiles[sFileName] = e;
			}

			// Don't close base file! we will provide a stream
			// pointer when the file is requested
			return true;
		}

		bool SavePack(const std::string& sFile, const std::string& sKey)
		{
			// Create/Overwrite the resource file
			std::ofstream ofs(sFile, std::ofstream::binary);
			if (!ofs.is_open()) return false;

			// Iterate through map
			uint32_t nIndexSize = 0; // Unknown for now
			ofs.write((char*)&nIndexSize, sizeof(uint32_t));
			uint32_t nMapSize = uint32_t(mapFiles.size());
			ofs.write((char*)&nMapSize, sizeof(uint32_t));
			for (auto& e : mapFiles)
			{
				// Write the path of the file
				size_t nPathSize = e.first.size();
				ofs.write((char*)&nPathSize, sizeof(uint32_t));
				ofs.write(e.first.c_str(), nPathSize);

				// Write the file entry properties
				ofs.write((char*)&e.second.nSize, sizeof(uint32_t));
				ofs.write((char*)&e.second.nOffset, sizeof(uint32_t));
			}

			// 2) Write the individual Data
			std::streampos offset = ofs.tellp();
			nIndexSize = (uint32_t)offset;
			for (auto& e : mapFiles)
			{
				// Store beginning of file offset within resource pack file
				e.second.nOffset = (uint32_t)offset;

				// Load the file to be added
				std::vector<uint8_t> vBuffer(e.second.nSize);
				std::ifstream i(e.first, std::ifstream::binary);
				i.read((char*)vBuffer.data(), e.second.nSize);
				i.close();

				// Write the loaded file into resource pack file
				ofs.write((char*)vBuffer.data(), e.second.nSize);
				offset += e.second.nSize;
			}

			// 3) Scramble Index
			std::vector<char> stream;
			auto write = [&stream](const char* data, size_t size) {
				size_t sizeNow = stream.size();
				stream.resize(sizeNow + size);
				memcpy(stream.data() + sizeNow, data, size);
			};

			// Iterate through map
			write((char*)&nMapSize, sizeof(uint32_t));
			for (auto& e : mapFiles)
			{
				// Write the path of the file
				size_t nPathSize = e.first.size();
				write((char*)&nPathSize, sizeof(uint32_t));
				write(e.first.c_str(), nPathSize);

				// Write the file entry properties
				write((char*)&e.second.nSize, sizeof(uint32_t));
				write((char*)&e.second.nOffset, sizeof(uint32_t));
			}
			std::vector<char> sIndexString = scramble(stream, sKey);
			uint32_t nIndexStringLen = uint32_t(sIndexString.size());
			// 4) Rewrite Map (it has been updated with offsets now)
			// at start of file
			ofs.seekp(0, std::ios::beg);
			ofs.write((char*)&nIndexStringLen, sizeof(uint32_t));
			ofs.write(sIndexString.data(), nIndexStringLen);
			ofs.close();
			return true;
		}

		ResourceBuffer GetFileBuffer(const std::string& sFile)
		{
			return ResourceBuffer(baseFile, mapFiles[sFile].nOffset, mapFiles[sFile].nSize);
		}

		bool Loaded()
		{
			return baseFile.is_open();
		}

	private:
		struct sResourceFile { uint32_t nSize; uint32_t nOffset; };
		std::map<std::string, sResourceFile> mapFiles;
		std::ifstream baseFile;

		std::vector<char> scramble(const std::vector<char>& data, const std::string& key)
		{
			if (key.empty()) return data;
			std::vector<char> o;
			size_t c = 0;
			for (auto s : data)	o.push_back(s ^ key[(c++) % key.size()]);
			return o;
		};

		std::string makeposix(const std::string& path)
		{
			std::string o;
			for (auto s : path) o += std::string(1, s == '\\' ? '/' : s);
			return o;
		}; 
	};
}