#include "arc_graph.hpp"
#include "i_graph.hpp"
#include <vector>
using namespace std;

ArcGraph::ArcGraph(int vertexCount)
{
    edges.reserve(0);
}

ArcGraph::ArcGraph(const IGraph& graph)
{
    int vertexCount = graph.VerticesCount();
    edges.reserve(vertexCount);

    for (int i = 0; i < vertexCount; ++i) {
        vector<int> nextVertices = graph.GetNextVertices(i);
        for (int j : nextVertices) {
            edges.emplace_back(i, j);
        }
    }
}

void ArcGraph::AddEdge(int from, int to)
{
    edges.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const
{
    int maxVertex = -1;
    for (const auto& edge : edges) {
        maxVertex = max(maxVertex, max(edge.first, edge.second));
    }
    return maxVertex + 1;
}

vector<int> ArcGraph::GetNextVertices(int vertex) const
{
    vector<int> nextVertices;
    for (const auto& edge : edges) {
        if (edge.first == vertex) {
            nextVertices.push_back(edge.second);
        }
    }
    return nextVertices;
}

vector<int> ArcGraph::GetPrevVertices(int vertex) const
{
    vector<int> prevVertices;
    for (const auto& edge : edges) {
        if (edge.second == vertex) {
            prevVertices.push_back(edge.first);
        }
    }
    return prevVertices;
}