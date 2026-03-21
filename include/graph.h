/*
TWISTS:

1) Weighted Edges - The values in edge[i][j] include the weight of the edge, with std::numeric_limits<double>::infinity() representing the absence of edge.
2) Degree Calculation - Methods to get the degree (count of edges) entering/leaving a vertex.
*/

#pragma once

#include "list.h"

struct Graph {
	virtual void addEdge(size_t i, size_t j, double weight = 1.0) = 0;
	virtual void removeEdge(size_t i, size_t j) = 0;
	virtual bool hasEdge(size_t i, size_t j) const = 0;
	virtual void outEdges(size_t i, List<size_t>& edges) const = 0;
	virtual void inEdges(size_t i, List<size_t>& edges) const = 0;
	
	virtual size_t outDegreeOf(size_t i) const = 0;
	virtual size_t inDegreeOf(size_t i) const = 0;

	virtual ~Graph() = default;
};
