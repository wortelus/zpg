// Creator: Daniel Slavík
// E-Mail: sla0331@vsb.cz
// Date of Creation:  22/10/2023

#include "scene_loader.h"
#include "../../assets/sphere.h"
#include "../../assets/tree.h"
#include "../../assets/suzi_flat.h"
#include "../../assets/suzi_smooth.h"
#include "../../assets/gift.h"
#include "../../assets/plain.h"

std::unique_ptr<Scene> SceneLoader::loadScene(int* scene_id, GLFWwindow &window_reference, const int &initial_width,
                            const int &initial_height) {
    switch (*scene_id) {
        case 0:
            return loadSceneA(window_reference, initial_width, initial_height);
        case 1:
            return loadSceneB(window_reference, initial_width, initial_height);
        case 2:
            return loadSceneC(window_reference, initial_width, initial_height);
        default:
            *scene_id = 0;
            return loadSceneA(window_reference, initial_width, initial_height);
    }

}

std::unique_ptr<Scene> SceneLoader::loadSceneA(GLFWwindow &window_reference, const int &initial_width, const int &initial_height) {
    const float height = -2;

    std::unique_ptr<Scene> scene = std::make_unique<Scene>(window_reference, initial_width, initial_height);
    auto sphere_south_object = scene->newObject(sphere, sizeof(sphere),
                                         glm::vec3(0.f, height, -2.f), "lambert");
    sphere_south_object->setColor(glm::vec3(0.385, 0.647, 0.812));
    sphere_south_object->setProperties(glm::vec3(0.385, 0.647, 0.812),
                                       glm::vec3(1.0, 1.0, 1.0),
                                       1.f);

    auto sphere_east_object = scene->newObject(sphere, sizeof(sphere),
                                        glm::vec3(-2.f, height, 0.f), "phong");
    sphere_east_object->setColor(glm::vec3(0.385, 0.647, 0.812));
    sphere_east_object->setProperties(glm::vec3(0.385, 0.647, 0.812),
                                      glm::vec3(1.0, 1.0, 1.0),
                                      0.32f);

    auto sphere_north_object = scene->newObject(sphere, sizeof(sphere),
                                         glm::vec3(0.f, height, 2.f), "blinn");
    sphere_north_object->setColor(glm::vec3(0.385, 0.647, 0.812));
    sphere_north_object->setProperties(glm::vec3(0.385, 0.647, 0.812),
                                       glm::vec3(1.0, 1.0, 1.0),
                                       0.32f);

    auto sphere_west_object = scene->newObject(sphere, sizeof(sphere),
                                        glm::vec3(2.f, height, 0.f), "constant");
    sphere_west_object->setColor(glm::vec3(0.385, 0.647, 0.812));

    std::unique_ptr<PhongLight> phong_light = std::make_unique<PhongLight>(glm::vec3(0.f, height, 0.f),
                                                                           glm::vec3(1.f, 1.f, 1.f),
                                                                           3.f,
                                                                           1.f, 1.f, 1.f);
    scene->appendLight(std::move(phong_light));

//    std::unique_ptr<Light> lambert_light = std::make_unique<Light>(glm::vec3(0.f, 0.f, 0.f),
//                                                                   glm::vec3(0.f, 1.f, 0.f),
//                                                                   1.f);
//    scene->newLight(std::move(lambert_light));

        return std::move(scene);
}

std::unique_ptr<Scene> SceneLoader::loadSceneB(GLFWwindow &window_reference, const int &initial_width, const int &initial_height) {
    std::unique_ptr<Scene> scene = std::make_unique<Scene>(window_reference, initial_width, initial_height);
    auto sphere_south_object = scene->newObject(sphere, sizeof(sphere),
                                                glm::vec3(0.f, 0.f, -2.f), "phong");
    sphere_south_object->setColor(glm::vec3(1.0, 1.0, 1.0));
    sphere_south_object->setAmbient(glm::vec3(1.0, 1.0, 0.0));
    sphere_south_object->setProperties(glm::vec3(1.0, 1.0, 0.0),
                                       glm::vec3(1.0, 1.0, 1.0),
                                       0.52f);

    std::unique_ptr<PhongLight> phong_light = std::make_unique<PhongLight>(glm::vec3(0.f, 0.f, -4.f),
                                                                           glm::vec3(1.f, 1.f, 1.f),
                                                                           3.f,
                                                                           1.f, 1.f, 1.f);

    scene->appendLight(std::move(phong_light));

    return std::move(scene);
}


std::unique_ptr<Scene> SceneLoader::loadSceneC(GLFWwindow &window_reference, const int &initial_width, const int &initial_height) {
    std::unique_ptr<Scene> scene = std::make_unique<Scene>(window_reference, initial_width, initial_height);
    auto sphere_obj = scene->newObject(sphere, sizeof(sphere),
                                       glm::vec3(-1.f, 1.f, -1.f), "phong");
    sphere_obj->setColor(glm::vec3(1.0, 1.0, 1.0));
    sphere_obj->setAmbient(glm::vec3(1.0, 1.0, 0.0));
    sphere_obj->setProperties(glm::vec3(1.0, 0.75, 0.0),
                              glm::vec3(1.0, 1.0, 1.0),
                              0.12f);

    auto tree_obj = scene->newObject(tree, sizeof(tree),
                                     glm::vec3(2.f, 0.f, 0.f), "phong");
    tree_obj->setColor(glm::vec3(1.0, 1.0, 1.0));
    tree_obj->setProperties(glm::vec3(1.0, 1.0, 0.0),
                              glm::vec3(1.0, 1.0, 1.0),
                              0.12f);


    auto suzi_flat_obj = scene->newObject(suziFlat, sizeof(suziFlat),
                                             glm::vec3(0.f, 0.f, -8.f), "phong");

    auto suzi_smooth_obj = scene->newObject(suziSmooth, sizeof(suziSmooth),
                                         glm::vec3(-2.f, 0.f, -8.f), "phong");

    auto plain_obj = scene->newObject(plain, sizeof(plain),
                                         glm::vec3(0.f, 0.f, 0.f), "lambert");
    plain_obj->setColor(glm::vec3(0.0, 0.1, 0.0));
    plain_obj->setProperties(glm::vec3(0.0, 0.0, 0.0),
                             glm::vec3(0.0, 1.0, 0.75),
                             0.1f);
    plain_obj->scale(glm::vec3(24.f, 1.f, 24.f));

    // lights
    glm::vec3 sun_position = glm::vec3(-4.f, 5.f, -4.f);
    auto sun_obj = scene->newObject(sphere, sizeof(sphere),
                                    sun_position, "constant");
    sun_obj->setColor(glm::vec3(1.0, 0.5, 0.0));
    sun_obj->setAmbient(glm::vec3(1.0, 1.0, 1.0));

    std::unique_ptr<PhongLight> phong_light = std::make_unique<PhongLight>(sun_position,
                                                                           glm::vec3(1.f, 1.f, 1.f),
                                                                           12.f,
                                                                           1.f, 1.f, 1.f);
    scene->appendLight(std::move(phong_light));

    return std::move(scene);
}