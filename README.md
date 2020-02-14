**Introduction**
============
This packages will showcase a user provision approach solution using Cognito Your User Pools with the AWS Mobile SDK for Android, Amazon FreeRTOS platform, and AWS Lambda function.

**How to Use**
======================================

1. Package android_app used to let user log in AWS Cognito Service to get legal token to access other AWS Service and do the following action:
      - Get device certificate thru AWS API Gateway functionality with the legal token.
      - Setup WiFi credential which you want your device to connect to.
      - Start TLS commnunication to provision user credential and Wi-Fi credential to the devcie.

2. Package amazon-freertos used to be applied on Realtek Ameba Z2 platform, and it will start its SoftAP mode if there's no legal certificate inside it.

3. Package lambda used to let AWS API Gateway to access it, it will create thing, policy, and certificate and return the certificate result to caller.
