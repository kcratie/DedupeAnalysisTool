#pragma once

#include <boost/utility.hpp>                // for boost::tie
#include <algorithm>
#include <vector>
#include "ContentSharingGraph.h"
//#include "TestDataset.h"

using namespace boost;
using namespace std;

namespace contentgraph {


class KCore {
public:
	KCore(CSGraph& g) :
		aGraph(g){}
//	virtual ~KCore(){}

//	void SetVertexNameProperty()
//	{
//		CSGraph::VertexIterator vi, vie;
//		size_t i = 0;
//		for (tie(vi, vie)=aGraph.GetVertices(); vi!=vie; vi++){
//			aGraph.Properties(*vi).name = VertexNames[i];
//			i++;
//		}
//	}

	CSGraph& SelectCore(CSGraph::DegreeSizeType k);
private:
	CSGraph& aGraph;
};
}

