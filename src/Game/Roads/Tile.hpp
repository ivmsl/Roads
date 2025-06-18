#ifndef TILE_HPP_
#define TILE_HPP_

enum class TileType {
    EMPTY,
    ROAD,
    ZONE,
    INTERSECTION,
    OBJECT,
    OTHER
};

enum class RoadDirection { HORIZONTAL, HORISONTAL_MIN, VERTICAL, VERTICAL_MIN, DIAGONAL, CIRCULAR};
enum class RoadMaterial { ASPHALT, GRAVEL, DIRT, CONCRETE };
enum class RoadClass { HATRED, SMALL, ALLEYWAY, HIGHWAY };

class IntersectionConnection {
    public:
    enum Type {
        START = 0 << 0,
        NORTH = 1 << 0,
        EAST = 1 << 1,
        SOUTH = 1 << 2,
        WEST = 1 << 3
    }; 
    
};

class Tile {
    
    protected:
        int x, y;
        TileType type;
    public:
        Tile(int x, int y, TileType type) : x(x), y(y), type(type) {}
        virtual ~Tile() = default;

        TileType GetType() const { return type; }
        virtual void Render() const = 0;
        virtual bool CanPlaceRoad() const { return false; }

        //Pointing-casing dla dziedzieczenia 
        template<typename T>
        T* As() { return dynamic_cast<T*>(this); }
};

class EmptyTile : public Tile {
    public:
        EmptyTile(int x, int y) : Tile(x, y, TileType::EMPTY) {}
        void Render() const override { } //override — zamienia virtualna funkcje bazowej klasy na rzecz
        bool CanPlaceRoad() const override { return true; } 
};


#endif
