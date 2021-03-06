/**
* If not stated otherwise in this file or this component's LICENSE
* file the following copyright and licenses apply:
*
* Copyright 2019 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/

#pragma once

#include <mutex>

#include "tptimer.h"

#include "Module.h"
#include "utils.h"
#include "AbstractPlugin.h"

namespace WPEFramework {

    namespace Plugin {

        // This is a server for a JSONRPC communication channel. 
        // For a plugin to be capable to handle JSONRPC, inherit from PluginHost::JSONRPC.
        // By inheriting from this class, the plugin realizes the interface PluginHost::IDispatcher.
        // This realization of this interface implements, by default, the following methods on this plugin
        // - exists
        // - register
        // - unregister
        // Any other methood to be handled by this plugin  can be added can be added by using the
        // templated methods Register on the PluginHost::JSONRPC class.
        // As the registration/unregistration of notifications is realized by the class PluginHost::JSONRPC,
        // this class exposes a public method called, Notify(), using this methods, all subscribed clients
        // will receive a JSONRPC message as a notification, in case this method is called.
        class FrameRate : public AbstractPlugin {
        private:

            // We do not allow this plugin to be copied !!
            FrameRate(const FrameRate&) = delete;
            FrameRate& operator=(const FrameRate&) = delete;

            //Begin methods
            uint32_t setCollectionFrequencyWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t startFpsCollectionWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t stopFpsCollectionWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t updateFpsWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setFrmMode(const JsonObject& parameters, JsonObject& response);
	    uint32_t getFrmMode(const JsonObject& parameters, JsonObject& response);
	    uint32_t getDisplayFrameRate(const JsonObject& parameters, JsonObject& response);
	    uint32_t setDisplayFrameRate(const JsonObject& parameters, JsonObject& response);
	    //End methods
            
            int getCollectionFrequency();
            void setCollectionFrequency(int frequencyInMs);
            bool startFpsCollection();
            bool stopFpsCollection();
            void updateFps(int newFpsValue);

            void fpsCollectionUpdate( int averageFps, int minFps, int maxFps );
            
            virtual void enableFpsCollection() {}
            virtual void disableFpsCollection() {}
            
            void onReportFpsTimer();
            void onReportFpsTimerTest();

        public:
            FrameRate();
            virtual ~FrameRate();
            virtual void Deinitialize(PluginHost::IShell* service) override;

        public:
            static FrameRate* _instance;
        private:
            int m_fpsCollectionFrequencyInMs;
            int m_minFpsValue;
            int m_maxFpsValue;
            int m_totalFpsValues;
            int m_numberOfFpsUpdates;
            bool m_fpsCollectionInProgress;
            //QTimer m_reportFpsTimer;
            TpTimer m_reportFpsTimer;
            int m_lastFpsValue;
            
            std::mutex m_callMutex;
        };
    } // namespace Plugin
} // namespace WPEFramework
