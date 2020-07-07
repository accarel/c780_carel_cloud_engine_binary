
/**
 * @file   https_server_IS.h
 * @author carel
 * @date   9 Jan 2020
 * @brief  functions to manage http server
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_HTTP_SERVER_IS_H_
#define MAIN_HTTP_SERVER_IS_H_

#include "esp_vfs.h"
#include "data_types_CAREL.h"

// Scratch buffer size
#define SCRATCH_BUFSIZE  8192
// Max length a file path can have on storage
#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + CONFIG_SPIFFS_OBJ_NAME_LEN)

struct file_server_data {
    // Base path of file storage
    char base_path[ESP_VFS_PATH_MAX + 1];

    // Scratch buffer for temporary storage during file transfer
    char scratch[SCRATCH_BUFSIZE];
};


C_RES HTTPServer__StartFileServer (httpd_handle_t server, const char *base_path);

#ifdef __CCL_DEBUG_MODE
esp_err_t HTTPServer__StopServer(httpd_handle_t server);
#endif

void SetConfigReceived(void);
C_BYTE IsConfigReceived(void);

void SetWpsMode(void);
void UnSetWpsMode(void);
C_BYTE IsWpsMode(void);
#endif
