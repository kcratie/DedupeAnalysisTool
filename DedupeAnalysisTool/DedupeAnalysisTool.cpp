#include <cstddef>
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <ctime>
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
		cout << "Running Dedup Analysis Tool" << endl;
		time_t tstart, tend;
		typedef contentgraph::FileDescType VertexProperties1;
		typedef ContentSharingGraph<VertexProperties1, EdgeProperties> FMGraph;
		time(&tstart);
		FMGraph csg;
		FileSystemMap fsm(csg);
		fsm.CreateMap("/home/kcratie/Documents/include");
		time(&tend);
		cout << "fsmap created in: " << difftime(tend, tstart)<< " seconds" <<endl;


		time(&tstart);
		KCore <FMGraph>kg(csg);
		kg.SelectCore(1275);
		time(&tend);
		cout << "k-core created in: " << difftime(tend, tstart)<< " seconds" <<endl;
		for_each(csg.GetVertices().first, csg.GetVertices().second, DisplayVertex<FMGraph>(csg));

	}
	catch (std::exception &e) {
		cout << e.what();
	}

	return status;
}

/*

 */

