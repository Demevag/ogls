#pragma once

#include "utils.h"

namespace dmvg::engine {

class Scene
{
public:
    virtual util::error init() = 0;
    void add_object(std::shared_ptr<Object> obj);
    void redraw();

private:
    std::list<std::shared_ptr<Object>> m_objects;
};

} // namespace dmvg::engine
