#pragma once
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>

template<typename T>
class Iterator
{
private:
    ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<T>>* hierarchy_;
    ds::amt::MultiWayExplicitHierarchyBlock<T>* current_;

public:
    Iterator(ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<T>>* hierarchy)
        : hierarchy_(hierarchy), current_(hierarchy->accessRoot()) {
    }

    const T& getCurrent() const {
        return current_->data_;
    }

    bool goToParent() {
        auto parent = hierarchy_->accessParent(*current_);
        if (parent) {
            current_ = parent;
            return true;
        }
        return false;
    }

    bool goToSon(size_t index) {
        auto son = hierarchy_->accessSon(*current_, index);
        if (son) {
            current_ = son;
            return true;
        }
        return false;
    }

    void printCurrentPath() const {
        ds::amt::ImplicitSequence<std::string> path2;
        auto node = current_;


        while (node != nullptr) {
			path2.insertLast().data_ = node->data_->getName();
            node = hierarchy_->accessParent(*node);
        }

        std::cout << "Cesta: ";

        for (size_t i = path2.size(); i > 0 ; i--)
        {
            std::cout << path2.access(i-1)->data_ << " -> ";
        }
        std::cout << "[TU]" << std::endl;
    }

    void printSons() const {
        size_t index = 0;
        size_t deg = hierarchy_->degree(*current_);


        std::cout << "Synovia (" << deg << "):" << std::endl;
        while (index < deg) {
            auto son = hierarchy_->accessSon(*current_, index);
            if (son) {
                std::cout << "[" << index << "] " << son->data_->getName() << std::endl;
            }
            index++;
        }
    }

    ds::amt::MultiWayExplicitHierarchyBlock<T>* getCurrentBlock() const {
        return current_;
    }


};

