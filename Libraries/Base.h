#pragma once
#include <string>

// =================================================================
//  ENTITY: Lop thuc the co so truu tuong
// =================================================================
class Entity {
    public:
        std::string Name;

        Entity(const std::string& name = "") : Name(name) {}
        virtual ~Entity() {}

        std::string getName() const { return Name; }
        void setName(const std::string& name) { Name = name; }

        // Ham thuan ao: Bat buoc moi thuc the phai cung cap chuoi thong tin tong quan
        virtual std::string GetInfo() const = 0;
};