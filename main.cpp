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
const int MAX_VERTICES = 10000000; // ��󶥵���
atomic<bool> LBitmap[MAX_VERTICES]; // �洢LBitmapֵ������
int P[MAX_VERTICES]; // �洢���ڵ�ֵ������

vector<vector<int>> G(MAX_VERTICES); // �ڽӱ�

deque<int> CQ; // ��ǰ�㼶����
deque<int> NQ; // ��һ�㼶����

atomic<int> syncVar(0);
mutex CQMutex; // ���ʵ�ǰ�㼶���еĻ�����
mutex NQMutex; // ������һ�㼶���еĻ�����

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
            printf("�߳� #%d ��������...\n", thread_id);
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
    // ����һ�������������¼�����Ƿ��Ѿ������ʹ�
    vector<bool> visited(numVertices, false);

    // ����һ���������洢�����ʵĶ���
    queue<int> q;

    // ����ʼ���������У������Ϊ�ѷ���
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        // �Ӷ�����ȡ��һ������
        int u = q.front();
        q.pop();

        const int deg = M / N + (u < M % N);
        mt19937 ran(u);
        uniform_int_distribution<int64_t> dis(0, N - 1);
        // ������ǰ������ھӶ���

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

    // ��ͼ�ı�ʾת��Ϊ�ڽӱ�
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
    cout << "ʱ��: " << timer.elapsed() << " ��" << endl;
    printf("BE = %lf", M / elapsed);
    system("pause");
    //cout << "���ڵ����� (P): ";
    //for (int i = 0; i < graph.size(); i++) {
    //    cout << P[i] << " ";
    //}
    //cout << endl;

    return 0;
}