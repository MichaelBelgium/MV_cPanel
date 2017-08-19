CMD:stats(playerid,params[])
{
	new string[128], stats[256], id, title[64];

	if(sscanf(params, "i", id))
	{
		format(title, sizeof(title), "Stats of %s (%i)", PlayerInfo[playerid][Name], playerid);

		format(string, sizeof(string), "Name: %s (ID: %i)\nLevel: %s (%i)\n\n", PlayerInfo[playerid][Name], playerid, GetPlayerLevel(PlayerInfo[playerid][Adminlevel]), PlayerInfo[playerid][Adminlevel]);
		strcat(stats, string);

		format(string, sizeof(string), "Kills: %i\nDeaths: %i\n\n",  PlayerInfo[playerid][Kills],  PlayerInfo[playerid][Deaths]);
		strcat(stats, string);

		format(string, sizeof(string), "Online time (session): %s\nOnline time (total): %s\n\n", MsToString(NetStats_GetConnectedTime(playerid),"%1h:%1m:%1s"), MsToString(((NetStats_GetConnectedTime(playerid)/1000)+PlayerInfo[playerid][OnlineTime])*1000,"%1h:%1m:%1s"));
		strcat(stats, string);
	}
	else
	{
		format(title, sizeof(title), "Stats of %s (%i)", PlayerInfo[id][Name], id);

		format(string, sizeof(string), "Name: %s (ID: %i)\nLevel: %s (%i)\n\n", PlayerInfo[id][Name], id, GetPlayerLevel(PlayerInfo[id][Adminlevel]), PlayerInfo[id][Adminlevel]);
		strcat(stats, string);

		format(string, sizeof(string), "Kills: %i\nDeaths: %i\n\n",  PlayerInfo[id][Kills], PlayerInfo[id][Deaths]);
		strcat(stats, string);

		format(string, sizeof(string), "Online time (session): %s\nOnline time (total): %s\n", MsToString(NetStats_GetConnectedTime(id),"%1h:%1m:%1s"), MsToString(((NetStats_GetConnectedTime(id)/1000)+PlayerInfo[id][OnlineTime])*1000,"%1h:%1m:%1s"));
		strcat(stats, string);
	}

	ShowPlayerDialog(playerid, DIALOG_NORESPONSE, DIALOG_STYLE_MSGBOX, title, stats, "OK", "");
	return 1;
}