#pragma once
#include <cstddef>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;
namespace contentgraph {

typedef unsigned char byte;

class ChunkDesc {
public:

	ChunkDesc();

	ChunkDesc(
		string const _Filename,
		size_t _Offset,
		size_t _Length,
		void * FileDescObj);

	ChunkDesc(ChunkDesc const& RHS);

	ChunkDesc& operator= (ChunkDesc const& RHS);

	bool operator== (ChunkDesc const & RHS);

	bool operator!= (ChunkDesc const & RHS);

	size_t Offset();

	void Offset(size_t Offset);

	void Filename(string const Filename);

	string const & Filename();

	void Length(size_t Length);

	size_t Length();

	void * FileDescObj();

	void FileDescObj(void *FileDescObj);

	void Debug();

private:
	string mFilename;
	size_t mOffset;
	size_t mLength;
	void * mFileDescObj;

};

}
