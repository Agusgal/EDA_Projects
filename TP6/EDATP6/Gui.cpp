#include "Gui.h"

Gui::Gui() {
    
    ev = {0};
    closeWindow = false;

    runningMain = true;

    display = NULL;
    queue = NULL;

    flipTimer = NULL;
    fps = 120;
    
    
    displaySizeX = 500;
    displaySizeY = 200;

    guiWindowSizeX = 500;

    selectedDisplay = 0;

}

int Gui::showMainWindow(void)
{
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    display = al_create_display(displaySizeX, displaySizeY);
    if (!display) {

        fprintf(stderr, "Failed to create display !\n");
        return -1;
    }

    al_set_window_title(display, "Display Selector");
    flipTimer = al_create_timer(1 / fps);

    if (configureEvents() == -1) {
        return -1;
    }

    if (configureImGui() == -1) {
        return -1;
    }

    al_start_timer(flipTimer);

    while (runningMain) {

        while (al_get_next_event(queue, &ev)) {

            ImGui_ImplAllegro5_ProcessEvent(&ev);	// Mandar el evento a Dear ImGui para que lo procese

            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                runningMain = false;
                return -1;
            }
            if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            {
                ImGui_ImplAllegro5_InvalidateDeviceObjects();
                al_acknowledge_resize(display);
                ImGui_ImplAllegro5_CreateDeviceObjects();
            }

            if (ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == flipTimer) {

                ImGui_ImplAllegro5_NewFrame();
                ImGui::NewFrame();
                ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);


                if (mainWindow()) {
                    runningMain = false;
                }

                ImGui::Render();

                al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));

                //Todo lo que dibuje aca va a quedar por detr�s de las ventanas de DearImGui

                ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

                //Todo lo que dibuje aca va a quedar por encima de las ventanas de DearImGui

                al_flip_display();

            }
        }
    }

    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();

    al_destroy_event_queue(queue);
    al_destroy_display(display);
    runningMain = true;
    return selectedDisplay;
}


int Gui::configureEvents(void)
{
    queue = al_create_event_queue();

    // Controla que la cola de eventos se haya generado
    if (!queue)
    {
        fprintf(stderr, "Failed to create event queue !\n");
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    al_register_event_source(queue, al_get_timer_event_source(flipTimer));

    return 0;
}


int Gui::configureImGui(void)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAllegro5_Init(display);
    ImGui::StyleColorsLight();

    return 0;
}

int Gui::mainWindow(void)
{
    static bool NoTitlebar = false;
    static bool NoMenu = true;
    static bool NoCollapse = false;
    static bool NoResize = false;
    static bool NoMove = false;
    static bool NoClose = true;
    static bool NoBackground = false;
    static bool NoScrollbar = true;
    static bool no_bring_to_front = false;

    ImGuiWindowFlags window_flags = 0;
    if (NoTitlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (NoScrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!NoMenu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (NoMove)            window_flags |= ImGuiWindowFlags_NoMove;
    if (NoResize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (NoCollapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (NoBackground)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::SetNextWindowSize(ImVec2(guiWindowSizeX, 300), ImGuiCond_Always); //Aca pongo tama�o de la pantalla
    ImGui::Begin("Configure Simulation2", NULL, window_flags);

    ImGui::RadioButton("Display a", &selectedDisplay, 1); ImGui::SameLine();
    
    ImGui::RadioButton("Display b", &selectedDisplay, 2); ImGui::SameLine();
   
    ImGui::RadioButton("Display c", &selectedDisplay, 3);
    
    if (ImGui::Button("Show Display")) 
    {
        ImGui::End();
        return selectedDisplay;
    }

    ImGui::End();
    return 0;
}