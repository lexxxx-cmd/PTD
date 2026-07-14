// ======================================================================================
// PCDReader - Lightweight PCD file reader (ASCII + binary, no PCL dependency)
// ======================================================================================

#ifndef PCD_READER_H_
#define PCD_READER_H_

#include <string>
#include <vector>

struct Point3D {
    double x, y, z;
    Point3D() : x(0), y(0), z(0) {}
    Point3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

struct PCDData {
    std::vector<Point3D> points;
};

/// Read ASCII or binary PCD file. Z+ = up (identity mapping).
bool read_pcd(const std::string& fname, PCDData& data);

#endif
