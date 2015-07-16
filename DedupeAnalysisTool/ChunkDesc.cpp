#include "ChunkDesc.h"
#include <iostream>

using namespace std;

namespace contentgraph {

ChunkDesc::ChunkDesc():
		mOffset(0),
		mLength(0),
		mFileDescObj(NULL)
	{}

ChunkDesc::ChunkDesc(
		string const _Filename,
		size_t _Offset,
		size_t _Length,
		void * FileDescObj):
			mFilename (_Filename),
			mOffset(_Offset),
			mLength(_Length),
			mFileDescObj(FileDescObj)
	{}

ChunkDesc::ChunkDesc(ChunkDesc const& RHS):
		mFilename(RHS.mFilename),
		mOffset(RHS.mOffset),
		mLength(RHS.mLength),
		mFileDescObj(RHS.mFileDescObj)
	{}

	ChunkDesc& ChunkDesc::operator= (ChunkDesc const& RHS){
		if (this != &RHS){
			mFilename = RHS.mFilename;
			mOffset = RHS.mOffset;
			mLength = RHS.mLength;
			mFileDescObj = RHS.mFileDescObj;
		}
		return *this;
	}

	bool ChunkDesc::operator== (ChunkDesc const & RHS){
		return mLength == RHS.mLength &&
				mOffset == RHS.mOffset &&
				mFilename.compare(RHS.mFilename) == 0;
	}

	bool ChunkDesc::operator!= (ChunkDesc const & RHS){
		return ! ((*this) == RHS);
	}

	size_t ChunkDesc::Offset(){
			return mOffset;
		}

	void ChunkDesc::Offset(size_t Offset){
			mOffset = Offset;
		}

	void ChunkDesc::Filename(string const Filename){
		mFilename = Filename;
	}

	string const & ChunkDesc::Filename(){
		return mFilename;
	}

	void ChunkDesc::Length(size_t Length){
		mLength = Length;
	}

	size_t ChunkDesc::Length(){
		return mLength;
	}


	void * ChunkDesc::FileDescObj(){
		return mFileDescObj;
	}

	void ChunkDesc::FileDescObj(void *FileDescObj){
		mFileDescObj = FileDescObj;
	}

	void ChunkDesc::Debug(){
		cout << "Chunk Desc Properties: " << this << endl;
		cout << "\tName: " << Filename() << endl;
		cout << "\tLength: " << Length() << endl;
		cout << "\tOffset: " << Offset() << endl;
		cout << "\tFile Desc Object: " << FileDescObj() << endl;
	}
}
