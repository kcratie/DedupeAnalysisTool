/*
 * TestDataset.h
 *
 *  Created on: Jun 21, 2015
 *      Author: kcratie
 */

#ifndef TESTDATASET_H_
#define TESTDATASET_H_
#include <string>
#include "ContentSharingGraph.h"

using namespace std;

// Make convenient labels for the vertices
enum {
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	N
};
const int NumVerticies = N;
const string VertexNames[] =
		{ "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
		   "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", };

typedef std::pair<int,int> Edge;
Edge EdgesStar [] = { Edge(F1,F2), Edge(F1,F3), Edge(F1,F4), Edge(F1,F5),
		Edge(F1,F6), Edge(F1,F7), Edge(F1,F8), Edge(F1,F9), Edge(F1,F10),
		Edge(F1,F11), Edge(F5,F15), Edge(F11,F12), Edge(F11,F13), Edge(F11,F14),
		Edge(F11,F15), Edge(F11,F16), Edge(F11,F17), Edge(F11,F18), Edge(F11,F19),
		Edge(F2,F3), Edge(F2,F4), Edge(F3,F4), };

Edge EdgesRing [] = { Edge(F1,F2), Edge(F2,F3), Edge(F3,F4), Edge(F4,F5),
		Edge(F5,F6), Edge(F6,F7), Edge(F7,F8), Edge(F8,F9), Edge(F9,F10),
		Edge(F1,F11), Edge(F5,F15), Edge(F11,F12), Edge(F12,F13), Edge(F13,F14),
		Edge(F14,F15), Edge(F15,F16), Edge(F16,F17), Edge(F17,F18), Edge(F18,F19),};

//int EdgeWeights [] = {52, 52, 52, 52, 52, 52, 52, 52, 52, 4, 4, 16, 16, 16, 16, 16, 16, 16, 16, 1, 1, 1,};
//#define SharedBytes EdgeWeights
contentgraph::EdgeProperties EdgeWeights [] = {52, 52, 52, 52, 52, 52, 52, 52, 52, 4, 4, 16, 16, 16, 16, 16, 16, 16, 16, 1, 1, 1,};

const int NumEdgesStar = sizeof(EdgesStar) / sizeof(EdgesStar[0]);
const int NumEdgesRing = sizeof(EdgesRing)/sizeof(EdgesRing[0]);




#endif /* TESTDATASET_H_ */
