#pragma once

#include "global.hpp"
#include "vector.hpp"
#include "matrix.hpp"
using std::make_pair;
using std::max;
using std::min;

class Model
{
    std::vector<Vector> vertex;
    std::vector<bool> removed;
    std::vector<std::set<Edge>> face;
    std::set<Edge> edge;
    std::priority_queue<std::pair<double, Edge>> heap;
    size_t faceNum;

    double edgeLen(const Edge &e)
    {
        return norm(vertex[e.first] - vertex[e.second]);
    }

public:
    void clear()
    {
        vertex.clear();
        removed.clear();
        face.clear();
        edge.clear();
        faceNum = 0;
    }

    size_t getFaceN()
    {
        return faceNum;
    }

    void loadModelFile(std::string filename)
    {
        clear();

        char buffer[BUFFER_SIZE];
        FILE *file = fopen(filename.c_str(), "r");
        std::vector<std::string> vertexIn;
        std::vector<std::string> faceIn;
        while (fgets(buffer, BUFFER_SIZE, file) != NULL)
        {
            int ptr = 0;
            while (buffer[ptr] != 0 && buffer[ptr] != 'v' && buffer[ptr] != 'f' && buffer[ptr] != '#')
                ptr++;
            if (buffer[ptr] == 'v')
                vertexIn.push_back(std::string(buffer));
            if (buffer[ptr] == 'f')
                faceIn.push_back(std::string(buffer));
        }
        fclose(file);

        size_t vertexN = vertexIn.size();
        vertex.resize(vertexN, Vector(3, 0));
        removed.resize(vertexN, false);
        face.resize(vertexN);
        faceNum = faceIn.size();

        for (int i = 0; i < vertexN; i++)
        {
            sscanf(vertexIn[i].c_str(), "%*s%lf%lf%lf", &vertex[i][0], &vertex[i][1], &vertex[i][2]);
        }

        for (const auto &f : faceIn)
        {
            int v[3];
            sscanf(f.c_str(), "%*s%d%d%d", v, v + 1, v + 2);
            v[0]--;
            v[1]--;
            v[2]--;
            face[v[0]].insert(make_pair(v[1], v[2]));
            face[v[1]].insert(make_pair(v[2], v[0]));
            face[v[2]].insert(make_pair(v[0], v[1]));
            std::sort(v, v + 3);
            edge.insert(make_pair(v[0], v[1]));
            edge.insert(make_pair(v[1], v[2]));
            edge.insert(make_pair(v[0], v[2]));
        }
    }

    void saveModelFile(std::string filename)
    {
        FILE *file = fopen(filename.c_str(), "w");
        size_t vertexN = vertex.size();
        std::vector<int> vertexID(vertexN, 0);
        int vertexReal = 0;

        for (int i = 0; i < vertexN; i++)
        {
            if (removed[i])
                continue;
            vertexID[i] = ++vertexReal;
            fprintf(file, "v %.8lf %.8lf %.8lf\n", vertex[i][0], vertex[i][1], vertex[i][2]);
        }

        for (int i = 0; i < vertexN; i++)
        {
            if (removed[i])
                continue;
            for (const auto &f : face[i])
            {
                if (i < f.first && i < f.second)
                {
                    fprintf(file, "f %d %d %d\n", vertexID[i], vertexID[f.first], vertexID[f.second]);
                }
            }
        }
    }

    Vector solveEquation(Matrix m, int n)
    {
        if (m.size() == 0)
            return Vector();
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
                if (fabs(m[i][i]) < fabs(m[j][i]))
                    m[i].swap(m[j]);
            if (fabs(m[i][i]) < EPSLOOSE)
                throw 200;
            m[i] = m[i] / m[i][i];
            for (int j = i + 1; j < n; j++)
                m[j] = m[j] - m[j][i] * m[i];
        }
        Vector v(n);
        for (int i = n - 1; i >= 0; i--)
        {
            v[i] = -m[i][n];
            for (int j = i + 1; j < n; j++)
            {
                v[i] -= m[i][j] * v[j];
            }
        }

        return v;
    }

    std::pair<Vector, double> getPosition(const Edge &e)
    {
        Matrix q(4, Vector(4, 0));
        for (const auto &f : face[e.first])
        {
            auto n = crossProduct(vertex[f.first] - vertex[e.first], vertex[f.second] - vertex[e.first]);
            n = n / norm(n);
            n.push_back(-innerProduct(vertex[e.first], n));
            outerProductFast(n, n, q);
        }
        for (const auto &f : face[e.second])
        {
            auto n = crossProduct(vertex[f.first] - vertex[e.second], vertex[f.second] - vertex[e.second]);
            n = n / norm(n);
            n.push_back(-innerProduct(vertex[e.second], n));
            outerProductFast(n, n, q);
        }

        Vector v;
        try
        {
            v = solveEquation(q, 3);
        }
        catch (...)
        {
            v = (vertex[e.first] + vertex[e.second]) / 2;
        }
        if (norm(v - vertex[e.first]) + norm(v - vertex[e.second]) > TOLERATE * norm(vertex[e.first] - vertex[e.second]))
        {
            v = (vertex[e.first] + vertex[e.second]) / 2;
        }
        v.push_back(1);
        double cost = innerProduct(innerProduct(v, q), v);
        v.pop_back();
        return make_pair(v, cost);
    }

    std::pair<Edge, Vector> selectEdge()
    {
        Edge idx = make_pair(-1, -1);
        Vector pos;
        std::pair<double, Edge> tmp;
        while (!heap.empty())
        {
            tmp = heap.top();
            heap.pop();
            if (edge.find(tmp.second) == edge.end())
                continue;
            if (removed[tmp.second.first] || removed[tmp.second.second])
                continue;
            auto act = getPosition(tmp.second);
            if (fabs(act.second + tmp.first) > EPS)
                continue;
            idx = tmp.second;
            pos = act.first;
            break;
        }
        return std::make_pair(idx, pos);
    }

    bool faceReverse(const Edge &e, const Vector &v1, const Vector &v2)
    {
        const auto &x = vertex[e.first];
        const auto &y = vertex[e.second];
        return innerProduct(crossProduct(x - v1, y - v1), crossProduct(x - v2, y - v2)) < 0;
        return 0;
    }

    void addToHeap(const Edge &e)
    {
        auto pos = getPosition(e);
        heap.push(make_pair(-pos.second, e));
    }

    void updateNeighborEdge(int v)
    {
        std::set<int> neighbor;
        for (const auto &f : face[v])
        {
            neighbor.insert(f.first);
            neighbor.insert(f.second);
        }
        for (auto x : neighbor)
        {
            addToHeap(make_pair(min(x, v), max(x, v)));
        }
    }

    void removeEdge(const Edge &e, const Vector &v)
    {
        std::vector<Edge> toRev;
        for (const auto &f : face[e.first])
        {
            if (f.first == e.second || f.second == e.second)
                continue;
            auto reverse = faceReverse(f, vertex[e.first], v);
            if (!reverse)
                continue;
            toRev.push_back(f);
            face[f.second].erase(make_pair(e.first, f.first));
            face[f.second].insert(make_pair(f.first, e.first));

            face[f.first].erase(make_pair(f.second, e.first));
            face[f.first].insert(make_pair(e.first, f.second));
        }
        for (const auto &f : toRev)
        {
            face[e.first].erase(f);
            face[e.first].insert(make_pair(f.second, f.first));
        }

        for (const auto &f : face[e.second])
        {
            face[f.second].erase(make_pair(e.second, f.first));
            auto reverse = faceReverse(f, vertex[e.second], v);
            if (f.first != e.first && f.second != e.first)
            {
                if (reverse)
                {
                    face[f.second].insert(make_pair(f.first, e.first));
                }
                else
                {
                    face[f.second].insert(make_pair(e.first, f.first));
                }
            }

            face[f.first].erase(make_pair(f.second, e.second));
            if (f.first != e.first && f.second != e.first)
            {
                if (reverse)
                {
                    face[f.first].insert(make_pair(e.first, f.second));
                }
                else
                {
                    face[f.first].insert(make_pair(f.second, e.first));
                }
            }

            if (f.first == e.first || f.second == e.first)
            {
                faceNum--;
            }
            else if (reverse)
            {
                face[e.first].insert(make_pair(f.second, f.first));
            }
            else
            {
                face[e.first].insert(f);
            }

            auto tmp = make_pair(min(e.second, f.first), max(e.second, f.first));
            if (edge.find(tmp) != edge.end())
                edge.erase(tmp);
            tmp = make_pair(min(e.second, f.second), max(e.second, f.second));
            if (edge.find(tmp) != edge.end())
                edge.erase(tmp);
            if (f.first != e.first && f.second != e.first)
            {
                edge.insert(make_pair(min(e.first, f.first), max(e.first, f.first)));
                edge.insert(make_pair(min(e.first, f.second), max(e.first, f.second)));
            }
        }

        edge.erase(e);
        vertex[e.first] = v;
        vertex[e.second].clear();
        removed[e.second] = true;
        face[e.second].clear();

        std::set<int> neighbor;
        for (const auto &f : face[e.first])
        {
            neighbor.insert(f.first);
            neighbor.insert(f.second);
        }
        for (auto nb : neighbor)
        {
            updateNeighborEdge(nb);
        }
    }

    void buildHeap()
    {
        while (!heap.empty())
            heap.pop();
        for (const auto &e : edge)
        {
            addToHeap(e);
        }
    }

    void simplify(size_t target)
    {
        buildHeap();
        while (faceNum > target)
        {
            auto e = selectEdge();
            if (e.first != make_pair(-1, -1))
                removeEdge(e.first, e.second);
            else
            {
                printf("ERROR: Cannot generate enough edges, program will save current result\n");
                return;
            }
        }
    }
};
