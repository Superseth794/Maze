//
//  Node.hpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#ifndef Node_hpp
#define Node_hpp

# include <SFML/Graphics.hpp>

# include "Layer.hpp"

namespace mz {

class Node : public Layer {
public:
    Node() = default;
    Node(Node const& node) = delete;
    Node(Node && node) = default; // TODO: make it move-assignable
    
    virtual ~Node() = default;
    
    virtual void display(sf::RenderTexture& texture) override;
    
private:
    float               m_height;
    sf::RenderTexture   m_texture;
    float               m_width;
};

}

#endif /* Node_hpp */
