#ifndef __CONFIG_SERVICE_H__
#define __CONFIG_SERVICE_H__

#define DATA_SSID_LEN 33
#define DATA_PASSWORD_LEN 33

class SharedData
{
public:
	static char ssid[DATA_SSID_LEN];
	static char password[DATA_PASSWORD_LEN];
	static void set_ssid(char *ssid);
	static void set_password(char *password);

	static int config_save();
	static int config_load();
};
#endif