#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

vector<vector<int>> generateRandomGraph(int numVertices, int numEdges) {
    // ��ʼ�������������
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, numVertices - 1);

    vector<vector<int>> graph(numVertices);

    // ������ɱ�
    for (int i = 0; i < numEdges; i++) {
        int u = distribution(gen);
        int v = distribution(gen);

        // ȷ���ߵ��������㲻�ظ��Ҳ����Ի�
        while (u == v || find(graph[u].begin(), graph[u].end(), v) != graph[u].end()) {
            v = distribution(gen);
        }

        // ��ӱߵ�ͼ��
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
    // ÿ���߱����������Σ�������Ҫ����2
    return count / 2;
}
void saveGraphToFile(const vector<vector<int>>& graph, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        // д�붥�����ͱ���
        file << graph.size() << " " << countEdges(graph) << endl;

        // д��ߵ���Ϣ
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
    // ����һ������ numVertices ��������ڽӱ�
    vector<vector<int>> graph(numVertices);

    // ����һ�������������
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, numVertices - 1);

    // �������ö�������
    vector<int> availableVertices(numVertices - 1);
    for (int i = 0; i < numVertices - 1; i++) {
        availableVertices[i] = i + 1;
    }

    for (int i = 0; i < numVertices; i++) {
        // Ϊÿ��������������Ķ���
        int remainingDegree = degree;

        // ϴ�ƿ��ö�������
        shuffle(availableVertices.begin(), availableVertices.end(), gen);

        int k = min(remainingDegree, static_cast<int>(availableVertices.size()));
        for (int j = 0; j < k; j++) {
            // ���ѡ��һ�����ö�������
            int vertex = availableVertices[j];

            // ��ӱ�
            graph[i].push_back(vertex);
            graph[vertex].push_back(i);

            // ����ʣ�����
            remainingDegree--;
        }

        // ���¿��ö�������
        availableVertices.resize(availableVertices.size() - k);
    }

    return graph;
}