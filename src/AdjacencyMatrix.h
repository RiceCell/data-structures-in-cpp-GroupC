/*
TWISTS:

1) Weighted Edges - The values in edge[i][j] include the weight of the edge, with std::numeric_limits<double>::infinit() representing the absence of edge.
2) Degree Calculation - Methods to get the degree (count of edges) entering/leaving a vertex.
*/

#pragma once

#include "list.h"
#include "graph.h"
#include <vector>
#include <cassert>
#include <limits>

class AdjacencyMatrix : public Graph {
public:
	AdjacencyMatrix(size_t numOfVertices)
		: numOfVertices{ numOfVertices },
		edgeMatrix( numOfVertices*numOfVertices, std::numeric_limits<double>::infinity() )
	{ }

	void addEdge(size_t i, size_t j, double weight = 1.0) override {
		assert(i < numOfVertices && j < numOfVertices);
		edgeMatrix[i * numOfVertices + j] = weight;
	}

	void removeEdge(size_t i, size_t j) override {
		assert(i < numOfVertices && j < numOfVertices);
		edgeMatrix[i * numOfVertices + j] = std::numeric_limits<double>::infinity();
	}

	bool hasEdge(size_t i, size_t j) const override {
		assert(i < numOfVertices && j < numOfVertices);
		return edgeMatrix[i * numOfVertices + j] != std::numeric_limits<double>::infinity();
	}

	void outEdges(size_t i, List<size_t>& edges) const override {
		assert(i < numOfVertices);
		for (size_t j = 0; j < numOfVertices; j++) {
			if (hasEdge(i, j)) {
				edges.add(edges.size(), j);
			}
		}
	}

	void inEdges(size_t i, List<size_t>& edges) const override {
		assert(i < numOfVertices);
		for (size_t j = 0; j < numOfVertices; j++) {
			if (hasEdge(j, i)) {
				edges.add(edges.size(), j);
			}
		}
	}

	size_t outDegreeOf(size_t i) const override {
		assert(i < numOfVertices);
		size_t outDegree{ 0 };
		for (size_t j = 0; j < numOfVertices; j++) {
			if (hasEdge(i, j)) {
				outDegree++;
			}
		}
		return outDegree;
	}

	size_t inDegreeOf(size_t i) const override {
		assert(i < numOfVertices);
		size_t inDegree{ 0 };
		for (size_t j = 0; j < numOfVertices; j++) {
			if (hasEdge(j, i)) {
				inDegree++;
			}
		}
		return inDegree;
	}

	~AdjacencyMatrix() { }

private:
	size_t numOfVertices;
	std::vector<double> edgeMatrix;
};