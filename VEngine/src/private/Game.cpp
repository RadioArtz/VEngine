#include "Game.hpp"
#include "TestScene.hpp"
#include <Transform.hpp>
#include <GameObject.h>

void RenderEngineMenuBar();
void RenderSceneHierarchy();
void RenderImportDialog();
void RenderEngineUI();
void DrawHierarchyElement(VEngine::GameObject* go);

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
                if (!go->transform.GetParent()) 
                {
                    DrawHierarchyElement(go);
                }
            }
        }
    }
    ImGui::End();
}

float loc[3]{ 0,0,0 };
float rot[3]{ 0,0,0 };

void DrawHierarchyElement(VEngine::GameObject* go)
{
    ImGui::PushID(go);
    if (ImGui::TreeNode(go->objectName.c_str()))
    {
        VEngine::Transform* trans = &go->transform;
        loc[0] = trans->GetLocation().x;
        loc[1] = trans->GetLocation().y;
        loc[2] = trans->GetLocation().z;
        ImGui::DragFloat3("Location", (float*)&loc);
        trans->SetRelativeLocation(loc[0], loc[1], loc[2]);

        rot[0] = trans->GetRotation().x;
        rot[1] = trans->GetRotation().y;
        rot[2] = trans->GetRotation().z;
        ImGui::DragFloat3("Rotation", (float*)&rot);
        trans->SetRelativeRotation(rot[0], rot[1], rot[2]);


        for (VEngine::Component* comp : go->components)
        {
            if (ImGui::TreeNode(typeid(*comp).name()))
            {
                if (typeid(*comp) == typeid(VEngine::Component))
                {
                    // render stuff depending on component type
                }
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    for (VEngine::Transform* trans : go->transform.children)
    {
        ImGui::Indent(10.0f);
        DrawHierarchyElement(trans->owningGameObject);
    }

    ImGui::PopID();
};

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