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

#include "kernel/interface/AFIMetaClassModule.h"
#include "kernel/interface/AFIConfigModule.h"
#include "game/interface/AFIPropertyConfigModule.h"

namespace ark
{

class AFCPropertyConfigModule : public AFIPropertyConfigModule
{
public:
    bool Init() override;
    bool PostInit() override;

    int CalculateBaseValue(const int nJob, const int nLevel, const std::string &strProperty) override;
    bool LegalLevel(const int nJob, const int nLevel) override;

protected:
    void Load();

private:
    //
    // different job, different PropertyID[Level->EffectData]
    AFMapEx<int, AFMapEx<int, std::string>> mhtCoefficienData;

    AFIMetaClassModule *m_pClassModule;
    AFIConfigModule *m_pConfigModule;
};

} // namespace ark