# Progressive TIN Densification

Standalone C++ implementation of the Progressive TIN Densification (PTD) algorithm for ground classification in LiDAR point clouds. 

PTD is a well-established, physically motivated method for ground extraction that incrementally builds a terrain model using a Triangulated Irregular Network (TIN). See *Axelsson, P. (2000). DEM Generation from Laser Scanner Data Using adaptive TIN Models. International Archives of Photogrammetry and Remote Sensing, 33(B4), 110–117. https://www.isprs.org/proceedings/xxxiii/congress/part4/111_xxxiii-part4.pdf*

For a ready to use API in R see the [lidR]() and [lasR]() R packages. For a ready to use API in python see the  [pylasr]() package.

### Compilation

```
mkdir build
cd build       
cmake ..
cmake --build .
```

### Usage

```cpp
#include "ptd/PTD.h"

// Register parameters
PTD::Parameters params;
params.max_iteration_angle = 30;
params.max_iteration_distance = 2;
params.spacing = 0.25;
params.seed_resolution_search = 10;
params.verbose = false;

// Register the bounding box of the points
PTD::Bbox bb;
bb.xmin = /* xmin */;
bb.ymin = /* ymin */;
bb.zmin = /* zmin */;
bb.xmax = /* xmax */;
bb.ymax = /* ymax */;
bb.zmax = /* zmax */;

// Optional: register a logger
PTD::Logger logger = [](const std::string& msg) { 
  printf("%s\n", msg.c_str()); 
};

try
{
  PTD::PTD ptd(params, bb);
  ptd.set_logger(logger);
  
  for (unsigned int i = 0 ; i < npoints ; i++)
  {
    // Optionnal: skip noise point or any irrelevant points
    // if (is_noise[i]) continue;
    
    // insert x, y, z and fid of the points (fits with any memory layout)
    // Note: fid is not mandatory to be the index of the point in an array.
    ptd.insert_point(x[i], y[i], z[i], i);
  }
  
  ptd.run();
  
  // return the fid of the ground points
  std::vector<unsigned int> gnd = ptd.get_ground_fid();
}
catch(std::exception& e)
{
  ...
}
```

### License

- [Incremental Delaunay](https://github.com/hporro/IncrementalDelaunay) (`src/hporro/`) by Heinich Porro. Licence MIT. Heavily modified by Jean-Romain Roussel.
- [Progressive TIN densification] (`src/ptd`) by Jean-Romain Roussel. Licence MIT.
- [Nanoflann](https://github.com/jlblancoc/nanoflann): Blanco, Jose Luis and Rai, Pranjal Kumar (2014). nanoflann: a C++ header-only fork of FLANN, a library for Nearest Neighbor (NN) with KD-trees}. License BSD.