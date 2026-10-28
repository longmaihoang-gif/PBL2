#pragma once
#include <string>

// =================================================================
//  ENTITY: Lop thuc the co so truu tuong
// =================================================================
class Entity {
        std::string Name;
    public:
        Entity(const std::string& name = "") : Name(name) {}
        virtual ~Entity() {}

        // Ham thuan ao: Bat buoc moi thuc the phai cung cap chuoi thong tin tong quan
        virtual std::string GetInfo() const = 0;
};