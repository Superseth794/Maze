//
//  AbstractShapeNode.inl
//  Maze
//
//  Created by Jaraxus on 11/08/2020.
//

namespace mz {

template <class ShapeNodeT, class ShapeT>
void AbstractShapeNode<ShapeNodeT, ShapeT>::draw(Camera const& camera) {
    camera.draw(m_shape, getGlobalTransform());
}

template <class ShapeNodeT, class ShapeT>
inline sf::Color const& AbstractShapeNode<ShapeNodeT, ShapeT>::getFillColor() const {
    return m_shape.getFillColor();
}

template <class ShapeNodeT, class ShapeT>
inline sf::FloatRect AbstractShapeNode<ShapeNodeT, ShapeT>::getGlobalBounds() const {
    return getGlobalTransform().getTransform().transformRect(getLocalBounds());
}

template <class ShapeNodeT, class ShapeT>
inline sf::FloatRect AbstractShapeNode<ShapeNodeT, ShapeT>::getLocalBounds() const {
    return m_shape.getLocalBounds();
}

template <class ShapeNodeT, class ShapeT>
inline sf::Color const& AbstractShapeNode<ShapeNodeT, ShapeT>::getOutlineColor() const {
    return m_shape.getOutlineColor();
}

template <class ShapeNodeT, class ShapeT>
inline float AbstractShapeNode<ShapeNodeT, ShapeT>::getOutlineThickness() const {
    return m_shape.getOutlineThickness();
}

template <class ShapeNodeT, class ShapeT>
inline std::shared_ptr<const sf::Texture> AbstractShapeNode<ShapeNodeT, ShapeT>::getTexture() const {
    return *m_texture;
}

template <class ShapeNodeT, class ShapeT>
inline sf::IntRect const& AbstractShapeNode<ShapeNodeT, ShapeT>::getTextureRect() const {
    return m_shape.getTextureRect();
}

template <class ShapeNodeT, class ShapeT>
inline void AbstractShapeNode<ShapeNodeT, ShapeT>::setFillColor(const sf::Color& color) {
    m_shape.setFillColor(color);
}

template <class ShapeNodeT, class ShapeT>
inline void AbstractShapeNode<ShapeNodeT, ShapeT>::setOutlineColor(const sf::Color& color) {
    m_shape.setOutlineColor(color);
}

template <class ShapeNodeT, class ShapeT>
inline void AbstractShapeNode<ShapeNodeT, ShapeT>::setOutlineThickness(float thickness) {
    m_shape.setOutlineThickness(thickness);
}

template <class ShapeNodeT, class ShapeT>
inline void AbstractShapeNode<ShapeNodeT, ShapeT>::setTexture(std::shared_ptr<sf::Texture> const& texture, bool resetRect) {
    m_texture = texture;
    m_shape.setTexture(*m_texture, resetRect);
}

template <class ShapeNodeT, class ShapeT>
inline void AbstractShapeNode<ShapeNodeT, ShapeT>::setTextureRect(const sf::IntRect& rect) {
    m_shape.setTextureRect(rect);
}

template <class ShapeNodeT, class ShapeT>
template <typename ...Args>
AbstractShapeNode<ShapeNodeT, ShapeT>::AbstractShapeNode(std::shared_ptr<sf::Texture> && texture, Args && ...args) :
m_shape(std::forward<Args>(args)...),
m_texture(std::forward<std::shared_ptr<sf::Texture>>(texture))
{
}

}
