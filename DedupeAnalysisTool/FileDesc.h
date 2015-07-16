#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <list>
#include <algorithm>

using namespace std;

namespace contentgraph {
//#define __DEBUG__FileDesc
template <
	typename DataChunk,
	typename VertexDescType>
class FileDesc {
public:
	typedef typename list<DataChunk>::iterator IteratorType;
	typedef pair<IteratorType, IteratorType> RangeType;

	FileDesc():
		mSize(0),
		mSharedBytesTotal(0),
		mVertexDesc(NULL)
	{}

	FileDesc(string Name):
			mName(Name),
			mSize(0),
			mSharedBytesTotal(0),
			mVertexDesc(NULL)
	{}

	FileDesc(FileDesc const & RHS):
			mName(RHS.mName),
			mSize(RHS.mSize),
			mSharedBytesTotal(RHS.mSharedBytesTotal),
			mPieces(RHS.mPieces),
			mVertexDesc(RHS.mVertexDesc)
	{}

	FileDesc(FileDesc&& RHS):
			mSize(RHS.mSize),
			mSharedBytesTotal(RHS.mSharedBytesTotal),
			mVertexDesc(RHS.mVertexDesc)

	{
		mName = std::move(RHS.mName);
		mPieces = std::move(RHS.mPieces);
	}

	FileDesc& operator=(FileDesc const & RHS)
	{
		if(this != &RHS){
			mName = RHS.mName;
			mPieces = RHS.mPieces;
			mSize = RHS.mSize;
			mSharedBytesTotal = RHS.mSharedBytesTotal;
		}
		return *this;
	}

	FileDesc& operator=(FileDesc&& RHS)
	{
		if(this != &RHS){
			mName = std::move(RHS.mName);
			mPieces = std::move(RHS.mPieces);
			mSize = RHS.mSize;
			mSharedBytesTotal = RHS.mSharedBytesTotal;
		}

		return *this;
	}

	FileDesc& operator+=(size_t SharedBytes){
		mSharedBytesTotal += SharedBytes;
		return *this;
	}

	void Name(string Filename) {mName=Filename;}
	string Name() {return mName;}
	size_t Size() {return mSize;}
	size_t NumChunks() {return mPieces.size();}
	size_t ChunkSize() {return tuple_size<DataChunk>::value;}

	size_t SharedBytesTotal(){
		return mSharedBytesTotal;
	}

	void SharedBytesTotal(size_t SharedBytes){
		mSharedBytesTotal = SharedBytes;
	}

	void AddChunk(DataChunk& Chunk)
	{
		mPieces.push_back(Chunk);
		mSize += Chunk.Length();
	}

	void VertexDesc(VertexDescType VertDesc){
		mVertexDesc = VertDesc;
	}

	VertexDescType VertexDesc(){
		return mVertexDesc;
	}

	RangeType ChunkRange()
	{
		return make_pair(mPieces.begin(), mPieces.end());
	}

	void Debug(){
		cout << "File Desc Properties: " << this << endl;
		cout << "\tName: " << Name() << endl;
		cout << "\tSize: " << Size() << endl;
		cout << "\tNumber of pieces: " << NumChunks() << endl;
	}
private:
	string mName;
	size_t mSize;
	size_t mSharedBytesTotal;
	list<DataChunk>mPieces;
	VertexDescType mVertexDesc;
};
#undef __DEBUG__FileDesc
} /* namespace contentgraph */
