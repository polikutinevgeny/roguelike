#pragma once
#include <cstddef>

namespace rogue {

class BSPTree;

class BSPTreeCallbackInterface {
public:
    virtual void VisitNode(BSPTree const* node) = 0;
};

class BSPTree {
public:
    BSPTree(int x, int y, int w, int h);

    ~BSPTree();

    void SplitRecursive(int nb, int minHSize, int minVSize, double maxHRatio, double maxVRatio);

    bool IsLeaf() const;

    void TraverseLevelOrder(BSPTreeCallbackInterface& callback) const;

    int x, y, w, h;

private:
    BSPTree(BSPTree const* father, bool left);

    void SplitOnce(bool horizontal, int position);

    BSPTree* left_;
    BSPTree* right_;
    BSPTree* father_;

    int position_;
    bool horizontal_;
    int level_;
};

}