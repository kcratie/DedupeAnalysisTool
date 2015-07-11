#pragma once
#include <cstddef>
#include <string>
#include <algorithm>
#include <sstream>

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
		mMatch(false),
		mFileDescObj(NULL)
	{}

	ChunkDesc(
		string const _Filename,
		size_t _Offset,
		size_t _Length,
		void * FileDescObj):
			mFilename (_Filename),
			mOffset(_Offset),
			mLength(_Length),
			mMatch(false),
			mFileDescObj(FileDescObj)
	{}

	ChunkDesc(ChunkDesc const& RHS):
		mHash(RHS.mHash), mFilename(RHS.mFilename),
		mOffset(RHS.mOffset), mLength(RHS.mLength),
		mMatch(RHS.mMatch), mFileDescObj(RHS.mFileDescObj)
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
			mFileDescObj = RHS.mFileDescObj;
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

	void Hash(
		byte* buff,
		size_t cnt,
		DigestType& md)	{
		md.update(buff, cnt);
		md.finalize(mHash);
	}

	void * FileDescObj(){
		return mFileDescObj;
	}

	void FileDescObj(void *FileDescObj){
		mFileDescObj = FileDescObj;
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
		cout << "Chunk Desc Properties: " << this << endl;
		cout << "\tName: " << Filename() << endl;
		cout << "\tLength: " << Length() << endl;
		cout << "\tOffset: " << Offset() << endl;
		cout << "\tFile Desc Object: " << FileDescObj() << endl;
		cout << "HASH: " << PrintHashValue() << endl;
	}
private:
	HashType mHash;
	string mFilename;
	size_t mOffset;
	size_t mLength;
	bool mMatch;
	void * mFileDescObj;

};

}
