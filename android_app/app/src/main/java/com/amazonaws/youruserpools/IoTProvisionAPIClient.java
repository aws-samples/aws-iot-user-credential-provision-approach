/*
 * Copyright 2010-2016 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

package cert_reply_1;

import java.util.*;

import cert_reply_1.model.CertReply;


@com.amazonaws.mobileconnectors.apigateway.annotation.Service(endpoint = "https://gqjtwdvfng.execute-api.ap-northeast-1.amazonaws.com/cert_test")
public interface IoTProvisionAPIClient {


    /**
     * A generic invoker to invoke any API Gateway endpoint.
     * @param request
     * @return ApiResponse
     */
    com.amazonaws.mobileconnectors.apigateway.ApiResponse execute(com.amazonaws.mobileconnectors.apigateway.ApiRequest request);

    /**
     *
     *
     * @param sn
     * @param authorization
     * @return CertReply
     */
    @com.amazonaws.mobileconnectors.apigateway.annotation.Operation(path = "/cert", method = "GET")
    CertReply certGet(
            @com.amazonaws.mobileconnectors.apigateway.annotation.Parameter(name = "sn", location = "query")
                    String sn,
            @com.amazonaws.mobileconnectors.apigateway.annotation.Parameter(name = "Authorization", location = "header")
                    String authorization);

    /**
     *
     *
     * @param sn
     * @return CertReply
     */
    @com.amazonaws.mobileconnectors.apigateway.annotation.Operation(path = "/cert", method = "POST")
    CertReply certPost(
            @com.amazonaws.mobileconnectors.apigateway.annotation.Parameter(name = "sn", location = "query")
                    String sn);

}

