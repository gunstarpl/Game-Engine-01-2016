#include "Precompiled.hpp"
#include "ResourceManager.hpp"
using namespace System;

namespace
{
    // Log error messages.
    #define LogInitializeError() "Failed to initialize the resource manager! "
}

ResourceManager::ResourceManager() :
    m_context(nullptr),
    m_initialized(false)
{
}

ResourceManager::~ResourceManager()
{
    if(m_initialized)
        this->Cleanup();
}

void ResourceManager::Cleanup()
{
    // Remove all resource pools.
    Utility::ClearContainer(m_pools);

    // Reset context reference.
    m_context = nullptr;

    // Reset initialization state.
    m_initialized = false;
}

bool ResourceManager::Initialize(Context& context)
{
    // Setup initialization routine.
    if(m_initialized)
        this->Cleanup();

    SCOPE_GUARD
    (
        if(!m_initialized)
            this->Cleanup();
    );

    // Check if the instance already exists.
    if(context.Has<ResourceManager>())
    {
        Log() << LogInitializeError() << "Context is invalid.";
        return false;
    }

    // Add instance to the context.
    context.Set(this);

    // Save context reference.
    m_context = &context;

    // Success!
    return m_initialized = true;
}

void ResourceManager::ReleaseUnused()
{
    if(!m_initialized)
        return;

    // Release all unused resources.
    for(auto& pair : m_pools)
    {
        auto& pool = pair.second;
        pool->ReleaseUnused();
    }
}

Context* ResourceManager::GetContext()
{
    return m_context;
}
