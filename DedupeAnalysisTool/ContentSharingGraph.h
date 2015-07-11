#pragma once

#include <boost/config.hpp>
#include <boost/version.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/static_assert.hpp>

using namespace boost;
using namespace std;

enum vertex_properties_t {
	vertex_properties
};
enum edge_properties_t {
	edge_properties
};

namespace boost {
	BOOST_INSTALL_PROPERTY(vertex, properties);
	BOOST_INSTALL_PROPERTY(edge, properties);
}

namespace contentgraph
{


template<typename VERTEXPROPERTIES, typename EDGEPROPERTIES>
class ContentSharingGraph {
public:

	/* an adjacency_list like we need it */
	typedef adjacency_list<
			setS, // disallow parallel edges
			listS, // vertex container
			undirectedS, // directed graph
			property<vertex_properties_t, VERTEXPROPERTIES>,
			property<edge_properties_t, EDGEPROPERTIES> > GraphContainer;

	/* a bunch of graph-specific typedefs */
	typedef typename graph_traits<GraphContainer>::vertex_descriptor VertexDescriptor;
	typedef typename graph_traits<GraphContainer>::edge_descriptor EdgeDescriptor;
	typedef std::pair<EdgeDescriptor, EdgeDescriptor> EdgePair;

	typedef typename graph_traits<GraphContainer>::vertex_iterator VertexIterator;
	typedef typename graph_traits<GraphContainer>::edge_iterator EdgeIterator;
	typedef typename graph_traits<GraphContainer>::adjacency_iterator AdjacencyIterator;
	typedef typename graph_traits<GraphContainer>::out_edge_iterator OutEdgeIterator;
	typedef typename graph_traits<GraphContainer>::in_edge_iterator InEdgeIterator;

	typedef typename graph_traits<GraphContainer>::degree_size_type DegreeSizeType;
	typedef typename graph_traits<GraphContainer>::vertices_size_type VerticesSizeType;

	typedef std::pair<AdjacencyIterator, AdjacencyIterator> AdjacencyVertexRange;
	typedef std::pair<OutEdgeIterator, OutEdgeIterator> OutEdgeRange;
	typedef std::pair<InEdgeIterator, InEdgeIterator> InEdgeRange;
	typedef std::pair<VertexIterator, VertexIterator> VertexRange;
	typedef std::pair<EdgeIterator, EdgeIterator> EdgeRange;

	ContentSharingGraph() :
		graph(new GraphContainer())
	{	}

	ContentSharingGraph(GraphContainer* gc) :
		graph(gc)
	{}

	ContentSharingGraph(const ContentSharingGraph& g) :
		graph(g.graph)
	{}

	ContentSharingGraph(ContentSharingGraph&& g){
		graph = g.graph;
		g.graph = NULL;
	}
	virtual ~ContentSharingGraph() {
	}

	ContentSharingGraph& operator=(const ContentSharingGraph &rhs) {
		if (this!= &rhs)
			graph = rhs.graph;
		return *this;
	}

	ContentSharingGraph& operator=(ContentSharingGraph&&rhs) {
		if (this!= &rhs){
			graph = rhs.graph;
			rhs.graph = NULL;
		}
		return *this;
	}

	void Clear() {
		graph->clear();
	}

	VertexDescriptor AddVertex(const VERTEXPROPERTIES& prop) {
		VertexDescriptor v = add_vertex(*graph);
		Properties(v) = prop;
		return v;
	}

	void RemoveVertex(const VertexDescriptor& v) {
		clear_vertex(v, *graph);
		remove_vertex(v, *graph);
	}

	void AddEdge(const VertexDescriptor& v1, const VertexDescriptor& v2,
			const EDGEPROPERTIES& prop_12/*, const EDGEPROPERTIES& prop_21*/) {
		/* TODO: maybe one wants to check if this edge could be inserted */
		EdgeDescriptor addedEdge1 = add_edge(v1, v2, *graph).first;
		//EdgeDescriptor addedEdge2 = add_edge(v2, v1, *graph).first;

		Properties(addedEdge1) = prop_12;
		//properties(addedEdge2) = prop_21;

		return;// EdgePair(addedEdge1, addedEdge2);
	}

	VERTEXPROPERTIES& Properties(const VertexDescriptor& v) {
		typename property_map<GraphContainer, vertex_properties_t>::type param =
				get(vertex_properties, *graph);
		return param[v];
	}

	const VERTEXPROPERTIES& Properties(const VertexDescriptor& v) const {
		typename property_map<GraphContainer, vertex_properties_t>::const_type param =
				get(vertex_properties, *graph);
		return param[v];
	}

	EDGEPROPERTIES& Properties(const EdgeDescriptor& v) {
		typename property_map<GraphContainer, edge_properties_t>::type param =
				get(edge_properties, *graph);
		return param[v];
	}

	const EDGEPROPERTIES& Properties(const EdgeDescriptor& v) const {
		typename property_map<GraphContainer, edge_properties_t>::const_type param =
				get(edge_properties, *graph);
		return param[v];
	}

	const GraphContainer& GetGraph() const {
		return *graph;
	}

	VertexRange GetVertices() const {
		return vertices(*graph);
	}

	AdjacencyVertexRange GetAdjacentVertices(const VertexDescriptor& v) const {
		return adjacent_vertices(v, *graph);
	}

	VerticesSizeType GetVertexCount() const {
		return num_vertices(*graph);
	}

	DegreeSizeType GetVertexDegree(const VertexDescriptor& v) const {
		return degree(v, *graph);
	}

	DegreeSizeType GetVertexOutDegree(const VertexDescriptor& v) const {
		return out_degree(v, *graph);
	}

	EdgeRange GetEdges(){
		return edges(*graph);
	}

	OutEdgeRange GetOutEdges(const VertexDescriptor& v){
		return out_edges(v, *graph);
	}

	InEdgeRange GetInEdges(const VertexDescriptor& v){
		return in_edges(v, *graph);
	}

	VertexDescriptor GetSourceVertex(EdgeDescriptor& e){
		return source(e, *graph);
	}

	VertexDescriptor GetTargetVertex(EdgeDescriptor& e){
		return target(e, *graph);
	}

	void PropertyPut(const VertexDescriptor& v, const VERTEXPROPERTIES& p){
		typename property_map<GraphContainer, vertex_properties_t>::type param =
						get(vertex_properties, *graph);
		put(param, v, p);
	}
protected:
	GraphContainer * graph;
};


struct VertexProperties {
	VertexProperties (): size(0) {}
	VertexProperties (const string& _n, const size_t _s=0) : name(_n), size(_s){}
	string name;
	size_t size;
};

struct EdgeProperties {
	EdgeProperties () : weight(0){}
	EdgeProperties (const size_t _w) : weight(_w){}
	size_t weight;
};

//typedef ContentSharingGraph<VertexProperties, EdgeProperties> Graph;
template <typename Graph>
struct Weight {
	Weight(Graph& g_) : g(g_) { }
	size_t operator()(const typename Graph::EdgeDescriptor e) const
	{
		return g.Properties(e).weight;
	}
	size_t operator()(const typename Graph::VertexDescriptor v) const
	{
		size_t vsz=0;
		typename Graph::OutEdgeIterator ei, eie;
		for (tie(ei, eie)=g.GetOutEdges(v); ei!=eie; ei++){
			vsz += g.Properties(*ei).weight;
		}
		return vsz;
	}
	Graph & g;
};

template <typename Graph>
struct CmpGtVertex {
	CmpGtVertex(Graph & _g) : weight(Weight<Graph>(_g)){}

	bool operator()(const typename Graph::VertexDescriptor &a, const typename Graph::VertexDescriptor &b) const
	{
		return weight(a) > weight(b);
		//return Graph.GetVertexOutDegree(a) > Graph.GetVertexOutDegree(b);
	}
	Weight<Graph> weight;
};

template <typename Graph>
struct DisplayVertex {
	DisplayVertex(Graph& g_) : g(g_), weight(Weight<Graph>(g_)) { }

  void operator()(const typename Graph::VertexDescriptor& v) const
  {
    std::cout << "vertex: " << g.Properties(v).Name() << " weight:"<<weight(v)<<endl;

    // Write out the outgoing edges
    std::cout << "\tout-edges: ";
    typename Graph::OutEdgeIterator oi, oie;
    typename Graph::EdgeDescriptor e;
    for (boost::tie(oi, oie) = g.GetOutEdges(v); oi != oie; ++oi)
    {
      e = *oi;
      typename Graph::VertexDescriptor src = g.GetSourceVertex(e), targ = g.GetTargetVertex(e);
      std::cout << "(" << g.Properties(src).Name()
    		    << "," << g.Properties(targ).Name() << ") ";
    }
    std::cout << std::endl;

    // Write out the incoming edges
    std::cout << "\tin-edges: ";
    typename Graph::InEdgeIterator ini, ine;
    for (boost::tie(ini, ine) = g.GetInEdges(v); ini != ine; ++ini)
    {
      e = *ini;
      typename Graph::VertexDescriptor src = g.GetSourceVertex(e), targ = g.GetTargetVertex(e);
      std::cout << "(" << g.Properties(src).Name()
                << "," << g.Properties(targ).Name() << ") ";
    }
    std::cout << std::endl;

    // Write out all adjacent vertices
    std::cout << "\tadjacent vertices: ";
    typename Graph::AdjacencyIterator ai, aie;
    for (boost::tie(ai,aie) = g.GetAdjacentVertices(v);  ai != aie; ++ai)
      std::cout << g.Properties(*ai).Name() <<  " ";
    std::cout << std::endl;
  }
  Graph & g;
  Weight<Graph> weight;
};

}
