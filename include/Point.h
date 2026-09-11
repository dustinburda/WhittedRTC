//
// Created by Dustin Burda on 10/5/24.
//

#ifndef WHITTED_POINT_H
#define WHITTED_POINT_H

#include "../include/Point.h"
#include "../include/Vector.h"

#include <string>
#include <sstream>

template<typename T, size_t N>
class Point {
public:
    Point() { std::memset(data_.data(), 0, N * sizeof (T)); }

    Point(const std::initializer_list<T>& data) {
        if(data.size() != N) {
            std::string error_msg = "Must initialize " + std::to_string(N) +
                                    "-tuple with " + std::to_string(N) + " elements, not "
                                    + std::to_string(data.size()) + " elements";
            throw std::logic_error(error_msg);
        }

        // Pad with zeroes
        std::memset(data_.data(), 0, N * sizeof (T));

        for(size_t index = 0; const T& element : data) {
            data_[index] = element;
            index++;
        }
    }

    Point(const Vector<T, N - 1>& v, T val) {
        std::memcpy(data_.data(), v.data_.data(), (N - 1) * sizeof(T));
        data_[N - 1] = val;
    }

    T& operator[](size_t i) {
        if(i >= N)
            throw std::logic_error("Index must be within bounds!");

        return data_[i];
    }

    T operator[](size_t i) const {
        if(i >= N)
            throw std::logic_error("Index must be within bounds!");

        return data_[i];
    }

    Point<T, N> operator-() const {
        Point<T, N> negative;

        for(std::size_t i = 0; i < N; i++)
            negative[i] = -data_[i];

        return negative;
    }

    Vector<T, N> HomogenousPointToCartesianVector() const {
        Vector<T, N> v;

        for(std::size_t i = 0; i < N - 1; i++)
            v[i] = data_[i];

        return v;
    }

    Vector<T, N> ToVector() const {
        Vector<T, N> v;

        for(std::size_t i = 0; i < N; i++)
            v[i] = data_[i];

        return v;
    }

    std::string toString() const {
        std::stringstream ss;

        ss << "Point: [";
        for(std::size_t i = 0; i < N; i++)
        {
            ss << " " <<  data_[i];
            if (i != N - 1)
                ss << ", ";
        }
        ss << "]";

        return ss.str();
    }

private:
    std::array<T, N> data_;
};

template<typename T, std::size_t N>
static Point<T, N> operator+(const Point<T,N>& p, const Vector<T,N>& v) {
    Point<T,N> translate;

    for(std::size_t i = 0; i < N; i++){
        translate[i] = p[i] + v[i];
    }

    return translate;
}

template<typename T, size_t N>
static Point<T, N> operator-(const Point<T,N>& p, const Vector<T,N>& v) {
    return p + (-v);
}

template<typename T, size_t N>
static Vector<T, N> operator-(const Point<T,N>& p1, const Point<T,N>& p2) {
    Vector<T,N> difference;

    for(std::size_t i = 0; i < N; i++){
        difference[i] = p1[i] -  p2[i];
    }

    return difference;
}

template<typename T, size_t N>
static Point<double, N> operator*(const Point<T,N>& p, double t) {
    Point<T,N> v;

    for(std::size_t i = 0; i < N; i++){
        v[i] = p[i] * t;
    }

    return v;
}

template<typename T, size_t N>
static Point<double, N> operator*(double t, const Point<T,N>& p) {
    return p * t;
}

template<typename T, size_t N>
static Point<double, N> operator/(const Point<T,N>& p, double t) {
    return p * (1/t);
}

template<typename T, size_t N>
static double DistanceSquared(const Point<T,N>& p1, const Point<T,N>& p2) {
    double distance_squared = 0;

    for(std::size_t i = 0; i < N; i++){
        distance_squared += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }

    return distance_squared;
}

template<typename T, size_t N>
static double Distance(const Point<T,N>& p1, const Point<T,N>& p2) {
    auto distance_squared = DistanceSquared(p1, p2);
    return std::sqrt(distance_squared);
}

using Point3D = Point<double, 3>;
using Point2D = Point<double, 2>;






#endif //WHITTED_POINT_H
