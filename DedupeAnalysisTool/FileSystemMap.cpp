/*
 * FileSystemMap.cpp
 *
 *  Created on: Jul 6, 2015
 *      Author: kcratie
 */
#include <cstddef>
#include <iostream>
#include <fstream>


#include "FileSystemMap.h"
//#include "Crypto.h"

using namespace std;
using namespace boost;

namespace contentgraph {
#define __DEBUG__FSM

FileSystemMap::FileSystemMap() :
	ChunkMap(ChunkMapType(1 << 16)) {
	FileCount = 0;
	DirCount = 0;
	OtherCount = 0;
	ErrCount = 0;
}

FileSystemMap::~FileSystemMap() {
	ChunkMap.clear();
}

size_t	//file size
FileSystemMap::ChunkFile(
		FileDescType& FileObj)
{
	ifstream ifl;
	ifl.open(FileObj.Name().c_str(), ifstream::in | ifstream::binary);
	if (!ifl.good()) {
		BOOST_THROW_EXCEPTION(std::exception());
	}

	Digest md;              // the hash object
	//Digest::value_type sha;      // the hash value
	size_t const length = 1 << 14;
	std::array<byte, length> buf;

	size_t chunkOffset = 0;
	while (!ifl.eof()) {
		ifl.read((char*) buf.data(), length);
		size_t cnt = ifl.gcount();
		ChunkDescType chunk(FileObj.Name(), chunkOffset, cnt);
		chunk.Hash(buf.data(), cnt, md);
		ChunkMap.insert(std::make_pair(chunk.Hash(), chunk));
		FileObj.AddChunk(chunk);
		chunkOffset += cnt;
	}

	ifl.close();
	return chunkOffset;
}

void
FileSystemMap::BuildMap(
		fs::path const& FullPathName)
{
	fs::directory_iterator end_iter;
	for (fs::directory_iterator dir_itr(FullPathName); dir_itr != end_iter;
			++dir_itr) {
		try {
			if (fs::is_directory(dir_itr->status())) {
				++DirCount;
#ifdef __DEBUG__FSM
				std::cout << "entering directory: "
						<< dir_itr->path().filename() << endl;
#endif
				BuildMap(dir_itr->path());
			} else if (fs::is_regular_file(dir_itr->status())) {
				++FileCount;
				string s(dir_itr->path().c_str());
#ifdef __DEBUG__FSM
				std::cout << "\tchunking file #" << FileCount << " " << s
						<< endl;
#endif

				FileDescType fd(s);
				ChunkFile(fd);
				FileMap.insert(make_pair(s, fd));

			} else {
				++OtherCount;
#ifdef __DEBUG__FSM
				std::cout << "Other found :" << dir_itr->path().filename()
						<< endl;
#endif
			}

		} catch (const std::exception & ex) {
			++ErrCount;
			std::cout << ex.what() << std::endl;
		}
	}
	return;
}
void FileSystemMap::CreateMap(string const& MountPoint) {

	fs::path full_path(fs::initial_path<fs::path>());

	full_path = fs::system_complete(fs::path(MountPoint));

	if (!fs::exists(full_path)) {
		BOOST_THROW_EXCEPTION(
				invalid_argument(string(MountPoint).append(" Not found")));
	}

	if (fs::is_directory(full_path)) {
		BuildMap(full_path);
	} else {
		BOOST_THROW_EXCEPTION(
				invalid_argument(
						string("Expecting directory, found :").append(
								full_path.c_str())));
	}
#ifdef __DEBUG__FSM
	std::cout << "\n" << FileCount << " files\n" << DirCount << " directories\n"
			<< OtherCount << " others\n" << ErrCount << " errors\n";
#endif
	return;

}

size_t FileSystemMap::TotalSize() {
	size_t ts = 0;
	for (auto i : ChunkMap) {
		ts += i.second.Length();
	}
	return ts;
}

void FileSystemMap::Debug() {
	cout << "fsmap properties:" << endl;
	cout << "\tnumber of ChunkMap elements: " << ChunkMap.size() << endl;
	cout << "\tsize of fs: " << TotalSize() << endl;
}

#undef __DEBUG__FSM
} /* namespace contentgraph */
