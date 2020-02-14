package com.amazonaws.youruserpools;


import android.content.Context;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;

import com.amazonaws.youruserpools.CognitoYourUserPoolsDemo.R;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.KeyStore;
import java.security.SecureRandom;

import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLServerSocket;
import javax.net.ssl.SSLServerSocketFactory;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.TrustManagerFactory;

/**
 * This is third tab page named STEP 3, used to
 * start TLS connection as a secure server.
 * After TLS client connecting to it, it will start
 * provision user credential ann WiFi credential.
 */
public class Step3 extends Fragment {
    private Button btnCon;
    WifiManager wifiManager;
    private String TAG = "STEP3";
    int ssl_result = -1;

    public Step3() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view =  inflater.inflate(R.layout.fragment_step3, container, false);
        btnCon = (Button) view.findViewById(R.id.button3);
        wifiManager = (WifiManager) view.getContext().getSystemService(Context.WIFI_SERVICE);

        btnCon.setOnClickListener(btnConOnClick);
        return view;
    }
    private Button.OnClickListener btnConOnClick = new Button.OnClickListener() {
        @Override
        public void onClick(View view) {
            WifiInfo info = wifiManager.getConnectionInfo();
            String ssid  = info.getSSID();
            System.out.println("========================ssid:"+ssid);
            System.out.println("========================bssid:"+info.getBSSID());
            if (AppHelper.getWifiSSID().length() == 0 || AppHelper.getWifiPassword().length() == 0) {
                toastMsg("Please input valid SSID in step 2");
                return;
            } else if (AppHelper.getCert().length() < 100 || AppHelper.getPrivateKey().length() < 100) {
                toastMsg("Please get valid cert/key in step 1");
                return;
            }

            new AsyncTask<Void, Void, Void>() {
                @Override
                protected Void doInBackground(Void... voids) {
                SSLServer server = null;

                try {
                    System.out.println("===================================>read store.bks");
                    server = new SSLServer(AppHelper.getKeyStream());
                } catch (Exception e) {
                    e.printStackTrace();
                }
                ssl_result = server.runServer();
                Handler handler = new Handler(getActivity().getMainLooper());
                handler.post(new Runnable() {
                    public void run() {
                        Toast toast;
                        if (ssl_result < 0) {
                            toast = Toast.makeText(getActivity(), "Provision credential failed!", Toast.LENGTH_LONG);
                        } else {
                            toast = Toast.makeText(getActivity(), "Provision credential done!", Toast.LENGTH_LONG);
                        }
                        toast.setGravity(Gravity.CENTER, 0, 0);
                        toast.show();
                    }
                });

                return null;
                }
            }.execute();
        }
    };

    public void toastMsg(String msg) {

        Toast toast = (Toast) Toast.makeText( getActivity(), msg, Toast.LENGTH_LONG);
        toast.show();

    }

    static class SSLServer {
        private SSLServerSocket serverSocket = null;
        private String TAG = "SSLServer";

        public SSLServer(InputStream inputStream) throws Exception {
            final String KEY_STORE_TYPE_BKS = "BKS";
            final String KEY_PASSWORD = "123456";
            KeyStore keyStore = KeyStore.getInstance(KEY_STORE_TYPE_BKS);

            keyStore.load(inputStream, KEY_PASSWORD.toCharArray());

            TrustManagerFactory trustManagerFactory = TrustManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
            trustManagerFactory.init(keyStore);

            KeyManagerFactory keyManagerFactory = KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
            keyManagerFactory.init(keyStore, KEY_PASSWORD.toCharArray());

            SSLContext sslContext = SSLContext.getInstance("TLS");
            sslContext.init(keyManagerFactory.getKeyManagers(), trustManagerFactory.getTrustManagers(), new SecureRandom());

            //  get SSLServerSocketFactory
            SSLServerSocketFactory socketFactory = sslContext.getServerSocketFactory();
            serverSocket = (SSLServerSocket) socketFactory.createServerSocket(7070);
        }

        public int runServer() {
            int ret = -1;
            try {
                Log.d(TAG, "===================================>Waiting for connection...");
                serverSocket.setNeedClientAuth(false);
                SSLSocket socket = (SSLSocket) serverSocket.accept();
                Log.d(TAG, "===================================>before send");
                DataOutputStream output = new DataOutputStream(socket.getOutputStream());
                // send private key
                Log.d(TAG, "===================================>AppHelper.PrivateKey:"+AppHelper.getPrivateKey());
                output.writeUTF(AppHelper.getPrivateKey());
                timeout(5000);

                //send cert
                Log.d(TAG, "===================================>AppHelper.cert:"+AppHelper.getCert());
                output.writeUTF(AppHelper.getCert());
                timeout(5000);

                //send WiFi SSID
                Log.d(TAG, "===================================>AppHelper.SSID:"+AppHelper.getWifiSSID());
                output.writeBytes(AppHelper.getWifiSSID());
                timeout(5000);

                //send WiFi password
                Log.d(TAG, "===================================>AppHelper.wifi_password:"+AppHelper.getWifiPassword());
                output.writeBytes(AppHelper.getWifiPassword());
                timeout(5000);

                output.close();
                socket.close();
                ret = 0;

            } catch (IOException ioException) {
                Log.d(TAG, ioException.toString());
            }
            return ret;
        }

        private void timeout(int msec)
        {
            try {
                Thread.sleep(msec);
            } catch (InterruptedException e) {
                Log.d(TAG, e.toString());
            }
        }
    }
}
