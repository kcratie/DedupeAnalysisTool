#include <cstddef>
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <ctime>

//#include <boost/config.hpp>
//#include <boost/utility.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/graphviz.hpp>
//#include <boost/throw_exception.hpp>

#include "KCore.h"
#include "FileSystemMap.h"

using namespace boost;
using namespace std;
using namespace contentgraph;

void DisplayUsage(){
	cerr<< "dat /path k-value";
}

int main(int argc, char* argv[]) {
	int status = 0;

	try {
		if (argc < 3)
		{
			DisplayUsage();
			BOOST_THROW_EXCEPTION(
					invalid_argument(string("Invalid program arguments")));
		}
		string dir(argv[1]);
		size_t k = atoi(argv[2]);
		cout << "Running Dedup Analysis Tool" << endl;
		time_t tstart, tend;
		time(&tstart);

		FileSystemMap::FMGraph csg;
		FileSystemMap fsm(csg);
		fsm.CreateMap(dir);
		time(&tend);
		cout << "fsmap created in: " << difftime(tend, tstart)<< " seconds" <<endl;

		time(&tstart);
		KCore <FileSystemMap::FMGraph>kg(csg);
		kg.SelectCore(k);
		time(&tend);

		for_each(csg.GetVertices().first, csg.GetVertices().second, DisplayVertex<FileSystemMap::FMGraph>(csg));
		cout << "k-core created in: " << difftime(tend, tstart)<< " seconds" <<endl;
	}
	catch (std::exception &e) {
		cerr << e.what();
		status =-1;
	}

	return status;
}
