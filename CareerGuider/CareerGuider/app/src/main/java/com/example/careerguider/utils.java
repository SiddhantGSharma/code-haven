package com.example.careerguider;
import android.os.Bundle;
import android.util.Log;
import android.content.Context;

import androidx.appcompat.app.AppCompatActivity;

import java.io.File;
import java.io.FileReader;
//import java.util.Iterator;
//import java.util.Map;
//import java.util.*;
import org.json.simple.*;
//import org.json.simple.parser.*;
import org.json.simple.parser.JSONParser;
import org.json.simple.JSONArray;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;

public class utils {

    protected JSONObject read_file(Context context) {

        JSONParser parser = new JSONParser();
        Log.println(Log.DEBUG, "Name","\n\n\nHERE");


        try {
            InputStream inputStream=context.getResources().openRawResource(R.raw.data);
            int isize=inputStream.available();
            byte[] buffer=new byte[isize];
            inputStream.read(buffer);
            inputStream.close();
            String json=new String(buffer,"UTF-8");
//            aa=json;

            Object obj = parser.parse(json);
            org.json.simple.JSONObject jsonObject = (org.json.simple.JSONObject)obj;
//            String name = (String)jsonObject.get("Name");
//            String course = (String)jsonObject.get("Course");
            JSONArray users = (JSONArray)jsonObject.get("users");
//            Iterator<Object> it = users.iterator();
//            while(it.hasNext()){
//                JSONObject jobj = (JSONObject) it.next();
//                System.out.println(jobj);
//            }
            for (Object o : users) {
                org.json.simple.JSONObject user = (org.json.simple.JSONObject) o;
                String name = (String) user.get("name");
//                System.out.println("\n\n\n"+name );
                Log.println(Log.DEBUG, "Name","\n\n\n"+name );
//                String course = (String) user.get("id");
//                System.out.println(name + " " + course);
            }
            return jsonObject;


        } catch (IOException e) {
            e.printStackTrace();
        } catch(Exception e) {
            e.printStackTrace();
        }
        return null;



    }
}
