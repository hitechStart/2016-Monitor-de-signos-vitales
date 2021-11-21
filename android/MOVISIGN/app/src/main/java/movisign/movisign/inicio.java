package movisign.movisign;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class inicio extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_inicio);

        //pausa la aplicacion por 3000 ms
        Thread pause=new Thread(){

            public void run(){

                try{
                    sleep(3000);

                }catch(InterruptedException e){
                    e.printStackTrace();
                }finally{
                    finish();
                    Intent menu=new Intent(inicio.this,conexion.class);
                    startActivity(menu);
                }

            }
        };

        pause.start();


    }

}

