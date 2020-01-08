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

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "Config.hpp"

#include "Events.hpp"
#include "Scene.hpp"

namespace NerdThings::Ngine {
    class Entity;

    /**
     * A component is an object that is attached to an entity and manipulates its behaviour.
     */
    class NEAPI Component {
        /**
         * The parent entity.
         *
         * @note Should *NEVER* be null, if it is something is very wrong.
         */
        Entity *m_parentEntity = nullptr;

        /**
         * Entity OnDraw reference.
         */
        EventAttachment<Graphics::Renderer *> m_onDrawRef;

        /**
         * Entity OnUpdate reference.
         */
        EventAttachment<> m_onUpdateRef;
    public:
        /**
         * Fired when an entity destroys this component.
         */
        Event<> OnDestroy;

        virtual ~Component();

        /**
         * Get the parent entity as a type
         *
         * @tparam EntityType The type to get the entity as.
         * @return The entity casted to the provided type.
         */
        template <typename EntityType = Entity>
        EntityType *GetEntity() const {
            return (EntityType *) m_parentEntity;
        }

        /**
         * Get the parent scene
         *
         * @tparam SceneType The type we want the scene as.
         * @return The parent entity's parent scene.
         */
        template <class SceneType = Scene>
        SceneType *GetScene() const {
            return (SceneType *) m_parentEntity->GetScene();
        }

        /**
         * Get the parent game.
         *
         * @return The game that we are a part of.
         */
        Game *GetGame() const;

        /**
         * Get the game resource manager.
         *
         * @return The game resource manager.
         */
        Filesystem::ResourceManager *GetResourceManager() const;

        /**
         * Subscribe to the entity's draw event.
         */
        void SubscribeToDraw();

        /**
         * Determine if the component is subscribed to the draw event.
         *
         * @return Whether or not the component is subscribed to draw.
         */
        bool SubscribedToDraw() const;

        /**
         * Unsubscribe from the entity's draw event.
         */
        void UnsubscribeFromDraw();

        /**
         * Subscribe to the entity's update event.
         */
        void SubscribeToUpdate();

        /**
         * Determine if the component is subscribed to the update event.
         *
         * @return Whether or not the component is subscribed to update.
         */
        bool SubscribedToUpdate() const;

        /**
         * Unsubscribe from the entity's update event.
         */
        void UnsubscribeFromUpdate();

        /**
         * Component draw code.
         */
        virtual void Draw(Graphics::Renderer *renderer_);

        /**
         * Component update code.
         */
        virtual void Update();

    protected:
        /**
         * Initialise component
         *
         * @note This is protected as `Component` MUST be used as a base class only.
         * @param parent_ The parent entity.
         */
        Component(Entity *parent_);
    };
}

#endif //COMPONENT_HPP
