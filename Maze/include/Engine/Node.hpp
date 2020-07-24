//
//  Node.hpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#ifndef Node_hpp
#define Node_hpp

# include <memory>
# include <utility>

# include <SFML/Graphics.hpp>

# include "Layer.hpp"

namespace mz {

class Node { //} : public Layer {
public:
    Node() noexcept = default;
//    Node(Node const& node) noexcept = delete;
    Node(Node && node) noexcept = default;
    
    Node& operator=(Node && node) noexcept = default;
    
    virtual ~Node() = default;
    
//    virtual void display(sf::RenderTexture& texture) override;
    
private:
//    float               m_height;
    std::unique_ptr<sf::RenderTexture>   m_texture;
//    float               m_width;
};

}

#endif /* Node_hpp */
