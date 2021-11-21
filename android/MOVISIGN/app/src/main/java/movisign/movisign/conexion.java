package movisign.movisign;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Set;

public class conexion extends Activity {
    // Debugging for LOGCAT
    private static final String TAG = "DeviceListActivity";
    private static final boolean D = true;

    // string que se le va enviar a la activity menu
    public static String EXTRA_DEVICE_ADDRESS = "device_address";

    // inicialiacion de variables bluetooth
    private BluetoothAdapter mBtAdapter;
    private ArrayAdapter<String> mPairedDevicesArrayAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_conexion);
    }

    @Override
    public void onResume()
    {
        super.onResume();
        //***************
        checkBTState();

        // inializacion del array adapter para emparejar de dispositivos
        mPairedDevicesArrayAdapter = new ArrayAdapter<String>(this, R.layout.activity_nombre);

        // busca y setea en el ListView para emparejar los dispositivos
        ListView pairedListView = (ListView) findViewById(R.id.ListView);
        pairedListView.setAdapter(mPairedDevicesArrayAdapter);
        pairedListView.setOnItemClickListener(mDeviceClickListener);

        // utiliza el metodo get de bluetooth adapter
        mBtAdapter = BluetoothAdapter.getDefaultAdapter();

        // obtiene el conjunto de dispositivos actualmente emparejados y los agrega en 'pairedDevices'
        Set<BluetoothDevice> pairedDevices = mBtAdapter.getBondedDevices();

        // introduce los dispositivos emparejados al array
        if (pairedDevices.size() > 0) {
            findViewById(R.id.textView).setVisibility(View.VISIBLE);//hace al titulo visible
            for (BluetoothDevice device : pairedDevices) {
                mPairedDevicesArrayAdapter.add(device.getName() + "\n" + device.getAddress());
            }
        } else {
            Toast.makeText(getBaseContext(), " No hay dispositivos emparejados ", Toast.LENGTH_SHORT).show();
        }
    }

    // Setea on-click listener para  crear la lista
    private AdapterView.OnItemClickListener mDeviceClickListener = new AdapterView.OnItemClickListener() {
        public void onItemClick(AdapterView<?> av, View v, int arg2, long arg3) {

            Toast.makeText(getBaseContext(), "Conectando...", Toast.LENGTH_SHORT).show();
            // obtene la direccion del dispositivo , la cual es de 17 chars ,en el View
            String info = ((TextView) v).getText().toString();
            String address = info.substring(info.length() - 17);

            // hace un intent para comenzar la proxima activity y le envia la direccion del dispositivo
            Intent i = new Intent(conexion.this, menu.class);
            i.putExtra(EXTRA_DEVICE_ADDRESS, address);
            startActivity(i);
        }
    };

    private void checkBTState() {
        // Checkea el dispositivo bluetooth para saber si esta encendido.
        mBtAdapter=BluetoothAdapter.getDefaultAdapter(); // checkea la salida del modulo HC-o6 !!!
        if(mBtAdapter==null) {
            Toast.makeText(getBaseContext(), "El dispositivo no soporta Bluetooth", Toast.LENGTH_SHORT).show();
        } else {
            if (mBtAdapter.isEnabled()) {
                Log.d(TAG, "...Bluetooth Activado...");
            } else {
                //enciende del dispositivo el Bluetooth
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);

            }
        }
    }
}