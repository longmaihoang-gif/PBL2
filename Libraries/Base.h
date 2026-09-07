#pragma once
#include <string>
#include <vector>

using namespace std;

// =================================================================
// 🏷️ ENTITY: Lớp thực thể cơ sở
// =================================================================
class Entity {
    public:
        string Name;
        vector<string> Tags;

        // Destructor ảo: Đảm bảo an toàn giải phóng bộ nhớ khi kế thừa
        virtual ~Entity() {}
};