#pragma once
#include <cstddef>
#include <string>
#include <algorithm>
//#include "Crypto.h"

using namespace std;
namespace contentgraph {

typedef unsigned char byte;

template <
	typename HashType,
	typename DigestType>
class ChunkDesc {
public:
	//const size_t shaSize = tuple_size<HashType>::value;

	ChunkDesc():
		mOffset(0),
		mLength(0),
		mMatch(false)
	{}

	ChunkDesc(
		string const _Filename,
		size_t _Offset,
		size_t _Length):
			mFilename (_Filename),
			mOffset(_Offset),
			mLength(_Length),
			mMatch(false)
	{}

	ChunkDesc(ChunkDesc const& RHS):
		mHash(RHS.mHash), mFilename(RHS.mFilename),
		mOffset(RHS.mOffset), mLength(RHS.mLength),
		mMatch(RHS.mMatch)
	{}
	//virtual ~ChunkDesc(){}
//	ChunkDesc& operator= (ChunkDesc&& RHS){
//		if (this != &RHS){
//			mHash = RHS.mHash;
//			mFilename = std::move(RHS.mFilename);
//			mOffset = RHS.mOffset;
//			mLength = RHS.mLength;
//			mMatch = RHS.mMatch;
//		}
//		return *this;
//	}
	ChunkDesc& operator= (ChunkDesc const& RHS){
		if (this != &RHS){
			mHash = RHS.mHash;
			mFilename = RHS.mFilename;
			mOffset = RHS.mOffset;
			mLength = RHS.mLength;
			mMatch = RHS.mMatch;
		}
		return *this;
	}

	bool operator== (ChunkDesc const & RHS){
		return mHash == RHS.mHash;
	}

	size_t Offset(){
			return mOffset;
		}

	void Offset(size_t Offset){
			mOffset = Offset;
		}

	void Filename(string const Filename){
		mFilename = Filename;
	}

	string const & Filename() {
		return mFilename;
	}

	void Length(size_t Length) {
		mLength = Length;
	}
	size_t Length(){
		return mLength;
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

	void
	Hash(
		byte* buff,
		size_t cnt,
		DigestType& md)
	{

		//Digest::value sha;      // the hash value
		md.update(buff, cnt);
		md.finalize(mHash);

	}
private:

	HashType mHash;
	string mFilename;
	size_t mOffset;
	size_t mLength;
	bool mMatch;

};

}
