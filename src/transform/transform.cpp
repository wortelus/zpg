// Creator: Daniel Slavík
// E-Mail: sla0331@vsb.cz
// Date of Creation:  2/10/2023

#include "transform.h"
#include "glm/ext/matrix_transform.hpp"

Translation::Translation(const glm::vec3& initial_translation) : translation(initial_translation) {
    this->matrix = glm::mat4(glm::translate(glm::mat4(1.0), this->translation));
}

const glm::mat4& Translation::getMatrix() {
    if (this->is_dirty) {
        this->matrix = glm::mat4(glm::translate(glm::mat4(1.0), this->translation));
        this->is_dirty = false;
    }
    return this->matrix;
}

void Translation::moveBy(const glm::vec3& offset) {
    this->translation += offset;
    this->is_dirty = true;
}

void Translation::setTranslation(const glm::vec3& new_translation) {
    this->translation = new_translation;
    this->is_dirty = true;
}

void Translation::update(const EventArgs& event_args) {
    const auto* uniform = static_cast<const EventPayload<glm::vec3>*>(&event_args);

    if (event_args.type == EventType::U_TRANSLATION) {
        this->moveBy(uniform->getPayload());
    } else if (event_args.type == EventType::S_TRANSLATION) {
        this->setTranslation(uniform->getPayload());
    }
}

Rotation::Rotation() : origin(glm::vec3(1.0f, 0.0f, 0.0f)), rotation(glm::vec3(0.0f, 0.0f, 0.0f)) {
    this->matrix = glm::mat4(glm::rotate(glm::mat4(1.0), .0f, this->origin));
}

Rotation::Rotation(const glm::vec3& initial_origin) : origin(initial_origin), rotation(glm::vec3(0.0f, 0.0f, 0.0f)) {
    this->matrix = glm::mat4(glm::rotate(glm::mat4(1.0), .0f, this->origin));
}

void Rotation::rotateBy(const glm::vec3& offset) {
    this->rotation += offset;
    this->is_dirty = true;
}

void Rotation::setRotation(const glm::vec3& new_rotation) {
    this->rotation.x = new_rotation.x;
    this->rotation.y = new_rotation.y;
    this->rotation.z = new_rotation.z;
    this->is_dirty = true;
}

const glm::mat4& Rotation::getMatrix() {
    if (this->is_dirty) {
        this->matrix = glm::mat4(
                glm::rotate(glm::mat4(1.0), glm::radians(this->rotation.x), glm::vec3(1.0, 0.0, 0.0)) *
                glm::rotate(glm::mat4(1.0), glm::radians(this->rotation.y), glm::vec3(0.0, 1.0, 0.0)) *
                glm::rotate(glm::mat4(1.0), glm::radians(this->rotation.z), glm::vec3(0.0, 0.0, 1.0)));
        this->is_dirty = false;
    }
    return this->matrix;
}

void Rotation::update(const EventArgs& event_args) {
    const auto* uniform = static_cast<const EventPayload<glm::vec3>*>(&event_args);
    if (event_args.type == EventType::U_ROTATION) {
        this->rotateBy(uniform->getPayload());
    } else if (event_args.type == EventType::S_ROTATION) {
        this->setRotation(uniform->getPayload());
    }
}

Scale::Scale(const glm::vec3& initial_scale) : scale(initial_scale) {
    this->matrix = glm::mat4(glm::scale(glm::mat4(1.0), this->scale));
}

void Scale::scaleBy(const glm::vec3& offset) {
    this->scale += offset;
    this->is_dirty = true;
}

void Scale::setScale(const glm::vec3& new_scale) {
    this->scale = new_scale;
    this->is_dirty = true;
}

const glm::mat4& Scale::getMatrix() {
    if (this->is_dirty) {
        matrix = glm::mat4(glm::scale(glm::mat4(1.0), this->scale));
        this->is_dirty = false;
    }
    return this->matrix;
}

void Scale::update(const EventArgs& event_args) {
    const auto* uniform = static_cast<const EventPayload<glm::vec3>*>(&event_args);
    if (event_args.type == EventType::U_SCALE) {
        this->scaleBy(uniform->getPayload());
    } else if (event_args.type == EventType::S_SCALE) {
        this->setScale(uniform->getPayload());
    }
}

RotationPoint::RotationPoint(const glm::vec3& rot_axis) : origin(glm::vec3(0.0f, 0.0f, 0.0f)), axis(rot_axis) {
    this->matrix = glm::mat4(glm::mat4(1.0));
}

RotationPoint::RotationPoint(const glm::vec3& rot_axis, const glm::vec3& initial_origin) : origin(initial_origin), axis(rot_axis) {
    this->matrix = glm::mat4(glm::mat4(1.0));
}

void RotationPoint::setOrigin(const glm::vec3& new_origin) {
    this->origin = new_origin;
    this->is_dirty = true;
}

void RotationPoint::setAxis(const glm::vec3 &new_axis) {
    this->axis = new_axis;
    this->is_dirty = true;
}

void RotationPoint::rotateBy(const float& offset) {
    this->angle += offset;
    if (this->angle > 360.0f) {
        this->angle -= static_cast<float>(static_cast<int>(this->angle / 360.0f)) * 360.0f;
    }
    this->is_dirty = true;
}

void RotationPoint::setRotation(const float& new_rotation) {
    this->angle = new_rotation;
    if (this->angle > 360.0f) {
        this->angle -= static_cast<float>(static_cast<int>(this->angle / 360.0f)) * 360.0f;
    }
    this->is_dirty = true;
}

void RotationPoint::update(const EventArgs &event_args) {
    switch (event_args.type) {
        case EventType::S_ROTATION_POINT_AXIS: {
            const auto* uniform = static_cast<const EventPayload<glm::vec3>*>(&event_args);
            this->setAxis(uniform->getPayload());
            break;
        } case EventType::S_ROTATION_POINT_ANGLE: {
            const auto* uniform = static_cast<const EventPayload<float>*>(&event_args);
            this->setRotation(uniform->getPayload());
            break;
        } case EventType::U_ROTATION_POINT_ANGLE: {
            const auto* uniform = static_cast<const EventPayload<float>*>(&event_args);
            this->rotateBy(uniform->getPayload());
            break;
        } case EventType::S_ROTATION_POINT_ORIGIN: {
            const auto* uniform = static_cast<const EventPayload<glm::vec3>*>(&event_args);
            this->setOrigin(uniform->getPayload());
            break;
        } default:
            break;
    }
}

const glm::mat4& RotationPoint::getMatrix() {
    if (this->is_dirty) {
        this->matrix = glm::mat4(
                glm::translate(glm::mat4(1.0), origin) *
                glm::rotate(glm::mat4(1.0), glm::radians(angle), axis) *
                glm::translate(glm::mat4(1.0), -origin));
        this->is_dirty = false;
    }
    return this->matrix;
}