#ifndef TE_TILED_MAP_H
#define TE_TILED_MAP_H

#include "texture.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <memory>

namespace te
{
    struct BoundingBox;
    class Mesh;

    struct TMX {
        TMX(const std::string& path, const std::string& file);

        struct Meta {
            std::string path;
            std::string file;
            Meta(std::string p, std::string f);
        } meta;

        enum class Orientation {
            ORTHOGONAL
        } orientation;
        enum class RenderOrder {
            RIGHT_DOWN
        } renderorder;

        unsigned width;
        unsigned height;
        unsigned tilewidth;
        unsigned tileheight;
        unsigned nextobjectid;

        struct Tileset {
            std::string name;
            unsigned firstgid;
            unsigned tilewidth;
            unsigned tileheight;
            int spacing;
            int margin;
            std::string image;
            unsigned imagewidth;
            unsigned imageheight;

            struct Tileoffset {
                int x;
                int y;
            } tileoffset;

            struct Terrain {
                std::string name;
                int tile;
            };
            std::vector<Terrain> terrains;

            unsigned tilecount;

            struct Tile {
                unsigned id;

                struct ObjectGroup {

                    enum class Type {
                        OBJECTGROUP,
                        NONE
                    } type;
                    std::string name;
                    bool visible;
                    float opacity;
                    int offsetx;
                    int offsety;

                    struct Object {
                        unsigned id;
                        std::string name;
                        std::string type;
                        enum class Shape {
                            RECTANGLE
                        } shape;
                        int x;
                        int y;
                        unsigned width;
                        unsigned height;
                        float rotation;
                        bool visible;
                    };
                    std::vector<Object> objects;
                } objectGroup;

                struct Frame {
                    unsigned tileid;
                    unsigned duration;
                };
                std::vector<Frame> animation;

                std::vector<unsigned> terrain;
            };
            std::vector<Tile> tiles;
        };
        std::vector<Tileset> tilesets;

        struct Layer {
            enum class Type {
                TILELAYER,
                OBJECTGROUP
            } type;
            std::string name;
            int x;
            int y;
            unsigned width;
            unsigned height;
            bool visible;
            float opacity;
            int offsetx;
            int offsety;
            std::vector<unsigned> data;
            std::vector<Tileset::Tile::ObjectGroup::Object> objects;
        };
        std::vector<Layer> layers;
    };

    class TiledMap {
    public:
        TiledMap(const std::string& path, const std::string& file, const glm::mat4& projection, const glm::mat4& model);
        ~TiledMap();
        TiledMap(TiledMap&&);
        TiledMap& operator=(TiledMap&&);

        void draw(const glm::mat4& viewTransform = glm::mat4()) const;
        bool checkCollision(const BoundingBox&) const;
        std::vector<BoundingBox> getIntersections(const BoundingBox&) const;

    private:
        TiledMap(const TiledMap&) = delete;
        TiledMap& operator=(const TiledMap&) = delete;

        struct Layer {
            std::vector<std::shared_ptr<Mesh>> meshes;
        };

        int getTilesetTextureIndex(unsigned tileID) const;
        void destroy();

        TMX mTMX;
        GLuint mShaderProgram;
        glm::mat4 mModelMatrix;
        std::vector<Layer> mLayers;
    };
}

#endif
