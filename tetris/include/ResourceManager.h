#pragma once

#include <memory>
#include <optional>
#include <ranges>

#include <Logger.h>
#include <Types.h>

#include <unordered_map>

class IResource;

template <std::derived_from<IResource> Resource>
struct ResourceParams
{
    ResourceType m_ResourceType = {};
    std::string m_Name = {};
    std::shared_ptr<Resource> m_Resource = {};
};

template <std::derived_from<IResource> Resource>
using Resources = std::vector<ResourceParams<Resource>>;

class ResourceManager
{
public:
    template <std::derived_from<IResource> Resource>
    static void RegisterResource(const ResourceParams<Resource>& params)
    {
        if (!find_private(params)) m_Resources<Resource>.push_back(params);
        else LOG_CRITICAL("This resource will not be loaded as it's already registered, please use GetFromCache() to retrieve it.");
    }

    template <std::derived_from<IResource> Resource>
    [[nodiscard]] static auto GetFromCache(const ResourceParams<Resource>& params) -> std::optional<decltype(std::declval<ResourceParams<Resource>>().m_Resource)> { return find_private(params).value().m_Resource; }

    template <std::derived_from<IResource> Resource>
    static auto GetAllFromCache() { return m_Resources<Resource>; }

private:
    template <std::derived_from<IResource> Resource>
    inline static Resources<Resource> m_Resources = {};

    template <std::derived_from<IResource> Resource, typename ReturnType = typename std::optional<ResourceParams<Resource>>::value_type>
    static auto find_private(const ResourceParams<Resource>& params) -> std::optional<ReturnType>
    {
        auto it = std::ranges::find(m_Resources<Resource>, params.m_Name, &ResourceParams<Resource>::m_Name);
        return it != m_Resources<Resource>.end() ? *it : std::optional<ReturnType>{};
    }
};