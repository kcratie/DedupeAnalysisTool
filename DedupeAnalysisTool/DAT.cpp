#include <cstddef>
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>
#include <array>
#include <unordered_map>

//#include <boost/progress.hpp>
#include <boost/config.hpp>
#include <boost/utility.hpp>                // for boost::tie
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/throw_exception.hpp>

//#include <boost/uuid/sha1.hpp>

//#include "ChunkDesc.h"
#include "ContentSharingGraph.h"
#include "KCore.h"
#include "TestDataset.h"
#include "FileSystemMap.h"
using namespace boost;
using namespace std;
using namespace contentgraph;


int main(int argc, char* argv[]) {
	int status = 0;

	try {
		FileSystemMap fsm;
		fsm.CreateMap("/home/kcratie/test");
//		fsm.Debug();

		typedef contentgraph::FileDescType VertexProperties1;
		typedef ContentSharingGraph<VertexProperties1, EdgeProperties> FMGraph;

		FMGraph csg;
//		FileSystemMap::FileMapRangeType fmr = fsm.FileMapRange();

//		for(;fmr.first!=fmr.second; fmr.first++)
//		{
//			FileSystemMap::FileMapItrType fmi = fmr.first;
//			FileDescType fd = (*fmi).second;
//			FMGraph::VertexDescriptor v = csg.AddVertex(fd);
//		}

		FileSystemMap::ChunkMapRangeType cmr = fsm.ChunkMapRange();
		ChunkDescType c1 = (*cmr.first).second;
//		c1.Debug();
		VertexProperties1& vp1 = *static_cast<VertexProperties1*>(c1.FileDescObj());
//		vp1.Debug();
		FMGraph::VertexDescriptor v1 = csg.AddVertex(vp1);
		cmr.first++;
		for(;cmr.first!=cmr.second; cmr.first++)
		{
			ChunkDescType c = (*cmr.first).second;
//			c.Debug();
			VertexProperties1& vp = *static_cast<VertexProperties1*>(c.FileDescObj());
			FMGraph::VertexDescriptor v = csg.AddVertex(vp);
			EdgeProperties ep(c.Length());
			csg.AddEdge(v1, v, ep);
		}
		for_each(csg.GetVertices().first, csg.GetVertices().second, DisplayVertex<FMGraph>(csg));
//		fsm.Debug();
		/*
		KCore <FMGraph>kg(csg);
//		for_each(csg.GetVertices().first, csg.GetVertices().second, DisplayVertex<FMGraph>(csg));
		kg.SelectCore(5);
		for_each(csg.GetVertices().first, csg.GetVertices().second, DisplayVertex<FMGraph>(csg));
		*/
	}
	catch (std::exception &e) {
		cout << e.what();
	}

//	csg.AddEdge();

//	CSGraph::GraphContainer gc(EdgesStar, EdgesStar + NumEdgesStar, EdgeWeights, NumVerticies);
//	CSGraph gStar(gc);
//	KCore kg(gStar);
//
//
//	//for_each(gStar.GetVertices().first, gStar.GetVertices().second, DisplayVertex(gStar));
//	kg.SelectCore(50);
//
//	for_each(gStar.GetVertices().first, gStar.GetVertices().second, DisplayVertex(gStar));
	return status;
}

/*

 */

