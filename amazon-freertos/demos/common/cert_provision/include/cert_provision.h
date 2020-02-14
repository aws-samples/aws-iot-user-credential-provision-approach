#ifndef _CERT_PROVISION_H
#define _CERT_PROVISION_H

#define MAX_WIFI_SSID_LENGTH          ( 128 )
#define MAX_WIFI_PASSWORD_LENGTH      ( 128 ) 


typedef struct {
    char wifi_ssid[MAX_WIFI_SSID_LENGTH];
    char wifi_password[MAX_WIFI_PASSWORD_LENGTH];
} WiFiInfo_t;


void vConfigureAgentTask( void * pvParameters );

int xCheckCredInFlash(void);

void ReadWifiInFo(WiFiInfo_t * pWifiInfo);


#endif /* _CERT_PROVISION_H */
