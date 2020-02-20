/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* TCP/IP abstraction includes. */
#include "aws_secure_sockets.h"

/* AWS library includes. */
#include "aws_wifi.h"

/* PKCS#11 includes. */
#include "aws_pkcs11.h"
#include "aws_pkcs11_config.h"

/* Client credential includes. */
#include "aws_clientcredential.h"
#include "aws_default_root_certificates.h"

/* Demo configuration */
#include "aws_demo_config.h"
#include "aws_dev_mode_key_provisioning.h"

/* mbedTLS includes. */
#include "mbedtls/base64.h"

#include <wlan/wlan_test_inc.h>
#include <wifi/wifi_conf.h>
#include <wifi/wifi_util.h>
#include "cert_provision.h"
#include "flash_api.h"
#include <device_lock.h>


/*-----------------------------------------------------------*/

#define TIME_TO_WAIT_UI_PROVISION       ( 20000 )
#define MAX_CERTIFICATE_LENGTH          ( 2048 )
#define MAX_KEYT_LENGTH                 ( 2048 )  
#define END_OF_RSA_KEY                  "-----END RSA PRIVATE KEY-----"
#define END_OF_CERTIFICATE              "-----END CERTIFICATE-----"
#define WIFI_SSID_NOT_FOUND             "cert not found"
#define WIFI_PASSWORD_NOT_FOUND         "password not found"
#define RX_CREDENTIAL_TPYE_CERTS        ( 1 )
#define RX_CREDENTIAL_TPYE_PRIV_KEY     ( 2 )
#define RX_CREDENTIAL_TPYE_WIFI_SSID    ( 3 )
#define RX_CREDENTIAL_TPYE_PASSWORD     ( 4 ) 
#define WIFI_INFO_FLASH_OFFSET (0x1D6000) 
#define PRIV_KEY_FLASH_OFFSET   (0x1D3000) //Flash location for Priv Key
#define PUB_KEY_FLASH_OFFSET    (0x1D4000) //Flash location for Pub Key


#define echoBUFFER_SIZES                ( 2000 ) /*_RB_ Want to be a multiple of the MSS but there is no MSS constant in the bastraction. */
/* The number of instances of the echo client task to create. */
#define echoNUM_ECHO_CLIENTS            ( 1 )
/* The echo server is assumed to be on port 7, which is the standard echo
 * protocol port. */
#define echoECHO_PORT                   ( 7070 )

/* The flag that turns on TLS for secure socket */
#define configTCP_ECHO_TASKS_SINGLE_TASK_TLS_ENABLED    ( 1 )


/*-----------------------------------------------------------*/

/*
 * PEM-encoded server certificate
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 */
#if ( configTCP_ECHO_TASKS_SINGLE_TASK_TLS_ENABLED == 1 )
/* default TLS Sercer certificate */
static const char cTls_SERVER_CERTIFICATE_PEM[] = "-----BEGIN CERTIFICATE-----\n"
"MIID5TCCAs2gAwIBAgIJAMBc77LhtML5MA0GCSqGSIb3DQEBCwUAMIGIMQswCQYD\n"
"VQQGEwJVUzELMAkGA1UECAwCV0ExDjAMBgNVBAcMBVBsYWNlMRQwEgYDVQQKDAtZ\n"
"b3VyQ29tcGFueTELMAkGA1UECwwCSVQxFjAUBgNVBAMMDXd3dy55b3Vycy5jb20x\n"
"ITAfBgkqhkiG9w0BCQEWEnlvdXJFbWFpbEB5b3VyLmNvbTAeFw0xOTA4MjgwODI2\n"
"NDdaFw0yMDA4MjcwODI2NDdaMIGIMQswCQYDVQQGEwJVUzELMAkGA1UECAwCV0Ex\n"
"DjAMBgNVBAcMBVBsYWNlMRQwEgYDVQQKDAtZb3VyQ29tcGFueTELMAkGA1UECwwC\n"
"SVQxFjAUBgNVBAMMDXd3dy55b3Vycy5jb20xITAfBgkqhkiG9w0BCQEWEnlvdXJF\n"
"bWFpbEB5b3VyLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMCj\n"
"YQ/VTCA+r9xgkx5HLcZ4F83aw1lwEXw0rvaOpoVTKJT0CgyE+9AmOC0hKnAsiboS\n"
"yQBLeGgaG7nZLwtDKqJ/x8x/f1n0e5mj/oVfPytnCX/prd6Kbwm90CDziqONDj5k\n"
"D0XBX1l6icBimxTj5ncESzw4NOBkSUF4qTYaE5L4DqqTWplpDT/0MJvLO0B/J8ck\n"
"68sMh54/ZkedJmL17YOChIZIeYpXOU8ceyL8z3QBq0o22mWy6Y5GursriqoM7NDV\n"
"IoQnKxy5xr8c4B/EXXMWmKLldkMB5g9Lp8537M1591Of1P+dyMLB/I03GzHtXPU3\n"
"X1AHuQNHc589AENNOAcCAwEAAaNQME4wHQYDVR0OBBYEFHS+IhCTX2xEmGkgVBYS\n"
"wlRwW2F3MB8GA1UdIwQYMBaAFHS+IhCTX2xEmGkgVBYSwlRwW2F3MAwGA1UdEwQF\n"
"MAMBAf8wDQYJKoZIhvcNAQELBQADggEBAB8591oodDZUf5Fy1Xdcrv2tjHuk2Wbh\n"
"qnzR586aapViuctGtyan0MEncV9XnTmE9vWPmfPKqrm+1AssTkzQsUYTfpIgpaIv\n"
"/umVDvtm1/5N7iuWLQbUgjlt4z0BZSXZCLA3KccgO/92VEKOgwVy8/VBiv8oRSae\n"
"uEsgftCQ4X+kAOEVhLdcxoJa7tDmcba+i11Vl+vJqu4qV36TPLzL89bRYqMSGzxC\n"
"jS9dqtXKEgSu1e+Uk/MHdty9JZCvi/AtMtTahbMUVLBFc2GXfCGKawM2QvyVyXAr\n"
"lbX0Tq/qUCYmZ1+vSCuK9bRBrEpQ0euLwJL3Fdxza3+Dl9/v2uhKz1A=\n"
"-----END CERTIFICATE-----";
    static const uint32_t ulTls_SERVER_CERTIFICATE_LENGTH = sizeof( cTls_SERVER_CERTIFICATE_PEM );
#endif


typedef struct PKCSContext
{    
    /* PKCS#11. */
    CK_FUNCTION_LIST_PTR xP11FunctionList;
    CK_SESSION_HANDLE xP11Session;
    CK_OBJECT_HANDLE xP11PrivateKey;
} PKCSContext_t;

/*-----------------------------------------------------------*/

void vWirteWifiInFo(WiFiInfo_t wifi_info)
{
    flash_t flash;
    
    device_mutex_lock(RT_DEV_LOCK_FLASH);
    flash_erase_sector(&flash, WIFI_INFO_FLASH_OFFSET);
    flash_stream_write(&flash, WIFI_INFO_FLASH_OFFSET, sizeof(WiFiInfo_t), (uint8_t *) &wifi_info);
    device_mutex_unlock(RT_DEV_LOCK_FLASH);
}

void vClearTLSTempInfo(void)
{
    flash_t flash;
    
    device_mutex_lock(RT_DEV_LOCK_FLASH);
    flash_erase_sector(&flash, PRIV_KEY_FLASH_OFFSET);
    flash_erase_sector(&flash, PUB_KEY_FLASH_OFFSET);
    device_mutex_unlock(RT_DEV_LOCK_FLASH);
}

void vReadWifiInFo(WiFiInfo_t *pWifiInfo)
{
    flash_t flash;
    WiFiInfo_t Wifi_info;
    
    if (pWifiInfo == NULL) {
        return;
    }
    device_mutex_lock(RT_DEV_LOCK_FLASH);
    flash_stream_read(&flash, WIFI_INFO_FLASH_OFFSET, sizeof(WiFiInfo_t), (uint8_t *) pWifiInfo);
    device_mutex_unlock(RT_DEV_LOCK_FLASH);
    configPRINTF( ( "wifi_info.wifi_ssid len:%d \r\n", strlen(Wifi_info.wifi_ssid) ) );
    configPRINTF( ( "wifi_info.wifi_password len:%d \r\n", strlen(Wifi_info.wifi_password )) );
}


/* Set the SoftAP network parameters for WIFI_ConfigureAP() test. */
static inline void prvSetSoftAPNetworkParameters( WIFINetworkParams_t * pxSoftAPNetworkParams )
{
    pxSoftAPNetworkParams->pcSSID = wificonfigACCESS_POINT_SSID_PREFIX;
    pxSoftAPNetworkParams->ucSSIDLength =
    sizeof( wificonfigACCESS_POINT_SSID_PREFIX );
    pxSoftAPNetworkParams->pcPassword = wificonfigACCESS_POINT_PASSKEY;
    pxSoftAPNetworkParams->ucPasswordLength =
    sizeof( wificonfigACCESS_POINT_PASSKEY );
    pxSoftAPNetworkParams->xSecurity = wificonfigACCESS_POINT_SECURITY;
    pxSoftAPNetworkParams->cChannel = wificonfigACCESS_POINT_CHANNEL;
}

void prvWifiAPStart( void )
{
    WIFINetworkParams_t xNetworkParams = { 0 };

    /* Set the network parameters with valid parameters */
    prvSetSoftAPNetworkParameters( &xNetworkParams );
    
    WIFI_ConfigureAP( &xNetworkParams );
}

int xgetConnectedClient( void )
{
    int client_number;
    int ret = -1;
    struct {
        int    count;
        rtw_mac_t mac_list[AP_STA_NUM];
    } client_info;
    client_info.count = AP_STA_NUM;
    wifi_get_associated_client_list(&client_info, sizeof(client_info));
    configPRINTF(("\n\rAssociated Client List:"));

    if(client_info.count == 0) {
        configPRINTF(("\n\rClient Num: 0\n\r"));
    } else {
        configPRINTF(("\n\rClient Num: %d", client_info.count));
        ret = client_info.count;
        for( client_number=0; client_number < client_info.count; client_number++ ) {
            configPRINTF(("\n\rClient %d:", client_number + 1));
            configPRINTF(("\n\r\tMAC => "MAC_FMT"", MAC_ARG(client_info.mac_list[client_number].octet)));
        }
        configPRINTF(("\n\r"));
    }

    return ret;
}

static int xCheckClientCredential( PKCSContext_t * pxCtx )
{
    BaseType_t xResult = 0;
    CK_SLOT_ID xSlotId = 0;
    CK_ULONG xCount = 1;
    CK_ATTRIBUTE xTemplate = { 0 };

    /* Get the default private key storage ID. */
    if( 0 == xResult )
    {
        xResult = ( BaseType_t ) pxCtx->xP11FunctionList->C_GetSlotList( CK_TRUE,
                                                                         &xSlotId,
                                                                         &xCount );
        configPRINTF(("C_GetSlotList ret:%d\r\n", xResult));
    } else {
        configPRINTF(("C_GetSlotList failed\r\n"));
    }

    /* Start a private session with the P#11 module. */
    if( 0 == xResult )
    {
        xResult = ( BaseType_t ) pxCtx->xP11FunctionList->C_OpenSession( xSlotId,
                                                                         CKF_SERIAL_SESSION,
                                                                         NULL,
                                                                         NULL,
                                                                         &pxCtx->xP11Session );
        configPRINTF(("C_OpenSession ret:%d\r\n", xResult));
    } else {
        configPRINTF(("C_OpenSession failed\r\n"));
    }

    /* Get the handle of the device private key. */
    if( 0 == xResult )
    {
        xTemplate.type = CKA_LABEL;
        xTemplate.ulValueLen = sizeof( pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS );
        xTemplate.pValue = &pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS;
        xResult = ( BaseType_t ) pxCtx->xP11FunctionList->C_FindObjectsInit( pxCtx->xP11Session,
                                                                             &xTemplate,
                                                                             1 );
        configPRINTF(("C_FindObjectsInit ret:%d\r\n", xResult));
    } else {
        configPRINTF(("C_FindObjectsInit failed\r\n"));
    }    

    if( 0 == xResult )
    {
        xResult = ( BaseType_t ) pxCtx->xP11FunctionList->C_FindObjects( pxCtx->xP11Session,
                                                                         &pxCtx->xP11PrivateKey,
                                                                         1,
                                                                         &xCount );
        configPRINTF(("C_FindObjects ret:%d\r\n", xResult));
    } else {
        configPRINTF(("C_FindObjects failed\r\n"));
    } 

    if( 0 == xResult )
    {
        xResult = ( BaseType_t ) pxCtx->xP11FunctionList->C_FindObjectsFinal( pxCtx->xP11Session );
        configPRINTF(("C_FindObjectsFinal ret:%d\r\n", xResult));
    } else {
        configPRINTF(("C_FindObjectsFinal failed\r\n"));
    } 

    pxCtx->xP11FunctionList->C_CloseSession( pxCtx->xP11Session );

    return xResult;
}


int xCheckCredInFlash( void )
{
    BaseType_t xResult = 0;
    PKCSContext_t * pxCtx = NULL;
    CK_C_GetFunctionList xCkGetFunctionList = NULL;

	/* Allocate an internal context. */
    pxCtx = ( PKCSContext_t * ) pvPortMalloc( sizeof( PKCSContext_t ) ); /*lint !e9087 !e9079 Allow casting void* to other types. */

    /* Initialize PKCS module. */
    if( NULL != pxCtx )
    {
        memset( pxCtx, 0, sizeof( PKCSContext_t ) );

        /* Get the function pointer list for the PKCS#11 module. */
        xCkGetFunctionList = C_GetFunctionList;
        xResult = ( BaseType_t ) xCkGetFunctionList( &pxCtx->xP11FunctionList );

        /* Ensure that the PKCS #11 module is initialized. */
        if( 0 == xResult )
        {
            xResult = ( BaseType_t ) pxCtx->xP11FunctionList->C_Initialize( NULL );
            configPRINTF(("C_Initialize ret:%d\r\n", xResult));
            /* It is ok if the module was previously initialized. */
            if( xResult == CKR_CRYPTOKI_ALREADY_INITIALIZED )
            {
                xResult = CKR_OK;
            }
        }
    }
    else
    {
        configPRINTF(("provision_cert_from_flash memory failed\r\n"));
        xResult = ( BaseType_t ) CKR_HOST_MEMORY;
    }
    
    /* Check Client Credential exist or not. */
    if( 0 == xResult )
    {
        xResult = xCheckClientCredential( pxCtx );
    }
    
    vPortFree(pxCtx);
    pxCtx = NULL;

    return xResult;    
}

uint32_t prvRxCredeantialsFromSocket( char * buffer,  int type, Socket_t xSocket )
{

    char * pcReceivedString;
    char cRxBuffers[ echoBUFFER_SIZES ];    
    int xReceivedBytes = 0;
    bool fgSucess = 0;
    int xReturned = 0;
    int MaxLoopCount = 1000;
    int counter = 0;
    int ret = 0;
    
    pcReceivedString = &( cRxBuffers[0] );
    memset( ( void * ) pcReceivedString, 0x00, echoBUFFER_SIZES );
    xReceivedBytes = 0;

    /* Receive data echoed back to the socket. */
    while( fgSucess == 0 && counter < MaxLoopCount )
    {
        counter++;
        memset( ( void * ) pcReceivedString, 0x00, echoBUFFER_SIZES );
        xReturned = SOCKETS_Recv( xSocket,            /* The socket being received from. */
                                  pcReceivedString ,  /* The buffer into which the received data will be written. */
                                  sizeof(cRxBuffers), /* The size of the buffer provided to receive the data. */
                                  0 );                /* No flags. */

        configPRINTF( ( "Received string %s\r\n", pcReceivedString ) );
        configPRINTF( ( "Received string len %d.\r\n", strlen(pcReceivedString) ) );
        configPRINTF( ( "Received bytels %d.\r\n", xReceivedBytes ) );
        configPRINTF( ( "xReturned %d.\r\n", xReturned ) );
        if (xReturned > 0) {
            memcpy( &buffer[xReceivedBytes], pcReceivedString, xReturned );
            xReceivedBytes += xReturned;
        
            if (type == RX_CREDENTIAL_TPYE_PRIV_KEY) {
                if (strlen(pcReceivedString) > strlen(END_OF_RSA_KEY)) {
                    ret = strncmp(END_OF_RSA_KEY, pcReceivedString + strlen(pcReceivedString) - strlen(END_OF_RSA_KEY), strlen(END_OF_RSA_KEY));
                    configPRINTF( ( "type %d, compare ret %d.\r\n", type, ret ) );
                    if( ret == 0 ) {
                        fgSucess = 1;
                        break;
                    }
                }
            }
            else if (type == RX_CREDENTIAL_TPYE_CERTS) {
                ret = strncmp(END_OF_CERTIFICATE, pcReceivedString + strlen(pcReceivedString) - strlen(END_OF_CERTIFICATE), strlen(END_OF_CERTIFICATE));
                configPRINTF( ( "type %d, compare ret %d.\r\n", type, ret ) );
                if (strlen(pcReceivedString) > strlen(END_OF_CERTIFICATE)) {
                    if( ret == 0 ) {
                        fgSucess = 1;
                        break;
                    }
                }
            }
            else if (type == RX_CREDENTIAL_TPYE_WIFI_SSID) {
                fgSucess = 1;
                ret = strncmp(WIFI_SSID_NOT_FOUND, pcReceivedString, strlen(WIFI_SSID_NOT_FOUND));
                configPRINTF( ( "type %d, compare ret %d.\r\n", type, ret ) );
                // set received length to 0 if it's identical with WIFI_SSID_NOT_FOUND.
                if( ret == 0 ) {
                    xReceivedBytes = 0;
                }
                break;
            }
            else if (type == RX_CREDENTIAL_TPYE_PASSWORD) {
                fgSucess = 1;
                ret = strncmp(WIFI_PASSWORD_NOT_FOUND, pcReceivedString, strlen(WIFI_PASSWORD_NOT_FOUND));
                configPRINTF( ( "type %d, compare ret %d.\r\n", type, ret ) );
                // set received length to 0 if it's identical with WIFI_PASSWORD_NOT_FOUND.
                if( ret == 0 ) {
                    xReceivedBytes = 0;
                }
                break;
            }
       }
    }
    if (!fgSucess) {
        if (counter >= MaxLoopCount) {
            configPRINTF(("timed out after retry to receive credenatials type %d after %d times\r\n", type, counter));
        }

        configPRINTF(("credenatials received fail for type %d\r\n", type));
        return 0;
    }
    else {
        configPRINTF(("credenatials received secuss for type %d\r\n", type));
    }
    return xReceivedBytes;
}


static void prvConfigureAgentTask( void * pvParameters )
{   
    Socket_t xSocket;
    SocketsSockaddr_t xEchoServerAddress;
    volatile uint32_t ulTxCount = 0UL;
    BaseType_t xReceivedBytes, xReturned;
    char * pcReceivedString;
    TickType_t xTimeOnEntering;
    char  local_certificate_pem[MAX_CERTIFICATE_LENGTH];
    uint32_t   local_certificate_pem_length = 0;
    char  local_privet_key[MAX_KEYT_LENGTH];
    uint32_t   local_privet_key_length = 0;
    ProvisioningParams_t xParams;
    int client_ip; 
    uint32_t   local_wifi_ssid_length = 0;
    uint32_t   local_wifi_password_length = 0;
    WiFiInfo_t wifi_info;
    
    if (pvParameters == NULL) {
        return;
    }
    client_ip = * (int *) pvParameters;
    memset( &local_certificate_pem[0], 0x00, MAX_CERTIFICATE_LENGTH );
    memset( &local_privet_key[0], 0x00, MAX_KEYT_LENGTH );
    /* Rx and Tx time outs are used to ensure the sockets do not wait too long for
     * missing data. */
    TickType_t xReceiveTimeOut = pdMS_TO_TICKS( 2000 );
    TickType_t xSendTimeOut = pdMS_TO_TICKS( 2000 );


    #if ( ipconfigUSE_TCP_WIN == 1 )
        WinProperties_t xWinProps;

        /* Fill in the buffer and window sizes that will be used by the socket. */
        xWinProps.lTxBufSize = ipconfigTCP_TX_BUFFER_LENGTH;
        xWinProps.lTxWinSize = configECHO_CLIENT_TX_WINDOW_SIZE;
        xWinProps.lRxBufSize = ipconfigTCP_RX_BUFFER_LENGTH;
        xWinProps.lRxWinSize = configECHO_CLIENT_RX_WINDOW_SIZE;
    #endif /* ipconfigUSE_TCP_WIN */  

    /* Echo requests are sent to the echo server.  The address of the echo
     * server is configured by the constants configECHO_SERVER_ADDR0 to
     * configECHO_SERVER_ADDR3 in FreeRTOSConfig.h. */
    xEchoServerAddress.usPort = SOCKETS_htons( echoECHO_PORT );

    xEchoServerAddress.ulAddress = SOCKETS_inet_addr_quick( (client_ip >> 0) & 0xff,
                                                            (client_ip >> 8) & 0xff,
                                                            (client_ip >> 16) & 0xff,
                                                            (client_ip >> 24) & 0xff);

    /* Create a TCP socket. */
    xSocket = SOCKETS_Socket( SOCKETS_AF_INET, SOCKETS_SOCK_STREAM, SOCKETS_IPPROTO_TCP );
    configASSERT( xSocket != SOCKETS_INVALID_SOCKET );

    /* Set a time out so a missing reply does not cause the task to block
     * indefinitely. */
    SOCKETS_SetSockOpt( xSocket, 0, SOCKETS_SO_RCVTIMEO, &xReceiveTimeOut, sizeof( xReceiveTimeOut ) );
    SOCKETS_SetSockOpt( xSocket, 0, SOCKETS_SO_SNDTIMEO, &xSendTimeOut, sizeof( xSendTimeOut ) );

    #if ( ipconfigUSE_TCP_WIN == 1 )
        {
            /* Set the window and buffer sizes. */
            SOCKETS_SetSockOpt( xSocket, 0, FREERTOS_SO_WIN_PROPERTIES, ( void * ) &xWinProps, sizeof( xWinProps ) );
        }
    #endif /* ipconfigUSE_TCP_WIN */

    #if ( configTCP_ECHO_TASKS_SINGLE_TASK_TLS_ENABLED == 1 )
        {
            /* Set the socket to use TLS. */
            SOCKETS_SetSockOpt( xSocket, 0, SOCKETS_SO_REQUIRE_TLS, NULL, ( size_t ) 0 );
            SOCKETS_SetSockOpt( xSocket, 0, SOCKETS_SO_TRUSTED_SERVER_CERTIFICATE, cTls_SERVER_CERTIFICATE_PEM, ulTls_SERVER_CERTIFICATE_LENGTH );
        }
    #endif /* configTCP_ECHO_TASKS_SINGLE_TASK_TLS_ENABLED */

    /* Connect to the echo server. */
    configPRINTF( ( "Connecting to echo server\r\n" ) );

    if( SOCKETS_Connect( xSocket, &xEchoServerAddress, sizeof( xEchoServerAddress ) ) == 0 )
    {
        configPRINTF( ( "Connected to echo server\r\n" ) );
        /* Send a number of echo requests. */
        local_privet_key_length = prvRxCredeantialsFromSocket(local_privet_key, RX_CREDENTIAL_TPYE_PRIV_KEY, xSocket);
        local_certificate_pem_length = prvRxCredeantialsFromSocket(local_certificate_pem, RX_CREDENTIAL_TPYE_CERTS, xSocket);
        local_wifi_ssid_length = prvRxCredeantialsFromSocket(wifi_info.wifi_ssid, RX_CREDENTIAL_TPYE_WIFI_SSID, xSocket);
        local_wifi_password_length = prvRxCredeantialsFromSocket(wifi_info.wifi_password, RX_CREDENTIAL_TPYE_PASSWORD, xSocket);

        configPRINTF( ( "local_privet_key_length %d \r\n",local_privet_key_length ) );
        configPRINTF( ( "local_certificate_pem_length %d \r\n",local_certificate_pem_length ) );
        configPRINTF( ( "wifi_info.wifi_ssid %s \r\n",wifi_info.wifi_ssid ) );
        configPRINTF( ( "wifi_info.wifi_password %s \r\n",wifi_info.wifi_password ) );

        xParams.ulClientPrivateKeyType = CKK_RSA;
        xParams.pcClientPrivateKey = ( uint8_t * ) &local_privet_key;
        xParams.ulClientPrivateKeyLength = local_privet_key_length;
        xParams.pcClientCertificate = ( uint8_t * ) &local_certificate_pem[0];
        xParams.ulClientCertificateLength = local_certificate_pem_length;
        
        /* write wifi credential to flash */
        vWirteWifiInFo(wifi_info);
        /* wirite client credential by pkcs */
        vAlternateKeyProvisioning( &xParams );
        /* Finished using the connected socket, initiate a graceful close:
         * FIN, FIN+ACK, ACK. */
        configPRINTF( ( "Shutting down connection to echo server.\r\n" ) );
        SOCKETS_Shutdown( xSocket, SOCKETS_SHUT_RDWR );

        /* Expect SOCKETS_Recv() to return an error once the shutdown is
         * complete. */
        xTimeOnEntering = xTaskGetTickCount();
        do
        {
            xReturned = SOCKETS_Recv( xSocket,                    /* The socket being received from. */
                                      &( pcReceivedString[ 0 ] ), /* The buffer into which the received data will be written. */
                                      echoBUFFER_SIZES,           /* The size of the buffer provided to receive the data. */
                                      0 );

            if( xReturned < 0 )
            {
                break;
            }
        } while( ( xTaskGetTickCount() - xTimeOnEntering ) < xReceiveTimeOut );
    }
    else
    {
        configPRINTF( ( "Echo demo failed to connect to echo server %d.%d.%d.%d echoECHO_PORT %d \r\n",
                        (client_ip >> 0) & 0xff,
                        (client_ip >> 8) & 0xff,
                        (client_ip >> 16) & 0xff,
                        (client_ip >> 24) & 0xff,
                        echoECHO_PORT ) );
        /* clear temp certificate for TLS connection. */
        vClearTLSTempInfo();
    }
    configPRINTF(("closing sockets \n"));
    /* Close this socket before looping back to create another. */
    xReturned = SOCKETS_Close( xSocket );
    configASSERT( xReturned == SOCKETS_ERROR_NONE );
    configPRINTF(("socket closed %d\n",xReturned));

    configPRINTF(("task done \n"));

}




void vConfigureAgentTask( void * pvParameters )
{

    /* Start a WiFi AP Mode before running the tests. */
    prvWifiAPStart();
    int client = 0;
    int client_ip = 0;
    for(;;) {
        client = xgetConnectedClient();
        if (client > 0) {
            vTaskDelay(3000);
            break;
        }
        vTaskDelay(1000);
    }
    client_ip = dhcp_get_latest_client_ip();
    configPRINTF(("client ip:%d.%d.%d.%d\r\n", 
        (client_ip >> 0) & 0xff,
        (client_ip >> 8) & 0xff,
        (client_ip >> 16) & 0xff,
        (client_ip >> 24) & 0xff));
    vTaskDelay( TIME_TO_WAIT_UI_PROVISION );
    prvConfigureAgentTask( (void *) &client_ip );    
    
    /* This task has finished.  FreeRTOS does not allow a task to run off the
     * end of its implementing function, so the task must be deleted. */
    vTaskDelete( NULL );
}

