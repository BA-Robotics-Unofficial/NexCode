#include "CodeEditor.h"

void line(Vec2 a, Vec2 b, ImColor color)
{
    a += {ImGui::GetWindowPos().x,ImGui::GetWindowPos().y};
    b += {ImGui::GetWindowPos().x,ImGui::GetWindowPos().y};
    //Draw line using imgui
    ImGui::GetWindowDrawList()->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), color, 1.0f);
}
void bezier(Vec2 a, Vec2 b, Vec2 c, ImColor color)
{
    float t = 0.0;
    Vec2 lst = a;
    while(t <= 1.0)
    {
        Vec2 p = a * (1.0 - t) * (1.0 - t) + b * 2.0 * (1.0 - t) * t + c * t * t;
        line(lst, p, color);
        lst = p;
        t += 0.01;
    }
}


CodeEditor::CodeEditor()
{

}

void CodeEditor::render()
{
    ImGui::Begin("Code Editor");
    Block block = Block(Vec2(32,32),Vec2(128,32),ImColor(255,0,0),BLOCK_OPTION_ANGLED);
    block.text = "Test Var";
    draw_block(block);
    ImGui::End();
}

Vec2 CodeEditor::screen_to_world(Vec2 pos)
{
    //Convert position to world space
    pos = (pos - this->pos) / this->zoom;
    return pos;
}

Vec2 CodeEditor::world_to_screen(Vec2 pos)
{
    //Convert position to screen space
    pos = (pos * this->zoom) + this->pos;
    return pos;
}

Vec2 CodeEditor::screen_size_to_world(Vec2 size)
{
    //Convert size to world space
    size = size / this->zoom;
    return size;
}
Vec2 CodeEditor::world_size_to_screen(Vec2 size)
{
    //Convert size to screen space
    size = size * this->zoom;
    return size;
}

void CodeEditor::draw_panel(Vec2 pos, Vec2 size, ImColor color)
{
    //Convert position to screen space
    pos = world_to_screen(pos);
    size = world_size_to_screen(size);
    //Draw smooth Bézier curves for corners with radius 10 pixels edges will be drawn as basic straight lines
    float radius = 6.0;
    bezier(pos + Vec2(radius, 0), pos, pos + Vec2(0, radius),color);
    bezier(pos + Vec2(size.x - radius, 0), pos + Vec2(size.x, 0), pos + Vec2(size.x, radius),color);
    bezier(pos + Vec2(size.x, size.y - radius), pos + size, pos + Vec2(size.x - radius, size.y),color);
    bezier(pos + Vec2(radius, size.y), pos + Vec2(0, size.y), pos + Vec2(0, size.y - radius),color);
    line(pos + Vec2(radius, 0), pos + Vec2(size.x - radius, 0),color);
    line(pos + Vec2(size.x, radius), pos + Vec2(size.x, size.y - radius),color);
    line(pos + Vec2(size.x - radius, size.y), pos + Vec2(radius, size.y),color);
    line(pos + Vec2(0, size.y - radius), pos + Vec2(0, radius),color);
}

void CodeEditor::draw_block(Block block)
{
    //Convert position to screen space
    Vec2 pos = world_to_screen(block.pos);
    Vec2 size = world_size_to_screen(block.size);
    //Draw smooth Bézier curves for corners with radius 10 pixels edges will be drawn as basic straight lines
    float radius = 4.0;
    if(block.options & BLOCK_OPTION_ROUND)
    {
        radius = size.y / 2;
    }
    if(block.options & BLOCK_OPTION_ANGLED)
    {
        //Draw triangle at the ends
        radius = size.y / 2;
        line(pos + Vec2(radius, 0), pos + Vec2(0, radius),block.color);
        line(pos + Vec2(size.x - radius, 0), pos + Vec2(size.x, radius),block.color);
        line(pos + Vec2(size.x, size.y - radius), pos + Vec2(size.x - radius, size.y),block.color);
        line(pos + Vec2(radius, size.y), pos + Vec2(0, size.y - radius),block.color);
    }
    else
    {
        bezier(pos + Vec2(radius, 0), pos, pos + Vec2(0, radius), block.color);
        bezier(pos + Vec2(size.x - radius, 0), pos + Vec2(size.x, 0), pos + Vec2(size.x, radius), block.color);
        bezier(pos + Vec2(size.x, size.y - radius), pos + size, pos + Vec2(size.x - radius, size.y), block.color);
        bezier(pos + Vec2(radius, size.y), pos + Vec2(0, size.y), pos + Vec2(0, size.y - radius), block.color);
    }
    line(pos + Vec2(size.x, radius), pos + Vec2(size.x, size.y - radius),block.color);
    line(pos + Vec2(0, size.y - radius), pos + Vec2(0, radius),block.color);

    if(block.options & BLOCK_OPTION_BOTTOM_NUB)
    {
        line(pos + Vec2(radius, size.y), pos + Vec2(radius+8, size.y),block.color);
        bezier(pos + Vec2(radius+8, size.y), pos + Vec2(radius+8, size.y + 8), pos + Vec2(radius+16, size.y + 8),block.color);
        line(pos + Vec2(radius+16, size.y + 8), pos + Vec2(radius+40, size.y + 8),block.color);
        bezier(pos + Vec2(radius+40, size.y + 8), pos + Vec2(radius+48, size.y + 8), pos + Vec2(radius+48, size.y),block.color);
        line(pos + Vec2(radius+48, size.y), pos + Vec2(size.x - radius, size.y),block.color);
    }
    else
    {
        line(pos + Vec2(size.x - radius, size.y), pos + Vec2(radius, size.y),block.color);
    }

    if(block.options & BLOCK_OPTION_TOP_NUB)
    {
        //Top nub fits bottom nub
        line(pos + Vec2(radius, 0), pos + Vec2(radius+8, 0),block.color);
        bezier(pos + Vec2(radius+8, 0), pos + Vec2(radius+8, 8), pos + Vec2(radius+16, 8),block.color);
        line(pos + Vec2(radius+16, 8), pos + Vec2(radius+40, 8),block.color);
        bezier(pos + Vec2(radius+40, 8), pos + Vec2(radius+48, 8), pos + Vec2(radius+48, 0),block.color);
        line(pos + Vec2(radius+48, 0), pos + Vec2(size.x - radius, 0),block.color);
    }
    else
    {
        line(pos + Vec2(size.x - radius, 0), pos + Vec2(radius, 0),block.color);
    }

    //Draw text centered in the block
    ImGui::SetCursorScreenPos(ImVec2(pos.x + size.x / 2 - ImGui::CalcTextSize(block.text.c_str()).x / 2, pos.y + size.y / 2 - ImGui::CalcTextSize(block.text.c_str()).y / 2));
    ImGui::Text(block.text.c_str());
}

Block::Block(Vec2 pos, Vec2 size, ImColor color, int options)
{
    this->pos = pos;
    this->size = size;
    this->color = color;
    this->options = options;
}
