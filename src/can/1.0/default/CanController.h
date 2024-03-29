/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "CanBus.h"
#include <automotive/filesystem>
#include <android/hardware/automotive/can/1.0/ICanController.h>
namespace fs = android::hardware::automotive::filesystem;
namespace android::hardware::automotive::can::V1_0::implementation {
    struct UsbCanIface {
        ICanController::InterfaceType iftype;
        std::string ifaceName;
    };

    struct CanController : public ICanController {
        Return<void> getSupportedInterfaceTypes(getSupportedInterfaceTypes_cb _hidl_cb) override;

        Return <ICanController::Result> upInterface(const ICanController::BusConfig &config) override;

        Return<bool> downInterface(const hidl_string &name) override;

    private:
        std::mutex mCanBusesGuard;
        std::map <std::string, sp<CanBus>> mCanBuses
        GUARDED_BY(mCanBusesGuard);
    };

    std::optional <UsbCanIface> findUsbDevice(const hidl_vec <hidl_string> &configSerialnos);

    std::optional <std::string> readSerialNo(const std::string &serialnoPath);

    std::optional <UsbCanIface> getIfaceName(fs::path serialPath);

    bool isValidName(const std::string &name);
}  // namespace android::hardware::automotive::can::V1_0::implementation
