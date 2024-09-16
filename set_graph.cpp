#include "set_graph.hpp"
#include "i_graph.hpp"
#include <vector>
#include <unordered_set>
#include <set>
#include <cassert>
using namespace std;
SetGraph::SetGraph(int vertexCount)
{
    adjSets.resize(vertexCount);
    prevSets.resize(vertexCount);
}

SetGraph::SetGraph(const IGraph& graph)
{
    adjSets.resize(graph.VerticesCount());
    prevSets.resize(graph.VerticesCount());

    for (int from = 0; from < graph.VerticesCount(); ++from) {
        vector<int> nextVertices = graph.GetNextVertices(from);
        for (int to : nextVertices) {
            adjSets[from].insert(to);
            prevSets[to].insert(from);
        }
    }
}

void SetGraph::AddEdge(int from, int to)
{
    int vertexCount = VerticesCount();
    assert(from >= 0 && from < vertexCount);
    assert(to >= 0 && to < vertexCount);

    adjSets[from].insert(to);
    prevSets[to].insert(from);
}

int SetGraph::VerticesCount() const
{
    return adjSets.size();
}

vector<int> SetGraph::GetNextVertices(int vertex) const
{
    int vertexCount = VerticesCount();
    assert(vertex >= 0 && vertex < vertexCount);

    vector<int> nextVertices;
    for (int v : adjSets[vertex]) {
        nextVertices.push_back(v);
    }

    return nextVertices;
}

vector<int> SetGraph::GetPrevVertices(int vertex) const
{
    int vertexCount = VerticesCount();
    assert(vertex >= 0 && vertex < vertexCount);

    vector<int> prevVertices;
    for (int v : prevSets[vertex]) {
        prevVertices.push_back(v);
    }

    return prevVertices;
}