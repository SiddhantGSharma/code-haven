package com.example.careerguider;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Context;


import android.content.Intent;
import android.os.Bundle;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import com.example.careerguider.utils;

import org.json.simple.JSONObject;
import java.io.*;
import org.json.simple.JSONArray;

import org.json.simple.parser.JSONParser;
import java.io.FileReader;
import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.utils.ColorTemplate;
import java.util.ArrayList;

import android.graphics.Color;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {
    // variable for our bar chart
    BarChart barChart;

    // variable for our bar data.
    BarData barData;

    // variable for our bar data set.
    BarDataSet barDataSet;

    // array list for storing entries.
    ArrayList barEntriesArrayList;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        utils u = new utils();
//        JSONObject data=u.read_file("data.json");
//        Log.println(Log.INFO, "Name", data.toString());
        Context context=getApplicationContext();

        JSONObject data=u.read_file(context);
        Log.println(Log.INFO, "Name", data.toString());


        barChart = findViewById(R.id.idBarChart);

        // calling method to get bar entries.
        getBarEntries();

        // creating a new bar data set.
        barDataSet = new BarDataSet(barEntriesArrayList, "Geeks for Geeks");

        // creating a new bar data and
        // passing our bar data set.
        barData = new BarData(barDataSet);

        // below line is to set data
        // to our bar chart.
        barChart.setData(barData);

        // adding color to our bar data set.
        barDataSet.setColors(ColorTemplate.MATERIAL_COLORS);

        // setting text color.
        barDataSet.setValueTextColor(Color.BLACK);

        // setting text size
        barDataSet.setValueTextSize(16f);
        barChart.getDescription().setEnabled(false);


    }




    private void getBarEntries() {
        // creating a new array list
        barEntriesArrayList = new ArrayList<>();

        // adding new entry to our array list with bar
        // entry and passing x and y axis value to it.
        barEntriesArrayList.add(new BarEntry(1f, 4));
        barEntriesArrayList.add(new BarEntry(2f, 6));
        barEntriesArrayList.add(new BarEntry(3f, 8));
        barEntriesArrayList.add(new BarEntry(4f, 2));
        barEntriesArrayList.add(new BarEntry(5f, 4));
        barEntriesArrayList.add(new BarEntry(6f, 1));
    }

}