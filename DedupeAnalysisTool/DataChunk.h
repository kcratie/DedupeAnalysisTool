#pragma once
#include <cstddef>
#include <string>
#include <algorithm>
#include <sstream>
#include <list>
#include "ChunkDesc.h"

using namespace std;
namespace contentgraph {

typedef unsigned char byte;

template <
	typename HashType,
	typename DigestType>
class DataChunk {
public:

	typedef std::pair<ChunkDesc, ChunkDesc> FistLastType;

	DataChunk() :
		mMatch(false)
	{}

	DataChunk(DataChunk const & RHS) :
		mMatch(RHS.mMatch),
		mDescList(RHS.mDescList),
		mHash(RHS.mHash)
	{}

	DataChunk(DataChunk&& RHS) :
		mMatch(RHS.mMatch)
	{
		mHash = std::move(RHS.mHash);
		mDescList = std::move(RHS.mDescList);
	}

	DataChunk& operator= (DataChunk const& RHS){
		if (this != &RHS){
			mMatch = RHS.mMatch;
			mHash = RHS.mHash;
			mDescList = RHS.mDescList;
		}
		return *this;
	}

	DataChunk& operator= (DataChunk&& RHS)
	{
		if (this != &RHS){
			mMatch(RHS.mMatch);
			mHash = std::move(RHS.mHash);
			mDescList = std::move(RHS.mDescList);
		}
		return *this;
	}

	bool operator== (DataChunk const & RHS){
		return mHash == RHS.mHash;
	}

	bool operator< (DataChunk const & RHS){
			return mHash < RHS.mHash;
	}

	bool Match(){
		return mMatch;
	}

	void Match(bool flag){
		mMatch = flag;
	}

	HashType& Hash() {
		return mHash;
	}

	size_t Length(){
		if (mDescList.begin() != mDescList.end())
			return mDescList.begin()->Length();
		return 0;
	}

	ChunkDesc& AddDescriptor(
		string const Filename,
		size_t Offset,
		size_t Length,
		void * FileDescObj)
	{
		mDescList.push_back(ChunkDesc(Filename, Offset, Length, FileDescObj));
		return mDescList.back();
//		auto p = std::make_pair(Filename, ChunkDesc(Filename, Offset, Length, FileDescObj));
//		auto i = mDescList.insert(p);
//		return i.first->second;
	}

	ChunkDesc& AddDescriptor(
		ChunkDesc& Desc)
	{
		auto i = mDescList.push_back(Desc);
		return mDescList.back();
	}

	void Hash(
		byte* buff,
		size_t cnt,
		DigestType& md)	{
		md.update(buff, cnt);
		md.finalize(mHash);
	}

	FistLastType ChunksFirstLast()
	{
		ChunkDesc & cd1 = *mDescList.begin(),
				& cd2 = *mDescList.rbegin();
		return std::make_pair(cd1, cd2);
	}

	const string PrintHashValue(){
		std::stringstream ss;
		int * begin = (int*)mHash.data();
		int * end = (int*)mHash.data()+(mHash.size()/sizeof(int));

		for(int* i=begin; i<end; i++){
			ss << std::hex << *i;
		}
		return ss.str();
	}

	void Debug(){
		cout << "Data Chunk: " << this << endl;
		cout << "Chunk Digest: " << PrintHashValue() << endl;
		for (auto i : mDescList){
			i.Debug();
		}
	}
private:
	bool mMatch;
	std::list<ChunkDesc> mDescList;
	HashType mHash;
};
}
