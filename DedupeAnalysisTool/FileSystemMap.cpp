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
//#define __DEBUG__FSM

FileSystemMap::FileSystemMap(FMGraph & CntGraph) :
	//ChunkMap(DataChunkMapType(1 << 16)),
		FileCount(0),
		DirCount(0),
		OtherCount(0),
		ErrCount(0),
		mCntGraph(CntGraph)
{}

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
	if (FileObj.Name().compare("/home/kcratie/Documents/include/mntent.h") == 0)
		int i =0;
	Digest md;              // the hash object
	//Digest::value_type sha;      // the hash value
	size_t const length = 1 << 8;
	std::array<byte, length> buf;

	size_t chunkOffset = 0;
	while (!ifl.eof()) {
		ifl.read((char*) buf.data(), length);
		size_t cnt = ifl.gcount();
		DataChunkType chunk;
		chunk.Hash(buf.data(), cnt, md);

		auto p = ChunkMap.insert(std::make_pair(chunk.Hash(), chunk));
		DataChunkType & dc = p.first->second;
		dc.AddDescriptor(FileObj.Name(), chunkOffset, cnt, &FileObj);
		//if (p.second)	//add chunk to file object only if unique
		FileObj.AddChunk(dc);
		chunkOffset += cnt;

		auto pfl = dc.ChunksFirstLast();
		if (pfl.first != pfl.second){
			FileDescType * fd1 = static_cast<FileDescType *>(pfl.first.FileDescObj());
			FileDescType * fd2 = static_cast<FileDescType *>(pfl.second.FileDescObj());
			if(fd1!=fd2){
				FMGraph::VertexDescriptor v1 = static_cast<FMGraph::VertexDescriptor>(fd1->VertexDesc());
				FMGraph::VertexDescriptor v2 = static_cast<FMGraph::VertexDescriptor>(fd2->VertexDesc());
				mCntGraph.AddEdge(v1, v2, EdgeProperties(dc.Length()));
				fd1->AddSharedBytesTotal(dc.Length());
				fd2->AddSharedBytesTotal(dc.Length());
				if (FileObj.Name().compare("/home/kcratie/Documents/include/mntent.h") == 0)
					int i =0;
			}
	//		EdgeProperties ep(dc.Length());
	//		mCntGraph.AddEdge(v1, v2, ep);
		}


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

				//FileDescType fd(s);
				auto ap = FileMap.insert(make_pair(s, FileDescType(s)));
				if (ap.second)	//new item was inserted
				{
					auto vd = mCntGraph.AddVertex(&ap.first->second);	//add the new file to the boost graph as a vertex
					ap.first->second.VertexDesc((void*)vd);				//add shortcut pointer to the vertex descriptor to Fileobj
				}
				ChunkFile((*ap.first).second);

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
void FileSystemMap::DisplayFileMap(){
	cout << "fsmap properties:" << endl;
	cout << "\tnumber of FileMap elements: " << FileMap.size() << endl;
	for (auto i : FileMap){
		cout << "\t\t: " << i.second.Name() << endl;
	}
}
void FileSystemMap::Debug() {
	DisplayFileMap();
	cout << "\tnumber of ChunkMap elements: " << ChunkMap.size() << endl;
//	cout << "\tsize of fs: " << TotalSize() << endl;
}

#undef __DEBUG__FSM
} /* namespace contentgraph */
