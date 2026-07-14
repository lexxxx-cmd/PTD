// ======================================================================================
// PTD CLI — Progressive TIN Densification: PCD input → LAS output
// Usage: ptd_cli --input <pcd> --output <las> [options]
// ======================================================================================

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "LASWriter.h"
#include "PCDReader.h"
#include "ptd/PTD.h"

static void print_usage() {
    std::cout <<
        "Usage: ptd_cli --input <pcd> --output <las> [options]\n"
        "  --input   Input PCD file\n"
        "  --output  Output LAS file\n"
        "  --spacing Grid spacing, meters (default: 0.25)\n"
        "  --max_angle  Max iteration angle, degrees (default: 30)\n"
        "  --max_dist   Max iteration distance, meters (default: 2)\n"
        "  --seed_res   Seed resolution search, meters (default: 10)\n"
        "  --max_iter   Max iterations (default: 50)\n"
        "  --buffer     Buffer size, meters (default: 30)\n";
}

int main(int argc, char* argv[]) {
    std::string input, output;
    PTD::Parameters params;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--input") == 0 && i + 1 < argc)
            input = argv[++i];
        else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc)
            output = argv[++i];
        else if (strcmp(argv[i], "--spacing") == 0 && i + 1 < argc)
            params.spacing = atof(argv[++i]);
        else if (strcmp(argv[i], "--max_angle") == 0 && i + 1 < argc)
            params.max_iteration_angle = atof(argv[++i]);
        else if (strcmp(argv[i], "--max_dist") == 0 && i + 1 < argc)
            params.max_iteration_distance = atof(argv[++i]);
        else if (strcmp(argv[i], "--seed_res") == 0 && i + 1 < argc)
            params.seed_resolution_search = atof(argv[++i]);
        else if (strcmp(argv[i], "--max_iter") == 0 && i + 1 < argc)
            params.max_iter = atoi(argv[++i]);
        else if (strcmp(argv[i], "--buffer") == 0 && i + 1 < argc)
            params.buffer_size = atof(argv[++i]);
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(); return 0;
        }
    }

    if (input.empty() || output.empty()) { print_usage(); return 1; }

    // 1. Read PCD
    PCDData data;
    if (!read_pcd(input, data)) {
        std::cerr << "Failed to read PCD: " << input << std::endl;
        return 1;
    }
    std::cout << "Loaded " << data.points.size() << " points" << std::endl;

    // 2. Compute bounding box
    PTD::Bbox bb;
    bb.xmin = bb.xmax = data.points[0].x;
    bb.ymin = bb.ymax = data.points[0].y;
    bb.zmin = bb.zmax = data.points[0].z;
    for (const auto& p : data.points) {
        if (p.x < bb.xmin) bb.xmin = p.x; if (p.x > bb.xmax) bb.xmax = p.x;
        if (p.y < bb.ymin) bb.ymin = p.y; if (p.y > bb.ymax) bb.ymax = p.y;
        if (p.z < bb.zmin) bb.zmin = p.z; if (p.z > bb.zmax) bb.zmax = p.z;
    }

    // 3. Run PTD
    PTD::PTD ptd(params, bb);

    auto t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < data.points.size(); ++i)
        ptd.insert_point(data.points[i].x, data.points[i].y, data.points[i].z, (unsigned int)i);
    ptd.run();
    std::vector<unsigned int> ground_fid = ptd.get_ground_fid();
    auto t1 = std::chrono::high_resolution_clock::now();

    double time_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    // Convert FIDs to ground index vector
    std::vector<int> ground_idx;
    ground_idx.reserve(ground_fid.size());
    for (unsigned int fid : ground_fid) ground_idx.push_back((int)fid);

    std::cout << "Ground: " << ground_idx.size()
              << " / Non-ground: " << (data.points.size() - ground_idx.size())
              << "  Time: " << time_ms << " ms" << std::endl;

    // 4. Write LAS
    if (!write_las(output, data.points, ground_idx)) {
        std::cerr << "Failed to write LAS: " << output << std::endl;
        return 1;
    }

    // 5. Write runtime.json
    std::string rt_path = output;
    size_t ext = rt_path.rfind(".las");
    if (ext == std::string::npos) ext = rt_path.rfind(".LAS");
    if (ext != std::string::npos)
        rt_path.replace(ext, 4, "_runtime.json");
    else
        rt_path += "_runtime.json";

    std::ofstream rt(rt_path);
    rt << "{\"infer_time_ms\": " << time_ms << "}" << std::endl;
    rt.close();

    std::cout << "Done. Output: " << output << ", " << rt_path << std::endl;
    return 0;
}
