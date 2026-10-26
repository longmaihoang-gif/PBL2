#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

// =================================================================
//  THƯ VIỆN ĐỒ HỌA DEAR IMGUI & GLFW
// =================================================================
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <GLFW/glfw3native.h>
#endif

// =================================================================
//  THƯ VIỆN DỰ ÁN PBL2
// =================================================================
#include "../Libraries/Base.h"
#include "../Libraries/Trade.h"
#include "../Libraries/Account.h"
#include "../Libraries/Item.h"
#include "../Libraries/Shop.h"
#include "../Libraries/HashTable.h"
#include "../Libraries/DLinkedList.h"
#include "../Libraries/DataManager.h"
#include "../Libraries/Cart.h"

using namespace std;

// =================================================================
//  HÀM MAIN (KHUNG TRỐNG KHỞI TẠO CỬA SỔ & VÒNG LẶP IMGUI)
// =================================================================
int main() {
    // 1. Khởi tạo GLFW & Tạo cửa sổ
    if (!glfwInit()) return 1;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "PBL2 - UI", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-Sync

    // 2. Khởi tạo ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Khởi tạo backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 3. Vong lap render chinh (Khung khoi tao giao dien ImGui)
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // -------------------------------------------------------------
        // TODO: Viết giao diện các Tab / Cửa sổ ở đây
        // -------------------------------------------------------------

        // Render ra màn hình
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 4. Giải phóng tài nguyên đồ họa
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
