/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#include "Entity.hpp"

#include "Component.hpp"

namespace ngine {
    void Entity::_addToScene(Scene *scene_) {
        if (m_parentScene != nullptr)
            throw std::runtime_error("Cannot add entity to more than one scene. Remove from current scene first.");

        // Set the parent scene
        m_parentScene = scene_;

        // TODO: Check scene is physics enabled too!!!
        // Create physics body if physics is enabled
        if (m_physicsEnabled) {
            // Create body
            m_physicsBody = new physics::PhysicsBody(m_parentScene->getPhysicsWorld());

            // Send initial position and rotation to body (TODO: body center system too!)
            m_physicsBody->setTransform(m_transform);
        }

        // Add to scene
        m_parentScene->_addEntity(this);
    }

    void Entity::_removeFromScene() {
        // Unbind update event
        unsubscribeFromUpdate();

        // Delete physics body
        if (m_physicsEnabled) {
            // Delete body
            delete m_physicsBody;
            m_physicsBody = nullptr;
        }

        // Remove from scene (and therefore unset entity parent too)
        m_parentEntity = nullptr;
        m_parentScene = nullptr;
    }

    void Entity::_doDestroy() {
        // Fire event
        OnDestroy();

        // Delete components
        for (const auto &c : m_components)
            delete c.second;
        m_components.clear();

        // Destroy physics body
        if (m_physicsBody != nullptr) m_physicsBody->destroy();

        // Detach all events
        OnTransformChanged.clear();
        OnUpdate.clear();

        // Unsubscribe from update
        unsubscribeFromUpdate();

        // Make our parents are null
        m_parentEntity = nullptr;
        m_parentScene = nullptr;
    }

    // TODO: Remove position and rotation from this constructor???
    Entity::Entity(const Vector2 position_, float rotation_, int depth_, bool canCull_, bool physicsEnabled_)
            : m_canCull(canCull_), m_depth(depth_), m_transform(position_, rotation_),
              m_physicsEnabled(physicsEnabled_),
              EntityContainer(EntityContainer::ENTITY) {}

    Entity::~Entity() {
        // Remove from parent (if not already)
        if (m_parentEntity != nullptr || m_parentScene != nullptr)
            getContainer()->removeChild(this);

        // Destroy everything (being thorough).
        _doDestroy();
    }

    void Entity::destroy() {
        if (getContainer() != nullptr) {
            // We have a container, so we detach from it
            getContainer()->removeChild(this);
        } else {
            // We don't have a container, so we we do this ourselves
            _doDestroy();
        }

        // Delete ourselves
        delete this;
    }

    bool Entity::removeComponent(const std::string &name_) {
        auto comp = getComponent<Component>(name_);

        if (comp != nullptr) {
            // Remove component from map
            m_components.erase(name_);

            // Delete if we should
            delete comp;

            return true;
        }

        // We don't have this component
        return false;
    }

    std::vector<Component *> Entity::getComponents() {
        std::vector<Component *> vec;

        for (auto &_Component : m_components) {
            vec.push_back(_Component.second);
        }

        return vec;
    }

    bool Entity::hasComponent(const std::string &name_) const {
        return m_components.find(name_) != m_components.end();
    }

    bool Entity::canCull() const {
        return m_canCull;
    }

    void Entity::setCanCull(bool canCull_) {
        m_canCull = canCull_;
    }

    bool Entity::checkForCulling(Rectangle cullArea_) {
        return !cullArea_.contains(getPosition());
    }

    EntityContainer *Entity::getContainer() const {
        if (m_parentEntity != nullptr)
            return static_cast<EntityContainer *>(m_parentEntity);
        else if (m_parentScene != nullptr)
            return static_cast<EntityContainer *>(m_parentScene);
        return nullptr;
    }

    Game *Entity::getGame() const {
        if (m_parentScene == nullptr) return nullptr;
        return m_parentScene->getGame();
    }

    filesystem::ResourceManager *Entity::getResourceManager() const {
        if (m_parentScene == nullptr) return nullptr;
        return m_parentScene->getResourceManager();
    }

    int Entity::getDepth() const {
        return m_depth;
    }

    void Entity::setDepth(int depth_) {
        if (m_parentScene != nullptr)
            m_parentScene->_updateEntityDepth(depth_, this);
        m_depth = depth_;
    }

    Transform2D Entity::getTransform() const {
        if (m_physicsEnabled) {
            return m_physicsBody->getTransform();
        } else {
            return m_transform;
        }
    }

    void Entity::setTransform(const Transform2D &transform_) {
        if (m_physicsEnabled) {
            m_physicsBody->setTransform(transform_);
        } else {
            m_transform = transform_;
        }
    }

    Vector2 Entity::getPosition() const {
        if (m_physicsEnabled) {
            return m_physicsBody->getPosition();
        } else {
            return m_transform.Position;
        }
    }

    void Entity::setPosition(Vector2 position_) {
        if (m_physicsEnabled) {
            m_physicsBody->setPosition(position_);
        } else {
            m_transform.Position = position_;
        }

        // Fire event
        OnTransformChanged(getTransform());
    }

    void Entity::moveBy(Vector2 moveBy_) {
        if (m_physicsEnabled) {
            // Not recommended, read warning in docs.
            auto p = m_physicsBody->getPosition();
            m_physicsBody->setPosition(p + moveBy_);
        } else {
            m_transform.Position += moveBy_;
        }

        // Fire event
        OnTransformChanged(getTransform());
    }

    Angle Entity::getRotation() const {
        if (m_physicsBody == nullptr) {
            return m_transform.Rotation;
        } else {
            return m_physicsBody->getTransform().Rotation;
        }
    }

    void Entity::setRotation(Angle rotation_) {
        if (m_physicsBody == nullptr) {
            // Set our rotation
            m_transform.Rotation = rotation_;
        } else {
            // Set the body rotation
            m_physicsBody->setRotation(rotation_);
        }

        // Fire event
        OnTransformChanged(getTransform());
    }

    bool Entity::isPhysicsEnabled() const {
        return m_physicsBody != nullptr;
    }

    physics::PhysicsBody *Entity::getPhysicsBody() {
        return m_physicsBody;
    }

    const physics::PhysicsBody *Entity::getPhysicsBody() const {
        return m_physicsBody;
    }

    physics::PhysicsWorld *Entity::getPhysicsWorld() {
        if (m_physicsBody == nullptr) return nullptr;
        return m_physicsBody->getWorld();
    }

    const physics::PhysicsWorld *Entity::getPhysicsWorld() const {
        if (m_physicsBody == nullptr) return nullptr;
        return m_physicsBody->getWorld();
    }

    bool Entity::getDoPersistentUpdates() const {
        return m_persistentUpdates;
    }

    void Entity::setDoPersistentUpdates(bool persistentUpdates_) {
        if (m_onUpdateRef.isAttached())
            throw std::runtime_error("This property cannot be changed once update has been subscribed.");
        m_persistentUpdates = persistentUpdates_;
    }

    bool Entity::subscribeToUpdate() {
        if (m_parentScene != nullptr) {
            if (!m_onUpdateRef.isAttached()) {
                if (m_persistentUpdates)
                    m_onUpdateRef = m_parentScene->OnPersistentUpdate
                            += new ClassMethodEventHandler<Entity>(this, &Entity::update);
                else
                    m_onUpdateRef = m_parentScene->OnUpdate
                            += new ClassMethodEventHandler<Entity>(this, &Entity::update);
                return true;
            } else {
                // The event already exists, so return true anyway to avoid errors.
                return true;
            }
        }
        return false;
    }

    bool Entity::subscribedToUpdate() const {
        if (m_parentScene == nullptr) return false;
        return m_onUpdateRef.isAttached();
    }

    void Entity::unsubscribeFromUpdate() {
        // Detach using the reference.
        m_onUpdateRef.detach();
    }

    void Entity::draw(graphics::Renderer *renderer_) {
        // Draw components
        for (const auto &comp : m_components) {
            comp.second->draw(renderer_);
        }

        // Debug draw
        if (m_physicsBody != nullptr) {
            if (m_physicsBody->getWorld()->DebugDraw) {
                m_physicsBody->debugDraw(renderer_);
            }
        }
    }

    void Entity::update() {
        // Fire update event
        OnUpdate();

        if (m_physicsBody != nullptr) {
            // As we are a physics entity, fire this each frame
            OnTransformChanged(getTransform());
        }
    }
}
