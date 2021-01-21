//
//  AbstractShapeNode.inl
//  Maze
//
//  Created by Jaraxus on 11/08/2020.
//

namespace mz {

template <class ShapeNodeT>
void AbstractShapeNode<ShapeNodeT>::draw(Camera const& camera) {
    camera.draw(this, m_shape, getGlobalTransform());
    Node::draw(camera);
}

template <class ShapeNodeT>
inline sf::Color const& AbstractShapeNode<ShapeNodeT>::getFillColor() const {
    return m_shape.getFillColor();
}

template <class ShapeNodeT>
inline sf::FloatRect AbstractShapeNode<ShapeNodeT>::getGlobalBounds() const {
    return getGlobalTransform().getTransform().transformRect(getLocalBounds());
}

template <class ShapeNodeT>
inline sf::FloatRect AbstractShapeNode<ShapeNodeT>::getLocalBounds() const {
    return m_shape.getLocalBounds();
}

template <class ShapeNodeT>
inline sf::Color const& AbstractShapeNode<ShapeNodeT>::getOutlineColor() const {
    return m_shape.getOutlineColor();
}

template <class ShapeNodeT>
inline float AbstractShapeNode<ShapeNodeT>::getOutlineThickness() const {
    return m_shape.getOutlineThickness();
}

template <class ShapeNodeT>
inline std::shared_ptr<const sf::Texture> AbstractShapeNode<ShapeNodeT>::getTexture() const {
    return *m_texture;
}

template <class ShapeNodeT>
inline sf::IntRect const& AbstractShapeNode<ShapeNodeT>::getTextureRect() const {
    return m_shape.getTextureRect();
}

template <class ShapeNodeT>
inline ShapeNodeT& AbstractShapeNode<ShapeNodeT>::setFillColor(const sf::Color& color) {
    m_shape.setFillColor(color);
    return *static_cast<ShapeNodeT*>(this);
}

template <class ShapeNodeT>
inline ShapeNodeT& AbstractShapeNode<ShapeNodeT>::setOutlineColor(const sf::Color& color) {
    m_shape.setOutlineColor(color);
    return *static_cast<ShapeNodeT*>(this);
}

template <class ShapeNodeT>
inline ShapeNodeT& AbstractShapeNode<ShapeNodeT>::setOutlineThickness(float thickness) {
    m_shape.setOutlineThickness(thickness);
    return *static_cast<ShapeNodeT*>(this);
}

template <class ShapeNodeT>
inline ShapeNodeT& AbstractShapeNode<ShapeNodeT>::setTexture(std::shared_ptr<sf::Texture> const& texture, bool resetRect) {
    m_texture = texture;
    m_shape.setTexture(*m_texture, resetRect);
    return *static_cast<ShapeNodeT*>(this);
}

template <class ShapeNodeT>
inline ShapeNodeT& AbstractShapeNode<ShapeNodeT>::setTextureRect(const sf::IntRect& rect) {
    m_shape.setTextureRect(rect);
    return *static_cast<ShapeNodeT*>(this);
}

template <class ShapeNodeT>
template <typename ...Args>
AbstractShapeNode<ShapeNodeT>::AbstractShapeNode(std::shared_ptr<sf::Texture> && texture, Args && ...args) :
m_shape(std::forward<Args>(args)...),
m_texture(std::forward<std::shared_ptr<sf::Texture>>(texture))
{
}

}
