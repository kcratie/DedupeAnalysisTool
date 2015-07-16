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
#include "DataChunk.h"
//#include "ChunkDesc.h"
#include "FileDesc.h"
#include "ContentSharingGraph.h"

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

typedef DataChunk<HashType, Digest> DataChunkType;
typedef FileDesc<DataChunkType, void*> FileDescType;

class FileSystemMap {
public:
//	typedef std::unordered_multimap<HashType, DataChunkType, std::hash<HashType>> DataChunkMapType;
	typedef std::map<HashType, DataChunkType/*, std::hash<HashType>*/> DataChunkMapType;
	typedef DataChunkMapType::iterator DataChunkMapItrType;
	typedef pair<DataChunkMapItrType, DataChunkMapItrType> ChunkMapRangeType;

	typedef map<string, FileDescType> FileMapType;
	typedef FileMapType::iterator FileMapItrType;
	typedef pair<FileMapItrType, FileMapItrType> FileMapRangeType;

	typedef contentgraph::FileDescType VertexPropType;
	typedef ContentSharingGraph<VertexPropType, EdgeProperties> FMGraph;

	FileSystemMap(FMGraph & CntGraph);
	virtual ~FileSystemMap();

	size_t ChunkFile(
			FileDescType& FileObj
			);

	void CreateMap(
			string const& MountPoint
			);

	FileMapRangeType FileMapRange()
	{
		return make_pair(FileMap.begin(), FileMap.end());
	}

//	size_t BucketCount(){
//		return ChunkMap.bucket_count();
//	}

	ChunkMapRangeType ChunkMapRange(/*size_t Bucket*/)
	{
		return make_pair(ChunkMap.begin(), ChunkMap.end());
	}

	void Debug();

private:
	void BuildMap(
			fs::path const& FullPathName
			);

	void DisplayFileMap();
	size_t TotalSize();

		size_t FileCount;
		size_t DirCount;
		size_t OtherCount;
		size_t ErrCount;
		//std::hash<HashType> mHasher;
		DataChunkMapType ChunkMap;
		FileMapType FileMap;

		FMGraph mCntGraph;
};

} /* namespace contentgraph */

