#pragma once

#include <imgui/imgui.h>


struct ScrollingBuffer {
    int maxSize;
    int offset;
    ImVector<ImVec2> data;
    ScrollingBuffer(int max_size = 2400) {
        maxSize = max_size;
        offset = 0;
        data.reserve(maxSize);
    }
    void AddPoint(float x, float y) {
        if (data.size() < maxSize)
            data.push_back(ImVec2(x, y));
        else {
            data[offset] = ImVec2(x, y);
            offset = (offset + 1) % maxSize;
        }
    }
    void Erase() {
        if (data.size() > 0) {
            data.shrink(0);
            offset = 0;
        }
    }
};