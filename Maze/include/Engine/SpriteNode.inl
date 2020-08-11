//
//  SpriteNode.inl
//  Maze
//
//  Created by Jaraxus on 11/08/2020.
//

namespace mz {

inline const sf::Color& SpriteNode::getColor() const {
    return m_sprite.getColor();
}

inline sf::FloatRect SpriteNode::getGlobalBounds() const {
    return getGlobalTransform().getTransform().transformRect(getLocalBounds());
}

inline sf::FloatRect SpriteNode::getLocalBounds() const {
    return m_sprite.getLocalBounds();
}

inline std::shared_ptr<sf::RenderTexture> const& SpriteNode::getTexture() const {
    return m_texture;
}

inline sf::IntRect const& SpriteNode::getTextureRect() const {
    return m_sprite.getTextureRect();
}

inline void SpriteNode::setColor(const sf::Color& color) {
    m_sprite.setColor(color);
}

inline void SpriteNode::setTexture(const sf::Texture& texture, bool resetRect) {
    m_sprite.setTexture(texture, resetRect);
}

inline void SpriteNode::setTextureRect(const sf::IntRect& rectangle) {
    m_sprite.setTextureRect(rectangle);
}

}
