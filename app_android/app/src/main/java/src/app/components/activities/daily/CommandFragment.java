package src.app.components.activities.daily;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import androidx.fragment.app.Fragment;

import src.app.R;
import src.app.com.CommandHelper;
import src.library.basis.W;

@SuppressWarnings({W.APP_OMIT_0, W.APP_OMIT_1, W.APP_OMIT_2, W.APP_OMIT_3})
public class CommandFragment extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle state) {
        View contentView = inflater.inflate(R.layout.daily_cmd_fragment, container, false);
        initializeRootView(contentView);
        return contentView;
    }

    protected void initializeRootView(View contentView) {
        ListView listView = contentView.findViewById(R.id.content_list);
        CommandHelper helper = new CommandHelper();

        helper.setListViewIfNeeded(listView);
        helper.addAction("alpha", this::onActionAlpha);
        helper.addAction("beta", this::onActionBeta);
    }

    protected void onActionAlpha() {
    }

    protected void onActionBeta() {
    }
}
