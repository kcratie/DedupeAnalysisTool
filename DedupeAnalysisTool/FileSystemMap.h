#pragma once

#include <cstddef>
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <functional>
#include <map>

#include "Crypto.h"
#include "ChunkDesc.h"
#include "FileDesc.h"

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
namespace fs = boost::filesystem;


typedef contentgraph::Digest::value_type HashType;

namespace std
{
template<>
struct hash<HashType> {
	size_t operator()(HashType const & h) const {
		_Hash_impl hi;
		return hi.hash(h.data(), h.size());
	}
};

}

using namespace std;
namespace contentgraph {

typedef ChunkDesc<HashType, Digest> ChunkDescType;
typedef FileDesc<ChunkDescType> FileDescType;

class FileSystemMap {
public:
	typedef std::unordered_multimap<HashType, ChunkDescType, std::hash<HashType>> ChunkMapType;

	typedef map<string, FileDescType> FileMapType;
	typedef FileMapType::iterator FileMapItrType;
	typedef pair<FileMapItrType, FileMapItrType> FileMapRangeType;

	FileSystemMap();
	virtual ~FileSystemMap();

	size_t ChunkFile(
			FileDescType& FileObj
			);

	void CreateMap(
			string const& MountPoint
			);

	size_t TotalSize();

	FileMapRangeType FileMapRange()
	{
		return make_pair(FileMap.begin(), FileMap.end());
	}
	void Debug();

private:
	void BuildMap(
			fs::path const& FullPathName
			);

		size_t FileCount;
		size_t DirCount;
		size_t OtherCount;
		size_t ErrCount;
		std::hash<HashType> mHasher;
		ChunkMapType ChunkMap;
		FileMapType FileMap;
};

} /* namespace contentgraph */

