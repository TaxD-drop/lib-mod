#include <jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <thread>
#include <chrono>
#include "modmenu.h"
#include "hooking.h"
#include "mod_interface.h"
#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"

#define LOG_TAG "libmod"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Exemplo de função original que queremos hookar
int (*orig_puts)(const char*) = nullptr;
int hooked_puts(const char* s) {
    LOGI("puts hookado: %s", s);
    return orig_puts(s);
}

// Thread do menu
void menuThread() {
    // Aguarda surface (em uma aplicação real usaria callbacks JNI)
    // Inicializa ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(1080, 1920);

    ImGui_ImplAndroid_Init(nullptr); // ANativeWindow* seria passado aqui
    ImGui_ImplOpenGL3_Init("#version 100");

    while (true) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplAndroid_NewFrame();
        ImGui::NewFrame();

        ModMenu::get().render();

        ImGui::Render();
        // ... desenho na tela (exige EGL context)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

// Construtor que roda automaticamente quando a lib é carregada
__attribute__((constructor))
void lib_main() {
    LOGI("libmod carregada!");

    // Inicia o menu em uma thread separada
    std::thread(menuThread).detach();

    // Instala hooks de exemplo
    Hooking::installHook((void*)&puts, (void*)&hooked_puts, (void**)&orig_puts);

    // Exemplo: carregar mods dinâmicos de /sdcard/mods/
    // (código omitido por brevidade)
}

// JNI_OnLoad – necessário para apps Android que carregam a lib diretamente
extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad chamado");
    lib_main();
    return JNI_VERSION_1_6;
}

// Se compilando para PC (teste)
#ifdef DESKTOP_TEST
int main() {
    lib_main();
    while(true) {} // loop simples
    return 0;
}
#endif