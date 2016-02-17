#pragma once

#include "Precompiled.hpp"
#include "System/Resource.hpp"
#include "Lua/State.hpp"

//
// Reference
//
//  Object which holds a reference to Lua object.
//  Can be also used as a resource bound to ScriptSystem.
//

namespace Lua
{
    // Reference class.
    class Reference : public System::Resource
    {
    public:
        // Type declarations.
        typedef int ReferenceID;

    public:
        Reference(System::ResourceManager* resourceManager);
        ~Reference();

        // Restores instance to it's original state.
        void Cleanup();

        // Loads the reference from a file.
        bool Load(std::string filename);

        // Gets the hosting Lua state.
        std::shared_ptr<Lua::State> GetState() const;

        // Gets the reference identificator.
        ReferenceID GetReference() const;

        // Checks if the reference is valid.
        bool IsValid() const;

    private:
        // Lua state reference.
        std::shared_ptr<Lua::State> m_state;

        // Lua value reference.
        ReferenceID m_reference;
    };
}
