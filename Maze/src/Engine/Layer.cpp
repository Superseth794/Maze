//
//  Layer.cpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#include "../../include/Engine/Layer.hpp"

namespace mz {

Layer& Layer::addChild(Layer && layer) {
    m_toAddChilds.emplace_back(std::make_unique<Layer>(std::forward<Layer>(layer)));
    return *m_toAddChilds.back();
}

void Layer::draw(sf::RenderTarget& target) {
    for (auto & child : m_childs)
        child->draw(target);
}

std::unique_ptr<Layer> Layer::extractChild(std::size_t childId) {
    if (childId != m_childs.size() - 1) {
        m_childs.back()->m_idInParentHierarchy = childId;
        std::swap(m_childs[childId], m_childs.back());
    }
    auto layerPtr {std::move(m_childs.back())};
    m_childs.pop_back();
    return layerPtr;
}

std::unique_ptr<Layer> Layer::extractChild(Layer & layer) {
    return extractChild(layer.m_idInParentHierarchy);
}

std::unique_ptr<Layer> Layer::extractFromParent() {
    assert(m_parent);
    return m_parent->extractChild(m_idInParentHierarchy);
}

void Layer::update(std::uint64_t timeElapsed) {
    clearRemoveBuffer();
    clearAddBuffer();
    
    m_relativeTransform = Transformable::getTransform();
    if (m_parent)
        m_globalTransform = m_parent->m_globalTransform.combine(m_relativeTransform);
    else
        m_globalTransform = m_relativeTransform;
    
    for (auto & child : m_childs)
        child->update(timeElapsed);
}

void Layer::clearAddBuffer() {
    for (auto & toAddChild : m_toAddChilds) {
        m_childs.emplace_back(std::move(toAddChild));
        m_childs.back()->m_parent = this;
        m_childs.back()->m_idInParentHierarchy = m_childs.size() - 1;
    }
    m_toAddChilds.clear();
}

void Layer::clearRemoveBuffer() {
    std::sort(m_toRemoveChilds.begin(), m_toRemoveChilds.end());
    
    std::size_t prevRemovedChildId = static_cast<std::size_t>(-1);
    for (std::size_t toRemoveId : m_toRemoveChilds) {
        if (toRemoveId == prevRemovedChildId)
            continue;
        stableRemoveChild(toRemoveId);
        prevRemovedChildId = toRemoveId;
    }
    
    m_toRemoveChilds.clear();
}

void Layer::stableRemoveChild(std::size_t childId) {
    assert(childId < m_childs.size());
    if (childId != m_childs.size() - 1) {
        m_childs.back()->m_idInParentHierarchy = childId;
        std::swap(m_childs[childId], m_childs.back());
    }
    m_childs.back()->m_parent = nullptr;
    m_childs.pop_back();
}

}
