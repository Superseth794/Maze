//
//  Drawable.inl
//  Maze
//
//  Created by Jaraxus on 04/09/2020.
//

namespace mz {

inline bool Drawable::isHidden() const {
    return m_isHidden;
}

inline Drawable& Drawable::setHidden(bool isHidden) {
    m_isHidden = isHidden;
    return *this;
}

}
