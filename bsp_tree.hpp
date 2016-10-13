#pragma once
#include <cstddef>

namespace rogue {

class BSPTree;

class BSPTreeCallbackInterface {
public:
    virtual void VisitNode(BSPTree* node) = 0;
};

class BSPTree {
public:
    BSPTree(int x, int y, int w, int h);

    ~BSPTree();

    void SplitRecursive(int nb, int minHSize, int minVSize, double maxHRatio, double maxVRatio);

    bool IsLeaf() const;

    bool TraverseLevelOrder(BSPTreeCallbackInterface* callback);

    int x, y, w, h;
    int position;
    bool horizontal;
    int level;

protected:
    BSPTree(BSPTree *father, bool left);

    void AddSon(BSPTree *data);

    void RemoveSons();
    void SplitOnce(bool horizontal, int position);

    BSPTree* GetLeft() const;
    BSPTree* GetRight() const;

    BSPTree* next_;
    BSPTree* father_;
    BSPTree* sons_;
};

}