#include "matrix_graph.hpp"
#include "i_graph.hpp"
#include <cassert>
#include <vector>

using namespace std;
MatrixGraph::MatrixGraph(int vertexCount)
{
    adjMatrix.resize(vertexCount, vector<bool>(vertexCount, false));
}

MatrixGraph::MatrixGraph(const IGraph& graph)
{
    int vertexCount = graph.VerticesCount();
    adjMatrix.resize(vertexCount, vector<bool>(vertexCount, false));

    for (int i = 0; i < vertexCount; ++i) {
        vector<int> nextVertices = graph.GetNextVertices(i);
        for (int j : nextVertices) {
            adjMatrix[i][j] = true;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to)
{
    int vertexCount = VerticesCount();
    assert(from >= 0 && from < vertexCount);
    assert(to >= 0 && to < vertexCount);

    adjMatrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const
{
    return adjMatrix.size();
}

vector<int> MatrixGraph::GetNextVertices(int vertex) const
{
    int vertexCount = VerticesCount();
    assert(vertex >= 0 && vertex < vertexCount);

    vector<int> nextVertices;
    for (int i = 0; i < vertexCount; ++i) {
        if (adjMatrix[vertex][i]) {
            nextVertices.push_back(i);
        }
    }

    return nextVertices;
}

vector<int> MatrixGraph::GetPrevVertices(int vertex) const
{
    int vertexCount = VerticesCount();
    assert(vertex >= 0 && vertex < vertexCount);

    vector<int> prevVertices;
    for (int i = 0; i < vertexCount; ++i) {
        if (adjMatrix[i][vertex]) {
            prevVertices.push_back(i);
        }
    }

    return prevVertices;
}