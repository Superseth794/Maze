//
//  AbstractShapeNode.inl
//  Maze
//
//  Created by Jaraxus on 11/08/2020.
//

namespace mz {

template <class ShapeT>
void AbstractShapeNode<ShapeT>::draw(Camera const& camera) {
    camera.draw(m_shape, getGlobalTransform());
}

template <class ShapeT>
inline sf::Color const& AbstractShapeNode<ShapeT>::getFillColor() const {
    return m_shape.getFillColor();
}

template <class ShapeT>
inline sf::FloatRect AbstractShapeNode<ShapeT>::getGlobalBounds() const {
    return getGlobalTransform().getTransform().transformRect(getLocalBounds());
}

template <class ShapeT>
inline sf::FloatRect AbstractShapeNode<ShapeT>::getLocalBounds() const {
    return m_shape.getLocalBounds();
}

template <class ShapeT>
inline sf::Color const& AbstractShapeNode<ShapeT>::getOutlineColor() const {
    return m_shape.getOutlineColor();
}

template <class ShapeT>
inline float AbstractShapeNode<ShapeT>::getOutlineThickness() const {
    return m_shape.getOutlineThickness();
}

template <class ShapeT>
inline std::shared_ptr<const sf::Texture> AbstractShapeNode<ShapeT>::getTexture() const {
    return *m_texture;
}

template <class ShapeT>
inline sf::IntRect const& AbstractShapeNode<ShapeT>::AbstractShapeNode<ShapeT>::getTextureRect() const {
    return m_shape.getTextureRect();
}

template <class ShapeT>
inline void AbstractShapeNode<ShapeT>::setFillColor(const sf::Color& color) {
    m_shape.setFillColor(color);
}

template <class ShapeT>
inline void AbstractShapeNode<ShapeT>::setOutlineColor(const sf::Color& color) {
    m_shape.setOutlineColor(color);
}

template <class ShapeT>
inline void AbstractShapeNode<ShapeT>::setOutlineThickness(float thickness) {
    m_shape.setOutlineThickness(thickness);
}

template <class ShapeT>
inline void AbstractShapeNode<ShapeT>::setTexture(std::shared_ptr<sf::Texture> const& texture, bool resetRect) {
    m_texture = texture;
    m_shape.setTexture(*m_texture, resetRect);
}

template <class ShapeT>
inline void AbstractShapeNode<ShapeT>::setTextureRect(const sf::IntRect& rect) {
    m_shape.setTextureRect(rect);
}

template <class ShapeT>
AbstractShapeNode<ShapeT>::AbstractShapeNode(ShapeT && shape, std::shared_ptr<sf::Texture> && texture) :
m_shape(std::forward<ShapeT>(shape)),
m_texture(std::forward<std::shared_ptr<ShapeT>>(texture))
{
}

}
