#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <list>
#include <algorithm>

using namespace std;

namespace contentgraph {
#define __DEBUG__FileDesc
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
	FileDesc(FileDesc const & RHS):
			mName(RHS.mName),
			mSize(RHS.mSize),
			mPieces(RHS.mPieces)
	{}
	FileDesc(FileDesc&& RHS):
			mSize(RHS.mSize)

	{
#ifdef __DEBUG__FileDesc
		cout<<"FileDesc rvalue copy constructor"<<endl;
#endif
		mName = std::move(RHS.mName);
		mPieces = std::move(RHS.mPieces);
	}
//	virtual ~FileDesc(){} accept implicit dtor

	FileDesc& operator=(FileDesc const & RHS)
	{
		if(this != &RHS){
			mName = std::move(RHS.mName);
			mPieces = std::move(RHS.mPieces);
			mSize = (RHS.mSize);
		}

		return *this;
	}

	FileDesc& operator=(FileDesc&& RHS)
	{
#ifdef __DEBUG__FileDesc
		cout<<"FileDesc rvalue assignemnt operator"<<endl;
#endif
		if(this != &RHS){
			mName = std::move(RHS.mName);
			mPieces = std::move(RHS.mPieces);
			mSize = (RHS.mSize);
		}

		return *this;
	}

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
	void Debug(){
		cout << "File Desc Properties: " << this << endl;
		cout << "\tName: " << Name() << endl;
		cout << "\tSize: " << Size() << endl;
		cout << "\tNumber of pieces: " << NumChunks() << endl;
	}
private:
	string mName;
	size_t mSize;
	list<ChunkDesc>mPieces;
};
#undef __DEBUG__FileDesc
} /* namespace contentgraph */
