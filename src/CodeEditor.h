#ifndef POMEGRANATE_ENGINE_CODEEDITOR_H
#define POMEGRANATE_ENGINE_CODEEDITOR_H
#include <ImGui/imgui.h>
#include<Pomegranate/vec_math.h>
#include<vector>
using namespace Pomegranate;

enum BlockOptions
{
    BLOCK_OPTION_NONE = 1 << 0,
    BLOCK_OPTION_TOP_NUB = 1 << 1,
    BLOCK_OPTION_BOTTOM_NUB = 1 << 2,
    BLOCK_OPTION_WRAP = 1 << 3,
    BLOCK_OPTION_ROUND = 1 << 4,
    BLOCK_OPTION_ANGLED = 1 << 5,
};

class Block
{
public:
    Vec2 pos;
    Vec2 size;
    ImColor color;
    int options;
    std::string text;
    std::vector<Block> children;
    Block(Vec2 pos, Vec2 size, ImColor color, int options);
};

class CodeEditor
{
public:
    Vec2 pos = Vec2(0,0);
    float zoom = 1.0;
    CodeEditor();
    void render();
    Vec2 screen_to_world(Vec2 pos);
    Vec2 world_to_screen(Vec2 pos);
    Vec2 screen_size_to_world(Vec2 size);
    Vec2 world_size_to_screen(Vec2 size);
    void draw_block(Block block);
    void draw_panel(Vec2 pos, Vec2 size,ImColor color);
};


#endif //POMEGRANATE_ENGINE_CODEEDITOR_H
