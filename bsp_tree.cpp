#include "bsp_tree.hpp"
#include "libtcod.hpp"
#include <stack>

namespace rogue {

BSPTree::BSPTree(int x, int y, int w, int h) :
    next_(NULL), father_(NULL), sons_(NULL), x(x), y(y), w(w), h(h), level(0) {
}

BSPTree::~BSPTree() {
    RemoveSons();
}

void BSPTree::AddSon(BSPTree* data) {
    data->father_ = this;
    BSPTree* last_son = sons_;
    while (last_son && last_son->next_)
        last_son = last_son->next_;
    if (last_son) {
        last_son->next_ = data;
    }
    else {
        sons_ = data;
    }
}

void BSPTree::RemoveSons() {
    BSPTree* node = sons_;
    while (node) {
        BSPTree* next_node = node->next_;
        node->RemoveSons();
        delete node;
        node = next_node;
    }
    sons_ = NULL;
}

void BSPTree::SplitOnce(bool horizontal, int position) {
    this->horizontal = horizontal;
    this->position = position;
    AddSon(new BSPTree(this, true));
    AddSon(new BSPTree(this, false));
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
    GetLeft()->SplitRecursive(nb - 1, min_h_size, min_v_size, max_h_ratio, max_v_ratio);
    GetRight()->SplitRecursive(nb - 1, min_h_size, min_v_size, max_h_ratio, max_v_ratio);
}

BSPTree* BSPTree::GetLeft() const {
    return sons_;
}
BSPTree* BSPTree::GetRight() const {
    return sons_ ? sons_->next_ : NULL;
}

bool BSPTree::IsLeaf() const {
    return sons_ == NULL;
}

bool BSPTree::TraverseLevelOrder(BSPTreeCallbackInterface* callback) {
    std::stack<BSPTree*> stack;
    stack.push(this);
    while (!stack.empty()) {
        BSPTree *node = stack.top();
        stack.pop();
        if (node->GetLeft()) 
            stack.push(node->GetLeft());
        if (node->GetRight()) 
            stack.push(node->GetRight());
        callback->VisitNode(node);
    }
    return true;
}

BSPTree::BSPTree(BSPTree* father, bool left) : next_(NULL), father_(NULL), sons_(NULL) {
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