#include "Game.hpp"
#include "TestScene.hpp"

void RenderEngineMenuBar();
void RenderSceneHierarchy();
void RenderImportDialog();
void RenderEngineUI();

GLFWwindow* GLWindow;


int VEngine::Game::EngineInit()
{
    std::string exeLocation = FilesystemExtensions::GetExeLocation();
    std::filesystem::path programRoot = std::filesystem::path(exeLocation).parent_path();
    std::filesystem::current_path(programRoot);

    glfwInit();
    EngineWindow = new VgineWindow();
    GLWindow = EngineWindow->GetInternalGLFWwindow();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_IsSRGB;

    ImGui_ImplGlfw_InitForOpenGL(GLWindow, true);
    ImGui_ImplOpenGL3_Init();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    Awesomescene = new TestScene{ "Test Scene" };
    Awesomescene->EngineWindow = EngineWindow;
    Awesomescene->OnBeginPlay();

    while (!glfwWindowShouldClose(GLWindow))
    {
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        glfwPollEvents();
        glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderEngineUI();

        Awesomescene->OnUpdate(deltaTime);
        Awesomescene->OnRender(deltaTime);

        glfwSwapBuffers(GLWindow);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    std::cout << "Closed Engine Window!" << std::endl;
    shouldExit = true;
    return 0;
}

bool ImportDialogOpen = false;

void RenderEngineUI() 
{
    RenderEngineMenuBar();
    RenderSceneHierarchy();

    if (ImportDialogOpen)
        RenderImportDialog();
}

void RenderEngineMenuBar() 
{
    if (ImGui::BeginMainMenuBar()) 
    {
        if (ImGui::BeginMenu("File")) 
        {
            if (ImGui::MenuItem("Import Asset")) { ImportDialogOpen =! ImportDialogOpen; }
            if (ImGui::MenuItem("Exit Engine")) { glfwSetWindowShouldClose(GLWindow, true); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Create")) 
        {
            if (ImGui::BeginMenu("3D")) 
            {
                if (ImGui::MenuItem("GameObject")) 
                {
                    VEngine::GameObject * newObj = new VEngine::GameObject("GameObject");
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

float loc[3]{ 0,0,0 };
void RenderSceneHierarchy()
{
    ImGui::Begin("Scene Hierarchy");
    ImGui::SetWindowSize(ImVec2(300, 500));
    for (VEngine::Scene* scn : VEngine::Scene::activeScenes)
    {
        if (ImGui::CollapsingHeader(scn->sceneName.c_str()))
        {
            for (VEngine::GameObject* go : scn->gameObjects)
            {
                ImGui::PushID(go);
                if (ImGui::TreeNode(go->objectName.c_str()))
                {
                    for (VEngine::Component* comp : go->components)
                    {
                        if (ImGui::TreeNode(typeid(*comp).name()))
                        {
                            if (typeid(*comp) == typeid(VEngine::TransformComponent))
                            {
                                VEngine::TransformComponent* transComp = (VEngine::TransformComponent*)comp;
                                loc[0] = transComp->transform.GetLocation().x;
                                loc[1] = transComp->transform.GetLocation().y;
                                loc[2] = transComp->transform.GetLocation().z;
                                ImGui::DragFloat3("Location", (float*)&loc);
                                transComp->transform.SetLocation(loc[0], loc[1], loc[2]);
                            }
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                ImGui::PopID();
            }
        }
    }
    ImGui::End();
}

void RenderImportDialog()
{
    bool pressed = false;
    ImGui::Begin("Import Model");
    ImGui::SetWindowSize(ImVec2(250, 400));
    std::string input = "C:/Users/mathi/Downloads/viking_room.obj";
    if (ImGui::Button("Import"))
    {
        pressed = true;
    }
    ImGui::SameLine();
    ImGui::InputText("Model Path", &input);
    if (pressed)
        VEngine::Game::Get().loader.loadModel(input);

    ImGui::End();
};