#pragma once

#include <boost/utility.hpp>                // for boost::tie
#include <algorithm>
#include <vector>
#include "ContentSharingGraph.h"
//#include "TestDataset.h"

using namespace boost;
using namespace std;

namespace contentgraph {

template <typename Graph>
class KCore {
public:
	KCore(Graph& g) :
		aGraph(g)
	{}
//	virtual ~KCore(){}


	Graph& SelectCore(size_t k){
		vector<typename Graph::VertexDescriptor> Core;
		typename Graph::VertexIterator vi, vie;

		for (tie(vi, vie)=aGraph.GetVertices(); vi!=vie; vi++){
			Core.push_back(*vi);
		}
		CmpGtVertex<Graph> cmpGT(aGraph);

		make_heap(Core.begin(), Core.end(), cmpGT);

		Weight<Graph> w(aGraph);
		//size_t t = w(Core[0]);
		while(Core.size() > 0 && w(Core[0]) < k )
		{
			pop_heap(Core.begin(), Core.end(), cmpGT);
			aGraph.RemoveVertex(Core.back());
			Core.pop_back();
			make_heap(Core.begin(), Core.end(), cmpGT);
			//t = w(Core[0]);
		}

		return aGraph;
	}

private:
	Graph& aGraph;
};

}

