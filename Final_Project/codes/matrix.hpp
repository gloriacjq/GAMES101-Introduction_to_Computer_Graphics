#pragma once

#include "global.hpp"
#include "vector.hpp"

typedef std::vector<Vector> Matrix;
typedef std::pair<int, int> Edge;

Matrix outerProduct(const Vector &a, const Vector &b)
{
    Matrix c(a.size(), Vector(b.size(), 0));
    for (int i = 0; i < a.size(); i++)
        for (int j = 0; j < b.size(); j++)
            c[i][j] = a[i] * b[j];
    return c;
}

void outerProductFast(const Vector &a, const Vector &b, Matrix &c)
{
    if (a.size() == 0)
        return;
    for (int i = 0; i < a.size(); i++)
        for (int j = 0; j < b.size(); j++)
            c[i][j] += a[i] * b[j];
}

Vector innerProduct(const Vector &a, const Matrix &b)
{
    if (a.size() == 0)
        return Vector();
    Vector c(b[0].size(), 0);
    for (int i = 0; i < b.size(); i++)
        for (int j = 0; j < b[0].size(); j++)
            c[j] += a[i] * b[i][j];
    return c;
}

Matrix operator+(const Matrix &a, const Matrix &b)
{
    Matrix c(a.size());
    for (int i = 0; i < a.size(); i++)
        c[i] = a[i] + b[i];
    return c;
}