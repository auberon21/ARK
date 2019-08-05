/*
 * This source file is part of ARK
 * For the latest info, see https://github.com/ArkNX
 *
 * Copyright (c) 2013-2019 ArkNX authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"),
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

#include "base/AFPlatform.hpp"
#include "base/AFMap.hpp"
#include "base/AFArrayMap.hpp"
#include "interface/AFIModule.hpp"

namespace ark {

class AFPluginManager;

class AFIPlugin
{
public:
    virtual ~AFIPlugin() = default;
    virtual int GetPluginVersion() = 0;
    virtual const std::string GetPluginName() = 0;

    virtual void Install() = 0;
    virtual void Uninstall() = 0;

    template<typename MODULE, typename DERIVED_MODULE>
    void RegisterModule()
    {
        ARK_ASSERT_RET_NONE((std::is_base_of<AFIModule, MODULE>::value));
        ARK_ASSERT_RET_NONE((std::is_base_of<MODULE, DERIVED_MODULE>::value));

        AFIModule* pRegModuleName = ARK_NEW DERIVED_MODULE();
        pRegModuleName->SetPluginManager(pPluginManager);
        pRegModuleName->SetName(GET_CLASS_NAME(MODULE));
        pPluginManager->AddModule(pRegModuleName->Name(), pRegModuleName);
        modules_.insert(pRegModuleName->Name(), pRegModuleName);
#ifdef ARK_PLATFORM_WIN
        std::string base_name = GET_CLASS_NAME(&AFIModule::Update);
        std::string child_name = GET_CLASS_NAME(&DERIVED_MODULE::Update);
        if (base_name != child_name)
#else
        AFIModule base;
        bool (AFIModule::*mfp)() = &AFIModule::Update;
        bool (DERIVED_MODULE::*child_mfp)() = &DERIVED_MODULE::Update;
        void* base_update_mfp = (void*)(base.*mfp);
        void* derived_update_mfp = (void*)(static_cast<DERIVED_MODULE*>(pRegModuleName)->*child_mfp);
        if (base_update_mfp == derived_update_mfp)
#endif
        {
            GetPluginManager()->AddUpdateModule(pRegModuleName);
        }
    }

    template<typename MODULE, typename DERIVED_MODULE>
    void DeregisterModule()
    {
        ARK_ASSERT_RET_NONE((std::is_base_of<AFIModule, MODULE>::value));
        ARK_ASSERT_RET_NONE((std::is_base_of<MODULE, DERIVED_MODULE>::value));

        AFIModule* pDeregModuleName = dynamic_cast<AFIModule*>(GetPluginManager()->template FindModule<MODULE>());
        ARK_ASSERT_RET_NONE(pDeregModuleName != nullptr);

        GetPluginManager()->RemoveModule(pDeregModuleName->Name());
        GetPluginManager()->RemoveUpdateModule(pDeregModuleName->Name());
        modules_.erase(pDeregModuleName->Name());
        ARK_DELETE(pDeregModuleName);
    }

    AFPluginManager* GetPluginManager() const
    {
        return pPluginManager;
    }

    void SetPluginManager(AFPluginManager* p)
    {
        pPluginManager = p;
    }

protected:
    // All registered modules
    AFMap<std::string, AFIModule> modules_;

private:
    AFPluginManager* pPluginManager{nullptr};
};

} // namespace ark