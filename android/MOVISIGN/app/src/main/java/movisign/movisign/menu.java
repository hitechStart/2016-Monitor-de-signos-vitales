package movisign.movisign;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class menu extends Activity  implements SensorEventListener{


   //inicializacion de variables

    Button btnPulso,btnTemp,btnO2,btnGrafico;

    TextView txtString;

    Handler bluetoothIn;

    final int handlerState = 0; //usada para identificar al handler message
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private StringBuilder recDataString = new StringBuilder();

    private ConnectedThread mConnectedThread;

    // SPP UUID service
    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    // String para la MAC address
       private static String address = null;


    private SensorManager senSensorManager;
    private Sensor senAccelerometer,proxSensor;

    private long lastUpdate = 0;
    private float last_x, last_y, last_z;
    private static final int SHAKE_THRESHOLD = 600;

    boolean flag=true;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_menu);

        //Para inicializar la instancia de SensorManager, invocamos getSystemService, que usaremos para acceder a los sensores
        // del sistema,pasando el nombre del servicio.
        senSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        //Con el gestor de sensores a nuestra disposición, obtenemos una referencia
        // al acelerómetro y al de proximidad ,invocando a getDefaultSensor en el gestor de sensores y pasando el tipo
        // de sensor que nos interesa.
        senAccelerometer = senSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        proxSensor= senSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);




        //vinculacion de los botones y textview a las respectivas views
        btnPulso = (Button) findViewById(R.id.button2);
        btnTemp = (Button) findViewById(R.id.button);
        btnO2 = (Button) findViewById(R.id.button3);
        btnGrafico = (Button) findViewById(R.id.button4);
        txtString = (TextView) findViewById(R.id.textView2);


        bluetoothIn = new Handler() {
            public void handleMessage(android.os.Message msg) {
                if (msg.what == handlerState) {										//Si es el mensaje que esperamos
                    String readMessage = (String) msg.obj;                            // msg.arg1 = bytes que envia connectthread
                    recDataString.append(readMessage);
                    int endOfLineIndex = recDataString.indexOf("~");                    // determinamos el fin de linea
                    if (endOfLineIndex > 0) {                                           //NOs aseguramos que haya datos despues de ~
                        String dataInPrint = recDataString.substring(0, endOfLineIndex);    // extraemos el string
                        txtString.setText("Datos recibidos = " + dataInPrint);

                        recDataString.delete(0, recDataString.length()); 					//limpiamos string data
                    }
                }
            }
        };

        btAdapter = BluetoothAdapter.getDefaultAdapter();       // get Bluetooth adapter

        checkBTState();


        // al pulsar el boton pulso envia datos al arduino y reproduce los audios
        btnPulso.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                mConnectedThread.write("a"); //enviamos via bluetooth un mensaje para indicarle que vamos a comenzar a usar el sensor
                //de pulso cardiaco comienza la tranasmision de audios para indicar las instrucciones para medir el pulso

                //Creamos el objeto de la clase MediaPlayer pasando ahora la referencia del objeto:
                MediaPlayer mp = MediaPlayer.create(getApplicationContext(), R.raw.audio8);

                //Iniciamos la reproducción del mp3
                mp.start();
                while(mp.isPlaying()){
                }

                mp = MediaPlayer.create(getApplicationContext(), R.raw.audio9);
                mp.start();
                while(mp.isPlaying()){
                }

                mp = MediaPlayer.create(getApplicationContext(), R.raw.audio10);
                mp.start();
                while(mp.isPlaying()){
                }

                mp = MediaPlayer.create(getApplicationContext(), R.raw.audio11);
                mp.start();
                while(mp.isPlaying()){
                }


            }
        });

//al pulsar e3l boton temperatura envia datos al arduino y reproduce audio
        btnTemp.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                mConnectedThread.write("b");//enviamos un mensaje al android para indicarle que vamos a comenzar a sensar la tempearatura
                //reproduccion de las instrucciones para usar el termometro
                //Creamos el objeto de la clase MediaPlayer pasando ahora la referencia del objeto:

                MediaPlayer mp = MediaPlayer.create(getApplicationContext(), R.raw.audio2);

                //Iniciamos la reproducción del mp3
                mp.start();
                while(mp.isPlaying()){
                }

                mp = MediaPlayer.create(getApplicationContext(), R.raw.audio3);
                mp.start();
                while(mp.isPlaying()){
                }

                mp = MediaPlayer.create(getApplicationContext(), R.raw.audio4);
                mp.start();
                while(mp.isPlaying()){
                }

                mp = MediaPlayer.create(getApplicationContext(), R.raw.audio5);
                mp.start();
                while(mp.isPlaying()){
                }

                mp = MediaPlayer.create(getApplicationContext(), R.raw.audio6);
                mp.start();
                while(mp.isPlaying()){
                }


            }
        });


        btnO2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                mConnectedThread.write("c");    // envia "c" via Bluetooth para comenzar a mostrar los datos obtenidos

            }
        });

        btnGrafico.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                mConnectedThread.write("d");    // Send "d" via Bluetooth para comenzar a graficar el  en el programa proccesing

            }
        });

    }


    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {

        return  device.createRfcommSocketToServiceRecord(BTMODULEUUID);
        //creates secure outgoing connecetion with BT device using UUID
    }

    @Override
    public void onResume() {
        super.onResume();

        //obtiene la MAC address enviada por la activity conexion via intent
        Intent intent = getIntent();

        address = intent.getStringExtra(conexion.EXTRA_DEVICE_ADDRESS);

        //crea device y le da un valor ,el de la MAC address
        BluetoothDevice device = btAdapter.getRemoteDevice(address);

        try {
            btSocket = createBluetoothSocket(device);
            //registramos el sensor utilizando uno de los métodos públicos de SensorManager registerListener.
            // Este método acepta tres argumentos, el contexto de la actividad, un sensor y la velocidad a la que se
            // nos envían los eventos del sensor.
            senSensorManager.registerListener(this, senAccelerometer, SensorManager.SENSOR_DELAY_NORMAL);
            senSensorManager.registerListener(this,proxSensor,SensorManager.SENSOR_DELAY_NORMAL);

        } catch (IOException e) {
            Toast.makeText(getBaseContext(), "La creacción del Socket fallo", Toast.LENGTH_LONG).show();
        }
        // Establece la conexion del socket bluetooth.
        try
        {
            btSocket.connect();
        } catch (IOException e) {
            try
            {
                btSocket.close();
            } catch (IOException e2)
            {


            }
        }
        mConnectedThread = new ConnectedThread(btSocket);
        mConnectedThread.start();

        //se envia un caracter al comenzar la transmision para checkear que el dispositivo esta conectado
        // si no es una excepcion se llamara al metodo write
        //mConnectedThread.write("x");
    }

    @Override
    public void onPause()
    {
        super.onPause();
        try
        {
            //cierra el socket bluetooth al dejar la activity
            btSocket.close();
            //dejamos de registrar el sensor
            senSensorManager.unregisterListener(this);
        } catch (IOException e2) {
        }
    }

    //Compruebamos que el dispositivo Bluetooth este disponible y solicitamos que se encienda si está desactivado
    private void checkBTState() {

        if(btAdapter==null) {
            Toast.makeText(getBaseContext(), "El dispositivo no soporta bluetooth", Toast.LENGTH_LONG).show();
        } else {
            if (btAdapter.isEnabled()) {
            } else {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);
            }
        }
    }
     //verificamos que el dispositivo cuente con los sensores :acelerometro y proximidad
    // para el acelerometro verificamos antes de enviar los datos al arduino , que el celular haya registrado algun movimiento
     // de su posicion inicial
    //el sensor de proximidad envia datos al arduino solo cuando el celular se acercarca al mismo.
    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {

        Sensor mySensor = sensorEvent.sensor;


        if (mySensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            float x = sensorEvent.values[0];
            float y = sensorEvent.values[1];
            float z = sensorEvent.values[2];

            long curTime = System.currentTimeMillis();

            if ((curTime - lastUpdate) > 100) {
                long diffTime = (curTime - lastUpdate);
                lastUpdate = curTime;

                float speed = Math.abs(x + y + z - last_x - last_y - last_z)/ diffTime * 10000;

                if (speed > SHAKE_THRESHOLD) {

                    mConnectedThread.write("e");//envia datos al arduino

                }

                last_x = x;
                last_y = y;
                last_z = z;
            }
        }
        else
        {
            if (mySensor.getType() == Sensor.TYPE_PROXIMITY && flag==true) {

                mConnectedThread.write("f");//envia datos al arduino

                MediaPlayer mp = MediaPlayer.create(getApplicationContext(), R.raw.audio1);

                //Iniciamos la reproducción del mp3
                mp.start();
                while(mp.isPlaying()){
                }
                flag=false;

            }


        }


    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }

    //creamos la clase cone
    //xion
    private class ConnectedThread extends Thread {
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;


        public ConnectedThread(BluetoothSocket socket) {
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {
                //creamos los I/O stream para la conexion
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }


        public void run() {
            byte[] buffer = new byte[256];
            int bytes;

            // mantenemos el while (true) para escuchar los mensajes recibidos
            while (true) {
                try {
                    bytes = mmInStream.read(buffer);        	//lee los byte del la entrada del buffer
                    String readMessage = new String(buffer, 0, bytes);
                   //envia los byte obtenidos del buffer al manejador de mensajes
                    bluetoothIn.obtainMessage(handlerState, bytes, -1, readMessage).sendToTarget();
                } catch (IOException e) {
                    break;
                }
            }
        }

        public void write(String input) {
            byte[] msgBuffer = input.getBytes();           //convierte la cadena que se introdujo en byte
            try {
                mmOutStream.write(msgBuffer);                // escribe el mensaje en bytr y lo envia atraves del bluetooth
            } catch (IOException e) {
                //if you cannot write, close the application
                Toast.makeText(getBaseContext(), "La Conexión fallo", Toast.LENGTH_LONG).show();
                finish();

            }
        }
    }
}

