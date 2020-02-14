package com.amazonaws.youruserpools;

import android.os.Bundle;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.view.ViewGroup;
import com.amazonaws.youruserpools.CognitoYourUserPoolsDemo.R;

import java.util.ArrayList;


/**
 * This activity used to do:
 * 1. Use AWS Cognito Service to get user ID Token.
 * 2. Get User credential by the user ID Token.
 * 3. Start a TLS server to let iot device connect to it.
 * 4. Provision User/WiFi credential to IoT device.
 */
public class CertProvisionActivity extends AppCompatActivity {

    private ViewPager viewPager;

    private TabLayout tabLayout;

    private ArrayList<Integer> pages;

    private static final int pageNumber = 3;

    private void initData() {

        pages = new ArrayList<>();

        for (int i = 0; i < pageNumber; i++) {
            pages.add(i);
        }

    }

    private void initViews() {

        viewPager = (ViewPager) findViewById(R.id.viewpager);

        tabLayout = (TabLayout) findViewById(R.id.tablayout);

        PageAdapter pageAdapter = new PageAdapter(getSupportFragmentManager(), pages.size());

        viewPager.setAdapter(pageAdapter);

        viewPager.addOnPageChangeListener(new TabLayout.TabLayoutOnPageChangeListener(tabLayout));

        tabLayout.setupWithViewPager(viewPager);

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cert_provision);

        initData();

        initViews();
    }

    private class PageAdapter extends FragmentPagerAdapter {

        private int numOfTabs;

        PageAdapter(FragmentManager fm, int numOfTabs) {
            super(fm);
            this.numOfTabs = numOfTabs;
        }

        @Override
        public int getCount() {
            return numOfTabs;
        }

        @Override
        public Fragment getItem(int position) {
            switch (position) {
                case 0:
                    return new Step1();
                case 1:
                    return new Step2();
                case 2:
                    return new Step3();
                default:
                    return null;
            }
        }

        @Override
        public CharSequence getPageTitle(int position) {
            return "Step " + (position+1);
        }
    }
}

