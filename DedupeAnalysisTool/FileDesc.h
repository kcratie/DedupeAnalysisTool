#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <list>
#include <algorithm>

using namespace std;

namespace contentgraph {

template <typename ChunkDesc>
class FileDesc {
public:
	typedef typename list<ChunkDesc>::iterator IteratorType;
	typedef pair<IteratorType, IteratorType> RangeType;

	FileDesc():
		mSize(0)
	{}
	FileDesc(string Name):
			mName(Name),
			mSize(0)
	{}
//	virtual ~FileDesc(){}
	void Name(string Filename) {mName=Filename;}
	string Name() {return mName;}
	size_t Size() {return mSize;}
	size_t NumChunks() {return mPieces.size();}
	size_t ChunkSize() {return tuple_size<ChunkDesc>::value;}
	void AddChunk(ChunkDesc& Chunk)
	{
		mPieces.push_back(Chunk);
		mSize += Chunk.Length();
	}

	RangeType ChunkRange()
	{
		return make_pair(mPieces.begin(), mPieces.end());
	}
private:
	string mName;
	size_t mSize;
	list<ChunkDesc>mPieces;
};

} /* namespace contentgraph */
