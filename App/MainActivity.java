package com.example.projectpathfinder;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;
import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MainActivity extends AppCompatActivity implements JoystickView.JoystickListener {
    public static final String BROKER = "tcp://mqtt.beebotte.com:1883";
    public static final String TOPIC = "project_pathfinder/motor_control";
    public static final String USERNAME = "token_jIKumPGtxHdtKuMa";
    public static final String PASSWORD = "";
    public MqttConnectOptions MQTT_CONNECTION_OPTIONS;
    public MqttAndroidClient CLIENT = new MqttAndroidClient(getBaseContext(), BROKER, MqttClient.generateClientId());

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        JoystickView joystick = new JoystickView(this);
        setContentView(R.layout.activity_main);

        MqttSetup(this);
        MqttConnect();

        CLIENT.setCallback(new MqttCallback() {
            @Override
            public void connectionLost(Throwable cause) {
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
            }
        });
    }

    @Override
    public void onJoystickMoved(float xPercent, float yPercent, int id)
    {
        MqttPublish(TOPIC, String.valueOf(yPercent));
        Log.d("Main method", "X percent: " + xPercent + " Y percent: " + yPercent);
    }

    void MqttSetup(Context context) {
        CLIENT = new MqttAndroidClient(getBaseContext(), BROKER, MqttClient.generateClientId());
        MQTT_CONNECTION_OPTIONS = new MqttConnectOptions();
        MQTT_CONNECTION_OPTIONS.setUserName(USERNAME);
        MQTT_CONNECTION_OPTIONS.setPassword(PASSWORD.toCharArray());
    }

    void MqttConnect() {
        try {

            final IMqttToken token = CLIENT.connect(MQTT_CONNECTION_OPTIONS);
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    // We are connected.
                    Toast.makeText(MainActivity.this, "MQTT: Online.", Toast.LENGTH_SHORT).show();
                    MqttSubscribe(TOPIC, (byte) 1);
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    // Something went wrong e.g. connection timeout or firewall problems.
                    Toast.makeText(MainActivity.this, "MQTT: Error connecting.", Toast.LENGTH_SHORT).show();
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    void MqttSubscribe(String topic, byte qos) {

        try {
            IMqttToken subToken = CLIENT.subscribe(topic, qos);
            subToken.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Log.d("MQTT:", "Subscribed to topic.");
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken,
                                      Throwable exception) {

                    Log.d("MQTT:", "Subscribing error.");
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    void MqttPublish(String topic, String msg) {

        MQTT_CONNECTION_OPTIONS.setWill(topic, msg.getBytes(), 0, false);
        try {
            IMqttToken token = CLIENT.publish(topic, new MqttMessage(msg.getBytes()));
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Log.d("MQTT:", "Data published.");
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.d("MQTT:", "Publish error.");
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}