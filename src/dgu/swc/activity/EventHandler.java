package dgu.swc.activity;

import org.json.JSONObject;

public interface EventHandler {

	void onMessage(JSONObject data);

	String getEvent();

}