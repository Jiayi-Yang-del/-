#include <iostream>
#include <queue>
#include <vector>
#include <atomic>
#include <pthread.h>
#include <random>
#include <ctime>
#include "Timer.h"
#include "graph.h"
#include <deque>
#include <ppl.h>
#include <mutex>

using namespace concurrency;
using namespace std;

const int thread_num = 4;
const int MAX_VERTICES = 10000000; // 最大顶点数
atomic<bool> LBitmap[MAX_VERTICES]; // 存储LBitmap值的数组
int P[MAX_VERTICES]; // 存储父节点值的数组

vector<vector<int>> G(MAX_VERTICES); // 邻接表

deque<int> CQ; // 当前层级队列
deque<int> NQ; // 下一层级队列

atomic<int> syncVar(0);
mutex CQMutex; // 访问当前层级队列的互斥锁
mutex NQMutex; // 访问下一层级队列的互斥锁

void* ParallelBFS(void* arg) {
    int r = *(int*)arg;
    int n = MAX_VERTICES;

    for (int i = 0; i < n; i++) {
        LBitmap[i].store(false);
    }

    P[r] = 0;
    CQ.push_back(r);

    while (!CQ.empty()) {
        NQ.clear();
        parallel_for(0, thread_num, [&](int thread_id) {
            printf("线程 #%d 正在运行...\n", thread_id);
            while (1) {
                CQMutex.lock();
                if (CQ.empty()) {
                    CQMutex.unlock();
                    break;
                }
                int64_t u = CQ.front();
                CQ.pop_front();
                CQMutex.unlock();
                for (auto v : G[u]) {
                    if (!LBitmap[v].exchange(true)) {
                            P[v] = u;
                            NQMutex.lock();
                            NQ.push_back(v);
                            NQMutex.unlock();
                     }
                    
                }
            }
            });
        CQ = move(NQ);
    }

    return NULL;
}
void* bfs(void* arg) {
    int numVertices = G.size();
    int start = *(int*)arg;;
    // 创建一个标记数组来记录顶点是否已经被访问过
    vector<bool> visited(numVertices, false);

    // 创建一个队列来存储待访问的顶点
    queue<int> q;

    // 将起始顶点加入队列，并标记为已访问
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        // 从队列中取出一个顶点
        int u = q.front();
        q.pop();

        const int deg = M / N + (u < M % N);
        mt19937 ran(u);
        uniform_int_distribution<int64_t> dis(0, N - 1);
        // 遍历当前顶点的邻居顶点

        for (int j = 0; j < deg; j++) {
            int64_t v = dis(ran);
            if (!visited[v]) {
                q.push(v);
                visited[v] = true;
                P[v] = u;
            }


        }
    }
    return NULL;
}
int main() {
    string filename = "./graph1000.txt";
    int N = 1e7;
    int M = N * 10;
    vector<vector<int>> graph = readGraphFromFile(filename);

    if (graph.empty()) {
        //graph = generateRandomGraph(10000000, 500000000);
        graph = generateRandomGraph(1e7, 1e8);
        //saveGraphToFile(graph, filename);
    }

    // 将图的表示转换为邻接表
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            G[i].push_back(graph[i][j]);
        }
    }

    Timer timer;
    pthread_t thread; //fork
    int startVertice = 0;

    pthread_create(&thread, NULL, bfs, (void*)&startVertice);
    pthread_join(thread, NULL);
    double elapsed = timer.elapsed();
    cout << "时间: " << timer.elapsed() << " 秒" << endl;
    printf("BE = %lf", M / elapsed);
    system("pause");
    //cout << "父节点数组 (P): ";
    //for (int i = 0; i < graph.size(); i++) {
    //    cout << P[i] << " ";
    //}
    //cout << endl;

    return 0;
}