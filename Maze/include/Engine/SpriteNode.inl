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
    return getGlobalTransform().getTransform().transformRect(m_sprite.getLocalBounds());
}

inline sf::FloatRect SpriteNode::getLocalBounds() const {
    return m_sprite.getLocalBounds();
}

inline std::shared_ptr<const sf::Texture> SpriteNode::getTexture() const {
    return m_texture;
}

inline sf::IntRect const& SpriteNode::getTextureRect() const {
    return m_sprite.getTextureRect();
}

inline SpriteNode& SpriteNode::setColor(sf::Color const& color) {
    m_sprite.setColor(color);
    return *this;
}

inline SpriteNode& SpriteNode::setTexture(std::shared_ptr<sf::Texture> const& texture, bool resetRect) {
    m_texture = texture;
    m_sprite.setTexture(*texture, resetRect);
    return *this;
}

inline SpriteNode& SpriteNode::setTextureRect(sf::IntRect const& rectangle) {
    m_sprite.setTextureRect(rectangle);
    return *this;
}

}
