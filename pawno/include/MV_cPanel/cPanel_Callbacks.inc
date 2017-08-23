forward OnAccountCheck(playerid);
forward OnVipLoaded(playerid);
forward OnPlayerLogin(playerid);
forward PlayerTimer(playerid);

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
		cache_get_value_name_int(0, "Duration", VipInfo[playerid][Duration]);
		cache_get_value_name_bool(0, "Toggle0", VipInfo[playerid][Toggle][0]);
		cache_get_value_name_bool(0, "Toggle1", VipInfo[playerid][Toggle][1]);
		cache_get_value_name_bool(0, "Toggle2", VipInfo[playerid][Toggle][2]);

		
		printf("VIP %s has connected",PlayerInfo[playerid][Name]);

		if(VipInfo[playerid][Duration] < gettime() && VipInfo[playerid][Duration] != 0)
		{
			VipInfo[playerid][Duration] = -1;
			SendClientMessage(playerid, -1,  COL_VIP_1"-[VIP]-"COL_VIP_2" Your VIP status has been expired.");

			mysql_format(gCon, query, sizeof(query), "DELETE FROM Vips WHERE Name = '%e'", PlayerInfo[playerid][Name]);
			mysql_tquery(gCon, query);
		}
		else
			SendClientMessage(playerid, -1, COL_VIP_1"-[VIP]-"COL_VIP_2" Your vip settings have been loaded.");
	}
	else
		VipInfo[playerid][Duration] = -1;
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
		SetPlayerCash(playerid, PlayerInfo[playerid][Money]);
		SetPlayerScore(playerid, PlayerInfo[playerid][Score]);

		mysql_format(gCon, query, sizeof(query), "SELECT * FROM Vips WHERE Name = '%e'", PlayerInfo[playerid][Name]);
		mysql_tquery(gCon, query, "OnVipLoaded", "i", playerid);
	}
	else
	{
		SendClientMessage(playerid, COLOR_RED, "Wrong password");
		ShowPlayerDialogEx(playerid, DIALOG_LOGIN);
	}
}

public PlayerTimer(playerid)
{
	if(IsPlayerVIP(playerid) && IsPlayerInAnyVehicle(playerid) && VipInfo[playerid][Toggle][2])
	{
		new Float:vhealth, veh = GetPlayerVehicleID(playerid);

		GetVehicleHealth(veh, vhealth);
		if(vhealth <= 300) RepairVehicle(veh);
	}
}