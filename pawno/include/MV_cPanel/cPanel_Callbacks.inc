forward OnAccountCheck(playerid);
forward OnVipLoaded(playerid);
forward OnPlayerLogin(playerid);

public OnAccountCheck(playerid)
{
	if(cache_num_rows() == 1)
		ShowPlayerDialogEx(playerid,DIALOG_LOGIN);
	else
		ShowPlayerDialogEx(playerid,DIALOG_REGISTER);
}

public OnVipLoaded(playerid)
{
	if(cache_num_rows() == 1)
	{
		cache_get_value_name_int(0, "Duration", PlayerInfo[playerid][VIP]);
		
		printf("VIP %s has connected",PlayerInfo[playerid][Name]);

		if(PlayerInfo[playerid][VIP] < gettime() && PlayerInfo[playerid][VIP] != 0)
		{
			PlayerInfo[playerid][VIP] = -1;
			SendClientMessage(playerid, COLOR_RED, "Your VIP status has been expired.");

			mysql_format(gCon, query, sizeof(query), "DELETE FROM Vips WHERE Name = '%e'", PlayerInfo[playerid][Name]);
			mysql_tquery(gCon, query);
		}
	}
	else
		PlayerInfo[playerid][VIP] = -1;
}

public OnPlayerLogin(playerid)
{
	if(cache_num_rows() == 1)
	{
		SendClientMessage(playerid, COLOR_GREEN, "Successfully logged in.");

		cache_get_value_name_int(0, "Score", PlayerInfo[playerid][Score]);
		cache_get_value_name_int(0, "Money", PlayerInfo[playerid][Money]);
		cache_get_value_name_int(0, "Adminlevel", PlayerInfo[playerid][Adminlevel]);
		cache_get_value_name_int(0, "Kills", PlayerInfo[playerid][Kills]);
		cache_get_value_name_int(0, "Deaths", PlayerInfo[playerid][Deaths]);
		cache_get_value_name_int(0, "OnlineTime", PlayerInfo[playerid][OnlineTime]);

		ResetPlayerMoney(playerid);
		GivePlayerMoney(playerid, PlayerInfo[playerid][Money]);
		SetPlayerScore(playerid, PlayerInfo[playerid][Score]);
	}
	else
	{
		SendClientMessage(playerid, COLOR_RED, "Wrong password");
		ShowPlayerDialogEx(playerid, DIALOG_LOGIN);
	}
}