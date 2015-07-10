#include <boost/utility.hpp>                // for boost::tie
#include <algorithm>
#include <vector>
#include "ContentSharingGraph.h"
#include "KCore.h"

using namespace boost;
using namespace std;

namespace contentgraph {

CSGraph&
KCore::SelectCore(
	CSGraph::DegreeSizeType k)
{
	vector<CSGraph::VertexDescriptor> Core;
	CSGraph::VertexIterator vi, vie;

	for (tie(vi, vie)=aGraph.GetVertices(); vi!=vie; vi++){
		Core.push_back(*vi);
	}
	CmpGtVertex cmpGT(aGraph);

	make_heap(Core.begin(), Core.end(), cmpGT);

	Weight w(aGraph);
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

}
