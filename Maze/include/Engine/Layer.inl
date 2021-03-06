//
//  Layer.inl
//  Maze
//
//  Created by Jaraxus on 03/08/2020.
//

namespace mz {

template <class LayerT>
LayerT& Layer::addChild(std::unique_ptr<LayerT> && layer) {
    m_toAddChilds.emplace_back(std::forward<std::unique_ptr<LayerT>>(layer));
    return static_cast<LayerT&>(*m_toAddChilds.back());
}

Layer& Layer::getChild(std::size_t childId) {
    return *m_childs[childId];
}

std::size_t Layer::getChildsCount() const {
    return m_childs.size();
}

sf::Transformable const& Layer::getGlobalTransform() const {
    return m_globalTransform;
}

std::size_t const& Layer::getIdInParentHierarchy() const {
    assert(m_parent);
    return m_idInParentHierarchy;
}

sf::Transformable const& Layer::getRelativeTransform() const {
    return m_relativeTransform;
}

Layer* Layer::getParent() {
    return m_parent;
}

void Layer::removeAllChildrens() {
    for (std::size_t i = 0; i < m_childs.size(); ++i)
        m_toRemoveChilds.push_back(i);
}

void Layer::removeChild(std::size_t childId) {
    m_toRemoveChilds.push_back(childId);
}

void Layer::removeChild(Layer & layer) {
    m_toRemoveChilds.push_back(layer.m_idInParentHierarchy);
}

void Layer::removeFromParent() {
    assert(m_parent);
    m_parent->removeChild(m_idInParentHierarchy);
}

}
