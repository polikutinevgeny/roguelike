#include "bsp_tree.hpp"
#include "libtcod.hpp"
#include <stack>

namespace rogue {

BSPTree::BSPTree(int x, int y, int w, int h) :
    left_(NULL), father_(NULL), right_(NULL), x(x), y(y), w(w), h(h), level(0) {
}

BSPTree::~BSPTree() {
    delete left_;
    delete right_;
}

void BSPTree::SplitOnce(bool horizontal, int position) {
    this->horizontal = horizontal;
    this->position = position;
    TCODRandom* rng = TCODRandom::getInstance();
    if (rng->getInt(1, 2) == 1) {
        left_ = new BSPTree(this, true);
        right_ = new BSPTree(this, false);
    }
    else {
        right_ = new BSPTree(this, true);
        left_ = new BSPTree(this, false);
    }
    left_->father_ = this;
    right_->father_ = this;
}

void BSPTree::SplitRecursive(int nb, int min_h_size, int min_v_size, double max_h_ratio, double max_v_ratio) {
    if (nb == 0 || (w < 2 * min_h_size && h < 2 * min_v_size)) {
        return;
    }
    bool horiz;
    TCODRandom* randomizer = TCODRandom::getInstance();
    if (h < 2 * min_v_size || w > h * max_h_ratio) {
        horiz = false;
    }
    else if (w < 2 * min_h_size || h > w * max_v_ratio) {
        horiz = true;
    }
    else {
        horiz = randomizer->getInt(0, 1) == 0;
    }
    int position;
    if (horiz) {
        position = randomizer->getInt(y + min_v_size, y + h - min_v_size);
    }
    else {
        position = randomizer->getInt(x + min_h_size, x + w - min_h_size);
    }
    SplitOnce(horiz, position);
    left_->SplitRecursive(nb - 1, min_h_size, min_v_size, max_h_ratio, max_v_ratio);
    right_->SplitRecursive(nb - 1, min_h_size, min_v_size, max_h_ratio, max_v_ratio);
}

bool BSPTree::IsLeaf() const {
    return !(left_ || right_);
}

void BSPTree::TraverseLevelOrder(BSPTreeCallbackInterface* callback) const {
    std::stack<const BSPTree*> stack;
    stack.push(this);
    while (!stack.empty()) {
        BSPTree const* node = stack.top();
        stack.pop();
        if (node->left_) 
            stack.push(node->left_);
        if (node->right_) 
            stack.push(node->right_);
        callback->VisitNode(node);
    }
}

BSPTree::BSPTree(BSPTree const* father, bool left) : left_(NULL), father_(NULL), right_(NULL) {
    if (father->horizontal) {
        x = father->x;
        w = father->w;
        y = left ? father->y : father->position;
        h = left ? father->position - y : father->y + father->h - father->position;
    }
    else {
        y = father->y;
        h = father->h;
        x = left ? father->x : father->position;
        w = left ? father->position - x : father->x + father->w - father->position;
    }
    level = father->level + 1;
}

}