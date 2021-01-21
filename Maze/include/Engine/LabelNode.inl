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

inline LabelNode& LabelNode::setCharacterSize(unsigned int size) {
    m_text.setCharacterSize(size);
    return *this;
}

inline LabelNode& LabelNode::setFillColor(sf::Color const& color) {
    m_text.setFillColor(color);
    return *this;
}

inline LabelNode& LabelNode::setFont(sf::Font const& font) {
    m_text.setFont(font);
    return *this;
}

inline LabelNode& LabelNode::setLetterSpacing(float spacingFactor) {
    m_text.setLetterSpacing(spacingFactor);
    return *this;
}

inline LabelNode& LabelNode::setLineSpacing(float spacingFactor) {
    m_text.setLineSpacing(spacingFactor);
    return *this;
}

inline LabelNode& LabelNode::setOutlineColor(sf::Color const& color) {
    m_text.setOutlineColor(color);
    return *this;
}

inline LabelNode& LabelNode::setOutlineThickness(float thickness) {
    m_text.setOutlineThickness(thickness);
    return *this;
}

inline LabelNode& LabelNode::setString(std::string const& string) {
    m_text.setString(static_cast<sf::String>(string));
    return *this;
}

inline LabelNode& LabelNode::setStyle(std::uint32_t style) {
    m_text.setStyle(static_cast<sf::Uint32>(style));
    return *this;
}

} // namespace mz
