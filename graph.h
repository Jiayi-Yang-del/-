#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

vector<vector<int>> generateRandomGraph(int numVertices, int numEdges) {
    // 初始化随机数生成器
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, numVertices - 1);

    vector<vector<int>> graph(numVertices);

    // 随机生成边
    for (int i = 0; i < numEdges; i++) {
        int u = distribution(gen);
        int v = distribution(gen);

        // 确保边的两个顶点不重复且不是自环
        while (u == v || find(graph[u].begin(), graph[u].end(), v) != graph[u].end()) {
            v = distribution(gen);
        }

        // 添加边到图中
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    return graph;
}
int countEdges(const vector<vector<int>>& graph) {
    int count = 0;
    for (const auto& edges : graph) {
        count += edges.size();
    }
    // 每条边被计算了两次，所以需要除以2
    return count / 2;
}
void saveGraphToFile(const vector<vector<int>>& graph, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        // 写入顶点数和边数
        file << graph.size() << " " << countEdges(graph) << endl;

        // 写入边的信息
        for (int u = 0; u < graph.size(); u++) {
            for (int v : graph[u]) {
                if (u < v) {
                    file << u << " " << v << endl;
                }
            }
        }

        file.close();
        cout << "Graph saved to " << filename << endl;
    }
    else {
        cerr << "Unable to open file: " << filename << endl;
    }
}

vector<vector<int>> readGraphFromFile(const string& filename) {
    ifstream file(filename);

    if (file.is_open()) {
        int numVertices, numEdges;
        file >> numVertices >> numEdges;

        vector<vector<int>> graph(numVertices);

        for (int i = 0; i < numEdges; i++) {
            int u, v;
            file >> u >> v;

            graph[u].push_back(v);
            graph[v].push_back(u);
        }

        file.close();

        return graph;
    }
    else {
        cerr << "Unable to open file: " << filename << endl;
        return vector<vector<int>>();
    }
}
vector<vector<int>> generateRandomGraph_regular(int numVertices, int degree) {
    // 创建一个包含 numVertices 个顶点的邻接表
    vector<vector<int>> graph(numVertices);

    // 创建一个随机数生成器
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, numVertices - 1);

    // 创建可用顶点数组
    vector<int> availableVertices(numVertices - 1);
    for (int i = 0; i < numVertices - 1; i++) {
        availableVertices[i] = i + 1;
    }

    for (int i = 0; i < numVertices; i++) {
        // 为每个顶点生成随机的度数
        int remainingDegree = degree;

        // 洗牌可用顶点数组
        shuffle(availableVertices.begin(), availableVertices.end(), gen);

        int k = min(remainingDegree, static_cast<int>(availableVertices.size()));
        for (int j = 0; j < k; j++) {
            // 随机选择一个可用顶点连接
            int vertex = availableVertices[j];

            // 添加边
            graph[i].push_back(vertex);
            graph[vertex].push_back(i);

            // 减少剩余度数
            remainingDegree--;
        }

        // 更新可用顶点数组
        availableVertices.resize(availableVertices.size() - k);
    }

    return graph;
}