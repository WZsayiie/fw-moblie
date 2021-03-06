package src.app.boot;

import android.app.Application;

import src.library.basis.W;
import src.library.foundation.L;

@SuppressWarnings({W.APP_OMIT_0, W.APP_OMIT_1, W.APP_OMIT_2, W.APP_OMIT_3})
public abstract class AppWrapper {

    private static Application sApp;

    private static synchronized Application storedApp(char intent, Application app) {
        switch (intent) {
            case '+': return sApp = app;
            case '?': return sApp;
            default : return null;
        }
    }

    public static void assignApp(Application app) {
        if (app == null) {
            L.e("try assign a null global app context");
            return;
        }

        L.i("assign global app context");
        storedApp('+', app);
    }

    public static Application getApp() {
        Application app = storedApp('?', null);
        if (app == null) {
            L.e("app context didn't assign");
        }
        return app;
    }
}
