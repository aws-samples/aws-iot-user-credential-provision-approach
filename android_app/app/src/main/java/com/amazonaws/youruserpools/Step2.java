package com.amazonaws.youruserpools;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.amazonaws.youruserpools.CognitoYourUserPoolsDemo.R;

/**
 * This is second tab page named STEP 2, used to
 * setup WiFi credential for device connecting to
 * after provisioning user credential.
 */
public class Step2 extends Fragment {
    private EditText editTextSSID;
    private EditText editTextPwd;
    private Button btnWriteWifi;

    public Step2() {
        // Required empty public constructor
    }

    public void toastMsg(String msg) {
        Toast toast = (Toast) Toast.makeText( getActivity(), msg, Toast.LENGTH_LONG);
        toast.show();

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view =  inflater.inflate(R.layout.fragment_step2, container, false);
        editTextSSID = (EditText) view.findViewById(R.id.ssid_input);
        editTextPwd = (EditText) view.findViewById(R.id.wifi_pwd_input);
        btnWriteWifi = (Button) view.findViewById(R.id.btn_write_wifi);

        btnWriteWifi.setOnClickListener(btnWifiWriteOnClick);

        return view;
    }

    public String formatCredentialText(String credentialText) {
        // Remove whitespace at end of PEM file (to eliminate differences between input files)
        credentialText = credentialText.trim();

        // Replace any CR/LF pairs with a newline character.
        credentialText = credentialText.replace("\r\n/", "\n");

        credentialText = credentialText + "\0";


        return credentialText;
    }

    private Button.OnClickListener btnWifiWriteOnClick = new Button.OnClickListener() {
        public void onClick(View view) {
            if (editTextSSID.getText().toString().length() > 0) {
                AppHelper.setWifiSSID(formatCredentialText(editTextSSID.getText().toString()));

            } else {
                AppHelper.setWifiSSID("cert not found");
            }

            if (editTextPwd.getText().toString().length() > 0) {
                AppHelper.setWifiPassword(formatCredentialText(editTextPwd.getText().toString()));
            } else {
                AppHelper.setWifiPassword("password not found");
            }

            toastMsg("get ssid:" + AppHelper.getWifiSSID()+",len:"+AppHelper.getWifiSSID().length());
            toastMsg("get wifi:" + AppHelper.getWifiPassword()+",len:"+AppHelper.getWifiPassword().length());
        }
    };
}
