package src.app.bootstrap;

import android.app.Application;

import src.app.boot.AppWrapper;
import src.app.mod.AppInfo;
import src.library.basis.W;

@SuppressWarnings({W.APP_OMIT_0, W.APP_OMIT_1, W.APP_OMIT_2, W.APP_OMIT_3})
public class App extends Application {

    static {
        System.loadLibrary("library");
    }
    
    @Override
    public void onCreate() {
        super.onCreate();

        //saving app context fot the first time,
        //it will be used by other parts.
        AppWrapper.assignApp(this);

        String packageName = AppInfo.getPackageName();
        String processName = AppInfo.getProcessName();
        if (packageName.equals(processName)) {
            Object ignored = MainAppWrapper.get();
        } else {
            Object ignored = TaskAppWrapper.get();
        }
    }
}
