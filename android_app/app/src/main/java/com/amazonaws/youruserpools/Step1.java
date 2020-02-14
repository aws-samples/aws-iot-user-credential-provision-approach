package com.amazonaws.youruserpools;


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
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.amazonaws.ClientConfiguration;
import com.amazonaws.mobileconnectors.apigateway.ApiClientFactory;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoUserPool;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoUserSession;
import com.amazonaws.youruserpools.CognitoYourUserPoolsDemo.R;

import static android.support.constraint.Constraints.TAG;

/**
 * This is first tab page named STEP 1, used to
 * get cert/private key by entering DSN.
 */
public class Step1 extends Fragment {

    private Button btnGenkey;
    private EditText editTextUID;
    private TextView certContent;
    private TextView keyContent;
    private static int creSize = 10;

    public Step1() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view =  inflater.inflate(R.layout.fragment_step1, container, false);
        btnGenkey = (Button) view.findViewById(R.id.button2);
        editTextUID = (EditText) view.findViewById(R.id.device_uid_input);
        certContent = (TextView) view.findViewById(R.id.textView15);
        keyContent = (TextView) view.findViewById(R.id.textView16);

        updateCertKeyView();

        btnGenkey.setOnClickListener(btnGenkeyOnClick);

        return view;
    }

    private Button.OnClickListener btnGenkeyOnClick = new Button.OnClickListener() {
        public void onClick(View view) {
            if( editTextUID.length() == 0 ) {
                toastMsg("Please input UID");
            } else if( editTextUID.length() < 8 ) {
                toastMsg("Please input correct UID");
            } else {

                new AsyncTask<String, Void, Void>() {
                    // Runs in UI before background thread is called
                    @Override
                    protected void onPreExecute() {
                        super.onPreExecute();
                        // Do something like display a progress bar
                    }

                    @Override
                    protected Void doInBackground(String... params) {

                        String idToken = params[0];
                        Log.d(TAG, "========================idToken:" + idToken);
                        Log.d(TAG, "========================UID:" + params[1]);
                        ApiClientFactory factory = new ApiClientFactory();
                        cert_reply_1.IoTProvisionAPIClient client = factory.build(cert_reply_1.IoTProvisionAPIClient.class);
                        cert_reply_1.model.CertReply crtRly = client.certGet(params[1], idToken);
                        Log.d(TAG, "cert=" + crtRly.getCertificatePem());
                        Log.d(TAG, "private key=" + crtRly.getPrivateKey());
                        Log.d(TAG, "cert len:" + crtRly.getCertificatePem().length());
                        Log.d(TAG, "private key len" + crtRly.getPrivateKey().length());

                        if (crtRly.getCertificatePem().length() > 0 &&
                                crtRly.getPrivateKey().length() > 0) {

                            AppHelper.setCert(formatCredentialText(crtRly.getCertificatePem()));
                            AppHelper.setPrivateKey(formatCredentialText(crtRly.getPrivateKey()));
                            Log.d(TAG, "========================AppHelper.cert:" + AppHelper.getCert());
                            Log.d(TAG, "========================AppHelper.PrivateKey:" + AppHelper.getPrivateKey());
                        }

                        Handler handler = new Handler(getActivity().getMainLooper());
                        handler.post(new Runnable() {
                            public void run() {
                                Toast toast;
                                if (AppHelper.getCert().length() > 0 && AppHelper.getPrivateKey().length() > 0) {
                                    toast = Toast.makeText(getActivity(), "Got Cert and Key successfully", Toast.LENGTH_LONG);
                                } else {
                                    toast = Toast.makeText(getActivity(), "Got Cert and Key failed", Toast.LENGTH_LONG);
                                }
                                toast.setGravity(Gravity.CENTER, 0, 0);
                                toast.show();
                            }
                        });
                        publishProgress();
                        return null;
                    }

                    // This is called from background thread but runs in UI
                    @Override
                    protected void onProgressUpdate(Void ...value) {
                        super.onProgressUpdate();
                        // Do things like update the progress bar
                    }

                    // This runs in UI when background thread finishes
                    @Override
                    protected void onPostExecute(Void value) {
                        super.onProgressUpdate();
                        Log.d(TAG, "onProgressUpdate...");
                        updateCertKeyView();
                    }

                }.execute(AppHelper.getIdToken(), editTextUID.getText().toString());
            }
        }

    };

    private void updateCertKeyView() {
        Log.d(TAG, "cert len:" + AppHelper.getCert().length());
        Log.d(TAG, "private key len" + AppHelper.getPrivateKey().length());
        if (AppHelper.getCert().length() > creSize)
            certContent.setText(AppHelper.getCert().substring(0,10) + "...");

        if (AppHelper.getPrivateKey().length() > creSize)
            keyContent.setText(AppHelper.getPrivateKey().substring(0,10) + "...");

    }

    public void toastMsg(String msg) {
        Toast toast = (Toast) Toast.makeText( getActivity(), msg, Toast.LENGTH_LONG);
        toast.show();

    }

    public String formatCredentialText(String credentialText) {
        // Remove whitespace at end of PEM file (to eliminate differences between input files)
        credentialText = credentialText.trim();

        // Replace any CR/LF pairs with a newline character.
        credentialText = credentialText.replace("\r\n/", "\n");

        return credentialText;
    }

}
