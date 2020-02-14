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

package cert_reply_1.model;


public class CertReply {
    @com.google.gson.annotations.SerializedName("certificatePem")
    private String certificatePem = null;
    @com.google.gson.annotations.SerializedName("privateKey")
    private String privateKey = null;

    /**
     * Gets certificatePem
     *
     * @return certificatePem
     **/
    public String getCertificatePem() {
        return certificatePem;
    }

    /**
     * Sets the value of certificatePem.
     *
     * @param certificatePem the new value
     */
    public void setCertificatePem(String certificatePem) {
        this.certificatePem = certificatePem;
    }

    /**
     * Gets privateKey
     *
     * @return privateKey
     **/
    public String getPrivateKey() {
        return privateKey;
    }

    /**
     * Sets the value of privateKey.
     *
     * @param privateKey the new value
     */
    public void setPrivateKey(String privateKey) {
        this.privateKey = privateKey;
    }

}
