**Introduction**
============
This packages will showcase a user provision approach solution using Cognito Your User Pools with the AWS Mobile SDK for Android, Amazon FreeRTOS platform, and AWS Lambda function.

**How to Use**
======================================

1. Package android_app including a patch from aws-sdk-android-samples (https://github.com/awslabs/aws-sdk-android-samples.git) used to patch the AmazonCognitoYourUserPoolsDemo:

      ``` C
      cd android_app/
      git clone https://github.com/awslabs/aws-sdk-android-samples.git
      cd aws-sdk-android-samples/AmazonCognitoYourUserPoolsDemo
      git apply ../../0001-port-user-provision-app.patch.patch
      ```

      then build a android application by android studio to let user log in AWS Cognito Service to get legal token to access other AWS Service and do the following action:
      - Get device certificate thru AWS API Gateway functionality with the legal token.
      - Setup WiFi credential which you want your device to connect to.
      - Start TLS commnunication to provision user credential and Wi-Fi credential to the devcie.

2. Package amazon-freertos Package android_app including a patch from Realtek Ameba Z2 repository (https://github.com/ambiot/amazon-freertos.git) used to be applied on Realtek Ameba Z2 platform, and it will start its SoftAP mode if there's no legal certificate inside it.
How to apply the patch:

      ``` C
      cd amazon-freertos/
      git clone https://github.com/ambiot/amazon-freertos.git
      cd amazon-freertos/
      git apply ../0001-port-user-provision-platform-code.patch
      ```

3. Package lambda used to let AWS API Gateway to access it, it will create thing, policy, and certificate and return the certificate result to caller.
