CMD:cmds(playerid,params[])
{
	new cmds[256*3];

	strcat(cmds, "General: /stats /admins /report\n");

	if(IsPlayerVIP(playerid)) strcat(cmds, "Vips: /vipveh /viprep /viprepair /viprep /vipnos /vipflip /vipunlimnos /vipheal /vipgodcar /viplock\n");
	if(IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) strcat(cmds, "Trial mod: /a(chat)\n");
	if(IsPlayerAdminEx(playerid, LEVEL_OWNER)) strcat(cmds, "Owner: /makevip /removevip\n");

	ShowPlayerDialog(playerid, DIALOG_NORESPONSE, DIALOG_STYLE_MSGBOX, "Available commands:", cmds, "OK", "");
	return 1;
}

CMD:commands(playerid, params[]) return cmd_cmds(playerid,params);

CMD:stats(playerid,params[])
{
	new string[128], stats[256], id, title[64];

	if(sscanf(params, "i", id))
	{
		format(title, sizeof(title), "Stats of %s (%i)", PlayerInfo[playerid][Name], playerid);

		format(string, sizeof(string), "Name: %s (ID: %i)\nLevel: %s (%i)\nVIP: %s\nPing: %i\nIP: %s\n\n", PlayerInfo[playerid][Name], playerid, GetPlayerLevel(PlayerInfo[playerid][Adminlevel]), PlayerInfo[playerid][Adminlevel], IsPlayerVIP(playerid) ? ("yes") : ("no"), GetPlayerPing(playerid), PlayerInfo[playerid][IP]);
		strcat(stats, string);

		format(string, sizeof(string), "Kills: %i\nDeaths: %i\nScore: %i\nMoney: %i\n\n",  PlayerInfo[playerid][Kills],  PlayerInfo[playerid][Deaths], GetPlayerScore(playerid), GetPlayerCash(playerid));
		strcat(stats, string);

		format(string, sizeof(string), "Online time (session): %s\nOnline time (total): %s\n\n", MsToString(NetStats_GetConnectedTime(playerid),"%1h:%1m:%1s"), MsToString(((NetStats_GetConnectedTime(playerid)/1000)+PlayerInfo[playerid][OnlineTime])*1000,"%1h:%1m:%1s"));
		strcat(stats, string);
	}
	else
	{
		format(title, sizeof(title), "Stats of %s (%i)", PlayerInfo[id][Name], id);

		format(string, sizeof(string), "Name: %s (ID: %i)\nLevel: %s (%i)\nVIP: %s\nPing: %i\nIP: %s\n\n", PlayerInfo[id][Name], id, GetPlayerLevel(PlayerInfo[id][Adminlevel]), PlayerInfo[id][Adminlevel], IsPlayerVIP(id) ? ("yes") : ("no"), GetPlayerPing(id), PlayerInfo[id][IP]);
		strcat(stats, string);

		format(string, sizeof(string), "Kills: %i\nDeaths: %i\nScore: %i\nMoney: %i\n\n",  PlayerInfo[id][Kills], PlayerInfo[id][Deaths], GetPlayerScore(id), GetPlayerCash(id));
		strcat(stats, string);

		format(string, sizeof(string), "Online time (session): %s\nOnline time (total): %s\n", MsToString(NetStats_GetConnectedTime(id),"%1h:%1m:%1s"), MsToString(((NetStats_GetConnectedTime(id)/1000)+PlayerInfo[id][OnlineTime])*1000,"%1h:%1m:%1s"));
		strcat(stats, string);
	}

	ShowPlayerDialog(playerid, DIALOG_NORESPONSE, DIALOG_STYLE_MSGBOX, title, stats, "OK", "");
	return 1;
}

CMD:admins(playerid, params[])
{
	new string[128], admins[256*5], count = 0;
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(PlayerInfo[i][Adminlevel] == 0) continue;
		format(string, sizeof(string), "Name: %s - Level: %s (%i)\n", PlayerInfo[i][Name], GetPlayerLevel(PlayerInfo[playerid][Adminlevel]), PlayerInfo[playerid][Adminlevel]);
		strcat(admins, string);

		count++;
	}

	if(count == 0)
		admins = "No admins online";
	else
	{
		format(string, sizeof(string), "There %s %i admin%s online:\n\n", count == 1 ? ("is") : ("are"), count, count == 1 ? ("s") : (""));
		strins(admins, string, 0);
	}

	ShowPlayerDialog(playerid, DIALOG_NORESPONSE, DIALOG_STYLE_MSGBOX, "Admins", admins, "OK", "");
	return 1;
}

CMD:report(playerid, params[])
{
	new id, reason[32], string[128];
	if(sscanf(params, "is[32]", id, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /report [playerid] [reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This playerid is not online.");

	format(string, sizeof(string), "Incomming report! %s (%i) about %s (%i): %s", PlayerInfo[playerid][Name], playerid, PlayerInfo[id][Name], id, reason);
	SendClientMessageToAdmins(COLOR_RED, string);

	SendClientMessage(playerid,COLOR_RED,"Your report has been sent to all online administrators.");
	return 1;
}

// ================= ADMIN COMMANDS ===========================
// CMD:cpanel(playerid, params[])
// {
// 	if(IsPlayerAdmin(playerid))
// 	{
// 		PlayerInfo[playerid][Adminlevel] = LEVEL_OWNER;
// 		SendClientMessage(playerid, COLOR_GREEN, "Server assigned you as an owner (level 5)");
// 	}
	
// 	ShowPlayerDialog(playerid,DIALOG_NORESPONSE,DIALOG_STYLE_LIST,"cPanel by {FF0A00}Michael@Belgium","Open player panel\nOpen server panel\nRestart server","Ok","Cancel");	
// 	return 1;
// }

CMD:a(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	new msg[128], string[256];
	if(sscanf(params, "s[128]", msg)) return SendClientMessage(playerid, COLOR_RED, "Usage: /a(chat) [message]");

	format(string, sizeof(string), COL_ADMIN_1"%s %s (%i):"COL_ADMIN_2" %s", GetPlayerLevel(PlayerInfo[playerid][Adminlevel]), PlayerInfo[playerid][Name], playerid, msg);
	SendClientMessageToAdmins(-1, string);
	return 1;
}

CMD:achat(playerid, params[]) return cmd_a(playerid, params);

CMD:makevip(playerid,params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_OWNER)) return 0;

	new id, duration, string[128];
	if(sscanf(params, "iI(0)", id, duration)) return SendClientMessage(playerid, COLOR_RED, "Usage: /makevip [playerid] [Optional: duration (in months)]");

	if(duration == 0)
	{
		mysql_format(gCon, query,sizeof(query),"INSERT INTO Vips (Name,Duration) VALUES ('%e', 0)",PlayerInfo[id][Name]);
		format(string, sizeof(string), "ever.");
	}
	else 
	{
		mysql_format(gCon, query,sizeof(query),"INSERT INTO Vips (Name,Duration) VALUES ('%e', UNIX_TIMESTAMP(DATE_ADD(NOW(), INTERVAL + %d MONTH)))",PlayerInfo[id][Name],duration);
		format(string, sizeof(string), "%i months.", duration);
	}

	mysql_tquery(gCon, query);

	PlayerInfo[id][VIP] = true;

	format(string, sizeof(string),  COL_VIP_1"-[VIP]-"COL_VIP_2" %s has made you a VIP! You are vip for %s", PlayerInfo[playerid][Name], string);
	SendClientMessage(id, -1, string);
	return 1;
}

CMD:removevip(playerid, params[])
{
	if(PlayerInfo[playerid][Adminlevel] != LEVEL_OWNER) return 0;

	new id;
	if(sscanf(params, "i", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /removevip [playerid]");
	if(!IsPlayerVIP(id)) return SendClientMessage(playerid, COLOR_RED, "This player isn't vip");

	PlayerInfo[id][VIP] = 0;
	mysql_format(gCon, query, sizeof(query), "DELETE FROM Vips WHERE Name = '%e'", PlayerInfo[id][Name]);
	mysql_tquery(gCon, query);

	SendClientMessage(id, -1, COL_VIP_1"-[VIP]-"COL_VIP_2" Your vip status has been removed.");
	return 1;
}

// ================= VIP COMMANDS ===========================
CMD:vipveh(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	return 1;
}

CMD:viprep(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	return 1;
}

CMD:viprepair(playerid, params[]) return cmd_viprep(playerid, params);

CMD:vipnos(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	return 1;
}

CMD:vipflip(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	return 1;
}

CMD:vipunlimnos(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	return 1;
}

CMD:vipheal(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	return 1;
}

CMD:vipgodcar(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	return 1;
}

CMD:viplock(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	return 1;
}