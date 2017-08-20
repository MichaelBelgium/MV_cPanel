forward OnAccountCheck(playerid);
forward OnVipLoaded(playerid);

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