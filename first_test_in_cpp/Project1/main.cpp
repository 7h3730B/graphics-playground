#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

#define WIDTH 800
#define HEIGHT 600
#define TITLE "Vulkan test"

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    // pointer to the GLFWwindow (gets initialised by glfwCreateWindow(...) in initWindow()
    GLFWwindow* window;
    // Pointer to the vulkan Instance
    VkInstance instance;

    void initVulkan() {
        createInstance();
    }

    void createInstance() {

        printAllExtensions();

        // Create a struct to give more information to vulkan (optional)
        VkApplicationInfo appInfo{};
        // Set the type of the struct
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Triangle Test";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Engineless :)";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // Tells the Vulkan Drive alot of necessary information
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        // Give the appInfo over to vulkan via the struct
        createInfo.pApplicationInfo = &appInfo;

        // Speciefies the global extensions needed (here glfw says Vulkan that extensions it needs)
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        // create the instants and check for a error with the type VkResult (VK_SUCCESS or any VK_ERROR)
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void printAllExtensions() {
        // get the count of all extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        
        // allocate a new array to store all extensions
        std::vector<VkExtensionProperties> extensions(extensionCount);

        // query the extension details
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        // print all extensions
        std::cout << "available extensions:\n";

        for (const auto& extension : extensions) {
            std::cout << '\t' << extension.extensionName << '\n';
        }
    }

    void initWindow() {
        // Init the glfw libary in order to create a window
        glfwInit();

        // tell glfw not to create an OpenGL context with a so called subsequent call and disable resizing the window
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // create the window and set the pointer in the private field window
        window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    }

    void mainLoop() {
        // Pools window events until the window is closed
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}