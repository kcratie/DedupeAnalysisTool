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
		fsm.Debug();

		typedef contentgraph::FileDescType VertexProperties1;
		typedef ContentSharingGraph<VertexProperties1, EdgeProperties> FMGraph;

		FMGraph csg;
		FileSystemMap::FileMapRangeType itr = fsm.FileMapRange();
		for(;itr.first!=itr.second; itr.first++)
		{
			FMGraph::VertexDescriptor v = csg.AddVertex(*itr.first);
		}

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

