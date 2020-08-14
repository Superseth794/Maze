//
//  LabelNode.inl
//  Maze
//
//  Created by Jaraxus on 14/08/2020.
//

namespace mz {

inline sf::Vector2f LabelNode::findCharacterPos(std::size_t index) const {
    return m_text.findCharacterPos(index);
}

inline unsigned int LabelNode::getCharacterSize() const {
    return m_text.getCharacterSize();
}

inline sf::Color const& LabelNode::getFillColor() const {
    return m_text.getFillColor();
}

inline sf::Font const* LabelNode::getFont() const {
    return m_text.getFont();
}

inline sf::FloatRect LabelNode::getGlobalBounds() const {
    return m_text.getGlobalBounds();
}

inline sf::FloatRect LabelNode::getLocalBounds() const {
    return m_text.getGlobalBounds();
}

inline float LabelNode::getLetterSpacing() const {
    return m_text.getLetterSpacing();
}

inline float LabelNode::getLineSpacing() const {
    return m_text.getLineSpacing();
}

inline sf::String const& LabelNode::getString() const {
    return m_text.getString();
}

inline sf::Uint32 LabelNode::getStyle() const {
    return m_text.getStyle();
}

inline sf::Color const& LabelNode::getOutlineColor() const {
    return m_text.getOutlineColor();
}

inline float LabelNode::getOutlineThickness() const {
    return m_text.getOutlineThickness();
}

inline void LabelNode::setCharacterSize(unsigned int size) {
    m_text.setCharacterSize(size);
}

inline void LabelNode::setFillColor(sf::Color const& color) {
    m_text.setFillColor(color);
}

inline void LabelNode::setFont(sf::Font const& font) {
    m_text.setFont(font);
}

inline void LabelNode::setLetterSpacing(float spacingFactor) {
    m_text.setLetterSpacing(spacingFactor);
}

inline void LabelNode::setLineSpacing(float spacingFactor) {
    m_text.setLineSpacing(spacingFactor);
}

inline void LabelNode::setOutlineColor(sf::Color const& color) {
    m_text.setOutlineColor(color);
}

inline void LabelNode::setOutlineThickness(float thickness) {
    m_text.setOutlineThickness(thickness);
}

inline void LabelNode::setString(std::string const& string) {
    m_text.setString(static_cast<sf::String>(string));
}

inline void LabelNode::setStyle(std::uint32_t style) {
    m_text.setStyle(static_cast<sf::Uint32>(style));
}

} // namespace mz
