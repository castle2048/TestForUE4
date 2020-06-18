package com.example.testreadjar2;

import com.example.testjar6.TestJar1;

public class TestCallJar {
    public static void main(String[] args) {
        System.out.printf("error: cc main \n");
        TestJar1.GetMsg();
    }

	public void TestCall(){
		System.out.printf("error: cc Android TestCall \n");
		TestJar1.GetMsg();
	}
}
