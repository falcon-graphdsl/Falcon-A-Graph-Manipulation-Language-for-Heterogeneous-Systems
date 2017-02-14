
#include "Galois/config.h"
#include "Galois/Galois.h"
#include "Galois/Accumulator.h"
#include "Galois/Bag.h"
#include "Galois/Statistic.h"
#include "Galois/Timer.h"
#include "Galois/UnionFind.h"
#include "Galois/Graph/LCGraph.h"
#include "Galois/Graph/TypeTraits.h"
#include "Galois/ParallelSTL/ParallelSTL.h"
#include "llvm/Support/CommandLine.h"
#include "Lonestar/BoilerPlate.h"

#include <iostream>
#include <deque>
#include <set>


namespace cll = llvm::cl;

static const char* name = "Single Source Shortest Path";
static const char* desc =
  "Computes the shortest path from a source node to all nodes in a directed "
  "graph using a modified chaotic iteration algorithm";
static const char* url = "single_source_shortest_path";
  typedef boost::counting_iterator<uint64_t> iterator;
static cll::opt<std::string> filename(cll::Positional, cll::desc("<input graph>"), cll::Required);
static cll::opt<std::string> transposeGraphName("graphTranspose", cll::desc("Transpose of input graph"));
static cll::opt<bool> symmetricGraph("symmetricGraph", cll::desc("Input graph is symmetric"));
static cll::opt<unsigned int> startNode("startNode", cll::desc("Node to start search from"), cll::init(0));//source noe
static cll::opt<unsigned int> reportNode("reportNode", cll::desc("Node to report distance to"), cll::init(1));//node whos dist printed
static cll::opt<int> stepShift("delta", cll::desc("Shift value for the deltastep"), cll::init(10));//delta 1024
cll::opt<unsigned int> memoryLimit("memoryLimit",
    cll::desc("Memory limit for out-of-core algorithms (in MB)"), cll::init(~0U));
static const bool trackWork = true;
static Galois::Statistic* BadWork;
static Galois::Statistic* WLEmptyWork;
