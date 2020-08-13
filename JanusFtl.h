/**
 * @file JanusFtl.h
 * @author Hayden McAfee (hayden@outlook.com)
 * @version 0.1
 * @date 2020-08-09
 * 
 * @copyright Copyright (c) 2020 Hayden McAfee
 * 
 */

#pragma once

extern "C"
{
    #include <plugins/plugin.h>
}

#include "IngestServer.h"
#include "CredStore.h"
#include "JanusSession.h"
#include "FtlStream.h"
#include <memory>
#include <map>
#include <mutex>

/**
 * @brief This class handles interactions with the Janus plugin API and Janus core.
 */
class JanusFtl
{
public:
    /* Init/Destroy */
    int Init(janus_callbacks* callback, const char* config_path);
    void Destroy();

    /* Public plugin methods */
    void CreateSession(janus_plugin_session* handle, int* error);
    struct janus_plugin_result* HandleMessage(
        janus_plugin_session* handle,
        char* transaction,
        json_t* message,
        json_t* jsep);
    json_t* HandleAdminMessage(json_t* message);
    void SetupMedia(janus_plugin_session* handle);
    void IncomingRtp(janus_plugin_session* handle, janus_plugin_rtp* packet);
    void IncomingRtcp(janus_plugin_session* handle, janus_plugin_rtcp* packet);
    void DataReady(janus_plugin_session* handle);
    void HangUpMedia(janus_plugin_session* handle);
    void DestroySession(janus_plugin_session* handle, int* error);
    json_t* QuerySession(janus_plugin_session* handle);

private:
    /* Members */
    janus_callbacks* janusCore;
    std::unique_ptr<IngestServer> ingestServer;
    std::shared_ptr<CredStore> credStore;
    std::mutex sessionsMutex;
    std::map<janus_plugin_session*, std::shared_ptr<JanusSession>> sessions;
    uint16_t minMediaPort = 9000;
    uint16_t maxMediaPort = 65535;
    std::map<uint16_t, std::shared_ptr<FtlStream>> ftlStreams;
    std::mutex ftlStreamsMutex;

    /* Private methods */
    uint16_t ingestMediaPortRequested(IngestConnection& connection);
};