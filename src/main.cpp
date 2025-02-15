#include <SFML/Graphics.hpp>
#include <chrono>

sf::Texture T_SPRITES;
constexpr std::array level = {
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
    2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 
    2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
    2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
    2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
    2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
    2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
    2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
    2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
    2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
    2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
};


class GameObject {
public:
    sf::Sprite sprite = sf::Sprite(T_SPRITES);
    sf::Vector2f position;
    int row;
    int frame = 0;
    sf::Clock animClock;

    void animation() {
        if (animClock.getElapsedTime().asMilliseconds() > 250) {
            frame = (frame + 1) % 4;
            sprite.setTextureRect(sf::IntRect({16 * frame, row*16}, {16, 16}));
            animClock.restart();
        }
    }
};


class Qubit : public GameObject {
public:
    bool state = false;
    bool isEntangled = false;
    Qubit* target = nullptr;

    Qubit(int _row, sf::Vector2f pos) {
        row = _row;
        position = pos;
        sprite.setTextureRect(sf::IntRect({0, row*16}, {16, 16}));
        sprite.setPosition({position.x * 64, position.y * 64});
        sprite.setScale({4.0f, 4.0f});
    }

    void init(int _row, sf::Vector2f pos) {
        row = _row;
        position = pos;
        sprite.setTextureRect(sf::IntRect({0, row*16}, {16, 16}));
        sprite.setPosition({position.x * 64, position.y * 64});
        sprite.setScale({4.0f, 4.0f});
    }

    void toggleState() {
        state = !state;
        row = state;
    }

    sf::Vector2f move(sf::Vector2f offset, const int* map) {
        if (position.x + offset.x < 0 || position.y + offset.y < 0)
            return {0, 0};
        if (position.x + offset.x >= 20 || position.y + offset.y >= 15)
            return {0, 0};
        sf::Vector2f newPos = sf::Vector2f(position.x + offset.x, position.y + offset.y);
        if (map[static_cast<int>(newPos.x + newPos.y * 20)] == 1) {
            return {0, 0};
        }
        if (map[static_cast<int>(newPos.x + newPos.y * 20)] == 5) {
            return newPos;
        }
        position = newPos;
        sprite.move({offset.x * 64, offset.y * 64});
        return newPos; 
    }

};


class XGate : public GameObject {
public:
    bool cooldown = false;
    XGate(int _row, sf::Vector2f pos) {
        row = _row;
        position = pos;
        sprite.setTextureRect(sf::IntRect({0, row * 16}, {16, 16}));
        sprite.setPosition({position.x * 64, position.y * 64});
        sprite.setScale({4.0f, 4.0f});
    }

    void applyEffect(Qubit& q) {
        if (cooldown)
            return;
        q.toggleState();
        cooldown = true;
    }
};


class Observer : public GameObject {
public:
    bool TargetState;
    Observer(int _row, sf::Vector2f pos, bool tar) {
        row = _row;
        TargetState = tar;
        position = pos;
        sprite.setTextureRect(sf::IntRect({2, row*16}, {16, 16}));
        sprite.setPosition({position.x * 64, position.y * 64});
        sprite.setScale({4.0f, 4.0f});
    }

    bool checkWin(Qubit &q) {
        return q.state == TargetState;
    }
};




class CNOT : public GameObject {
public:
    CNOT(int _row, sf::Vector2f pos) {
        row = _row;
        position = pos;
        sprite.setTextureRect(sf::IntRect({2, row*16}, {16, 16}));
        sprite.setPosition({position.x * 64, position.y * 64});
        sprite.setScale({4.0f, 4.0f});
    }
};

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool load(const std::filesystem::path& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        m_vertices.resize(width * height * 6);

        // populate the vertex array, with two triangles per tile
        for (unsigned int i = 0; i < width; ++i)
        {
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                const int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                const int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                const int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the triangles' vertices of the current tile
                sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

                // define the 6 corners of the two triangles
                triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                triangles[5].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);

                // define the 6 matching texture coordinates
                triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
                triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
                triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            }
        }

        return true;
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        // apply the transform
        states.transform *= getTransform();
        states.transform.scale({4.0f, 4.0f});

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture     m_tileset;
};

TileMap map;
int levelIdx = 0;
bool inAnimation = false;
float animationRadius = 0;


class Level {
public:
    std::vector<int> levelMap;
    std::vector<int> levelData;
    std::vector<XGate> xgates;
    Qubit qa = Qubit(0, {0, 0});
    Qubit qb = Qubit(0, {0, 0});
    Observer oba = Observer(0, {0, 0}, 1);
    Observer obb = Observer(0, {0, 0}, 0);
    bool successA = false;
    bool successB = false;

    void renderLevel(sf::RenderWindow& window) {
        map.load("E:/0_Files/0Documents/Coding/Gamejam/Qubit Tango/Qubit-Tango/assets/Tileset.png", {16, 16}, levelMap.data(), 20, 15);

        qa.animation();
        qb.animation();
        oba.animation();
        obb.animation();
        for (auto& x : xgates)
            x.animation();

        window.clear();
        window.draw(map);
        window.draw(oba.sprite);
        window.draw(obb.sprite);
        for (auto& x : xgates)
            window.draw(x.sprite);
        if (!successA) window.draw(qa.sprite);
        if (!successB) window.draw(qb.sprite);
        window.draw(qb.sprite);
    }

    void checkWin() {
        if (successA && successB) {
            inAnimation = true;
        }
    }

};

void update(const sf::Event::KeyPressed* keyPressed, Level& level) {
    sf::Vector2f newPos = {0, 0};
    if (keyPressed->scancode == sf::Keyboard::Scancode::W)
        newPos = level.qa.move({0, -1}, level.levelData.data());
    else if (keyPressed->scancode == sf::Keyboard::Scancode::S)
        newPos = level.qa.move({0, 1}, level.levelData.data());
    else if (keyPressed->scancode == sf::Keyboard::Scancode::A)
        newPos = level.qa.move({-1, 0}, level.levelData.data());
    else if (keyPressed->scancode == sf::Keyboard::Scancode::D)
        newPos = level.qa.move({1, 0}, level.levelData.data());
    else if (keyPressed->scancode == sf::Keyboard::Scancode::R)
        level.qa.toggleState();
    for (auto &x : level.xgates) {
        if (x.position == newPos)
            x.applyEffect(level.qa);
        else {
            x.cooldown = false;
        }
    } 
    if (newPos == level.oba.position)
        level.successA = level.oba.checkWin(level.qa);
    newPos = {0, 0};
    if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
        newPos = level.qb.move({0, -1}, level.levelData.data());
    else if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
        newPos = level.qb.move({0, 1}, level.levelData.data());
    else if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
        newPos = level.qb.move({-1, 0}, level.levelData.data());
    else if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
        newPos = level.qb.move({1, 0}, level.levelData.data());
    for (auto &x : level.xgates) {
        if (x.position == newPos)
            x.applyEffect(level.qb);
    } 
    if (newPos == level.obb.position)
        level.successB = level.obb.checkWin(level.qb);
    
    level.checkWin();
}

Level level1 = {
    {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 
        2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
        2, 2, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 2, 2, 
        2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
        2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
    },
    {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 
        2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
        2, 2, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 2, 2, 2, 
        2, 2, 2, 1, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 
        2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
    },  // 9 for xgate, 5 for observer
    {XGate(4, sf::Vector2f(9, 8))},
    Qubit(0, {4, 7}),
    Qubit(0, {-5, -5}),
    Observer(5, {15, 7}, 1),
    Observer(5, {-5, -5}, 0),
    false, true
};

Level level2 = {

};

Level levelWin = {
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {},
    Qubit(0, {10, 7}),
    Qubit(1, {11, 7}),
    Observer(10, {0, 0}, 0),
    Observer(10, {0, 0}, 0),
    false, false
};

Level levelList[] = {level1, levelWin};


int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1280u, 960u}), "Qubit Tango");
    window.setFramerateLimit(60);

    T_SPRITES.loadFromFile("E:/0_Files/0Documents/Coding/Gamejam/Qubit Tango/Qubit-Tango/assets/Sprites.png");


    while (window.isOpen())
    {        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                update(keyPressed, levelList[levelIdx]);
            }
        }
        if (inAnimation) {
            animationRadius += 50;
            sf::CircleShape circle;
            circle.setFillColor(sf::Color::Black);
            circle.setPosition({levelList[levelIdx].oba.position.x*64+32-animationRadius, levelList[levelIdx].oba.position.y*64+32-animationRadius});
            circle.setRadius(animationRadius);
            window.draw(circle);
            if (animationRadius > 1280) {
                inAnimation = false;
                levelIdx++;
            }
        }
        else {
            levelList[levelIdx].renderLevel(window);
        }
        window.display();
    }
}
