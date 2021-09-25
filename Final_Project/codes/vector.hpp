#pragma once

#include <cmath>

class Vector
{
    double a[4];
    int _size;

public:
    Vector()
    {
        _size = 0;
    }
    Vector(int n)
    {
        _size = n;
    }
    Vector(int n, double x)
    {
        _size = n;
        for (int i = 0; i < n; i++)
            a[i] = x;
    }
    void swap(Vector &b)
    {
        for (int i = 0; i < _size; i++)
            std::swap(a[i], b.a[i]);
    }
    int size() const
    {
        return _size;
    }
    void clear()
    {
        _size = 0;
    }
    double &operator[](int x)
    {
        return a[x];
    }
    const double &operator[](int x) const
    {
        return a[x];
    }
    void push_back(double x)
    {
        a[_size++] = x;
    }
    void pop_back()
    {
        _size--;
    }
    Vector operator+(const Vector &b) const
    {
        Vector c(_size);
        for (int i = 0; i < _size; i++)
            c[i] = a[i] + b[i];
        return c;
    }

    Vector operator-(const Vector &b) const
    {
        Vector c(_size);
        for (int i = 0; i < _size; i++)
            c[i] = a[i] - b[i];
        return c;
    }

    friend Vector operator*(const double &a, const Vector &b)
    {
        Vector c(b.size());
        for (int i = 0; i < b.size(); i++)
            c[i] = a * b[i];
        return c;
    }

    friend Vector operator/(const Vector &a, const double &b)
    {
        Vector c(a.size());
        for (int i = 0; i < a.size(); i++)
            c[i] = a[i] / b;
        return c;
    }
};

inline Vector crossProduct(const Vector &a, const Vector &b)
{
    Vector c(3);
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
    return c;
}

inline double innerProduct(const Vector &a, const Vector &b)
{
    double c = 0;
    for (int i = 0; i < a.size(); i++)
        c += a[i] * b[i];
    return c;
}

inline double norm(const Vector &v)
{
    double t = 0;
    for (int i = 0; i < v.size(); i++)
        t += v[i] * v[i];
    return sqrt(t);
}


