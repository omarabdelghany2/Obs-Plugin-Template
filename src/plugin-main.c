// /*
// obs-localvocal
// Copyright (C) 2023 Roy Shilkrot roy.shil@gmail.com

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>
// */

// #include <obs-module.h>
// #include <plugin-support.h>
// #include <obs-frontend-api.h>
// #include "obs-websocket-api.h"

// OBS_DECLARE_MODULE()
// OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

// // static void on_frontend_event(enum obs_frontend_event event, void *private_data);



// static void on_frontend_event(enum obs_frontend_event event, void *private_data)
// {
//     obs_log(LOG_INFO, "Frontend event received: %d", event);
// }

// bool obs_module_load(void)
// {
//     const char *module_name = obs_module_name();
//     obs_log(LOG_INFO, "plugin loaded successfully (version %s)", PLUGIN_VERSION);
//     obs_log(LOG_INFO, "Plugin loaded. Module name: %s", module_name);

//     obs_frontend_add_event_callback(on_frontend_event, NULL);
//     return true;
// }

// void obs_module_unload(void)
// {
//     obs_log(LOG_INFO, "plugin unloaded");
//     obs_frontend_remove_event_callback(on_frontend_event, NULL);
// }

// MODULE_EXPORT const char *obs_module_description(void)
// {
//     return "Local Vocal Plugin";
// }

// MODULE_EXPORT const char *obs_module_name(void)
// {
//     return "localvocal";
// }













// #include <obs-module.h>

// #include "obs-websocket-api.h"

// typedef unsigned int uint;


// OBS_DECLARE_MODULE()
// OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

// obs_websocket_vendor vendor;



// bool obs_module_load(void)
// {
// 	blog(LOG_INFO, "Example obs-websocket-api plugin loaded!");
// 	return true;
// }

// void example_request_cb(obs_data_t *request_data, obs_data_t *response_data, void *priv_data);
// void obs_module_post_load(void)
// {
// 	vendor = obs_websocket_register_vendor("api_example_plugin");
// 	if (!vendor) {
// 		blog(LOG_ERROR, "Vendor registration failed! (obs-websocket should have logged something if installed properly.)");
// 		return;
// 	}

// 	if (!obs_websocket_vendor_register_request(vendor, "example_request", example_request_cb, NULL))
// 		blog(LOG_ERROR, "Failed to register `example_request` request with obs-websocket.");

// 	uint api_version = obs_websocket_get_api_version();
// 	if (api_version == 0) {
// 		blog(LOG_ERROR, "Unable to fetch obs-websocket plugin API version.");
// 		return;
// 	} else if (api_version == 1) {
// 		blog(LOG_WARNING, "Unsupported obs-websocket plugin API version for calling requests.");
// 		return;
// 	}

// 	struct obs_websocket_request_response *response = obs_websocket_call_request("GetVersion",NULL);
// 	if (!response) {
// 		blog(LOG_ERROR, "Failed to call GetVersion due to obs-websocket not being installed.");
// 		return;
// 	}
// 	blog(LOG_INFO, "[obs_module_post_load] Called GetVersion. Status Code: %u | Comment: %s | Response Data: %s",
//                 response->status_code, response->comment, response->response_data);
// 	obs_websocket_request_response_free(response);
    
// }

// void obs_module_unload(void)
// {
// 	blog(LOG_INFO, "Example obs-websocket-api plugin unloaded!");
// }

// void example_request_cb(obs_data_t *request_data, obs_data_t *response_data, void *priv_data)
// {
// 	if (obs_data_has_user_value(request_data, "ping"))
// 		obs_data_set_bool(response_data, "pong", true);

// 	UNUSED_PARAMETER(priv_data);
// }











#include <obs-websocket-api.h>
#include <obs-module.h>
#include "../.deps/include/util/platform.h"
#include <obs-frontend-api.h>


OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")
static obs_websocket_vendor my_vendor;


void change_transition_video_path(obs_source_t *transition, const char *new_path,int new_point,const char *transition_name);


// Allocate memory for the obs_data struct


void obs_frontend_get_transitionsss(struct obs_frontend_source_list *sources,obs_data_t *request_data)
{

    obs_frontend_get_transitions(sources);
    if(sources){blog(LOG_INFO, " we found all transitions: ");}


    const char *transition_name = obs_data_get_string(request_data, "sceneTransitionName");
    const char *transition_type = obs_data_get_string(request_data, "sceneTransitionType");
    obs_data_t *settings = obs_data_get_obj(request_data, "sceneTransitionSettings");
    int transition_point = obs_data_get_int(settings, "transitionPoint");
     const char *transition_path = obs_data_get_string(settings, "video");



    blog(LOG_INFO, "here we will print the request data and then the reposce data");
    blog(LOG_INFO, "here we will print the scene transition namee %s",transition_name);
    blog(LOG_INFO, "here we will print the scene transition Type %s",transition_type);
    blog(LOG_INFO, "here we will print the scene transition point %d",transition_point);
    blog(LOG_INFO, "here we will print the scene transition path %s",transition_path);

    for (size_t i = 0; i < sources->sources.num; i++) {
        obs_source_t *source = sources->sources.array[i];

        // Check if the source is a transition and has the name "stinger"
        if (source && strcmp(obs_source_get_name(source), transition_name) == 0) {
            blog(LOG_INFO, " we foun the transiton that is named  %s ",transition_name);
            // Change the video path of the "stinger" transition
            change_transition_video_path(source,transition_path,transition_point,transition_name);
            blog(LOG_INFO, " we changed the path: ");
            break; // Stop after finding and modifying the stinger transition
        }
    }
    
}
void change_transition_video_path(obs_source_t *transition, const char *new_path,int new_point,const char *transition_name)
{
    
    if (transition ) 
    {
        blog(LOG_INFO, " we entered tha function that change the path  and the path is %s",new_path);

        // Get the private settings of the transition
        obs_data_t *settings = obs_source_get_settings(transition);
        blog(LOG_INFO, " this source settings is %s",obs_source_get_settings(transition));
            if(settings)
            {
                obs_data_set_string(settings, "path", new_path);
                obs_data_set_int(settings, "transition_point", new_point);
                blog(LOG_INFO, "Setting stinger transition properties: path=%s, transition_point=%d",new_path, new_point);
                obs_source_update(transition, settings);
                blog(LOG_INFO, "obs_source_update called successfully.");
                obs_data_release(settings);
            }
            else
            {
                blog(LOG_ERROR, "Failed to get settings for the transition.");
            }
    }
    else
    {
        blog(LOG_ERROR, "Failed to get transition.");
    }

    


    // // Set the new video path
    // obs_data_set_string(settings, "video_path", new_path);
    // obs_data_set_int(settings, "transition_point", new_point);

    // // Apply the settings to the transition
    // obs_source_update(transition, settings);
    // // Release the settings object
    // obs_data_release(settings);

    // blog(LOG_INFO, " we ended the function that change the path ");
}

// Event callback function
void change_transition_cb(obs_data_t *request_data, obs_data_t *response_data, void *priv_data)

{   
    
    // blog(LOG_INFO, "here we will print the request data and then the reposce data");
    // blog(LOG_INFO, "here we will print the scene transition namee %s",obs_data_get_string(request_data,"sceneTransitionName"));
    // blog(LOG_INFO, "here we will print the scene transition Type %s",obs_data_get_string(request_data,"sceneTransitionType"));
    // obs_data_t *settings=obs_data_get_obj(request_data,"sceneTransitionSettings");
    // blog(LOG_INFO, "we have passed getting the object");
    // blog(LOG_INFO, "here we will print the scene transition point %d",obs_data_get_int(settings,"transitionPoint"));
    // blog(LOG_INFO, "here we will print the scene transition path %s",obs_data_get_string(settings,"video"));

    

    struct obs_frontend_source_list sources;
    da_init(sources.sources);
    obs_frontend_get_transitionsss(&sources, request_data);
    da_free(sources.sources);
}



// Module initialization
bool obs_module_load(void)
{
    

    blog(LOG_ERROR, "loaded vocalPlugin.");


    return true;
}



void obs_module_post_load(void)
{
    my_vendor = obs_websocket_register_vendor("OWN3D");

    if (my_vendor) {
        blog(LOG_ERROR, "succefully created by the power of vscode .",my_vendor);
    }

    // if (!obs_websocket_register_event_callback(event_callback, NULL)) {
    //     blog(LOG_ERROR, "Failed to register event callback.");
    // }

    int *int_data = (int *)malloc(sizeof(int));
    *int_data = 5200; // Initialize the integer
    void *priv_data = (void *)int_data;


    bool x=obs_websocket_vendor_register_request(my_vendor,"CreateSceneTransition",change_transition_cb,priv_data);


    if (x) {
        blog(LOG_ERROR, "succefully created the request",my_vendor);
    }



}
// Module deinitialization
void obs_module_unload(void)
{
    blog(LOG_ERROR, "unloaddedd .");
}







